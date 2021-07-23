

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0500 */
/* at Fri Jul 23 21:26:35 2021
 */
/* Compiler settings for .\atl_msr_so.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __atl_msr_so_h__
#define __atl_msr_so_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IVirtMsrSo_FWD_DEFINED__
#define __IVirtMsrSo_FWD_DEFINED__
typedef interface IVirtMsrSo IVirtMsrSo;
#endif 	/* __IVirtMsrSo_FWD_DEFINED__ */


#ifndef __VirtMsrSo_FWD_DEFINED__
#define __VirtMsrSo_FWD_DEFINED__

#ifdef __cplusplus
typedef class VirtMsrSo VirtMsrSo;
#else
typedef struct VirtMsrSo VirtMsrSo;
#endif /* __cplusplus */

#endif 	/* __VirtMsrSo_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IVirtMsrSo_INTERFACE_DEFINED__
#define __IVirtMsrSo_INTERFACE_DEFINED__

/* interface IVirtMsrSo */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IVirtMsrSo;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("8A1AD13E-6D31-47CF-AE77-F5DAE207D58C")
    IVirtMsrSo : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE OpenService( 
            BSTR DeviceClass,
            BSTR DeviceName,
            IDispatch *pDispatch,
            /* [retval][out] */ LONG *pRc) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CheckHealth( 
            LONG level,
            /* [retval][out] */ LONG *pRc) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ClaimDevice( 
            LONG ClaimTimeOut,
            /* [retval][out] */ LONG *pRc) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ClearInput( 
            /* [retval][out] */ LONG *pRc) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CloseService( 
            /* [retval][out] */ LONG *pRc) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE COFreezeEvents( 
            VARIANT_BOOL Freeze,
            LONG *pRc) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DirectIO( 
            LONG Command,
            /* [out][in] */ LONG *pData,
            /* [out][in] */ BSTR *pString,
            /* [retval][out] */ LONG *pRc) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ReleaseDevice( 
            /* [retval][out] */ LONG *pRc) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetPropertyNumber( 
            LONG PropIndex,
            /* [retval][out] */ LONG *pNumber) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetPropertyString( 
            LONG PropIndex,
            /* [retval][out] */ BSTR *pString) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetPropertyNumber( 
            LONG PropIndex,
            LONG Number) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetPropertyString( 
            LONG PropIndex,
            BSTR PropString) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetOpenResult( 
            /* [retval][out] */ LONG *pRc) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ReadWeight( 
            /* [out] */ LONG *pWeightData,
            LONG timeout,
            /* [retval][out] */ LONG *pRC) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IVirtMsrSoVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IVirtMsrSo * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IVirtMsrSo * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IVirtMsrSo * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IVirtMsrSo * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IVirtMsrSo * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IVirtMsrSo * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IVirtMsrSo * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *OpenService )( 
            IVirtMsrSo * This,
            BSTR DeviceClass,
            BSTR DeviceName,
            IDispatch *pDispatch,
            /* [retval][out] */ LONG *pRc);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CheckHealth )( 
            IVirtMsrSo * This,
            LONG level,
            /* [retval][out] */ LONG *pRc);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ClaimDevice )( 
            IVirtMsrSo * This,
            LONG ClaimTimeOut,
            /* [retval][out] */ LONG *pRc);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ClearInput )( 
            IVirtMsrSo * This,
            /* [retval][out] */ LONG *pRc);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CloseService )( 
            IVirtMsrSo * This,
            /* [retval][out] */ LONG *pRc);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *COFreezeEvents )( 
            IVirtMsrSo * This,
            VARIANT_BOOL Freeze,
            LONG *pRc);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DirectIO )( 
            IVirtMsrSo * This,
            LONG Command,
            /* [out][in] */ LONG *pData,
            /* [out][in] */ BSTR *pString,
            /* [retval][out] */ LONG *pRc);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ReleaseDevice )( 
            IVirtMsrSo * This,
            /* [retval][out] */ LONG *pRc);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetPropertyNumber )( 
            IVirtMsrSo * This,
            LONG PropIndex,
            /* [retval][out] */ LONG *pNumber);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetPropertyString )( 
            IVirtMsrSo * This,
            LONG PropIndex,
            /* [retval][out] */ BSTR *pString);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetPropertyNumber )( 
            IVirtMsrSo * This,
            LONG PropIndex,
            LONG Number);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetPropertyString )( 
            IVirtMsrSo * This,
            LONG PropIndex,
            BSTR PropString);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetOpenResult )( 
            IVirtMsrSo * This,
            /* [retval][out] */ LONG *pRc);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ReadWeight )( 
            IVirtMsrSo * This,
            /* [out] */ LONG *pWeightData,
            LONG timeout,
            /* [retval][out] */ LONG *pRC);
        
        END_INTERFACE
    } IVirtMsrSoVtbl;

    interface IVirtMsrSo
    {
        CONST_VTBL struct IVirtMsrSoVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IVirtMsrSo_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IVirtMsrSo_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IVirtMsrSo_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IVirtMsrSo_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IVirtMsrSo_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IVirtMsrSo_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IVirtMsrSo_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IVirtMsrSo_OpenService(This,DeviceClass,DeviceName,pDispatch,pRc)	\
    ( (This)->lpVtbl -> OpenService(This,DeviceClass,DeviceName,pDispatch,pRc) ) 

#define IVirtMsrSo_CheckHealth(This,level,pRc)	\
    ( (This)->lpVtbl -> CheckHealth(This,level,pRc) ) 

#define IVirtMsrSo_ClaimDevice(This,ClaimTimeOut,pRc)	\
    ( (This)->lpVtbl -> ClaimDevice(This,ClaimTimeOut,pRc) ) 

#define IVirtMsrSo_ClearInput(This,pRc)	\
    ( (This)->lpVtbl -> ClearInput(This,pRc) ) 

#define IVirtMsrSo_CloseService(This,pRc)	\
    ( (This)->lpVtbl -> CloseService(This,pRc) ) 

#define IVirtMsrSo_COFreezeEvents(This,Freeze,pRc)	\
    ( (This)->lpVtbl -> COFreezeEvents(This,Freeze,pRc) ) 

#define IVirtMsrSo_DirectIO(This,Command,pData,pString,pRc)	\
    ( (This)->lpVtbl -> DirectIO(This,Command,pData,pString,pRc) ) 

#define IVirtMsrSo_ReleaseDevice(This,pRc)	\
    ( (This)->lpVtbl -> ReleaseDevice(This,pRc) ) 

#define IVirtMsrSo_GetPropertyNumber(This,PropIndex,pNumber)	\
    ( (This)->lpVtbl -> GetPropertyNumber(This,PropIndex,pNumber) ) 

#define IVirtMsrSo_GetPropertyString(This,PropIndex,pString)	\
    ( (This)->lpVtbl -> GetPropertyString(This,PropIndex,pString) ) 

#define IVirtMsrSo_SetPropertyNumber(This,PropIndex,Number)	\
    ( (This)->lpVtbl -> SetPropertyNumber(This,PropIndex,Number) ) 

#define IVirtMsrSo_SetPropertyString(This,PropIndex,PropString)	\
    ( (This)->lpVtbl -> SetPropertyString(This,PropIndex,PropString) ) 

#define IVirtMsrSo_GetOpenResult(This,pRc)	\
    ( (This)->lpVtbl -> GetOpenResult(This,pRc) ) 

#define IVirtMsrSo_ReadWeight(This,pWeightData,timeout,pRC)	\
    ( (This)->lpVtbl -> ReadWeight(This,pWeightData,timeout,pRC) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IVirtMsrSo_INTERFACE_DEFINED__ */



#ifndef __atl_msr_soLib_LIBRARY_DEFINED__
#define __atl_msr_soLib_LIBRARY_DEFINED__

/* library atl_msr_soLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_atl_msr_soLib;

EXTERN_C const CLSID CLSID_VirtMsrSo;

#ifdef __cplusplus

class DECLSPEC_UUID("9EEDF76A-2924-4CC6-A1BE-0F7144FE3E80")
VirtMsrSo;
#endif
#endif /* __atl_msr_soLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


