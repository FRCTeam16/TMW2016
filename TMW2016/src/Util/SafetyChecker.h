/*
 * SafetyChecker.h
 */

#ifndef SRC_UTIL_SAFETYCHECKER_H_
#define SRC_UTIL_SAFETYCHECKER_H_

class SafetyChecker {
public:
	SafetyChecker(bool simulateError_=false) : simulateError(simulateError_) {}
	virtual ~SafetyChecker() {}
	bool CheckLimits();
	void HaltPeriodic();
	bool IsFailed() const;
private:
	const bool simulateError;
	bool failed = false;
	int simulatedErrorCounter = 0;
};

#endif /* SRC_UTIL_SAFETYCHECKER_H_ */
