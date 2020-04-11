/*
Copyright by 3S Smart Software Solutions, 87439 Kempten, Germany
Author: A. Fauter
*/

#include <ntddk.h>

//Functions for registry-handling
unsigned char SysReadStringFromRegistryToBuffer(char* szKeyName, char* szValName, char* szOutBuffer, int ulMaxBuffSize)
{//this function reads from registry into szOutBuffer.
//the KeyName and ValueName can both be specified as normal, zero-terminated c-strings.
//the result also will be written as normal, zero-terminated c-string.
//this makes it unnecessary for the caller, to deal with UNICODE_STRINGs....
//Attention: in this version the length of the read string is limited to 100 characters,
//because it seems unnecessary here to deal with dynamic memory.
	NTSTATUS ntRes;
	char pcBuffKey[400];
	char pcBuffVal[100];
	char pcBuffRes[200];
	UNICODE_STRING ustrResult;
	int i = 0;
	RTL_QUERY_REGISTRY_TABLE rtlqrt[2];

	ustrResult.Buffer = (unsigned short*)pcBuffRes;
	ustrResult.Length = 0;
	ustrResult.MaximumLength = 198;
	memset(pcBuffRes,0,sizeof(pcBuffRes));

	memset((char*)rtlqrt,0,sizeof(rtlqrt));

	memset(pcBuffKey,0,sizeof(pcBuffKey));
	while(szKeyName[i] != '\0' && i < 200)
	{
		pcBuffKey[2*i] = szKeyName[i];
		i++;
	}
	memset(pcBuffVal,0,sizeof(pcBuffVal));
	i = 0;
	while(szValName[i] != '\0' && i < 50)
	{
		pcBuffVal[2*i] = szValName[i];
		i++;
	}

	rtlqrt[0].Flags = RTL_QUERY_REGISTRY_DIRECT;
	rtlqrt[0].Name = (unsigned short*)pcBuffVal;
	rtlqrt[0].EntryContext = &ustrResult;
	rtlqrt[0].DefaultType = REG_SZ;
	rtlqrt[0].DefaultData = L"VALUE_NOT_FOUND";
	rtlqrt[0].DefaultLength = sizeof(pcBuffVal);

	ntRes = RtlQueryRegistryValues(RTL_REGISTRY_ABSOLUTE,(unsigned short*)pcBuffKey,rtlqrt,NULL,NULL);

	if(!(NT_SUCCESS(ntRes)))
		return 0;
	if(wcscmp(L"VALUE_NOT_FOUND",(short*)pcBuffVal) == 0)
		return 0;
	memset(szOutBuffer,0,ulMaxBuffSize);
	for(i = 0; i < ulMaxBuffSize && i < 50 && i <= ustrResult.Length; i++)
		szOutBuffer[i] = ((char*)ustrResult.Buffer)[i*2];
	return 1;
}

unsigned char SysReadUlongFromRegistry(char* szKeyName, char* szValName, unsigned long* pulOut, unsigned long* pulDefaultVal)
{//this function reads from registry into pulOut.
//the KeyName and ValueName can both be specified as normal, zero-terminated c-strings.
//this makes it unnecessary for the caller, to deal with UNICODE_STRING and other shit....
	NTSTATUS ntRes;
	char pcBuffKey[400];
	char pcBuffVal[100];
    char bCheckOnly = 0;
	int i = 0;
    unsigned long ulDummy1 = 0,ulDummy2 = (unsigned long)-1;
	RTL_QUERY_REGISTRY_TABLE rtlqrt[2];

    //if a NULLpointer is passed to the function, prevent system-crash by overwriting pulOut, pulDefaultVal.
    //if one of the pointers is NULL, the caller just wants to check, if a special entry is existing.
    //in that case return 1 only if a value different from -1 was read.
    if(pulOut == NULL)
    {
        bCheckOnly = 1;
        pulOut = &ulDummy1;
    }
    if(pulDefaultVal == NULL)
    {
        bCheckOnly = 1;
        pulDefaultVal = &ulDummy2;
    }

	memset((char*)rtlqrt,0,sizeof(rtlqrt));

	memset(pcBuffKey,0,sizeof(pcBuffKey));
	while(szKeyName[i] != '\0' && i < 199)
	{
		pcBuffKey[2*i] = szKeyName[i];
		i++;
	}
    //check, if the last character of szKeyName was a \(backslash).
    //if yes, clear it, or the function will fail anyway.
    if(szKeyName[i-1] == '\\' && i < 199)
        pcBuffKey[i] = '\0';            

	memset(pcBuffVal,0,sizeof(pcBuffVal));
	i = 0;
	while(szValName[i] != '\0' && i < 49)
	{
		pcBuffVal[2*i] = szValName[i];
		i++;
	}

	rtlqrt[0].Flags = RTL_QUERY_REGISTRY_DIRECT;
	rtlqrt[0].Name = (unsigned short*)pcBuffVal;
	rtlqrt[0].EntryContext = pulOut;
	rtlqrt[0].DefaultType = REG_DWORD;
	rtlqrt[0].DefaultData = pulDefaultVal;
	rtlqrt[0].DefaultLength = sizeof(unsigned long);

	ntRes = RtlQueryRegistryValues(RTL_REGISTRY_ABSOLUTE,(unsigned short*)pcBuffKey,rtlqrt,NULL,NULL);

	if(!(NT_SUCCESS(ntRes)))
		return 0;
    if(bCheckOnly && ulDummy1 == ulDummy2)
        return 0;
	return 1;
}

