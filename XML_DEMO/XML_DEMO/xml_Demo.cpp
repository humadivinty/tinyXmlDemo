#include <afx.h>
#include <string>
#include <iostream>
#include "tinyxml/tinyxml.h"
#include "tinyxml/tinystr.h"
using namespace std;


CString GetAppPath()
{
	TCHAR modulePath[MAX_PATH];
	GetModuleFileName(NULL, modulePath, MAX_PATH);
	CString strModulePath(modulePath);
	 strModulePath = strModulePath.Left(strModulePath.ReverseFind(_T('\\')));
	return strModulePath;
}

bool CreateXmlFile(string& szFileName)
{
	try
	{
		TiXmlDocument *myDocument = new TiXmlDocument();
		TiXmlDeclaration* pDeclaration = new TiXmlDeclaration("1.0", "GB2312", "");
		myDocument->LinkEndChild(pDeclaration);
		TiXmlElement* rootElement = new TiXmlElement("Persons");
		myDocument->LinkEndChild(rootElement);
		TiXmlElement* PersonElement = new TiXmlElement("Person");
		rootElement->LinkEndChild(PersonElement);
		//设置Person元素的属性
		PersonElement->SetAttribute("ID", "1");
		//创建name元素、age元素，并连接
		TiXmlElement* NameElement = new TiXmlElement("name");
		TiXmlElement* AgeElement = new TiXmlElement("age");
		PersonElement->LinkEndChild(NameElement);
		PersonElement->LinkEndChild(AgeElement);

		TiXmlText* nameContent = new TiXmlText("周星星");
		TiXmlText* ageContent = new TiXmlText("22");
		NameElement->LinkEndChild(nameContent);
		AgeElement->LinkEndChild(ageContent);
		
		//保存XML文件
		char namePATH[MAX_PATH]= {0};
		//GetModuleFileName(NULL,(LPWSTR)namePATH,MAX_PATH-1);
		//string seperator;
		//seperator.append("\\");
		string fullPath = szFileName;
		//fullPath  namePATH;
		//fullPath +=seperator;
		//fullPath +=  szFileName;
		bool bret;
		bret = myDocument->SaveFile(fullPath.c_str());
		cout<<bret<<endl;
	}
	catch(...)
	{
		return false;
	}
	return true;
};


long GetFile_size( char* filename )
{
	FILE* fp = fopen( filename, "r" );
	if( !fp ) { return -1; }
	fseek( fp, 0L, SEEK_END );
	long isize = ftell( fp );
	fclose( fp );

	return isize;
}

/*
*函数名: SelectElementByName
*说明：根据传入的元素名返回相应的节点元素
参数:
* InputInfo : 输入的XML路径 或 XML 二进制文件内容
*pElementName: 需要查找的元素名
*iInputType: 说明第一个参数 InputInfo 的类型，当值为1时传入为路径 ，值为2是传入的为二进制内容
*/
TiXmlElement SelectElementByName(char* InputInfo, char* pElementName, int iInputType);
TiXmlElement* ReadElememt(TiXmlElement* InputElement,  char* pName);

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

	//TiXmlElement pTempEle = SelectElementByName(pFileBuffer,"PlateName" , 2);
	TiXmlElement pTempEle = SelectElementByName(".\\222.xml","PlateName" , 1);
	printf("find the Name : %s, Text = %s\n", pTempEle.Value(), pTempEle.GetText());

	if (pFileBuffer)
	{
		delete[] pFileBuffer;
		pFileBuffer = NULL;
	}
}

TiXmlElement SelectElementByName(char* InputInfo, char* pName, int iXMLType)
{
	//注：XMLTYPE 为1时，InputInfo为XML路径，当为2时,InputInfo为二进制文件内容
	TiXmlDocument cXmlDoc;
	TiXmlElement* pRootElement = NULL;
	if (iXMLType == 1)
	{
		if (!cXmlDoc.LoadFile(InputInfo))
		{
			printf("parse XML file failed \n");
			return NULL;
		}
	}
	else if (iXMLType == 2)
	{
		if (!cXmlDoc.Parse(InputInfo))
		{
			printf("parse XML failed \n");
			return NULL;
		}
	}

	pRootElement = cXmlDoc.RootElement();
	if (NULL == pRootElement)
	{
		printf("no have root Element\n");
		return NULL;
	}
	else
	{
		TiXmlElement* pTempElement = NULL;
		pTempElement = ReadElememt(pRootElement, pName);
		if (pTempElement)
		{
			printf("find the Name : %s, Text = %s\n", pTempElement->Value(), pTempElement->GetText());
		}
		return *pTempElement;
	}	
}

TiXmlElement* ReadElememt(TiXmlElement* InputElement, char* pName)
{
	TiXmlElement* ptemp = NULL;
	if (InputElement && 0 == strcmp(pName,InputElement->Value()))
	{
		printf("Find the element :%s \n", InputElement->Value());
		ptemp = InputElement;
		return ptemp;
	}
	else
	{
		printf("%s \n", InputElement->Value());
	}

	TiXmlElement* tmpElement = InputElement;
	if (tmpElement->FirstChildElement())
	{
		ptemp = ReadElememt(tmpElement->FirstChildElement(), pName);
	}
	if (!ptemp)
	{
		tmpElement = tmpElement->NextSiblingElement();
		if (tmpElement)
		{
			ptemp = ReadElememt(tmpElement ,  pName);
		}
	}
	return ptemp;
}