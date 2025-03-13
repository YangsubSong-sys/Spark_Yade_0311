// File.cpp: implementation of the CFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BiOpTroCon.h"
#include "File.h"

#include "FileSysInfo.h"
//extern CSysInfoPieceRange  SysInfoPieceRange;
//extern 
//extern CSysInfoPieceRange  SysInfoPieceRange;

 


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFileS::CFileS()
{
//SysInfoPieceRange.m_nPieceCount=0;
}

CFileS::~CFileS()
{

}


//////////////////////////////////////////////////////////////////////////
/*
사용법:
  szDir : 지우려는 폴더명
 recur : 0 =  이면 자기(szDir 에서 지정한)는 지우지 않습니다.
             1 =  자기까지 완전 삭제.. -_-;
 
코드 입니다
 */
int DeleteAllFiles(LPCSTR szDir, int recur) 
{ 
 HANDLE hSrch;
  WIN32_FIND_DATA wfd;
  int res = 1;
 
 TCHAR DelPath[MAX_PATH];
  TCHAR FullPath[MAX_PATH];
  TCHAR TempPath[MAX_PATH];
  
  lstrcpy(DelPath, szDir);
  lstrcpy(TempPath, szDir);
  if (lstrcmp(DelPath+lstrlen(DelPath)-4,_T("\\*.*")) != 0)
   lstrcat(DelPath, _T("\\*.*"));
 
 hSrch = FindFirstFile(DelPath, &wfd);
  if (hSrch == INVALID_HANDLE_VALUE)
  { 
  if (recur > 0) 
   RemoveDirectory(TempPath);
 
  return -1;
  }
  while(res)
  {
   wsprintf(FullPath, _T("%s\\%s"), TempPath, wfd.cFileName);
 
  if (wfd.dwFileAttributes & FILE_ATTRIBUTE_READONLY)
   {
    SetFileAttributes(FullPath, FILE_ATTRIBUTE_NORMAL);
   }
 
  if(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
   {
    if(lstrcmp(wfd.cFileName, _T(".")) && lstrcmp(wfd.cFileName, _T("..")))
    {
     recur++;
     DeleteAllFiles(FullPath, recur);
     recur--;
    }
   }
   else
   {
    DeleteFile(FullPath);
   }
   res = FindNextFile(hSrch, &wfd);
  }
  FindClose(hSrch);
  
  if (recur > 0) 
  RemoveDirectory(TempPath);
 
 return 0;
 }


////////////////////////////////////////////////////////////////////////////////////////	
						
						
/*

  	#include <windows.h>
 
#include <tlhelp32.h>
 
#include <iostream>	
 
#include <string>
 
#include "psapi.h"
 



DWORD GetProcessByFileName(char* name){
 
    DWORD process_id_array[1024];
 
    DWORD bytes_returned;
 
    DWORD num_processes;
 
    HANDLE hProcess;
 
    char image_name[256];
 
    char buffer[256];
 
	int i;
 
    DWORD exitcode;
 
    EnumProcesses(process_id_array, 256*sizeof(DWORD), &bytes_returned);
 
    num_processes = (bytes_returned/sizeof(DWORD));
 
    for (i = 0; i < num_processes; i++) {
 
        hProcess=OpenProcess(PROCESS_ALL_ACCESS,TRUE,process_id_array[i]);
 
        if(GetModuleBaseName(hProcess,0,image_name,256)){
 
            if(!stricmp(image_name,name)){
 
                CloseHandle(hProcess);
 
                return process_id_array[i];
 
            }
 
        }
 
        CloseHandle(hProcess);
 
    }
 
    return 0;
 
}
 
void __cdecl main(int argc, char *argv[])
 
{
 
	DWORD dwPID;
 
	dwPID = GetProcessByFileName("calc.exe");
 
	printf("%lu", (unsigned long)dwPID);
 
	return;
 
}
*/
