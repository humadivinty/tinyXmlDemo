#include <afx.h>
#include <string>
#include <iostream>
#include "tinyxml/tinyxml.h"
#include "tinyxml/tinystr.h"
using namespace std;



CString GetAppPath();

bool CreateXmlFile(string& szFileName);

bool ReadXmlFile(string& szFileName);

long GetFile_size( char* filename );

TiXmlElement SelectElementByName(char* InputInfo, char* pName, int iXMLType);
//bool ReadElememt(TiXmlElement* InputElement);
TiXmlElement* ReadElememt(TiXmlElement* InputElement,  char* pName);

TiXmlElement ReadXmlByStack(char* InputInfo, char* pName,int iXMLType, bool& bFind);