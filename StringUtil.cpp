#include "StdAfx.h"
#include "util.h"
#include "StringUtil.h"
#include <io.h>
#include <vector>
#include <algorithm>

void StringUtil::Split(tstring& toSplit, tstring delim, vector<tstring>& resultRef, bool eager){
	size_t index=0,next;

	resultRef.clear();
	do{
		next  = toSplit.find_first_of(delim,index);
		if ( next != string::npos) 
			resultRef.push_back(toSplit.substr(index,next-index));
		else
			resultRef.push_back(toSplit.substr(index));
		if ( eager )
			index = toSplit.find_first_not_of(delim,next);
		else 
			index = next+1;
	}while( next != string::npos && index < toSplit.size());
}

tstring StringUtil::GetParentDirectory(tstring& fileName){
	{
		size_t slashIndex;
		slashIndex = fileName.find_last_of(_T("/"));
		if ( slashIndex != string::npos){
			return fileName.substr(0,slashIndex);
		}else{
			slashIndex = fileName.find_last_of(_T("\\"));
			if ( slashIndex != string::npos){
				return fileName.substr(0,slashIndex);
			}else{
				return _T("./");
			}
		}
	}
}

tstring StringUtil::GetLastDirectoryElement(tstring& filename){
	// watch out for mixed case , i.e. a directory with both
	// forward and backward slashes.
	size_t slashIndex1 = filename.find_last_of(_T("/"));
	size_t slashIndex2 = filename.find_last_of(_T("\\"));
	if ( slashIndex1 != string::npos || slashIndex2 != string::npos){
		if (slashIndex1 != string::npos && slashIndex2 != string::npos )
		{
			size_t slashIndex = slashIndex1;
			if ( slashIndex2 > slashIndex )
				slashIndex = slashIndex2;
			return filename.substr(slashIndex+1);
		}else if ( slashIndex1 != string::npos ){
			return filename.substr(slashIndex1+1);
		}else{
			return filename.substr(slashIndex2+1);
		}
	}else{
		return filename;
	}
}

tstring StringUtil::TrimWhiteSpace(tstring toTrim)
{
	return toTrim;
}

void StringUtil::SplitQuotedStrings(tstring &toSplit, vector<tstring>& resultRef)
{
	size_t index=0,first,second;
	resultRef.clear();

	do{
		first = toSplit.find_first_of(_T("\""),index);
		if ( first == tstring::npos )
			break;
		second = toSplit.find_first_of(_T("\""),first+1);
		if ( first != tstring::npos && second != tstring::npos)
			// do not include the quotes themselves
			resultRef.push_back(toSplit.substr(first+1, second-first-1));
		index = second+1;
	}while(second != tstring::npos);
}	

bool StringUtil::MakeDirectories(tstring& path)
{
	vector<tstring> dirs;
	while ( _taccess( path.c_str() ,0) != 0 ){
		// parent directory does not exist , create it
		dirs.push_back(path);
		path = GetParentDirectory(path);
	}
	//--- Reverse the vector
	reverse(dirs.begin(), dirs.end());
	for (unsigned int i =0; i < dirs.size(); ++i)
	{
		int error= 	_tmkdir(dirs[i].c_str());
		if (error != 0 )
			return false;
	}
	return true;
}