unsigned char SysWriteUlongToRegistry(char* szKeyName, char* szValName, unsigned long ulIn)
{//this function reads from registry into pulOut.
//the KeyName and ValueName can both be specified as normal, zero-terminated c-strings.
//this makes it unnecessary for the caller, to deal with UNICODE_STRING and other shit....
	NTSTATUS ntRes;
	char pcBuffKey[400];
	char pcBuffVal[100];
	int i = 0;

	memset(pcBuffKey,0,sizeof(pcBuffKey));
	while(szKeyName[i] != '\0' && i < 200)
	{
		pcBuffKey[2*i] = szKeyName[i];
		i++;
	}
	memset(pcBuffVal,0,sizeof(pcBuffVal));
	i = 0;
	while(szValName[i] != '\0' && i < 50)
	{
		pcBuffVal[2*i] = szValName[i];
		i++;
	}

	ntRes = RtlWriteRegistryValue(RTL_REGISTRY_ABSOLUTE,(unsigned short*)pcBuffKey,(unsigned short*)pcBuffVal,REG_DWORD,&ulIn,sizeof(unsigned long));

	if(!(NT_SUCCESS(ntRes)))
		return 0;
	return 1;
}

void* SysFileOpen(char *szFileName, char *szMode)
{
	HANDLE hFile = NULL;
	UNICODE_STRING ustrFilePath;
	char pcBuffer[400];
	char pcHBuf[200];
	OBJECT_ATTRIBUTES objattr;
	IO_STATUS_BLOCK	iostat;
	NTSTATUS ntRes = STATUS_SUCCESS; 
    int i = 0;

    if(strlen(szFileName) > 200)
        return NULL;

	strcpy(pcHBuf,szFileName);

    memset(pcBuffer,0,sizeof(pcBuffer));
    while(pcHBuf[i] != '\0' && i < 200)
    {
        pcBuffer[i*2] = pcHBuf[i];
        i++;
    }
    ustrFilePath.MaximumLength = 400;
    ustrFilePath.Length = 2*strlen(pcHBuf);
    ustrFilePath.Buffer = (unsigned short*)pcBuffer;

	InitializeObjectAttributes(&objattr,&ustrFilePath,OBJ_CASE_INSENSITIVE,NULL,NULL);

	//use ZwCreateFile to open a file.
	if(*szMode == 'r')
		ntRes = ZwCreateFile(&hFile,FILE_READ_ATTRIBUTES | FILE_READ_EA | FILE_READ_DATA | SYNCHRONIZE,
						&objattr,&iostat,NULL,
						FILE_ATTRIBUTE_ARCHIVE,0,FILE_OPEN,FILE_NON_DIRECTORY_FILE|FILE_SYNCHRONOUS_IO_NONALERT,
						NULL,0); 
	else 
		ntRes = ZwCreateFile(&hFile,FILE_WRITE_DATA | FILE_WRITE_ATTRIBUTES | FILE_WRITE_EA | SYNCHRONIZE,
						&objattr,&iostat,NULL,
						FILE_ATTRIBUTE_ARCHIVE,0,FILE_OVERWRITE_IF,FILE_NON_DIRECTORY_FILE|FILE_SYNCHRONOUS_IO_NONALERT,
						NULL,0); 
					
	if(ntRes != STATUS_SUCCESS)
		return NULL;

	return (void *)hFile;
}

short SysFileClose(void* pFile)
{
	short sRes = 0;
	//Call ZwClose();
	if(pFile == NULL)
		return 0;
	sRes = ZwClose(pFile) == STATUS_SUCCESS;

	return sRes;
}

unsigned long SysFileRead(void *pFile, unsigned char *pbyBuffer, unsigned long ulSize)
{
	//call ZwReadFile();
	NTSTATUS ntRes;
	IO_STATUS_BLOCK iostat;

    if(pFile == NULL)
        return 0;

	ntRes = ZwReadFile((HANDLE)pFile,NULL,NULL,NULL,&iostat,pbyBuffer,ulSize,(LARGE_INTEGER*)NULL,NULL);

	if(!NT_SUCCESS(ntRes))
		return 0;

	return iostat.Information;
}

unsigned long SysFileWrite(void *pFile, unsigned char *pbyBuffer, unsigned long ulSize)
{
	//call ZwWriteFile();
	NTSTATUS ntRes;
	IO_STATUS_BLOCK iostat;

    if(pFile == NULL)
        return 0;

	ntRes = ZwWriteFile((HANDLE)pFile,NULL,NULL,NULL,&iostat,pbyBuffer,ulSize,(LARGE_INTEGER*)NULL,NULL);

	if(!NT_SUCCESS(ntRes))
		return 0;
			   
	return iostat.Information;
}

short SysFileDelete(char *szFileName)
{
	FILE_DISPOSITION_INFORMATION fdinf;
	NTSTATUS ntRes1, ntRes2;
	IO_STATUS_BLOCK iostat;
	HANDLE hFile = SysFileOpen(szFileName,"w");
	
	if(hFile != NULL)
	{
		fdinf.DeleteFile = TRUE;
		ntRes1 = ZwSetInformationFile(hFile,&iostat,&fdinf,sizeof(FILE_DISPOSITION_INFORMATION),
										FileDispositionInformation);
		ntRes2 = SysFileClose(hFile); //now the file is actually deleted, if this is the last opened handle.
		if(NT_SUCCESS(ntRes1) && NT_SUCCESS(ntRes2))
			return 1;
	}
	return 0;
}

unsigned long SysGetFileSize(void* pFile)
{
    IO_STATUS_BLOCK iosb;
    FILE_STANDARD_INFORMATION fsi;
    
    if(pFile == NULL)
        return 0;

    ZwQueryInformationFile((HANDLE)pFile,&iosb,&fsi,sizeof(FILE_STANDARD_INFORMATION),FileStandardInformation); 

    return fsi.EndOfFile.u.LowPart;
}