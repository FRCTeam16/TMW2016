/*
 * DataLoggable.h
 */

#ifndef SRC_UTIL_DATALOGGABLE_H_
#define SRC_UTIL_DATALOGGABLE_H_

#include <fstream>
#include <vector>

class DataLoggable
{
public:
	virtual ~DataLoggable() {}
	virtual std::vector<std::string> GetHeaders() = 0;
	virtual void Log(std::ofstream &outstream) = 0;
};

#endif /* SRC_UTIL_DATALOGGABLE_H_ */
