#pragma once
#include<Windows.h>
#include<fstream>
#include<vector>
#include<string>
#include<sstream>
using namespace std;

typedef vector<wstring> vecLinesContainer;

class SimpleFile
{
public:
	SimpleFile();
	
	BOOLEAN ReadFile(WCHAR* pathName);
	BOOLEAN WriteFile(WCHAR* pathName, WCHAR* fileLines);

	vecLinesContainer getFileLines(WCHAR* pathName);

private:
	vecLinesContainer FileContent;
};
