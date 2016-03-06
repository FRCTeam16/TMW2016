/*
 * VisionDataParser.cpp
 */

#include "VisionDataParser.h"
#include <algorithm>
#include <iostream>
#include <vector>

VisionDataParser::VisionDataParser() {
	std::cout << "VisionDataParser constructed\n";
}

VisionDataParser::~VisionDataParser() {
}

void VisionDataParser::AddRawData(const char* buffer, const int nchars) {
	std::lock_guard<std::mutex> lock(data_mutex);
//	std::cout << "RECV BUFFER: " << buffer << "\n";
    if (buffer == nullptr) {
        std::cerr << "Null buffer detected, skipping\n";
        return;
    }

    for (int i=0; i<nchars; i++) {
        if (buffer[i] == START_FRAME) {
            if (!parsing) {
                parsing = true;
            } else {
                std::cerr << "Parser State Error: Detected start frame while reading a frame, discarding previous contents\n";
                Reset();
            }
        } else if (buffer[i] == END_FRAME) {
            if (parsing) {
                ProcessDataFrame();

                // TODO: Move to better place
                data_frame.clear();             // clear state flags
                data_frame.str(std::string());  // clear internal buffer
                parsing = false;                // reset parsing flag

            } else {
            	Reset();
                std::cerr << "Parser State Error: Detected end frame while not parsing, discarding previous contents\n";
            }
        } else if (buffer[i] == NO_DATA) {
            if (parsing) {
                std::cerr << "Parser State Error: Detected no data while reading a frame, discarding previous contents\n";
                Reset();
            } else {
            	Reset();
                continue;
            }
        } else {
            if (parsing) {
                data_frame << buffer[i];
            } else {
//                std::cerr << "Parser State Error: Unexpected data character [" << buffer[i] << "] while not reading a frame, ignoring\n";
            }
        }
    }
}

VisionData VisionDataParser::GetVisionData() {
	std::lock_guard<std::mutex> lock(data_mutex);
	return visionData;
}

void VisionDataParser::SetVisionData(const VisionData vd) {
	visionData = vd;
}

void VisionDataParser::ProcessDataFrame() {
	std::vector<float> data;
	std::string token;
	while (getline(data_frame, token, ',')) {
		try {
			float value = stof(token);
			data.push_back(value);
//            cout << "Token: " << setw(8) << token << " Value: " << setw(8) << value << '\n';
		} catch (std::invalid_argument &e) {
			std::cerr << "ERROR: Invalid vision argument for " << token << ':' << e.what() << '\n';
			return;
		}
	}

	if (data.size() == 12) {
		GoalInfo predicted(data[0], data[1], data[2], data[3]);
		GoalInfo      left(data[4], data[5], data[6], data[7]);
		GoalInfo     right(data[8], data[9], data[10], data[11]);

//		for (int i=0 ;i<3; i++) {
//			std::cout << "<";
//			std::for_each(data.begin() + i*3, data.begin() + i*3 + 4, [](float &f) { std::cout << "[" << f << "]"; });
//			std::cout << "> ";
//		}
//		std::cout << "\n";

		SetVisionData(VisionData(predicted, left, right));
	} else {
		std::cerr << "VisionDataParser Expected 12 data items, but got " << data.size() << '\n';
	}
}

void VisionDataParser::Reset() {
    data_frame.clear();             // clear state flags
    data_frame.str(std::string());  // clear internal buffer
    parsing = false;                // reset parsing flag
    SetVisionData(emptyData);   	// reset vision data
}

