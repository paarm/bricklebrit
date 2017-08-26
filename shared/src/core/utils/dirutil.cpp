#include "dirutil.h"

string DirUtil::concatPath(const string &rPart1, const string &rPart2) {
	string path=rPart1;
#ifdef WINDOWS
	if(!path.empty() && path[path.length()-1]!='\\') {
		path+='\\';
	}
#else
	if(!path.empty() && path[path.length()-1]!='/') {
		path+='/';
	}
#endif
	path+=rPart2;
	return path;
}
