// VirtMsrSo.cpp : Implementation of CVirtMsrSo

#include "stdafx.h"
#include "VirtMsrSo.h"
#include "LogFile.h"
#include "atlstr.h"
#include "OposScal.h" 
#include <stdlib.h> 
// CVirtMsrSo

// Also need to create the Windows Registry entry for this Service Object.
// The name is the VersionIndependentProgID attribute in the .rgs file
// If you are on a 64bit machine:
//  [HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\OLEforRetail\ServiceOPOS\MSR\MyVirtualDevice]@="atl_msr_so.OPOS.VirtMsrSo"
// If you are on a 32bit machine:
//  [HKEY_LOCAL_MACHINE\SOFTWARE\OLEforRetail\ServiceOPOS\MSR\MyVirtualDevice]@="atl_msr_so.OPOS.VirtMsrSo"

static CLogFile myLogFile;
CString m_CommPort=_T("");
CString   token=_T("");
CString   opened=_T("0");
// --------   Helper methods follow

const wchar_t *CVirtMsrSo::m_SecureMag_rs232 = L"SECUREMAG_RS232";
const wchar_t *CVirtMsrSo::m_SecureMag_usbhid = L"SECUREMAG_USBHID";

// Service Oject calls a Common Object event request method. The
// methods SOData, SODirectIO, SOError, SOOutputComplete, and
// SOStatusUpdate are exposed.
void CVirtMsrSo::FireDataEvent(LONG lData)
{
	if (m_PIDX_Claimed && m_PIDX_DataEventEnabled) {
		VARIANT v; v.vt = VT_I4; v.lVal = lData; // 10;
		// turn this off so that only one event at a time is delivered
		m_PIDX_DataEventEnabled = 0;
		m_pDriver.Invoke1(L"SOData", &v);
	}
} 
   
DWORD GetPhysicalDriveSerialNumber(UINT nDriveNumber, CString& strSerialNumber)
{
    DWORD dwResult = NO_ERROR;
    strSerialNumber.Empty();

    // Format physical drive path (may be '\\.\PhysicalDrive0', '\\.\PhysicalDrive1' and so on).
    CString strDrivePath;
    strDrivePath.Format(_T("\\\\.\\PhysicalDrive%u"), nDriveNumber);

    // call CreateFile to get a handle to physical drive
    HANDLE hDevice = ::CreateFile(strDrivePath, 0, FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL, OPEN_EXISTING, 0, NULL);

    if(INVALID_HANDLE_VALUE == hDevice)
        return ::GetLastError();

    // set the input STORAGE_PROPERTY_QUERY data structure
    STORAGE_PROPERTY_QUERY storagePropertyQuery;
    ZeroMemory(&storagePropertyQuery, sizeof(STORAGE_PROPERTY_QUERY));
    storagePropertyQuery.PropertyId = StorageDeviceProperty;
    storagePropertyQuery.QueryType = PropertyStandardQuery;

    // get the necessary output buffer size
    STORAGE_DESCRIPTOR_HEADER storageDescriptorHeader = { 0 };
    DWORD dwBytesReturned = 0;
    if(!::DeviceIoControl(hDevice, IOCTL_STORAGE_QUERY_PROPERTY,
        &storagePropertyQuery, sizeof(STORAGE_PROPERTY_QUERY),
        &storageDescriptorHeader, sizeof(STORAGE_DESCRIPTOR_HEADER),
        &dwBytesReturned, NULL))
    {
        dwResult = ::GetLastError();
        ::CloseHandle(hDevice);
        return dwResult;
    }

    // allocate the necessary memory for the output buffer
    const DWORD dwOutBufferSize = storageDescriptorHeader.Size;
    BYTE* pOutBuffer = new BYTE[dwOutBufferSize];
    ZeroMemory(pOutBuffer, dwOutBufferSize);

    // get the storage device descriptor
    if (!::DeviceIoControl(hDevice, IOCTL_STORAGE_QUERY_PROPERTY,
        &storagePropertyQuery, sizeof(STORAGE_PROPERTY_QUERY),
        pOutBuffer, dwOutBufferSize,
        &dwBytesReturned, NULL))
    {
        dwResult = ::GetLastError();
        delete[]pOutBuffer;
        ::CloseHandle(hDevice);
        return dwResult;
    }

    // Now, the output buffer points to a STORAGE_DEVICE_DESCRIPTOR structure
    // followed by additional info like vendor ID, product ID, serial number, and so on.
    STORAGE_DEVICE_DESCRIPTOR* pDeviceDescriptor = (STORAGE_DEVICE_DESCRIPTOR*)pOutBuffer;
    const DWORD dwSerialNumberOffset = pDeviceDescriptor->SerialNumberOffset;
    if (dwSerialNumberOffset != 0)
    {
        // finally, get the serial number
        strSerialNumber = CString(pOutBuffer + dwSerialNumberOffset);
    }

    // perform cleanup and return
    delete[]pOutBuffer;
    ::CloseHandle(hDevice);
    return dwResult;
}
void CVirtMsrSo::FireStatusUpdateEvent(LONG lStatus)
{
	if (m_PIDX_Claimed && m_PIDX_DataEventEnabled) {
		VARIANT v; v.vt = VT_I4; v.lVal = lStatus;

		m_pDriver.Invoke1(L"SOStatusUpdate", &v);
	}
}

