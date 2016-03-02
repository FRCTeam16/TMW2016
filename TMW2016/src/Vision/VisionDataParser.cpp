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
            } else {
            	Reset();
                std::cerr << "Parser State Error: Detected end frame while not parsing, discarding previous contents\n";
            }
        } else if (buffer[i] == NO_DATA) {
            if (parsing) {
                std::cerr << "Parser State Error: Detected no data while reading a frame, discarding previous contents\n";
                Reset();
            } else {
                continue;
            }
        } else {
            if (parsing) {
                data_frame << buffer[i];
            } else {
                std::cerr << "Parser State Error: Unexpected data character while not reading a frame, ignoring\n";
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

	if (data.size() == 4) {
		{
			std::cout << "Vision Frame: ";
			std::for_each(data.begin(), data.end(), [](float &f) { std::cout << "[" << f << "]"; });
			std::cout << "\t\t(" << data_frame << ")\n";
		}
		SetVisionData(VisionData(data[0], data[1], data[2], data[3]));
	} else {
		std::cerr << "Expected 4 data items, but got " << data.size() << '\n';
	}
}

void VisionDataParser::Reset() {
    data_frame.clear();             // clear state flags
    data_frame.str(std::string());  // clear internal buffer
    parsing = false;                // reset parsing flag
    SetVisionData(VisionData());    // reset vision data
}

