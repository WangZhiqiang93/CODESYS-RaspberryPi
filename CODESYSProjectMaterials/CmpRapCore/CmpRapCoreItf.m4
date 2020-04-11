/**
 * <interfacename>CmpRapCore</interfacename>
 * <description></description>
 *
 * <copyright></copyright>
 */

SET_INTERFACE_NAME(`CmpRapCore')

/** EXTERN LIB SECTION BEGIN **/
/*  Comments are ignored for m4 compiler so restructured text can be used. changecom(`/*', `*/') */

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

DEF_API(`void',`CDECL',`moduleinit',`(moduleinit_struct *p)',1,0x271B3579,0x03050F1E)

/**
 * <description>spi_writebyte</description>
 */
typedef struct tagspi_writebyte_struct
{
	RTS_IEC_BYTE byReg;					/* VAR_INPUT */	
	RTS_IEC_INT SPI_WriteByte;			/* VAR_OUTPUT */	
} spi_writebyte_struct;

DEF_API(`void',`CDECL',`spi_writebyte',`(spi_writebyte_struct *p)',1,0x0A92F672,0x03050F1E)

/**
 * <description>writecspin_0</description>
 */
typedef struct tagwritecspin_0_struct
{
	RTS_IEC_INT WriteCSPIN_0;			/* VAR_OUTPUT */	
} writecspin_0_struct;

DEF_API(`void',`CDECL',`writecspin_0',`(writecspin_0_struct *p)',1,0x70C8F91E,0x03050F1E)

/**
 * <description>writecspin_1</description>
 */
typedef struct tagwritecspin_1_struct
{
	RTS_IEC_INT WriteCSPIN_1;			/* VAR_OUTPUT */	
} writecspin_1_struct;

DEF_API(`void',`CDECL',`writecspin_1',`(writecspin_1_struct *p)',1,0xD0F3C969,0x03050F1E)

#ifdef __cplusplus
}
#endif

/** EXTERN LIB SECTION END **/

