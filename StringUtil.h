#pragma once
#include "util.h"
#include <string>
#include <vector>

/**
* This class provides some useful string utilities, like 
* splitting string with a delimiter and returning the split strings.
*/
class StringUtil
{
public:
	/**
	* Splits a string on the basis of the provided delimiter. The result
	* vector will be cleared before the resulting strings are added to the 
	* the result vector.
	* @param toSplit the string to split.
	* @param delim the delimiter to split the string with.
	* @param resultRef  the vector with the result.
	* @param eager if set to true , the algorithm will match as many delimiters as
	* needed, if set to false two delimiters in sequence will produce one empty string result.
	*/
	static void Split(tstring& toSplit, 
		tstring delim, 
		vector<tstring>& resultRef,bool eager=true);

	/**
	* Determines the directory of a filename. If the filename is in a root directory 
	* for example ( "huis.obj" ) the string "./" will be returned. This methods works
	* on files with '/' or '\' as path separators. A mixed case will probably fail.
	* @param fileName the filename to analyze.
	* @return the directory.
	*/
	static tstring GetParentDirectory(tstring& fileName);
	/**
	* Gets the last part of a directory, this can be a file or a
	* directory. 
	*/
	static tstring GetLastDirectoryElement(tstring& filename);
	/**
	* Creates the directories recursively.
	* @param path the directory path to create.
	* @return true if creating the path was succesful , false otherwise.
	*/
	static bool MakeDirectories(tstring& path);

	/**
	* Strips the tstring of whitespace
	* @param toTrim the tstring to trim
	* @return the result of the trim operation.
	*/
	static tstring TrimWhiteSpace(tstring toTrim);

	/**
	* Splits quoted strings , for example "test" "second test".
	* @param toSplit the string to split
	* @param resultRef the vector with the results
	*/
	static void SplitQuotedStrings(tstring& toSplit, vector<tstring>& resultRef);
	
};
