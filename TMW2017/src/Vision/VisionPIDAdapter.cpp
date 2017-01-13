/*
 * VisionPIDAdapter.cpp
 */

#include "VisionPIDAdapter.h"


VisionPIDAdapter::VisionPIDAdapter() {
}

VisionPIDAdapter::~VisionPIDAdapter() {
}

void VisionPIDAdapter::Update(const float value) {
	inputValue = value;
}

double VisionPIDAdapter::PIDGet() {
	return inputValue;
}

void VisionPIDAdapter::PIDWrite(float output) {
	outputValue = output;
}

double VisionPIDAdapter::GetOutputValue() {
	return outputValue;
}