void CVirtMsrSo::FireErrorEvent(LONG lResultCode, LONG lResultCodeExtended, LONG lErrorLocus, LONG *plErrorResponse)
{
	if (m_PIDX_Claimed && m_PIDX_DataEventEnabled) {
		VARIANT vRet;
		VARIANT v[3];
		v[0].vt = VT_I4; v[0].lVal = lResultCode;
		v[1].vt = VT_I4; v[1].lVal = lResultCodeExtended;
		v[2].vt = VT_I4; v[2].lVal = lErrorLocus;

		m_pDriver.InvokeN(L"SOStatusUpdate", v, 3, &vRet);
		if (plErrorResponse && vRet.vt == VT_I4) {
			*plErrorResponse = vRet.lVal;
		}
	}
}
// -------- Standard OPOS Service Object methods follow

// The OPenService() method is called by the Open() method of the OPOS Common Control object.
//
// WARNING: When using the Class View of Visual Studio to add this method to the Service
//          Object interface you may run into an error from Visual Studio.  It appears that
//          OpenService() is also part of the Windows API and the Visual Studio Intellisense
//          may emit errors when attempting to add this method.
//          You may need to close Visual Studio and delete the .ncb Intellisense data file
//          in order to recover.
//          You may also need to create this method with a different name such as CheckService
//          and then once created, do a Find in order to find all usages of CheckService and
//          change them to OpenService.
#include "md5.h"
#include "HardDriveSerialNumer.h"
#include <iostream>
inline CString toCString(std::string const& str)
{
    return CString(str.c_str()); 
}

