//______________________________________________________________________________

// [] FileName: IniFile.cpp
// [] History :
//		1. 2007.09.25 : Created by Jung Youn Kap
//______________________________________________________________________________

#include "stdafx.h"
#include "IniFile.h"

//______________________________________________________________________________

CIniFile::CIniFile()
{
	Path = _T("");
}

//______________________________________________________________________________

CIniFile::CIniFile(CString file_path)
{
	Path = file_path;
}

//______________________________________________________________________________

CIniFile::~CIniFile()
{
}

//______________________________________________________________________________

int CIniFile::ReadInt(CString strSection, CString strKey, int iDefault)
{
	int iVal = GetPrivateProfileInt(strSection, strKey, iDefault, Path);
	return iVal;
}

//______________________________________________________________________________

int CIniFile::ReadInt(CString strSection, int no, int iDefault)
{
	CString strKey;
	strKey.Format("%d", no);
	int iVal = GetPrivateProfileInt(strSection, strKey, iDefault, Path);
	return iVal;
}

//______________________________________________________________________________

CString CIniFile::ReadString(CString strSection, CString strKey, CString strDefault)
{
	char buff[LINE_LENGTH+1];
	GetPrivateProfileString(strSection, strKey, strDefault, buff, LINE_LENGTH, Path);
	CString strVal = buff;
	return strVal;
}

//______________________________________________________________________________

CString CIniFile::ReadString(CString strSection, int no, CString strDefault)
{
	CString strKey;
	strKey.Format("%d", no);
	CString strVal = ReadString(strSection, strKey, strDefault);
	return strVal;
}

//______________________________________________________________________________

float CIniFile::ReadFloat(CString strSection, CString strKey, float fDefault)
{
	CString strDefault;
	strDefault.Format("%f", fDefault);
	CString strVal = ReadString(strSection, strKey, strDefault);
	float fVal;
	sscanf(strVal, "%f", &fVal);
	return fVal;
}

//______________________________________________________________________________

float CIniFile::ReadFloat(CString strSection, int no, float fDefault)
{
	CString strKey;
	strKey.Format("%d", no);
	CString strDefault;
	strDefault.Format("%f", fDefault);
	CString strVal = ReadString(strSection, strKey, strDefault);
	float fVal;
	sscanf(strVal, "%f", &fVal);
	return fVal;
}

//______________________________________________________________________________

double CIniFile::ReadDouble(CString strSection, CString strKey, double dDefault)
{
	CString strDefault;
	strDefault.Format("%lf", dDefault);
	CString strVal = ReadString(strSection, strKey, strDefault);
	double dVal;
	sscanf(strVal, "%lf", &dVal);
	return dVal;
}

//______________________________________________________________________________

double CIniFile::ReadDouble(CString strSection, int no, double dDefault)
{
	CString strKey;
	strKey.Format("%d", no);
	CString strDefault;
	strDefault.Format("%lf", dDefault);
	CString strVal = ReadString(strSection, strKey, strDefault);
	double dVal;
	sscanf(strVal, "%lf", &dVal);
	return dVal;
}

//______________________________________________________________________________

int CIniFile::ReadHex(CString strSection, CString strKey, int iDefault)
{
	CString strDefault;
	strDefault.Format("%X", iDefault);
	CString strVal = ReadString(strSection, strKey, strDefault);
	int iVal;
	sscanf(strVal, "%X", &iVal);
	return iVal;
}

//______________________________________________________________________________

int CIniFile::ReadHex(CString strSection, int no, int iDefault)
{
	CString strKey;
	strKey.Format("%d", no);
	CString strDefault;
	strDefault.Format("%X", iDefault);
	CString strVal = ReadString(strSection, strKey, strDefault);
	int iVal;
	sscanf(strVal, "%X", &iVal);
	return iVal;
}

//______________________________________________________________________________

void CIniFile::WriteInt(CString strSection, CString strKey, int iVal)
{
	CString strVal;
	strVal.Format("%d", iVal);
	WriteString(strSection, strKey, strVal);
}

//______________________________________________________________________________

void CIniFile::WriteInt(CString strSection, int no, int iVal)
{
	CString strKey;
	strKey.Format("%d", no);
	CString strVal;
	strVal.Format("%d", iVal);
	WriteString(strSection, strKey, strVal);
}

//______________________________________________________________________________

void CIniFile::WriteString(CString strSection, CString strKey, CString strVal)
{
	WritePrivateProfileString(strSection, strKey, strVal, Path);
}

//______________________________________________________________________________

void CIniFile::WriteString(CString strSection, int no, CString strVal)
{
	CString strKey;
	strKey.Format("%d", no);
	WriteString(strSection, strKey, strVal);
}

//______________________________________________________________________________

void CIniFile::WriteFloat(CString strSection, CString strKey, float fVal)
{
	CString strVal;
	strVal.Format("%f", fVal);
	WriteString(strSection, strKey, strVal);
}

//______________________________________________________________________________

void CIniFile::WriteFloat(CString strSection, int no, float fVal)
{
	CString strKey;
	strKey.Format("%d", no);
	CString strVal;
	strVal.Format("%f", fVal);
	WriteString(strSection, strKey, strVal);
}

//______________________________________________________________________________

void CIniFile::WriteDouble(CString strSection, CString strKey, double dVal)
{
	CString strVal;
	strVal.Format("%lf", dVal);
	WriteString(strSection, strKey, strVal);
}

//______________________________________________________________________________

void CIniFile::WriteDouble(CString strSection, int no, double dVal)
{
	CString strKey;
	strKey.Format("%d", no);
	CString strVal;
	strVal.Format("%lf", dVal);
	WriteString(strSection, strKey, strVal);
}

//______________________________________________________________________________

void CIniFile::WriteHex(CString strSection, CString strKey, int iVal)
{
	CString strVal;
	strVal.Format("%X", iVal);
	WriteString(strSection, strKey, strVal);
}

//______________________________________________________________________________

void CIniFile::WriteHex(CString strSection, int no, int iVal)
{
	CString strKey;
	strKey.Format("%d", no);
	CString strVal;
	strVal.Format("%X", iVal);
	WriteString(strSection, strKey, strVal);
}

//______________________________________________________________________________


