#include "nodeidgenerator.h"
#include <algorithm>

NodeIdGenerator::NodeIdGenerator() {
}

void NodeIdGenerator::resetNumber(int number) {
	mNextNumber=number;
}

//void NodeIdGenerator::updateLatestNumberIfHigher(int rNumber) {
//	mNextNumber=std::max(mNextNumber, rNumber);
//}

int NodeIdGenerator::allocateNextNumber() {
	mNextNumber++;
	return mNextNumber-1;
}

int NodeIdGenerator::getNextFreeNumber() {
	return mNextNumber;
}

