#pragma once
#include <string>
using namespace std;
class DirUtil
{
private:
	DirUtil() {}
public:
	static string concatPath(const string &rPart1, const string &rPart2);
	static string subPart1FromPart2(const string &rPart1, const string &rPart2);
	static string trimFileName(const string &rPathWithFile);
};
