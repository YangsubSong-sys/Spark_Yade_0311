//______________________________________________________________________________
//
// File Name:	IniFile.h
// History	:
//		1. 2007.09.18: Created by Jung Youn Kap
//		2.
//______________________________________________________________________________

#ifndef _INI_FILE_H
#define _INI_FILE_H

//______________________________________________________________________________

const int LINE_LENGTH = 2000;

//______________________________________________________________________________

class  CIniFile
{
public:
	CString Path;

public:
	CIniFile();
	CIniFile(CString file_path);
	~CIniFile();
	void SetPath(CString file_path) { Path = file_path; }
	CString GetPath() { return Path; }
	//Reading
	int ReadInt(CString strSection, CString strKey, int iDefault=0);
	int ReadInt(CString strSection, int no, int iDefault=0);
	CString ReadString(CString strSection, CString strKey, CString strDefault="");
	CString ReadString(CString strSection, int no, CString strDefault="");
	float ReadFloat(CString strSection, CString strKey, float fDefault=0.0f);
	float ReadFloat(CString strSection, int no, float fDefault=0.0f);
	double ReadDouble(CString strSection, CString strKey, double dDefault=0.0);
	double ReadDouble(CString strSection, int no, double dDefault=0.0);
	int ReadHex(CString strSection, CString strKey, int iDefault=0);
	int ReadHex(CString strSection, int no, int iDefault=0);
	// Writing
	void WriteInt(CString strSection, CString strKey, int iVal);
	void WriteInt(CString strSection, int no, int iVal);
	void WriteString(CString strSection, CString strKey, CString strVal);
	void WriteString(CString strSection, int no, CString strVal);
	void WriteFloat(CString strSection, CString strKey, float fVal);
	void WriteFloat(CString strSection, int no, float fVal);
	void WriteDouble(CString strSection, CString strKey, double dVal);
	void WriteDouble(CString strSection, int no, double dVal);
	void WriteHex(CString strSection, CString strKey, int iVal);
	void WriteHex(CString strSection, int no, int iVal);
};

//______________________________________________________________________________

#endif
