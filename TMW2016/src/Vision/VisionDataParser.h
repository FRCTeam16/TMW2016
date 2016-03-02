/*
 * VisionDataParser.h
 */

#ifndef SRC_VISION_VISIONDATAPARSER_H_
#define SRC_VISION_VISIONDATAPARSER_H_

#include "VisionData.h"
#include <mutex>
#include <sstream>

/**
 * Parses a stream of characters extracting data frames of the form:
 * 	<a,b,c,d> where a b c d are float values.
 *
 * 	Values of 'x' are interpreted as no-data and ignored.
 *
 *  Public method uses mutex guards to ensure atomic access
 *  to visionData structure, since writer and reader will be operating
 *  on two different threads.
 */
class VisionDataParser {
public:
    VisionDataParser();
    virtual ~VisionDataParser();
    void AddRawData(const char* buffer, const int nchars);
    VisionData GetVisionData();
private:
    void SetVisionData(const VisionData vd);
    void ProcessDataFrame();
    void Reset();

    const char START_FRAME = '<';
    const char END_FRAME = '>';
    const char NO_DATA = 'x';

    std::mutex data_mutex;
    VisionData visionData;
    std::stringstream data_frame;
    bool parsing = false;
};

#endif /* SRC_VISION_VISIONDATAPARSER_H_ */
