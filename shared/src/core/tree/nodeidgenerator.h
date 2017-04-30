#pragma once

class NodeIdGenerator
{
private:
	int			mLatestNumber=0;
	NodeIdGenerator();

public:
	static NodeIdGenerator& getInstance() {
		static NodeIdGenerator rNodeIdGenerator;
		return rNodeIdGenerator;
	}
	void resetNumber();
	void updateLatestNumberIfHigher(int rNumber);
	int getNextNumber();
};