STDMETHODIMP CVirtMsrSo::OpenService(BSTR DeviceClass, BSTR DeviceName, IDispatch* pDispatch, LONG* pRc)
{
	m_pDriver = pDispatch;
	opened=_T("0");
	int out = 0,i=10;
	MasterHardDiskSerial a;
	char SerialNumber[1024] = {""}; 
	memset(&SerialNumber,0,sizeof(SerialNumber));
	out = a.GetSerialNo(SerialNumber); 
   
	std::string s(SerialNumber);
	
	myLogFile.OpenLogFileIfClosed (L"C:\\Users\\Public\\Downloads\\VirtMsrLog.log");
	wchar_t  xBuff[128];
	wsprintf (xBuff, L"OpenService called: %s %s  \r\n", DeviceClass, DeviceName);
	myLogFile.PrintLog (xBuff);

	m_RegistryData.SetTheRegistryKey (DeviceClass, DeviceName);
	m_RegistryData.ReadRegistry ();
	  m_CommPort=_T("");
	  token==_T("");
	for (int iLoop = 0; iLoop < m_RegistryData.m_RegistryData.GetSize (); iLoop++) {
		std::wstring j1 = m_RegistryData.m_RegistryData.GetKeyAt(iLoop);
		std::wstring j2 = m_RegistryData.m_RegistryData.GetValueAt(iLoop); 
		if(j1==L"Port")
			m_CommPort=CString(j2.c_str());
		if(j1==L"token")
			token=CString(j2.c_str());
	}
	CString md=toCString(md5(s+"123123"));

	   
	 myLogFile.OpenLogFileIfClosed (L"C:\\Users\\Public\\Downloads\\VirtMsrLog.log");
	wchar_t  xBuff1[128]; 
	wsprintf (xBuff1, L"Token: %s uuu  \r\n", token.Trim());
	myLogFile.PrintLog (xBuff1);



	if(token.Trim().Compare(md.Trim())!=0)
	{
	 *pRc = OPOS_E_NOSERVICE;
	  return S_OK; 
	}

	m_CommPort += _T(":"); 
	m_PIDX_ServiceObjectVersion = lVersionNumber;
	m_PIDX_Claimed = 0;
	m_PIDX_DataEventEnabled = 0;
	m_PIDX_DeviceEnabled = 0;
	m_PIDX_FreezeEvents = 0;
	m_PIDX_OutputID = 0;
	m_PIDX_ResultCode = 0;
	m_PIDX_ResultCodeExtended = 0;
	m_PIDX_State = 0;
	m_PIDX_AutoDisable = 0;
	m_PIDX_BinaryConversion = 0;
	m_PIDX_DataCount = 0;
	m_PIDX_PowerNotify = 0;
	m_PIDX_PowerState = 0;
  
	

if ( m_BarCodeReader.IsOpen() )
    {
      //m_BarCodeReader.Close(); 
	  *pRc = OPOS_SUCCESS;
	  return S_OK; 
    }
  else
    { 
	
		if (m_CommPort != _T(":"))
        { 
          if ( m_BarCodeReader.Open(m_CommPort))
            {
               
			   myLogFile.OpenLogFileIfClosed (L"C:\\Users\\Public\\Downloads\\VirtMsrLog.log");
	wchar_t  xBuff1[128];


	if( m_BarCodeReader.IsOpen())
	wsprintf (xBuff1, L"Openport: %s usccess  \r\n", m_CommPort);
	else

	wsprintf (xBuff1, L"Openport: %s uuu  \r\n", m_CommPort);
	myLogFile.PrintLog (xBuff1);

			  *pRc = OPOS_SUCCESS;
			  opened=_T("1");
			   return S_OK; 
            }
          else 
            {
             // m_BarCodeReader.Close(); 
			  *pRc = OPOS_E_NOSERVICE;
			  return S_OK; 
            }
        }
     }
  
		return S_OK; 
}

STDMETHODIMP CVirtMsrSo::CheckHealth(LONG level, LONG* pRc)
{
	myLogFile.OpenLogFileIfClosed (L"C:\\Users\\Public\\Downloads\\VirtMsrLog.log");
	wchar_t  xBuff[128];
	wsprintf (xBuff, L"CheckHealth called %d  \r\n", level);
	myLogFile.PrintLog (xBuff);

	 
			if ( m_BarCodeReader.IsOpen() )
            {
         		m_PIDX_ResultCode = 0;
				m_PIDX_ResultCodeExtended = 0;
				*pRc = OPOS_SUCCESS;
				return S_OK;
            }
				*pRc = OPOS_E_NOSERVICE;
				return S_OK;

}

STDMETHODIMP CVirtMsrSo::ClaimDevice(LONG ClaimTimeOut, LONG* pRc)
{
	myLogFile.OpenLogFileIfClosed (L"C:\\Users\\Public\\Downloads\\VirtMsrLog.log");
	myLogFile.PrintLog (L"ClaimDevice called. \n");

 
				m_PIDX_Claimed = 1;
	*pRc = OPOS_SUCCESS;
	return S_OK;
	 
	
}

STDMETHODIMP CVirtMsrSo::ClearInput(LONG* pRc)
{
	myLogFile.OpenLogFileIfClosed (L"C:\\Users\\Public\\Downloads\\VirtMsrLog.log");
	myLogFile.PrintLog (L"ClearInput called. \n");

 

	*pRc = OPOS_SUCCESS;
	return S_OK;
}

