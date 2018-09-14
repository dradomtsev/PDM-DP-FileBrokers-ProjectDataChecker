// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"

UI::UI() : stWorkDir() , stStartChkPeriod(), stEndChkPeriod()
{
	stFileINI_Name = TEXT("\\#Res-[ArM]1IM-P-ZZ-SP-GetFilesAttribsINI.ini");
};
UI::~UI() {};
UI * UI::GetUIInst()
{
	static UI UIInst;
	return &UIInst;
};
DWORD UI::FindExePath()
{
	//0. Get EXE Path for futher searching of INI-file
	DWORD dwErrorCode = -1;
	DWORD dStatus = -1;
	SetLastError(ERROR_SUCCESS);
	int iSize = 0;
	std::size_t nLocCRLF = 0;
	TCHAR tModulePath[MAX_PATH] = { 0 };
	try
	{
		iSize = GetModuleFileName(NULL, tModulePath, MAX_PATH);
		dwErrorCode = GetLastError();
		if (iSize == 0)
			ErrorHandle::GetErrorHandleInst()->ErrorExit(_T("FindExePath->GetModuleFileName()"), _T("."), dwErrorCode);
		throw dwErrorCode;
	}
	catch (DWORD Err_code)
	{
		switch (Err_code)
		{
		case ERROR_SUCCESS:
		case ERROR_INSUFFICIENT_BUFFER:
			break;
		default:
			ErrorHandle::GetErrorHandleInst()->ErrorExit(_T("FindExePath->GetModuleFileName()"),_T("."), dwErrorCode);
		}
	}
	
	this->stModuleName = tModulePath;
	nLocCRLF = stModuleName.find_last_of('\\');
	stModuleName.erase(0, nLocCRLF+1);
	
	try
	{
		dStatus = PathRemoveFileSpec(tModulePath);
		this->stModulePath = tModulePath;
		if (dStatus == 0)
		{
			dStatus = FALSE;
		}
		else
			dStatus = TRUE;
		dwErrorCode = GetLastError();
		throw dwErrorCode;
	}
	catch (DWORD Err_code)
	{
		switch (Err_code)
		{
		case ERROR_SUCCESS:
			break;
		default:
			ErrorHandle::GetErrorHandleInst()->ErrorExit(_T("FindExePath->PathRemoveFileSpec()"), _T("."), dwErrorCode);
			return FALSE;
		}
	}
	SetLastError(ERROR_SUCCESS);
	return 0;
};
DWORD UI::InitbyINI()
{
	DWORD lpdwFlags;
	DWORD liFileSize;
	std::basic_string<TCHAR> stCurDirBuff;
	HANDLE hFile = nullptr;
	DWORD dwErrorCode = -1;
	DWORD dStatus = -1;
	BOOL bStatus = FALSE;
	SetLastError(ERROR_SUCCESS);
	//1. Set curent directory of INI file
	try
	{
		dStatus = SetCurrentDirectory(&this->stModulePath[0]);
		dwErrorCode = GetLastError();
		if (dStatus == 0)
		{
			Logger::GetLogInstance()->PrepareTXTLOG("Function->InitbyINI(SetCurrentDirectory()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", this->stModulePath);
			ErrorHandle::GetErrorHandleInst()->ErrorExit(_T("InitDirbyINI->SetCurrentDirectory()"), _T("."), dwErrorCode);
		}
		throw dwErrorCode;
	}
	catch(DWORD Err_code)
	{
		switch (Err_code)
		{
		case ERROR_SUCCESS:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->InitbyINI(SetCurrentDirectory()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", this->stModulePath);
			break;
		default:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->InitbyINI(SetCurrentDirectory()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", this->stModulePath);
			ErrorHandle::GetErrorHandleInst()->ErrorExit(_T("InitDirbyINI->SetCurrentDirectory()"), _T("."), dwErrorCode);
		}
	}
	//2. Open INI-file with initialization string and last-check time
	SetLastError(ERROR_SUCCESS);
	try
	{
		stCurDirBuff.assign(this->stModulePath);
		stCurDirBuff.append(this->stFileINI_Name);
		hFile = CreateFile( stCurDirBuff.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,NULL);
		dwErrorCode = GetLastError();
		if (hFile == INVALID_HANDLE_VALUE) 
		{
			CloseHandle(hFile);
			Logger::GetLogInstance()->PrepareTXTLOG("Function->InitbyINI(CreateFile()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", stCurDirBuff);
			ErrorHandle::GetErrorHandleInst()->ErrorExit(_T("InitDirbyINI->CreateFile()"), _T("."), dwErrorCode);
		}
		throw dwErrorCode;
	}
	catch(DWORD Err_code)
	{
		switch (Err_code)
		{
		case ERROR_SUCCESS:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->InitbyINI(CreateFile()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", stCurDirBuff);
			break;
		default:
			CloseHandle(hFile);
			Logger::GetLogInstance()->PrepareTXTLOG("Function->InitbyINI(CreateFile()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", stCurDirBuff);
			ErrorHandle::GetErrorHandleInst()->ErrorExit(_T("InitDirbyINI->CreateFile()"), _T("."), dwErrorCode);
		}
	}
	//3. Get INI-file handle Information
	SetLastError(ERROR_SUCCESS);
	try
	{
		bStatus = GetHandleInformation(hFile,&lpdwFlags);
		dwErrorCode = GetLastError();
		if (bStatus == 0)
		{
			CloseHandle(hFile);
			Logger::GetLogInstance()->PrepareTXTLOG("Function->InitbyINI(GetHandleInformation()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", stCurDirBuff);
			ErrorHandle::GetErrorHandleInst()->ErrorExit(_T("InitDirbyINI->GetHandleInformation()"), _T("."), dwErrorCode);
		}
		throw dwErrorCode;
	}
	catch(DWORD Err_code)
	{
		switch (Err_code)
		{
		case ERROR_SUCCESS:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->InitbyINI(GetHandleInformation()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", stCurDirBuff);
			break;
		default:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->InitbyINI(GetHandleInformation()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", stCurDirBuff);
			ErrorHandle::GetErrorHandleInst()->ErrorExit(_T("InitDirbyINI->GetHandleInformation"), _T("."), dwErrorCode);
		}
	}
	// 4. Get INI-file size
	SetLastError(ERROR_SUCCESS);
	try
	{
		liFileSize = GetFileSize(hFile,NULL);
		dwErrorCode = GetLastError();
		if (bStatus == 0)
		{
			CloseHandle(hFile);
			Logger::GetLogInstance()->PrepareTXTLOG("Function->InitbyINI(GetFileSizeEx()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", stCurDirBuff);
			ErrorHandle::GetErrorHandleInst()->ErrorExit(_T("InitDirbyINI->GetFileSizeEx()"), _T("."), dwErrorCode);
			return FALSE;
		}
		throw dwErrorCode;
	}
	catch(DWORD Err_code)
	{
		switch (Err_code)
		{
		case ERROR_SUCCESS:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->InitbyINI(GetFileSizeEx()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", stCurDirBuff);
			break;
		default:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->InitbyINI(GetFileSizeEx()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", stCurDirBuff);
			ErrorHandle::GetErrorHandleInst()->ErrorExit(_T("InitDirbyINI->GetFileSizeEx()"), _T("."), dwErrorCode);
			return FALSE;
		}
	}
	//5. Read INI-file
	SetLastError(ERROR_SUCCESS);
	DWORD nBytesToRead = liFileSize;
	std::basic_string<TCHAR> stINIbuffer(nBytesToRead,0);
	//TCHAR *InPointDirBuff = new TCHAR[nBytesToRead];
	//InPointDirBuff[nBytesToRead/2] = 0;
	DWORD nBytesReaded;
	LARGE_INTEGER iDist = { 0 };
	LARGE_INTEGER iPos = { 0 };
	BOOL bErrorFlag = FALSE;
	try
	{	
		bErrorFlag = SetFilePointerEx(hFile, iDist, &iPos, FILE_BEGIN);
		dwErrorCode = GetLastError();
		if (dStatus == INVALID_SET_FILE_POINTER)
		{
			CloseHandle(hFile);
			Logger::GetLogInstance()->PrepareTXTLOG("Function->InitbyINI(SetFilePointerEx()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", stCurDirBuff);
			ErrorHandle::GetErrorHandleInst()->ErrorExit(_T("InitDirbyINI->SetFilePointerEx()"), _T("."), dwErrorCode);
			return FALSE;
		}
		throw dwErrorCode;
	}
	catch (DWORD Err_code)
	{
		switch (Err_code)
		{
		case ERROR_SUCCESS:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->InitbyINI(SetFilePointerEx()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", stCurDirBuff);
			break;
		default:
			CloseHandle(hFile);
			Logger::GetLogInstance()->PrepareTXTLOG("Function->InitbyINI(SetFilePointerEx()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", stCurDirBuff);
			ErrorHandle::GetErrorHandleInst()->ErrorExit(_T("InitDirbyINI->SetFilePointerEx()"), _T("."), dwErrorCode);
		}
	}
	SetLastError(ERROR_SUCCESS);
	try
	{
		if (dStatus != INVALID_SET_FILE_POINTER)
			bStatus = ReadFile(hFile, &stINIbuffer[0], nBytesToRead, &nBytesReaded, NULL);	// read all data from INI-file
		dwErrorCode = GetLastError();
		throw dwErrorCode;
	}
	catch (DWORD Err_code)
	{
		switch (Err_code)
		{
		case ERROR_SUCCESS:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->InitbyINI(ReadFile()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", stCurDirBuff);
			break;
		default:
			CloseHandle(hFile);
			Logger::GetLogInstance()->PrepareTXTLOG("Function->InitbyINI(ReadFile()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", stCurDirBuff);
			ErrorHandle::GetErrorHandleInst()->ErrorExit(_T("InitDirbyINI->+ReadFile()"), _T("."), dwErrorCode);
		}
	}
	
	std::vector<std::basic_string<TCHAR>> vstINI;
	std::size_t nStartFrom = 0;
	std::size_t nLocCRLF = 0;
	std::basic_string<TCHAR> stCRLF = _T("\r\n");
	//std::basic_string<TCHAR> stTir = _T("-");
	//std::basic_string<TCHAR> stComa = _T(";");
	std::size_t nLoc = 0;
	
	SetLastError(ERROR_SUCCESS);
	try
	{
		while (nLocCRLF != std::basic_string<TCHAR>::npos)
		{
			nLocCRLF = stINIbuffer.find(stCRLF, nStartFrom);
			vstINI.push_back(stINIbuffer.substr(nStartFrom, (nLocCRLF - nStartFrom)));
			stINIbuffer.erase(nStartFrom, nLocCRLF + 2);
		}
		//
		nLoc = vstINI[0].find_first_of('-', nStartFrom);
		vstINI[0].erase(nStartFrom, nLoc + 1);
		while (nLoc != std::basic_string<TCHAR>::npos)
		{
			nLoc = vstINI[0].find(';', nStartFrom);
			vstFolderstoParse.push_back(vstINI[0].substr(nStartFrom, (nLoc - nStartFrom)));
			vstINI[0].erase(nStartFrom, nLoc + 1);
		}
		vstINI.erase(vstINI.begin());
		nLoc = 0;
		//
		nLoc = vstINI[0].find_first_of('-', nStartFrom);
		vstINI[0].erase(nStartFrom, nLoc + 1);
		while (nLoc != std::basic_string<TCHAR>::npos)
		{
			
			nLoc = vstINI[0].find(';', nStartFrom);
			vstFolderstoIgnore.push_back(vstINI[0].substr(nStartFrom, (nLoc - nStartFrom)));
			vstINI[0].erase(nStartFrom, nLoc + 1);
		}
		vstINI.erase(vstINI.begin());
		nLoc = 0;
		//
		nLoc = vstINI[0].find_first_of('-', nStartFrom);
		vstINI[0].erase(nStartFrom, nLoc + 1);
		while (nLoc != std::basic_string<TCHAR>::npos)
		{
			nLoc = vstINI[0].find(';', nStartFrom);
			stChkStartDateTime.assign(vstINI[0].substr(nStartFrom, (nLoc - nStartFrom)).c_str());
			vstINI[0].erase(nStartFrom, nLoc + 1);
		}
		vstINI.erase(vstINI.begin());
		nLoc = 0;
		//
		nLoc = vstINI[0].find_first_of('-', nStartFrom);
		vstINI[0].erase(nStartFrom, nLoc + 1);
		while (nLoc != std::basic_string<TCHAR>::npos)
		{
			nLoc = vstINI[0].find(';', nStartFrom);
			stChkEndDateTime.assign(vstINI[0].substr(nStartFrom, (nLoc - nStartFrom)).c_str());
			vstINI[0].erase(nStartFrom, nLoc + 1);
		}
		vstINI.erase(vstINI.begin());
		nLoc = 0;
		//
		nLoc = vstINI[0].find_first_of('-', nStartFrom);
		vstINI[0].erase(nStartFrom, nLoc + 1);
		while (nLoc != std::basic_string<TCHAR>::npos)
		{
			nLoc = vstINI[0].find(';', nStartFrom);
			vstRootFolders.push_back(vstINI[0].substr(nStartFrom, (nLoc - nStartFrom)));
			vstINI[0].erase(nStartFrom, nLoc + 1);
		}
		vstINI.erase(vstINI.begin());
		nLoc = 0;
		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		nLoc = vstINI[0].find_first_of('-', nStartFrom);
		vstINI[0].erase(nStartFrom, nLoc + 1);
		while (nLoc != std::basic_string<TCHAR>::npos)
		{
			nLoc = vstINI[0].find(';', nStartFrom);
			vstErrorsTableName.assign(vstINI[0].substr(nStartFrom, (nLoc - nStartFrom)).c_str());
			vstINI[0].erase(nStartFrom, nLoc + 1);
		}
		vstINI.erase(vstINI.begin());
		nLoc = 0;
		//
		nLoc = vstINI[0].find_first_of('-', nStartFrom);
		vstINI[0].erase(nStartFrom, nLoc + 1);
		while (nLoc != std::basic_string<TCHAR>::npos)
		{
			nLoc = vstINI[0].find(';', nStartFrom);
			vstErrorsTableFields.push_back(vstINI[0].substr(nStartFrom, (nLoc - nStartFrom)));
			vstINI[0].erase(nStartFrom, nLoc + 1);
		}
		vstINI.erase(vstINI.begin());
		nLoc = 0;
		//
		nLoc = vstINI[0].find_first_of('-', nStartFrom);
		vstINI[0].erase(nStartFrom, nLoc + 1);
		while (nLoc != std::basic_string<TCHAR>::npos)
		{

			nLoc = vstINI[0].find(';', nStartFrom);
			vstExtensTableName.assign(vstINI[0].substr(nStartFrom, (nLoc - nStartFrom)).c_str());
			vstINI[0].erase(nStartFrom, nLoc + 1);
		}
		vstINI.erase(vstINI.begin());
		nLoc = 0;
		//
		nLoc = vstINI[0].find_first_of('-', nStartFrom);
		vstINI[0].erase(nStartFrom, nLoc + 1);
		while (nLoc != std::basic_string<TCHAR>::npos)
		{
			nLoc = vstINI[0].find(';', nStartFrom);
			vstExtensTableFields.push_back(vstINI[0].substr(nStartFrom, (nLoc - nStartFrom)));
			vstINI[0].erase(nStartFrom, nLoc + 1);
		}
		vstINI.erase(vstINI.begin());
		nLoc = 0;
		//
		nLoc = vstINI[0].find_first_of('-', nStartFrom);
		vstINI[0].erase(nStartFrom, nLoc + 1);
		while (nLoc != std::basic_string<TCHAR>::npos)
		{
			nLoc = vstINI[0].find(';', nStartFrom);
			vstFilesTableName.assign(vstINI[0].substr(nStartFrom, (nLoc - nStartFrom)).c_str());
			vstINI[0].erase(nStartFrom, nLoc + 1);
		}
		vstINI.erase(vstINI.begin());
		nLoc = 0;
		//
		nLoc = vstINI[0].find_first_of('-', nStartFrom);
		vstINI[0].erase(nStartFrom, nLoc + 1);
		while (nLoc != std::basic_string<TCHAR>::npos)
		{
			nLoc = vstINI[0].find(';', nStartFrom);
			vstFilesTableFields.push_back(vstINI[0].substr(nStartFrom, (nLoc - nStartFrom)));
			vstINI[0].erase(nStartFrom, nLoc + 1);
		}
		vstINI.erase(vstINI.begin());
		nLoc = 0;
		//
		nLoc = vstINI[0].find_first_of('-', nStartFrom);
		vstINI[0].erase(nStartFrom, nLoc + 1);
		while (nLoc != std::basic_string<TCHAR>::npos)
		{
			nLoc = vstINI[0].find(';', nStartFrom);
			vstFoldersTableName.assign(vstINI[0].substr(nStartFrom, (nLoc - nStartFrom)).c_str());
			vstINI[0].erase(nStartFrom, nLoc + 1);
		}
		vstINI.erase(vstINI.begin());
		nLoc = 0;
		//
		nLoc = vstINI[0].find_first_of('-', nStartFrom);
		vstINI[0].erase(nStartFrom, nLoc + 1);
		while (nLoc != std::basic_string<TCHAR>::npos)
		{
			nLoc = vstINI[0].find(';', nStartFrom);
			vstFoldersTableFields.push_back(vstINI[0].substr(nStartFrom, (nLoc - nStartFrom)));
			vstINI[0].erase(nStartFrom, nLoc + 1);
		}
		vstINI.erase(vstINI.begin());
		nLoc = 0;
		//
		nLoc = vstINI[0].find_first_of('-', nStartFrom);
		vstINI[0].erase(nStartFrom, nLoc + 1);
		while (nLoc != std::basic_string<TCHAR>::npos)
		{
			nLoc = vstINI[0].find(';', nStartFrom);
			vstUsersTableName.assign(vstINI[0].substr(nStartFrom, (nLoc - nStartFrom)).c_str());
			vstINI[0].erase(nStartFrom, nLoc + 1);
		}
		vstINI.erase(vstINI.begin());
		nLoc = 0;
		//
		nLoc = vstINI[0].find_first_of('-', nStartFrom);
		vstINI[0].erase(nStartFrom, nLoc + 1);
		while (nLoc != std::basic_string<TCHAR>::npos)
		{
			nLoc = vstINI[0].find(';', nStartFrom);
			vstUsersTableFields.push_back(vstINI[0].substr(nStartFrom, (nLoc - nStartFrom)));
			vstINI[0].erase(nStartFrom, nLoc + 1);
		}
		vstINI.erase(vstINI.begin());
		nLoc = 0;
		//
		nLoc = vstINI[0].find_first_of('-', nStartFrom);
		vstINI[0].erase(nStartFrom, nLoc + 1);
		while (nLoc != std::basic_string<TCHAR>::npos)
		{
			nLoc = vstINI[0].find(';', nStartFrom);
			vstProjectsTableName.assign(vstINI[0].substr(nStartFrom, (nLoc - nStartFrom)).c_str());
			vstINI[0].erase(nStartFrom, nLoc + 1);
		}
		vstINI.erase(vstINI.begin());
		nLoc = 0;
		//
		nLoc = vstINI[0].find_first_of('-', nStartFrom);
		vstINI[0].erase(nStartFrom, nLoc + 1);
		while (nLoc != std::basic_string<TCHAR>::npos)
		{
			nLoc = vstINI[0].find(';', nStartFrom);
			vstProjectsTableFields.push_back(vstINI[0].substr(nStartFrom, (nLoc - nStartFrom)));
			vstINI[0].erase(nStartFrom, nLoc + 1);
		}
		vstINI.erase(vstINI.begin());
		nLoc = 0;
		//
		nLoc = vstINI[0].find_first_of('-', nStartFrom);
		vstINI[0].erase(nStartFrom, nLoc + 1);
		while (nLoc != std::basic_string<TCHAR>::npos)
		{
			nLoc = vstINI[0].find(';', nStartFrom);
			vstCompaniesTableName.assign(vstINI[0].substr(nStartFrom, (nLoc - nStartFrom)).c_str());
			vstINI[0].erase(nStartFrom, nLoc + 1);
		}
		vstINI.erase(vstINI.begin());
		nLoc = 0;
		//
		nLoc = vstINI[0].find_first_of('-', nStartFrom);
		vstINI[0].erase(nStartFrom, nLoc + 1);
		while (nLoc != std::basic_string<TCHAR>::npos)
		{
			nLoc = vstINI[0].find(';', nStartFrom);
			vstCompaniesTableFields.push_back(vstINI[0].substr(nStartFrom, (nLoc - nStartFrom)));
			vstINI[0].erase(nStartFrom, nLoc + 1);
		}
		vstINI.erase(vstINI.begin());
		nLoc = 0;
		//
		nLoc = vstINI[0].find_first_of('-', nStartFrom);
		vstINI[0].erase(nStartFrom, nLoc + 1);
		while (nLoc != std::basic_string<TCHAR>::npos)
		{
			nLoc = vstINI[0].find(';', nStartFrom);
			vstRolesTableName.assign(vstINI[0].substr(nStartFrom, (nLoc - nStartFrom)).c_str());
			vstINI[0].erase(nStartFrom, nLoc + 1);
		}
		vstINI.erase(vstINI.begin());
		nLoc = 0;
		//
		nLoc = vstINI[0].find_first_of('-', nStartFrom);
		vstINI[0].erase(nStartFrom, nLoc + 1);
		while (nLoc != std::basic_string<TCHAR>::npos)
		{
			nLoc = vstINI[0].find(';', nStartFrom);
			vstRolesTableFields.push_back(vstINI[0].substr(nStartFrom, (nLoc - nStartFrom)));
			vstINI[0].erase(nStartFrom, nLoc + 1);
		}
		vstINI.erase(vstINI.begin());
		nLoc = 0;
		//
		nLoc = vstINI[0].find_first_of('-', nStartFrom);
		vstINI[0].erase(nStartFrom, nLoc + 1);
		while (nLoc != std::basic_string<TCHAR>::npos)
		{
			nLoc = vstINI[0].find(';', nStartFrom);
			vstSubsystemsTableName.assign(vstINI[0].substr(nStartFrom, (nLoc - nStartFrom)).c_str());
			vstINI[0].erase(nStartFrom, nLoc + 1);
		}
		vstINI.erase(vstINI.begin());
		nLoc = 0;
		//
		nLoc = vstINI[0].find_first_of('-', nStartFrom);
		vstINI[0].erase(nStartFrom, nLoc + 1);
		while (nLoc != std::basic_string<TCHAR>::npos)
		{
			nLoc = vstINI[0].find(';', nStartFrom);
			vstSubsystemsTableFields.push_back(vstINI[0].substr(nStartFrom, (nLoc - nStartFrom)));
			vstINI[0].erase(nStartFrom, nLoc + 1);
		}
		vstINI.erase(vstINI.begin());
		nLoc = 0;
		//
		nLoc = vstINI[0].find_first_of('-', nStartFrom);
		vstINI[0].erase(nStartFrom, nLoc + 1);
		while (nLoc != std::basic_string<TCHAR>::npos)
		{
			nLoc = vstINI[0].find(';', nStartFrom);
			vstDatatypesTableName.assign(vstINI[0].substr(nStartFrom, (nLoc - nStartFrom)).c_str());
			vstINI[0].erase(nStartFrom, nLoc + 1);
		}
		vstINI.erase(vstINI.begin());
		nLoc = 0;
		//
		nLoc = vstINI[0].find_first_of('-', nStartFrom);
		vstINI[0].erase(nStartFrom, nLoc + 1);
		while (nLoc != std::basic_string<TCHAR>::npos)
		{
			nLoc = vstINI[0].find(';', nStartFrom);
			vstDatatypesTableFields.push_back(vstINI[0].substr(nStartFrom, (nLoc - nStartFrom)));
			vstINI[0].erase(nStartFrom, nLoc + 1);
		}
		vstINI.erase(vstINI.begin());
		nLoc = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		nLoc = vstINI[0].find_first_of('-', nStartFrom);
		vstINI[0].erase(nStartFrom, nLoc + 1);
		while (nLoc != std::basic_string<TCHAR>::npos)
		{
			nLoc = vstINI[0].find(';', nStartFrom);
			vstMySQl_Hostname.assign(vstINI[0].substr(nStartFrom, (nLoc - nStartFrom)).c_str());
			vstINI[0].erase(nStartFrom, nLoc + 1);
		}
		vstINI.erase(vstINI.begin());
		nLoc = 0;
		//
		nLoc = vstINI[0].find_first_of('-', nStartFrom);
		vstINI[0].erase(nStartFrom, nLoc + 1);
		while (nLoc != std::basic_string<TCHAR>::npos)
		{
			nLoc = vstINI[0].find(';', nStartFrom);
			vstMySQl_Login.assign(vstINI[0].substr(nStartFrom, (nLoc - nStartFrom)).c_str());
			vstINI[0].erase(nStartFrom, nLoc + 1);
		}
		vstINI.erase(vstINI.begin());
		nLoc = 0;
		//
		nLoc = vstINI[0].find_first_of('-', nStartFrom);
		vstINI[0].erase(nStartFrom, nLoc + 1);
		while (nLoc != std::basic_string<TCHAR>::npos)
		{
			nLoc = vstINI[0].find(';', nStartFrom);
			vstMySQl_Password.assign(vstINI[0].substr(nStartFrom, (nLoc - nStartFrom)).c_str());
			vstINI[0].erase(nStartFrom, nLoc + 1);
		}
		vstINI.erase(vstINI.begin());
		nLoc = 0;
		//
		nLoc = vstINI[0].find_first_of('-', nStartFrom);
		vstINI[0].erase(nStartFrom, nLoc + 1);
		while (nLoc != std::basic_string<TCHAR>::npos)
		{
			nLoc = vstINI[0].find(';', nStartFrom);
			vstMySQl_DefSchema.assign(vstINI[0].substr(nStartFrom, (nLoc - nStartFrom)).c_str());
			vstINI[0].erase(nStartFrom, nLoc + 1);
		}
		vstINI.erase(vstINI.begin());
		nLoc = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		nLoc = vstINI[0].find_first_of('-', nStartFrom);
		vstINI[0].erase(nStartFrom, nLoc + 1);
		while (nLoc != std::basic_string<TCHAR>::npos)
		{
			nLoc = vstINI[0].find(';', nStartFrom);
			vstChk_FileMaskCommon.assign(vstINI[0].substr(nStartFrom, (nLoc - nStartFrom)).c_str());
			vstINI[0].erase(nStartFrom, nLoc + 1);
		}
		vstINI.erase(vstINI.begin());
		nLoc = 0;
		//
		nLoc = vstINI[0].find_first_of('-', nStartFrom);
		vstINI[0].erase(nStartFrom, nLoc + 1);
		while (nLoc != std::basic_string<TCHAR>::npos)
		{
			nLoc = vstINI[0].find(';', nStartFrom);
			vstChk_Cyrillic.assign(vstINI[0].substr(nStartFrom, (nLoc - nStartFrom)).c_str());
			vstINI[0].erase(nStartFrom, nLoc + 1);
		}
		vstINI.erase(vstINI.begin());
		nLoc = 0;
		//
		nLoc = vstINI[0].find_first_of('-', nStartFrom);
		vstINI[0].erase(nStartFrom, nLoc + 1);
		while (nLoc != std::basic_string<TCHAR>::npos)
		{
			nLoc = vstINI[0].find(';', nStartFrom);
			vstChk_Company.assign(vstINI[0].substr(nStartFrom, (nLoc - nStartFrom)).c_str());
			vstINI[0].erase(nStartFrom, nLoc + 1);
		}
		vstINI.erase(vstINI.begin());
		nLoc = 0;
		//
		nLoc = vstINI[0].find_first_of('-', nStartFrom);
		vstINI[0].erase(nStartFrom, nLoc + 1);
		while (nLoc != std::basic_string<TCHAR>::npos)
		{
			nLoc = vstINI[0].find(';', nStartFrom);
			vstChk_Role.assign(vstINI[0].substr(nStartFrom, (nLoc - nStartFrom)).c_str());
			vstINI[0].erase(nStartFrom, nLoc + 1);
		}
		vstINI.erase(vstINI.begin());
		nLoc = 0;
		//
		nLoc = vstINI[0].find_first_of('-', nStartFrom);
		vstINI[0].erase(nStartFrom, nLoc + 1);
		while (nLoc != std::basic_string<TCHAR>::npos)
		{
			nLoc = vstINI[0].find_first_of(';', nStartFrom);
			vstChk_Stage.assign(vstINI[0].substr(nStartFrom, (nLoc - nStartFrom)).c_str());
			vstINI[0].erase(nStartFrom, nLoc + 1);
		}
		vstINI.erase(vstINI.begin());
		nLoc = 0;
		//
		nLoc = vstINI[0].find_first_of('-', nStartFrom);
		vstINI[0].erase(nStartFrom, nLoc + 1);
		while (nLoc != std::basic_string<TCHAR>::npos)
		{
			nLoc = vstINI[0].find(';', nStartFrom);
			vstChk_FindDStageShaPubZZ.assign(vstINI[0].substr(nStartFrom, (nLoc - nStartFrom)).c_str());
			vstINI[0].erase(nStartFrom, nLoc + 1);
		}
		vstINI.erase(vstINI.begin());
		nLoc = 0;
		//
		nLoc = vstINI[0].find_first_of('-', nStartFrom);
		vstINI[0].erase(nStartFrom, nLoc + 1);
		while (nLoc != std::basic_string<TCHAR>::npos)
		{
			nLoc = vstINI[0].find(';', nStartFrom);
			vstChk_FileMaskDStageShaPubZZ.assign(vstINI[0].substr(nStartFrom, (nLoc - nStartFrom)).c_str());
			vstINI[0].erase(nStartFrom, nLoc + 1);
		}
		vstINI.erase(vstINI.begin());
		nLoc = 0;
		dwErrorCode = GetLastError();
		throw dwErrorCode;
	}
	catch (DWORD Err_code)
	{
		switch (Err_code)
		{
		case ERROR_SUCCESS:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->InitbyINI(ParseINI()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", stCurDirBuff);
			break;
		default:
			CloseHandle(hFile);
			Logger::GetLogInstance()->PrepareTXTLOG("Function->InitbyINI(ParseINI()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", stCurDirBuff);
			ErrorHandle::GetErrorHandleInst()->ErrorExit(_T("InitDirbyINI->ParseINI()"), _T("."), dwErrorCode);
		}
	}
	CloseHandle(hFile);
	return dwErrorCode;
}
DWORD UI::ConvertDateTime()
{
	DWORD dwErrorcode;
	std::size_t nStartFrom = 0;
	//std::basic_string<TCHAR> stComa = _T(":");
	std::size_t nLoc = 0;

	//LPSYSTEMTIME lpSystemTime;

	while (nLoc != std::basic_string<TCHAR>::npos)
	{
		nLoc = stChkStartDateTime.find(':', nStartFrom);
		vstChkStartDateTime.push_back(stChkStartDateTime.substr(nStartFrom, (nLoc - nStartFrom)));
		stChkStartDateTime.erase(nStartFrom, nLoc + 1);
	}
	nLoc = 0;
	//while (nLoc != std::basic_string<TCHAR>::npos)
	//{
	//	nLoc = stChkEndDateTime.find(':', nStartFrom);
	//	vstChkEndDateTime.push_back(stChkEndDateTime.substr(nStartFrom, (nLoc - nStartFrom)));
	//	stChkEndDateTime.erase(nStartFrom, nLoc + 1);
	//}
	//nLoc = 0;
	//std::wcout << vstChkStartDateTime[0].c_str();

	stStartChkPeriod.wYear = _wtoi(vstChkStartDateTime[0].c_str());
	stStartChkPeriod.wMonth = _wtoi(vstChkStartDateTime[1].c_str());
	stStartChkPeriod.wDay = _wtoi(vstChkStartDateTime[2].c_str());
	stStartChkPeriod.wHour = _wtoi(vstChkStartDateTime[3].c_str());
	stStartChkPeriod.wMinute = _wtoi(vstChkStartDateTime[4].c_str());
	stStartChkPeriod.wSecond = _wtoi(vstChkStartDateTime[5].c_str());

	//stEndChkPeriod.wYear = _wtoi(vstChkEndDateTime[0].c_str());
	//stEndChkPeriod.wMonth = _wtoi(vstChkEndDateTime[1].c_str());
	//stEndChkPeriod.wDay = _wtoi(vstChkEndDateTime[2].c_str());
	//stEndChkPeriod.wHour = _wtoi(vstChkEndDateTime[3].c_str());
	//stEndChkPeriod.wMinute = _wtoi(vstChkEndDateTime[4].c_str());
	//stEndChkPeriod.wSecond = _wtoi(vstChkEndDateTime[5].c_str());

	GetSystemTime(&stEndChkPeriod);

	//std::cout << stEndChkPeriod.wYear << " " << stEndChkPeriod.wMonth << " " << stEndChkPeriod.wDay << " " << stEndChkPeriod.wHour << " " << stEndChkPeriod.wMinute << " " << stEndChkPeriod.wSecond;

	dwErrorcode = ERROR_SUCCESS;
	return dwErrorcode;
};