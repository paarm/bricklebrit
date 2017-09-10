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

string DirUtil::subPart1FromPart2(const string &rPart1, const string &rPart2) {
	string path="";
	if (rPart1.length()<rPart2.length()) {
        path=rPart2.substr(rPart1.length()+1, rPart2.length()-rPart1.length());
	}
#ifdef WINDOWS
	if(!path.empty() && path[0]=='\\') {
		path=path.substr(1, path.length()-1);
	}
#else
	if(!path.empty() && path[0]=='/') {
		path=path.substr(1, path.length()-1);
	}
#endif
	return path;
}

string DirUtil::trimFileName(const string &rPathWithFile) {
	string path=rPathWithFile;
#ifdef WINDOWS
	if(!path.empty() && path[path.length()-1]!='\\' && path[path.length()-1]!='/') {
		for (int i=path.length()-1;i>0;i--) {
			if (path[i]=='\\' || path[i]=='/') {
				path=path.substr(0,i);
				break;
			}
		}
	}
#else
	if(!path.empty() && path[path.length()-1]!='/') {
		for (int i=path.length()-1;i>0;i--) {
			if (path[i]=='/') {
				path=path.substr(0,i);
				break;
			}
		}
	}
#endif
	return path;
}