STDMETHODIMP CVirtMsrSo::CloseService(LONG* pRc)
{
	myLogFile.OpenLogFileIfClosed (L"C:\\Users\\Public\\Downloads\\VirtMsrLog.log");
	myLogFile.PrintLog (L"CloseService called. \n");

	m_PIDX_Claimed = 0;
	m_PIDX_DataEventEnabled = 0;
	m_PIDX_DeviceEnabled = 0;
	m_PIDX_FreezeEvents = 0;
	m_PIDX_OutputID = 0;
	m_PIDX_ResultCode = 0;
	m_PIDX_ResultCodeExtended = 0;
	m_PIDX_State = 0;
	m_PIDX_AutoDisable = 0;
	m_PIDX_BinaryConversion = 0;
	m_PIDX_DataCount = 0;
	m_PIDX_PowerNotify = 0;
	m_PIDX_PowerState = 0;

 
	 m_BarCodeReader.Close();
	*pRc = OPOS_SUCCESS;
	return S_OK;
}

STDMETHODIMP CVirtMsrSo::COFreezeEvents(VARIANT_BOOL Freeze, LONG* pRc)
{
	myLogFile.OpenLogFileIfClosed (L"C:\\Users\\Public\\Downloads\\VirtMsrLog.log");
	myLogFile.PrintLog (L"COFreezeEvents called. \n");

	*pRc = OPOS_SUCCESS;
	return S_OK;
}

STDMETHODIMP CVirtMsrSo::DirectIO(LONG Command, LONG* pData, BSTR* pString, LONG* pRc)
{
	myLogFile.OpenLogFileIfClosed (L"C:\\Users\\Public\\Downloads\\VirtMsrLog.log");
	myLogFile.PrintLog (L"DirectIO called. \n");

	*pRc = OPOS_SUCCESS;
	return S_OK;
}

STDMETHODIMP CVirtMsrSo::ReleaseDevice(LONG* pRc)
{
	myLogFile.OpenLogFileIfClosed (L"C:\\Users\\Public\\Downloads\\VirtMsrLog.log");
	myLogFile.PrintLog (L"ReleaseDevice called. \n");

	m_PIDX_Claimed = 0;

 

	*pRc = OPOS_SUCCESS;
	return S_OK;
}

STDMETHODIMP CVirtMsrSo::GetPropertyNumber(LONG PropIndex, LONG* pNumber)
{
	HRESULT  hrStatus = S_OK;

	myLogFile.OpenLogFileIfClosed (L"C:\\Users\\Public\\Downloads\\VirtMsrLog.log");

	switch (PropIndex) {
		case PIDX_ServiceObjectVersion:
			*pNumber = m_PIDX_ServiceObjectVersion;
			break;
		case PIDX_Claimed:
			*pNumber = m_PIDX_Claimed;
			break;
		case PIDX_DataEventEnabled:
			*pNumber = m_PIDX_DataEventEnabled;
			break;
		case PIDX_DeviceEnabled:
			*pNumber = m_PIDX_DeviceEnabled;
			break;
		case PIDX_FreezeEvents:
			*pNumber = m_PIDX_FreezeEvents;
			break;
		case PIDX_OutputID:
			*pNumber = m_PIDX_OutputID;
			break;
		case PIDX_ResultCode:
			*pNumber = m_PIDX_ResultCode;
			break;
		case PIDX_ResultCodeExtended:
			*pNumber = m_PIDX_ResultCodeExtended;
			break;
		case PIDX_State:
			*pNumber = m_PIDX_State;
			break;
		case PIDX_AutoDisable:
			*pNumber = m_PIDX_AutoDisable;
			break;
		case PIDX_BinaryConversion:
			*pNumber = m_PIDX_BinaryConversion;
			break;
		case PIDX_DataCount:
			*pNumber = m_PIDX_DataCount;
			break;
		case PIDX_PowerNotify:
			*pNumber = m_PIDX_PowerNotify;
			break;
		case PIDX_PowerState:
			*pNumber = m_PIDX_PowerState;
			break;

		case 9002:
			*pNumber = SCAL_WU_GRAM;
			break;
		case 9001:
			*pNumber = 9001;
			break;

		default:
			hrStatus = E_INVALIDARG;
			break;

	}

	wchar_t  xBuff[128];
	wsprintf (xBuff, L"GetPropertyNumber called %d %d  \r\n", PropIndex, *pNumber);
	myLogFile.PrintLog (xBuff);
	return hrStatus;
}

