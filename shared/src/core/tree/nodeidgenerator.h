#pragma once

class NodeIdGenerator
{
private:
	int			mNextNumber=0;
	NodeIdGenerator();

public:
	static NodeIdGenerator& getInstance() {
		static NodeIdGenerator rNodeIdGenerator;
		return rNodeIdGenerator;
	}
	void resetNumber(int number=0);
	//void updateLatestNumberIfHigher(int rNumber);
	int allocateNextNumber();
	int getNextFreeNumber();
};

