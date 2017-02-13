#include "ToolFunction.h"


void main()
{
	char chFileName[MAX_PATH] = ".\\222.xml";
	long iFileSize = GetFile_size(chFileName);

	char* pFileBuffer = new char[iFileSize];

	FILE* xmlFile = fopen(chFileName, "rb");
	size_t iReadSize = 0;
	if (xmlFile)
	{
		iReadSize = fread(pFileBuffer, iFileSize, 1, xmlFile);
		fclose(xmlFile);
		xmlFile = NULL;
	}

	//TiXmlElement pTempEle = SelectElementByName(".\\222.xml","PlateName" , 1);
	//printf("find the Name : %s, Text = %s\n", pTempEle.Value(), pTempEle.GetText());

	//pTempEle = SelectElementByName(pFileBuffer,"PlateName" , 2);
	bool bFind = false;
	TiXmlElement pTempEle = ReadXmlByStack( ".\\222.xml", "PlateName",1, bFind);
	if (bFind)
	{
		printf("find the Name : %s, Text = %s\n", pTempEle.Value(), pTempEle.GetText());
	}	


	if (pFileBuffer)
	{
		delete[] pFileBuffer;
		pFileBuffer = NULL;
	}
}