STDMETHODIMP CVirtMsrSo::GetPropertyString(LONG PropIndex, BSTR* pString)
{
	myLogFile.OpenLogFileIfClosed (L"C:\\Users\\Public\\Downloads\\VirtMsrLog.log");
	wchar_t  xBuff[128];
	wsprintf (xBuff, L"GetPropertyString called %d  \r\n", PropIndex);
	myLogFile.PrintLog (xBuff);

	HRESULT  hrStatus = S_OK;

	pString = NULL;

	switch (PropIndex) {
	 
		default:
			hrStatus = E_INVALIDARG;
			break;
	}

	return S_OK;
}

STDMETHODIMP CVirtMsrSo::SetPropertyNumber(LONG PropIndex, LONG Number)
{
	HRESULT  hrStatus = S_OK;

	myLogFile.OpenLogFileIfClosed (L"C:\\Users\\Public\\Downloads\\VirtMsrLog.log");
	wchar_t  xBuff[128];
	wsprintf (xBuff, L"SetPropertyNumber called %d %d  \r\n", PropIndex, Number);
	myLogFile.PrintLog (xBuff);

	switch (PropIndex) {
		case PIDX_DataEventEnabled:
			m_PIDX_DataEventEnabled = Number;
			break;
		case PIDX_DeviceEnabled:
			m_PIDX_DeviceEnabled = Number;
			break;
		case PIDX_FreezeEvents:
			m_PIDX_FreezeEvents = Number;
			break;
		case PIDX_OutputID:
			m_PIDX_OutputID = Number;
			break;
		case PIDX_ResultCode:
			m_PIDX_ResultCode = Number;
			break;
		case PIDX_ResultCodeExtended:
			m_PIDX_ResultCodeExtended = Number;
			break;
		case PIDX_AutoDisable:
			m_PIDX_AutoDisable = Number;
			break;
		case PIDX_BinaryConversion:
			m_PIDX_BinaryConversion = Number;
			break;
		 
		default:
			hrStatus = E_INVALIDARG;
			break;
	}

	return hrStatus;
}

STDMETHODIMP CVirtMsrSo::SetPropertyString(LONG PropIndex, BSTR PropString)
{
	myLogFile.OpenLogFileIfClosed (L"C:\\Users\\Public\\Downloads\\VirtMsrLog.log");
	wchar_t  xBuff[128];
	wsprintf (xBuff, L"SetPropertyString called %d  \r\n", PropIndex);
	myLogFile.PrintLog (xBuff);
	return S_OK;
}

STDMETHODIMP CVirtMsrSo::GetOpenResult(LONG* pRc)
{
	myLogFile.OpenLogFileIfClosed (L"C:\\Users\\Public\\Downloads\\VirtMsrLog.log");
	myLogFile.PrintLog (L"GetOpenResult called \n");

	*pRc = OPOS_SUCCESS;
	return S_OK;
}

STDMETHODIMP CVirtMsrSo::ReadWeight(LONG* pWeightData, LONG timeout, LONG* pRC)
{
	 myLogFile.OpenLogFileIfClosed (L"C:\\Users\\Public\\Downloads\\VirtMsrLog.log");
	  wchar_t  xBuff[5120];
	CString d=_T("");
	CString data=_T("");
	int i=0;
	*pRC = OPOS_SUCCESS;
	if(opened.Compare(_T("1"))!=0)
	{
	 wsprintf (xBuff, L"ReadWeight error %s  \r\n", opened);
	 *pWeightData=0;  
	 return S_OK;
	}
//	m_BarCodeReader.Close();
	if ( !m_BarCodeReader.IsOpen() ) 
	if (!m_BarCodeReader.Open(m_CommPort))
	{
		*pWeightData=0; 
	    return S_OK;
	  
	}
	else

	{
 wsprintf (xBuff, L"dang mo roi  \r\n");
	}

	while (i<3)
	{
		if(m_BarCodeReader.Read(d,data)>0)
		{ 
			  
		  i=3;
		}
		else 
		{
			wsprintf (xBuff, L"ReadWeight 1111 %s  \r\n", d);
		}
		i++;
	}

	 
   wchar_t* txt1 = d.GetBuffer(d.GetAllocLength()) ;
     char* t=new CHAR[d.GetLength()];
   for(int i=0;i<d.GetLength();i++)
	 t[i]=txt1[i]; 
    float num1 = atof(t);  
	wsprintf (xBuff, L"ReadWeight kkkk %s \r\n", d);
	 
	myLogFile.PrintLog (xBuff); 
	*pWeightData=num1*1000000; 
	return S_OK;
}

