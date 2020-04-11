 /**
 * <interfacename>CmpRapCore</interfacename>
 * <description></description>
 *
 * <copyright></copyright>
 */


	
	
#ifndef _CMPRAPCOREITF_H_
#define _CMPRAPCOREITF_H_

#include "CmpStd.h"

 

 




/** EXTERN LIB SECTION BEGIN **/
/*  Comments are ignored for m4 compiler so restructured text can be used.  */

#ifdef __cplusplus
extern "C" {
#endif

/**
 *{attribute 'conditionalshow'}
 *{attribute 'conditionalshow' := 'SomeText'}
 *{attribute 'hide'}
 */
#define ERROR_NO_ERROR    RTS_IEC_INT_C(0x0)	
#define ERROR_TIME_OUT    RTS_IEC_INT_C(0x1)	
/* Typed enum definition */
#define ERROR    RTS_IEC_INT

/**
 * <description>moduleinit</description>
 */
typedef struct tagmoduleinit_struct
{
	RTS_IEC_INT ModuleInit;				/* VAR_OUTPUT */	
} moduleinit_struct;

void CDECL CDECL_EXT moduleinit(moduleinit_struct *p);
typedef void (CDECL CDECL_EXT* PFMODULEINIT_IEC) (moduleinit_struct *p);
#if defined(CMPRAPCORE_NOTIMPLEMENTED) || defined(MODULEINIT_NOTIMPLEMENTED)
	#define USE_moduleinit
	#define EXT_moduleinit
	#define GET_moduleinit(fl)  ERR_NOTIMPLEMENTED
	#define CAL_moduleinit(p0) 
	#define CHK_moduleinit  FALSE
	#define EXP_moduleinit  ERR_OK
#elif defined(STATIC_LINK)
	#define USE_moduleinit
	#define EXT_moduleinit
	#define GET_moduleinit(fl)  CAL_CMGETAPI( "moduleinit" ) 
	#define CAL_moduleinit  moduleinit
	#define CHK_moduleinit  TRUE
	#define EXP_moduleinit  s_pfCMRegisterAPI2( (const CMP_EXT_FUNCTION_REF*)"moduleinit", (RTS_UINTPTR)moduleinit, 1, 0x271B3579, 0x03050F1E) 
#elif defined(MIXED_LINK) && !defined(CMPRAPCORE_EXTERNAL)
	#define USE_moduleinit
	#define EXT_moduleinit
	#define GET_moduleinit(fl)  CAL_CMGETAPI( "moduleinit" ) 
	#define CAL_moduleinit  moduleinit
	#define CHK_moduleinit  TRUE
	#define EXP_moduleinit  s_pfCMRegisterAPI2( (const CMP_EXT_FUNCTION_REF*)"moduleinit", (RTS_UINTPTR)moduleinit, 1, 0x271B3579, 0x03050F1E) 
#elif defined(CPLUSPLUS_ONLY)
	#define USE_CmpRapCoremoduleinit
	#define EXT_CmpRapCoremoduleinit
	#define GET_CmpRapCoremoduleinit  ERR_OK
	#define CAL_CmpRapCoremoduleinit  moduleinit
	#define CHK_CmpRapCoremoduleinit  TRUE
	#define EXP_CmpRapCoremoduleinit  s_pfCMRegisterAPI2( (const CMP_EXT_FUNCTION_REF*)"moduleinit", (RTS_UINTPTR)moduleinit, 1, 0x271B3579, 0x03050F1E) 
#elif defined(CPLUSPLUS)
	#define USE_moduleinit
	#define EXT_moduleinit
	#define GET_moduleinit(fl)  CAL_CMGETAPI( "moduleinit" ) 
	#define CAL_moduleinit  moduleinit
	#define CHK_moduleinit  TRUE
	#define EXP_moduleinit  s_pfCMRegisterAPI2( (const CMP_EXT_FUNCTION_REF*)"moduleinit", (RTS_UINTPTR)moduleinit, 1, 0x271B3579, 0x03050F1E) 
#else /* DYNAMIC_LINK */
	#define USE_moduleinit  PFMODULEINIT_IEC pfmoduleinit;
	#define EXT_moduleinit  extern PFMODULEINIT_IEC pfmoduleinit;
	#define GET_moduleinit(fl)  s_pfCMGetAPI2( "moduleinit", (RTS_VOID_FCTPTR *)&pfmoduleinit, (fl) | CM_IMPORT_EXTERNAL_LIB_FUNCTION, 0x271B3579, 0x03050F1E)
	#define CAL_moduleinit  pfmoduleinit
	#define CHK_moduleinit  (pfmoduleinit != NULL)
	#define EXP_moduleinit   s_pfCMRegisterAPI2( (const CMP_EXT_FUNCTION_REF*)"moduleinit", (RTS_UINTPTR)moduleinit, 1, 0x271B3579, 0x03050F1E) 
#endif


/**
 * <description>spi_writebyte</description>
 */
typedef struct tagspi_writebyte_struct
{
	RTS_IEC_BYTE byReg;					/* VAR_INPUT */	
	RTS_IEC_INT SPI_WriteByte;			/* VAR_OUTPUT */	
} spi_writebyte_struct;

void CDECL CDECL_EXT spi_writebyte(spi_writebyte_struct *p);
typedef void (CDECL CDECL_EXT* PFSPI_WRITEBYTE_IEC) (spi_writebyte_struct *p);
#if defined(CMPRAPCORE_NOTIMPLEMENTED) || defined(SPI_WRITEBYTE_NOTIMPLEMENTED)
	#define USE_spi_writebyte
	#define EXT_spi_writebyte
	#define GET_spi_writebyte(fl)  ERR_NOTIMPLEMENTED
	#define CAL_spi_writebyte(p0) 
	#define CHK_spi_writebyte  FALSE
	#define EXP_spi_writebyte  ERR_OK
#elif defined(STATIC_LINK)
	#define USE_spi_writebyte
	#define EXT_spi_writebyte
	#define GET_spi_writebyte(fl)  CAL_CMGETAPI( "spi_writebyte" ) 
	#define CAL_spi_writebyte  spi_writebyte
	#define CHK_spi_writebyte  TRUE
	#define EXP_spi_writebyte  s_pfCMRegisterAPI2( (const CMP_EXT_FUNCTION_REF*)"spi_writebyte", (RTS_UINTPTR)spi_writebyte, 1, 0x0A92F672, 0x03050F1E) 
#elif defined(MIXED_LINK) && !defined(CMPRAPCORE_EXTERNAL)
	#define USE_spi_writebyte
	#define EXT_spi_writebyte
	#define GET_spi_writebyte(fl)  CAL_CMGETAPI( "spi_writebyte" ) 
	#define CAL_spi_writebyte  spi_writebyte
	#define CHK_spi_writebyte  TRUE
	#define EXP_spi_writebyte  s_pfCMRegisterAPI2( (const CMP_EXT_FUNCTION_REF*)"spi_writebyte", (RTS_UINTPTR)spi_writebyte, 1, 0x0A92F672, 0x03050F1E) 
#elif defined(CPLUSPLUS_ONLY)
	#define USE_CmpRapCorespi_writebyte
	#define EXT_CmpRapCorespi_writebyte
	#define GET_CmpRapCorespi_writebyte  ERR_OK
	#define CAL_CmpRapCorespi_writebyte  spi_writebyte
	#define CHK_CmpRapCorespi_writebyte  TRUE
	#define EXP_CmpRapCorespi_writebyte  s_pfCMRegisterAPI2( (const CMP_EXT_FUNCTION_REF*)"spi_writebyte", (RTS_UINTPTR)spi_writebyte, 1, 0x0A92F672, 0x03050F1E) 
#elif defined(CPLUSPLUS)
	#define USE_spi_writebyte
	#define EXT_spi_writebyte
	#define GET_spi_writebyte(fl)  CAL_CMGETAPI( "spi_writebyte" ) 
	#define CAL_spi_writebyte  spi_writebyte
	#define CHK_spi_writebyte  TRUE
	#define EXP_spi_writebyte  s_pfCMRegisterAPI2( (const CMP_EXT_FUNCTION_REF*)"spi_writebyte", (RTS_UINTPTR)spi_writebyte, 1, 0x0A92F672, 0x03050F1E) 
#else /* DYNAMIC_LINK */
	#define USE_spi_writebyte  PFSPI_WRITEBYTE_IEC pfspi_writebyte;
	#define EXT_spi_writebyte  extern PFSPI_WRITEBYTE_IEC pfspi_writebyte;
	#define GET_spi_writebyte(fl)  s_pfCMGetAPI2( "spi_writebyte", (RTS_VOID_FCTPTR *)&pfspi_writebyte, (fl) | CM_IMPORT_EXTERNAL_LIB_FUNCTION, 0x0A92F672, 0x03050F1E)
	#define CAL_spi_writebyte  pfspi_writebyte
	#define CHK_spi_writebyte  (pfspi_writebyte != NULL)
	#define EXP_spi_writebyte   s_pfCMRegisterAPI2( (const CMP_EXT_FUNCTION_REF*)"spi_writebyte", (RTS_UINTPTR)spi_writebyte, 1, 0x0A92F672, 0x03050F1E) 
#endif


/**
 * <description>writecspin_0</description>
 */
typedef struct tagwritecspin_0_struct
{
	RTS_IEC_INT WriteCSPIN_0;			/* VAR_OUTPUT */	
} writecspin_0_struct;

void CDECL CDECL_EXT writecspin_0(writecspin_0_struct *p);
typedef void (CDECL CDECL_EXT* PFWRITECSPIN_0_IEC) (writecspin_0_struct *p);
#if defined(CMPRAPCORE_NOTIMPLEMENTED) || defined(WRITECSPIN_0_NOTIMPLEMENTED)
	#define USE_writecspin_0
	#define EXT_writecspin_0
	#define GET_writecspin_0(fl)  ERR_NOTIMPLEMENTED
	#define CAL_writecspin_0(p0) 
	#define CHK_writecspin_0  FALSE
	#define EXP_writecspin_0  ERR_OK
#elif defined(STATIC_LINK)
	#define USE_writecspin_0
	#define EXT_writecspin_0
	#define GET_writecspin_0(fl)  CAL_CMGETAPI( "writecspin_0" ) 
	#define CAL_writecspin_0  writecspin_0
	#define CHK_writecspin_0  TRUE
	#define EXP_writecspin_0  s_pfCMRegisterAPI2( (const CMP_EXT_FUNCTION_REF*)"writecspin_0", (RTS_UINTPTR)writecspin_0, 1, 0x70C8F91E, 0x03050F1E) 
#elif defined(MIXED_LINK) && !defined(CMPRAPCORE_EXTERNAL)
	#define USE_writecspin_0
	#define EXT_writecspin_0
	#define GET_writecspin_0(fl)  CAL_CMGETAPI( "writecspin_0" ) 
	#define CAL_writecspin_0  writecspin_0
	#define CHK_writecspin_0  TRUE
	#define EXP_writecspin_0  s_pfCMRegisterAPI2( (const CMP_EXT_FUNCTION_REF*)"writecspin_0", (RTS_UINTPTR)writecspin_0, 1, 0x70C8F91E, 0x03050F1E) 
#elif defined(CPLUSPLUS_ONLY)
	#define USE_CmpRapCorewritecspin_0
	#define EXT_CmpRapCorewritecspin_0
	#define GET_CmpRapCorewritecspin_0  ERR_OK
	#define CAL_CmpRapCorewritecspin_0  writecspin_0
	#define CHK_CmpRapCorewritecspin_0  TRUE
	#define EXP_CmpRapCorewritecspin_0  s_pfCMRegisterAPI2( (const CMP_EXT_FUNCTION_REF*)"writecspin_0", (RTS_UINTPTR)writecspin_0, 1, 0x70C8F91E, 0x03050F1E) 
#elif defined(CPLUSPLUS)
	#define USE_writecspin_0
	#define EXT_writecspin_0
	#define GET_writecspin_0(fl)  CAL_CMGETAPI( "writecspin_0" ) 
	#define CAL_writecspin_0  writecspin_0
	#define CHK_writecspin_0  TRUE
	#define EXP_writecspin_0  s_pfCMRegisterAPI2( (const CMP_EXT_FUNCTION_REF*)"writecspin_0", (RTS_UINTPTR)writecspin_0, 1, 0x70C8F91E, 0x03050F1E) 
#else /* DYNAMIC_LINK */
	#define USE_writecspin_0  PFWRITECSPIN_0_IEC pfwritecspin_0;
	#define EXT_writecspin_0  extern PFWRITECSPIN_0_IEC pfwritecspin_0;
	#define GET_writecspin_0(fl)  s_pfCMGetAPI2( "writecspin_0", (RTS_VOID_FCTPTR *)&pfwritecspin_0, (fl) | CM_IMPORT_EXTERNAL_LIB_FUNCTION, 0x70C8F91E, 0x03050F1E)
	#define CAL_writecspin_0  pfwritecspin_0
	#define CHK_writecspin_0  (pfwritecspin_0 != NULL)
	#define EXP_writecspin_0   s_pfCMRegisterAPI2( (const CMP_EXT_FUNCTION_REF*)"writecspin_0", (RTS_UINTPTR)writecspin_0, 1, 0x70C8F91E, 0x03050F1E) 
#endif


/**
 * <description>writecspin_1</description>
 */
typedef struct tagwritecspin_1_struct
{
	RTS_IEC_INT WriteCSPIN_1;			/* VAR_OUTPUT */	
} writecspin_1_struct;

void CDECL CDECL_EXT writecspin_1(writecspin_1_struct *p);
typedef void (CDECL CDECL_EXT* PFWRITECSPIN_1_IEC) (writecspin_1_struct *p);
#if defined(CMPRAPCORE_NOTIMPLEMENTED) || defined(WRITECSPIN_1_NOTIMPLEMENTED)
	#define USE_writecspin_1
	#define EXT_writecspin_1
	#define GET_writecspin_1(fl)  ERR_NOTIMPLEMENTED
	#define CAL_writecspin_1(p0) 
	#define CHK_writecspin_1  FALSE
	#define EXP_writecspin_1  ERR_OK
#elif defined(STATIC_LINK)
	#define USE_writecspin_1
	#define EXT_writecspin_1
	#define GET_writecspin_1(fl)  CAL_CMGETAPI( "writecspin_1" ) 
	#define CAL_writecspin_1  writecspin_1
	#define CHK_writecspin_1  TRUE
	#define EXP_writecspin_1  s_pfCMRegisterAPI2( (const CMP_EXT_FUNCTION_REF*)"writecspin_1", (RTS_UINTPTR)writecspin_1, 1, 0xD0F3C969, 0x03050F1E) 
#elif defined(MIXED_LINK) && !defined(CMPRAPCORE_EXTERNAL)
	#define USE_writecspin_1
	#define EXT_writecspin_1
	#define GET_writecspin_1(fl)  CAL_CMGETAPI( "writecspin_1" ) 
	#define CAL_writecspin_1  writecspin_1
	#define CHK_writecspin_1  TRUE
	#define EXP_writecspin_1  s_pfCMRegisterAPI2( (const CMP_EXT_FUNCTION_REF*)"writecspin_1", (RTS_UINTPTR)writecspin_1, 1, 0xD0F3C969, 0x03050F1E) 
#elif defined(CPLUSPLUS_ONLY)
	#define USE_CmpRapCorewritecspin_1
	#define EXT_CmpRapCorewritecspin_1
	#define GET_CmpRapCorewritecspin_1  ERR_OK
	#define CAL_CmpRapCorewritecspin_1  writecspin_1
	#define CHK_CmpRapCorewritecspin_1  TRUE
	#define EXP_CmpRapCorewritecspin_1  s_pfCMRegisterAPI2( (const CMP_EXT_FUNCTION_REF*)"writecspin_1", (RTS_UINTPTR)writecspin_1, 1, 0xD0F3C969, 0x03050F1E) 
#elif defined(CPLUSPLUS)
	#define USE_writecspin_1
	#define EXT_writecspin_1
	#define GET_writecspin_1(fl)  CAL_CMGETAPI( "writecspin_1" ) 
	#define CAL_writecspin_1  writecspin_1
	#define CHK_writecspin_1  TRUE
	#define EXP_writecspin_1  s_pfCMRegisterAPI2( (const CMP_EXT_FUNCTION_REF*)"writecspin_1", (RTS_UINTPTR)writecspin_1, 1, 0xD0F3C969, 0x03050F1E) 
#else /* DYNAMIC_LINK */
	#define USE_writecspin_1  PFWRITECSPIN_1_IEC pfwritecspin_1;
	#define EXT_writecspin_1  extern PFWRITECSPIN_1_IEC pfwritecspin_1;
	#define GET_writecspin_1(fl)  s_pfCMGetAPI2( "writecspin_1", (RTS_VOID_FCTPTR *)&pfwritecspin_1, (fl) | CM_IMPORT_EXTERNAL_LIB_FUNCTION, 0xD0F3C969, 0x03050F1E)
	#define CAL_writecspin_1  pfwritecspin_1
	#define CHK_writecspin_1  (pfwritecspin_1 != NULL)
	#define EXP_writecspin_1   s_pfCMRegisterAPI2( (const CMP_EXT_FUNCTION_REF*)"writecspin_1", (RTS_UINTPTR)writecspin_1, 1, 0xD0F3C969, 0x03050F1E) 
#endif


#ifdef __cplusplus
}
#endif

/** EXTERN LIB SECTION END **/




typedef struct
{
	IBase_C *pBase;
} ICmpRapCore_C;

#ifdef CPLUSPLUS
class ICmpRapCore : public IBase
{
	public:
};
	#ifndef ITF_CmpRapCore
		#define ITF_CmpRapCore static ICmpRapCore *pICmpRapCore = NULL;
	#endif
	#define EXTITF_CmpRapCore
#else	/*CPLUSPLUS*/
	typedef ICmpRapCore_C		ICmpRapCore;
	#ifndef ITF_CmpRapCore
		#define ITF_CmpRapCore
	#endif
	#define EXTITF_CmpRapCore
#endif

#ifdef CPLUSPLUS_ONLY
  #undef CPLUSPLUS_ONLY
#endif

#endif /*_CMPRAPCOREITF_H_*/
