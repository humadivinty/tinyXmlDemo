#include "ToolFunction.h"
#include <stack>

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

//读取xml的信息
bool ReadXmlFile(string& szFileName)
{
	try
	{
		//CString appPath =	GetAppPath();
		//string seperator = "\\";
		//string fullPath = appPath.GetBuffer() + seperator + szFileName;
		//string fullPath = (char*)(appPath.GetBuffer(0));
		string fullPath("F:\\test.xml");
		//fullPath +=seperator;
		//fullPath +=  szFileName;
		//创建一个XML的文档对象
		TiXmlDocument* myDocument = new TiXmlDocument(fullPath.c_str());
		myDocument->LoadFile();
		//获取根元素
		TiXmlElement* rootElement = myDocument->RootElement();
		//获取第一个节点
		TiXmlElement* FirstPerson = rootElement->FirstChildElement();
		//获取第一个节点的name节点和age节点，还有ID属性
		TiXmlElement* nameElement = FirstPerson->FirstChildElement();
		TiXmlElement* ageElement = nameElement->NextSiblingElement();
		TiXmlAttribute* IDAttribute = FirstPerson->FirstAttribute();

		//输出第一个Person的name内容
		cout<<nameElement->FirstChild()->Value()<<endl;
		cout<<ageElement->FirstChild()->Value()<<endl;
		cout<<FirstPerson->FirstAttribute()->Value()<<endl;
	}
	catch (...)
	{

	}
	return true;
}

long GetFile_size( char* filename )
{
	FILE* fp = fopen( filename, "r" );
	if( !fp ) { return -1; }
	fseek( fp, 0L, SEEK_END );
	long isize = ftell( fp );
	fclose( fp );

	return isize;
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
			return TiXmlElement("");
		}
	}
	else if (iXMLType == 2)
	{
		if (!cXmlDoc.Parse(InputInfo))
		{
			printf("parse XML failed \n");
			return TiXmlElement("");
		}
	}

	pRootElement = cXmlDoc.RootElement();
	if (NULL == pRootElement)
	{
		printf("no have root Element\n");
		return TiXmlElement("");
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

TiXmlElement ReadXmlByStack(char* InputInfo, char* pName,int iXMLType, bool& bFind)
{
	TiXmlDocument cXmlDoc;
	TiXmlElement* pRootElement = NULL;
	stack<TiXmlElement*> ElementStack;
	bFind = false;

	if (iXMLType == 1)
	{
		if (!cXmlDoc.LoadFile(InputInfo))
		{
			printf("parse XML file failed \n");
			return TiXmlElement("");
		}
	}
	else if (iXMLType == 2)
	{
		if (!cXmlDoc.Parse(InputInfo))
		{
			printf("parse XML failed \n");
			return TiXmlElement("");
		}
	}

	pRootElement = cXmlDoc.RootElement();
	if (NULL == pRootElement)
	{
		printf("no have root Element\n");
		return TiXmlElement("");
	}
	else
	{
		ElementStack.push(pRootElement);
		TiXmlElement* pTempElement = NULL;		
		while(ElementStack.size() > 0)
		{
			pTempElement = ElementStack.top();
			printf("%s\n", pTempElement->Value());

			ElementStack.pop();

			if (0 == strcmp(pTempElement->Value(), pName) )
			{
				bFind = true;
				break;
			}
			
			TiXmlElement* pTempSibLing = pTempElement->FirstChildElement();
			do
			{
				if (pTempSibLing)
				{
					ElementStack.push(pTempSibLing);
					pTempSibLing = pTempSibLing->NextSiblingElement();
				}
			}while(pTempSibLing);
		}

		while(ElementStack.size() > 0)
		{
			ElementStack.pop();
		}

		if (bFind)
		{			
			return *pTempElement;
		}
		else
		{
			return TiXmlElement("");
		}		
	}
}
