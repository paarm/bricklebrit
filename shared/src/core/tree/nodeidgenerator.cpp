#include "nodeidgenerator.h"
#include <algorithm>

NodeIdGenerator::NodeIdGenerator() {
}

void NodeIdGenerator::resetNumber() {
	mLatestNumber=0;
}

void NodeIdGenerator::updateLatestNumberIfHigher(int rNumber) {
	mLatestNumber=std::max(mLatestNumber, rNumber);
}

int NodeIdGenerator::getNextNumber() {
	mLatestNumber++;
	return mLatestNumber;
}

