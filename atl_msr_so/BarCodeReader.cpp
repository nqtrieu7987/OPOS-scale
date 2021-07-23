//
//
//  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
//  KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
//  IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
//  PURPOSE. IT CAN BE DISTRIBUTED FREE OF CHARGE AS LONG AS THIS HEADER 
//  REMAINS UNCHANGED.
//
//  Email:  yetiicb@hotmail.com
//
//  Copyright (C) 2002-2003 Idael Cardoso. 
//


#include "stdafx.h" 
#include "BarCodeReader.h"
#include <atlstr.h>
#include "LogFile.h"
#include <time.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CBarCodeReader::CBarCodeReader()
  :CSerialPort()
  {

  }

CBarCodeReader::~CBarCodeReader()
  {

  }

BOOL CBarCodeReader::Open(LPCTSTR PortName)
  {
    return CSerialPort::Open(PortName, 9600, 8, NOPARITY, ONESTOPBIT, GENERIC_READ);
  }

BOOL CBarCodeReader::IsOpen()
  {
    return CSerialPort::IsOpen();
  }
void trimwhitespace(CString &cs){
cs.Remove(' ');
cs.Remove('\t');
cs.Remove('\n');
cs.Remove('\f');
cs.Remove('\r');
}
static CLogFile myLogFile;
DWORD CBarCodeReader::Read(CString &Value,CString &data)
  {
	 
    const TCHAR CRLF[] = {13, 10, 0};
  myLogFile.OpenLogFileIfClosed (L"C:\\Users\\Public\\Downloads\\VirtMsrLog1.log");
	  wchar_t  xBuff[5120];
    char Buffer[2048],Buffer1[2048],Buffer2[2048];
	CString m_Data=_T("");
	 time_t curr_time;
  curr_time = time(NULL);
  //sử dụng hàm ctime để chuyển đổi sang dạng localtime rồi hiển thị dưới dạng ký tự
  char *tm = ctime(&curr_time);

	int i=0;
	while (i<20)
	{
	  DWORD r = CSerialPort::Read(Buffer, sizeof(Buffer) -1);
	 if (r > 0)
      {
        Buffer[r] = 0;
        m_Data += (LPCSTR)Buffer;
      }
	 i++;
	}
 	 
	wsprintf (xBuff, L"time done: %s   \r\n", tm);
	myLogFile.PrintLog (xBuff);
	 int index = 0;
     CString field,lfield=_T(""); 
	 int pos = m_Data.Find((LPCTSTR)CRLF); 
	 CString dd(m_Data);
	 data= dd; 
     while (pos > 0)
      {	
		
		   field= m_Data.Left(pos); 
		   if(field.Find(_T("kg"))>0 && field.Find(_T("/"))<0)
			{
			  lfield=field;  
			}
			m_Data.Delete(0,pos+2);
			pos = m_Data.Find((LPCTSTR)CRLF);   
         
      }

	  if(lfield!=_T(""))
	  { 
		if(lfield.Find(_T(":"))>0)
		{
		  pos=lfield.Find(_T(":"));
          lfield.Delete(0,pos+1);
		}
       
	 	pos=lfield.Find(_T("kg"));
	    lfield.Delete(pos,pos+2); 
		trimwhitespace(lfield);
	 	Value=lfield; 
	    return 1;
	  }
	  else
		  Value="0";
		  return 0;
  }

void CBarCodeReader::Close()
  {
    CSerialPort::Close();
  }
