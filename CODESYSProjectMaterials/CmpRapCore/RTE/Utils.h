/*
Copyright by 3S Smart Software Solutions, 87439 Kempten, Germany
Author: A. Fauter
*/

//Prototypes of Utility-function, usefull when writing NT4.0-drivers.
unsigned char SysReadStringFromRegistryToBuffer(char* szKeyName, char* szValName, char* szOutBuffer, int ulMaxBuffSize);
unsigned char SysReadUlongFromRegistry(char* szKeyName, char* szValName, unsigned long* pulOut, unsigned long* pulDefaultVal);
unsigned char SysWriteUlongToRegistry(char* szKeyName, char* szValName, unsigned long ulIn);

void* SysFileOpen(char *szFileName, char *szMode);
short SysFileClose(void* pFile);
unsigned long SysFileRead(void *pFile, unsigned char *pbyBuffer, unsigned long ulSize);
unsigned long SysFileWrite(void *pFile, unsigned char *pbyBuffer, unsigned long ulSize);
short SysFileDelete(char *szFileName);
unsigned long SysGetFileSize(void* pFile);
