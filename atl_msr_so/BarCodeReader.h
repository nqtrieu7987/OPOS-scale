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


#if !defined(AFX_BARCODEREADER_H__D9BDE233_E3D1_4169_A9AC_56D71D230F3B__INCLUDED_)
#define AFX_BARCODEREADER_H__D9BDE233_E3D1_4169_A9AC_56D71D230F3B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SerialPort.h"
#include <atlstr.h>

class CBarCodeReader : protected CSerialPort  
{
public:
	
	CBarCodeReader();
	virtual ~CBarCodeReader();

	BOOL Open(LPCTSTR PortName);
  virtual void Close();
  virtual BOOL IsOpen();
	DWORD Read(CString &Value,CString &data);
private:
	CString m_Data;
};

#endif // !defined(AFX_BARCODEREADER_H__D9BDE233_E3D1_4169_A9AC_56D71D230F3B__INCLUDED_)
