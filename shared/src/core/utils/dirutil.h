#pragma once
#include <string>
using namespace std;
class DirUtil
{
private:
	DirUtil() {}
public:
	static string concatPath(const string &rPart1, const string &rPart2);
};
