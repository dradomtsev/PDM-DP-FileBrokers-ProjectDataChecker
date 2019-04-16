// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"

FileProcess::FileProcess() : sFileInfoInst()
{
	iIndex			= 0;
	npos			= -1;
	//vstExistExtens	= { _T(" ") };
	//vstExistDirs	= { _T(" ") };
	curSysTime		= { 0 };
	curFileTime		= { 0 };
	//tStartChkPeriod = { 0 };
	//tEndChkPeriod	= { 0 };
	//sFileInfoInst	= { 0 };
	regFindDStageShaPubZZ	= UI::GetUIInst().vstChk_FindDStageShaPubZZ.c_str();
	regMaskDStageShaPubZZ	= UI::GetUIInst().vstChk_FileMaskDStageShaPubZZ.c_str();
	regMaskDCompanyShaPubZZ = (_T("[\\[\\(](\\w+)[\\]\\)]"));
	regMaskCommon			= UI::GetUIInst().vstChk_FileMaskCommon.c_str();
	regMaskCyrillic			= UI::GetUIInst().vstChk_Cyrillic.c_str();
	regMaskCompany			= UI::GetUIInst().vstChk_Company.c_str();
	regMaskRole				= UI::GetUIInst().vstChk_Role.c_str();
	regMaskStage			= UI::GetUIInst().vstChk_Stage.c_str();
	stRegFileStructbyMask	= _T("-");
	stRegRoleMask = (_T("^(\\d[a-zA-Z][a-zA-Z][a-zA-Z]|\\d[a-zA-Z][a-zA-Z]|\\d[a-zA-Z]|[a-zA-Z][a-zA-Z][a-zA-Z][a-zA-Z]|[a-zA-Z][a-zA-Z][a-zA-Z]|[a-zA-Z][a-zA-Z]|[a-zA-Z])"));
	stOfficeOwnerFileMask = _T("\\~\\$");
};
FileProcess::~FileProcess() {};

DWORD FileProcess::IterDirs(std::basic_string<TCHAR> stWorkDir)
{
	DWORD dwErrorCode = -1;
	DWORD dStatus = -1;
	BOOL bStatus = FALSE;
	SetLastError(ERROR_SUCCESS);

	// Init main recursion loop & write results to DB
	int iCounter = 0;
	try
	{
		bStatus = this->IterObjects(stWorkDir, _T(""), iCounter);
		dwErrorCode = GetLastError();
		throw  dwErrorCode;
	}
	catch (DWORD& dwErrorCode)
	{
		switch (dwErrorCode)
		{
		case ERROR_SUCCESS:
		case ERROR_NO_MORE_FILES:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->IterDirs(IterObjects()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", "MySQL tables");
			break;
		default:
			DBProcess::dbProcInstance().DBCloseMYSQLConnection();
			Logger::GetLogInstance()->PrepareTXTLOG("Function->IterDirs(IterObjects()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", "MySQL tables");
			ErrorHandle::GetErrorHandleInst()->ErrorExit(_T("IterDirs(IterObjects())"), (LPTSTR)stWorkDir.c_str(), dwErrorCode);
		}
	}
	return  dwErrorCode;
}
DWORD FileProcess::IterObjects(std::basic_string<TCHAR> twrkDir,const std::basic_string<TCHAR> twrkDirName, int iCounter)
{
	WIN32_FIND_DATA fdFileData = { 0 };
	HANDLE hFileDataFindFirst = nullptr;
	//HANDLE hFileReparse = nullptr;
	SetLastError(ERROR_SUCCESS);
	DWORD dwErrorCode = -1;
	BOOL bStatus = FALSE;
	BOOL bmatchResult = FALSE;
	//0. Set current working directory
	std::basic_string<TCHAR> twrkDirtemp = twrkDir + twrkDirName;

	wprintf_s(_T("Working in %s\r\n"), twrkDirtemp.c_str());

	std::basic_string<TCHAR> wPathInsert(_T("\\\\?\\"));
	//std::basic_regex<TCHAR> stRegSearch(_T("\\\?"));
	//std::basic_string<TCHAR> wPathInsert(_T("\\\\?\\UNC\\"));
	//std::basic_regex<TCHAR> stRegSearch(_T("\[UNC]"));
	//bmatchResult = std::regex_search(twrkDirtemp, stRegSearch);
	//if(bmatchResult != TRUE)
		twrkDirtemp.insert(0, wPathInsert.c_str());

	
	try
	{
		bStatus = SetCurrentDirectory((LPCWSTR)twrkDirtemp.c_str());
		dwErrorCode = GetLastError();
		++iCounter;
		//if (dwErrorCode == ERROR_FILENAME_EXCED_RANGE)
		//{
		//	Logger::GetLogInstance()->PrepareTXTLOG("Function->IterObjects(SetCurrentDirectory()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", twrkDirtemp);
		//	Logger::GetLogInstance()->PrepareMySQLLOG("Error path is greater than MAX_PATH (260 symbols)", "Function->IterObjects(SetCurrentDirectory())", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCode, twrkDirtemp);
		//	return dwErrorCode;
		//}
		//switch (bStatus)
		//{
		//	
		//	case FALSE:
		//	{
		//		switch (dwErrorCode)
		//		{
		//			case ERROR_SUCCESS:
		//				break;
		//			case ERROR_FILE_NOT_FOUND:
		//			case ERROR_PATH_NOT_FOUND:
		//			case ERROR_ACCESS_DENIED:
		//			{
		//				Logger::GetLogInstance()->PrepareMySQLLOG("Error cannot set current directory", "Function->IterObjects(SetCurrentDirectory())", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCode, twrkDirtemp);
		//				return dwErrorCode;
		//			}
		//			default:
		//			{
		//				Logger::GetLogInstance()->PrepareTXTLOG("Function->IterObjects(SetCurrentDirectory()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", twrkDirtemp);
		//				Logger::GetLogInstance()->PrepareMySQLLOG("Error cannot set current directory", "Function->IterObjects(SetCurrentDirectory())", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCode, twrkDirtemp);
		//				ErrorHandle::GetErrorHandleInst()->ErrorExit(_T("IterObjects->SetCurrentDirectory()"), (LPTSTR)twrkDirtemp.c_str(), dwErrorCode);
		//			}
		//		}
		//	}
		//}
		throw  dwErrorCode;
	}
	catch (DWORD)
	{
		switch (dwErrorCode)
		{
		case ERROR_SUCCESS:
		{
			Logger::GetLogInstance()->PrepareTXTLOG("Function->IterObjects(SetCurrentDirectory()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", twrkDirtemp);
			break;
		}
		case ERROR_FILENAME_EXCED_RANGE:
		case ERROR_NO_MORE_FILES:
		case ERROR_FILE_NOT_FOUND:
		case ERROR_PATH_NOT_FOUND:
		case ERROR_ACCESS_DENIED:
		{
			Logger::GetLogInstance()->PrepareTXTLOG("Function->IterObjects(SetCurrentDirectory()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", twrkDirtemp);
			Logger::GetLogInstance()->PrepareMySQLLOG("Error cannot set current directory", "Function->IterObjects(SetCurrentDirectory())", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCode, twrkDirtemp);
			bStatus = SetCurrentDirectory((LPCWSTR)&twrkDir[0]);
			wprintf_s(_T("Going back to %s\r\n"), twrkDir.c_str());
			SetLastError(ERROR_SUCCESS);
			dwErrorCode = ERROR_SUCCESS;
			return dwErrorCode;
		}
		default:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->IterObjects(SetCurrentDirectory()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", twrkDirtemp);
			Logger::GetLogInstance()->PrepareMySQLLOG("Error cannot set current directory", "Function->IterObjects(SetCurrentDirectory())", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCode, twrkDirtemp);
			ErrorHandle::GetErrorHandleInst()->ErrorExit(_T("IterObjects->SetCurrentDirectory()"), (LPTSTR)twrkDirtemp.c_str(), dwErrorCode);
		}
	}
	try
	{
		twrkDirtemp.erase(0, 4);
		if (!twrkDirtemp.empty() && (twrkDirtemp[twrkDirtemp.length() - 1] != _T('\\')))
			twrkDirtemp.append(_T("\\"));
		hFileDataFindFirst = FindFirstFile((twrkDirtemp + _T('*')).c_str(), &fdFileData);
		dwErrorCode = GetLastError();
		if (hFileDataFindFirst == INVALID_HANDLE_VALUE)
		{
			Logger::GetLogInstance()->PrepareTXTLOG("Function->IterObjects(FindFirstFile()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", twrkDirtemp);
			Logger::GetLogInstance()->PrepareMySQLLOG("Error cannot find first file in current directory", "Function->IterObjects(FindFirstFile())", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCode, twrkDirtemp);
			FindClose(hFileDataFindFirst);
			return  dwErrorCode;
		}
		throw  dwErrorCode;
	}
	catch (DWORD)
	{
		switch (dwErrorCode)
		{
		case ERROR_SUCCESS:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->IterObjects(FindFirstFile()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", twrkDirtemp);
			break;
		default:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->IterObjects(FindFirstFile()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", twrkDirtemp);
			Logger::GetLogInstance()->PrepareMySQLLOG("Error cannot find first file in current directory", "Function->IterObjects(FindFirstFile())", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCode, twrkDirtemp);
			ErrorHandle::GetErrorHandleInst()->ErrorExit(_T("IterObjects->FindFirstFile()"), (LPTSTR)twrkDirtemp.c_str(), dwErrorCode);
			return FALSE;
		}
	}
	try
	{
		SetLastError(ERROR_SUCCESS);
		do
		{
			this->sFileInfoInst.sFileName = fdFileData.cFileName;
			//if (fdFileData.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT)
			//{
			//	if (fdFileData.dwReserved0 & IO_REPARSE_TAG_SYMLINK)
			//	{
			//		//stFilePathtemp.insert(0, _T("\\\\?\\"));
			//		stFilePathtemp.append(twrkDirtemp);
			//		stFilePathtemp.append(this->sFileInfoInst.sFileName);
			//		hFileReparse = CreateFile(stFilePathtemp.c_str(), FILE_READ_ATTRIBUTES, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS | FILE_ATTRIBUTE_REPARSE_POINT | FILE_FLAG_OPEN_REPARSE_POINT, NULL); //READ_CONTROL
			//		dwErrorCode = GetLastError();
			//		if (dwErrorCode == ERROR_ACCESS_DENIED && hFileReparse == INVALID_HANDLE_VALUE)
			//		{
			//			Logger::GetLogInstance()->PrepareTXTLOG("Function->IterObjects(CreateFile_Reparse()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", twrkDirtemp);
			//			Logger::GetLogInstance()->PrepareMySQLLOG("Error access denied->invalid file handle value", "Function->IterObjects(CreateFile_Reparse())", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCode, twrkDirtemp);
			//			if (hFileReparse != nullptr)
			//				CloseHandle(hFileReparse);
			//			return dwErrorCode;
			//		}
			//		if (dwErrorCode == ERROR_FILENAME_EXCED_RANGE)
			//		{
			//			Logger::GetLogInstance()->PrepareTXTLOG("Function->IterObjects(CreateFile_Reparse()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", twrkDirtemp);
			//			Logger::GetLogInstance()->PrepareMySQLLOG("Error path is greater than MAX_PATH (260 symbols)", "Function->IterObjects(CreateFile_Reparse())", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCode, twrkDirtemp);
			//			if (hFileReparse != nullptr)
			//				CloseHandle(hFileReparse);
			//			return dwErrorCode;
			//		}
			//	}
			//	if (hFileReparse != INVALID_HANDLE_VALUE)
			//	{
			//		bStatus = GetFileInformationByHandle(hFileReparse, &hfiFileReparse);
			//		TCHAR Path[MAX_PATH];
			//		dwErrorCode = GetFinalPathNameByHandle(hFileReparse, Path, MAX_PATH, FILE_NAME_NORMALIZED);
			//		dwErrorCode = GetLastError();
			//		if (bStatus != FALSE)
			//		{
			//			fdFileData.dwFileAttributes = hfiFileReparse.dwFileAttributes;
			//			CloseHandle(hFileReparse);
			//		}
			//	}
			//}
			if (fdFileData.dwFileAttributes & FILE_ATTRIBUTE_DEVICE)
				continue;
			else if (fdFileData.dwFileAttributes & FILE_ATTRIBUTE_INTEGRITY_STREAM)
				continue;
			else if (fdFileData.dwFileAttributes & FILE_ATTRIBUTE_NO_SCRUB_DATA)
				continue;
			else if (fdFileData.dwFileAttributes & FILE_ATTRIBUTE_OFFLINE)
				continue;
			else if (fdFileData.dwFileAttributes & FILE_ATTRIBUTE_SPARSE_FILE)
				continue;
			else if (fdFileData.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM)
				continue;
			else if (fdFileData.dwFileAttributes & FILE_ATTRIBUTE_TEMPORARY)
				continue;
			else if (fdFileData.dwFileAttributes & FILE_ATTRIBUTE_VIRTUAL)
				continue;
			else if (std::regex_search(this->sFileInfoInst.sFileName, this->stOfficeOwnerFileMask))
				continue;
			else if (fdFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				// Examine dirs names for "." and dirs for ignore
				this->iIndex = this->sFileInfoInst.sFileName.find_last_of('.',0);
				if (this->iIndex != this->npos)
					if (this->iIndex == 0 || this->iIndex == 1)
						continue;
				int iDirStatus = 0;
				for (std::vector<std::basic_string<TCHAR>>::iterator it = UI::GetUIInst().vstFolderstoIgnore.begin(); it != UI::GetUIInst().vstFolderstoIgnore.end(); ++it)
				{
					if (it->compare(this->sFileInfoInst.sFileName) == 0)
						iDirStatus = -1;
				}
				if (iDirStatus != -1)
				{
					this->IterObjects(twrkDirtemp, fdFileData.cFileName, iCounter);
					continue;
				}
			}
			else //default:// If file with attributes: FILE_ATTRIBUTE_ARCHIVE | FILE_ATTRIBUTE_COMPRESSED | FILE_ATTRIBUTE_ENCRYPTED | FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_NORMAL | FILE_ATTRIBUTE_NOT_CONTENT_INDEXED | FILE_ATTRIBUTE_READONLY
			{
				iIndex = this->sFileInfoInst.sFileName.find_last_of('.');
				if (this->iIndex != this->npos)
					if (this->iIndex == 0 || this->iIndex == 1)
						continue;
				int iDirStatus = 0;
				for (std::vector<std::basic_string<TCHAR>>::iterator it = UI::GetUIInst().vstFolderstoIgnore.begin(); it != UI::GetUIInst().vstFolderstoIgnore.end(); ++it)
				{
					if (it->compare(this->sFileInfoInst.sFileName) != 0)
						continue;
					else
					{
						iDirStatus = -1;
						break;
					}
				}
				if (iDirStatus != -1)
				{
					//if (UI::GetUIInst().vstDaysToParse.compare(_T(" ")) == 0)
						//this->IterProcessFiles(hFileDataFindFirst, twrkDirtemp, twrkDirName);
					//else
					//{
					int iMaxTimeFlag	= CompareFileTime(&fdFileData.ftCreationTime, &fdFileData.ftLastWriteTime);
					switch (iMaxTimeFlag)
					{
						case -1:
							this->curFileTime = fdFileData.ftLastWriteTime;
							break;
						case 0:
							this->curFileTime = fdFileData.ftCreationTime;
							break;
						case 1:
							this->curFileTime = fdFileData.ftCreationTime;
							break;
					}
					int iStartLater		= CompareFileTime(&UI::GetUIInst().tStartChkPeriod, &this->curFileTime);
					int iEndEarlier		= CompareFileTime(&UI::GetUIInst().tEndChkPeriod, &this->curFileTime);
					//std::cout << std::endl << iStartLater << "________" << iEndEarlier;
					if (iStartLater == -1 && iEndEarlier == 1)
					{
						this->IterProcessFiles(hFileDataFindFirst, twrkDirtemp, twrkDirName);
					}
					//}
				}
				else
					break;
			}
		} while (FindNextFile(hFileDataFindFirst, &fdFileData) != 0);

		dwErrorCode = GetLastError();
		throw  dwErrorCode;
	}
	catch (DWORD)
	{
		switch (dwErrorCode)
		{
		case ERROR_SUCCESS:
		case ERROR_NO_MORE_FILES:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->IterObjects(Main \"do-while loop\"): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", twrkDirtemp);
			break;
		default:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->IterObjects(Main \"do-while loop\"()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", twrkDirtemp);
			Logger::GetLogInstance()->PrepareMySQLLOG("Error cannot find first file in current directory", "Function->IterObjects(Main \"do-while loop\"())", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCode, twrkDirtemp);
			ErrorHandle::GetErrorHandleInst()->ErrorExit(_T("IterObjects->Main \"do-while loop\" "), (LPTSTR)twrkDirtemp.c_str(), dwErrorCode);
			return FALSE;
		}
	}
	try
	{
		SetLastError(ERROR_SUCCESS);
		bStatus = SetCurrentDirectory((LPCWSTR)&twrkDir[0]);
		wprintf_s(_T("Going back to %s\r\n"), twrkDir.c_str());
		dwErrorCode = GetLastError();
		//if (dwErrorCode == ERROR_FILENAME_EXCED_RANGE)
		//{
		//	Logger::GetLogInstance()->PrepareTXTLOG("Function->IterObjects(SetCurrentDirectory_Parent()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", twrkDirtemp);
		//	Logger::GetLogInstance()->PrepareMySQLLOG("Error path is greater than 260 symbols", "Function->IterObjects(SetCurrentDirectory_Parent())", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCode, twrkDirtemp);
		//	return TRUE;
		//}
		//if (bStatus == FALSE && dwErrorCode != ERROR_FILE_NOT_FOUND)
		//{
		//	Logger::GetLogInstance()->PrepareTXTLOG("Function->IterObjects(SetCurrentDirectory_Parent()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", twrkDirtemp);
		//	Logger::GetLogInstance()->PrepareMySQLLOG("Error cannot set parent directory", "Function->IterObjects(SetCurrentDirectory_Parent())", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCode, twrkDirtemp);
		//	ErrorHandle::GetErrorHandleInst()->ErrorExit(_T("IterObjects->SetCurrentDirectory_Parent()"), (LPTSTR)twrkDirtemp.c_str(), dwErrorCode);
		//}
		throw  dwErrorCode;
	}
	catch (DWORD)
	{
		switch (dwErrorCode)
		{
		case ERROR_SUCCESS:
		case ERROR_NO_MORE_FILES:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->IterObjects(SetCurrentDirectory_Parent()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", twrkDirtemp);
			break;
		case ERROR_FILE_NOT_FOUND:
		case ERROR_ACCESS_DENIED:
		case ERROR_FILENAME_EXCED_RANGE:
		case ERROR_PATH_NOT_FOUND:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->IterObjects(SetCurrentDirectory_Parent()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", twrkDirtemp);
			Logger::GetLogInstance()->PrepareMySQLLOG("Error cannot set parent directory", "Function->IterObjects(SetCurrentDirectory_Parent())", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCode, twrkDirtemp);
			SetLastError(ERROR_SUCCESS);
			dwErrorCode = ERROR_SUCCESS;
			break;
		default:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->IterObjects(SetCurrentDirectory_Parent()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", twrkDirtemp);
			Logger::GetLogInstance()->PrepareMySQLLOG("Error cannot set parent directory", "Function->IterObjects(SetCurrentDirectory_Parent())", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCode, twrkDirtemp);
			ErrorHandle::GetErrorHandleInst()->ErrorExit(_T("IterObjects->SetCurrentDirectory_Parent()"), (LPTSTR)twrkDirtemp.c_str(), dwErrorCode);
		}
	}
	FindClose(hFileDataFindFirst);
	return  dwErrorCode;
};
DWORD FileProcess::IterProcessFiles(HANDLE hFileDataFindFirst, std::basic_string<TCHAR> twrkDirtemp, std::basic_string<TCHAR> twrkDirName)
{
	DWORD dwErrorCodePExist = 0;
	DWORD dwErrorCodeFAttrib = 0;
	DWORD dwErrorCode = -1;
	HANDLE hFile = nullptr;
	std::basic_string<TCHAR> stFilePathtemp;
	SetLastError(ERROR_SUCCESS);

	stFilePathtemp.insert(0, _T("\\\\?\\"));
	stFilePathtemp.append(twrkDirtemp);
	stFilePathtemp.append(this->sFileInfoInst.sFileName);
	BOOL bPathExist = FALSE;
	DWORD dwFileAttributes;
	try
	{
		bPathExist = PathFileExists(stFilePathtemp.c_str());
		dwErrorCodePExist = GetLastError();
		dwFileAttributes = GetFileAttributes(stFilePathtemp.c_str());
		dwErrorCodeFAttrib = GetLastError();
		if (bPathExist != 0 && dwFileAttributes != INVALID_FILE_ATTRIBUTES)
		{
			hFile = CreateFile(stFilePathtemp.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); //READ_CONTROL
			dwErrorCode = GetLastError();
			if (dwErrorCode == ERROR_ACCESS_DENIED && hFile == INVALID_HANDLE_VALUE)
			{
				Logger::GetLogInstance()->PrepareTXTLOG("Function->IterProcessFiles(CreateFile()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", stFilePathtemp);
				Logger::GetLogInstance()->PrepareMySQLLOG("Error access denied->invalid file handle value", "Function->IterProcessFiles(CreateFile())", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCode, stFilePathtemp);
				//if (hFile != nullptr)
					//CloseHandle(hFile);
				return dwErrorCode;
			}
			if (dwErrorCode == ERROR_FILENAME_EXCED_RANGE)
			{
				Logger::GetLogInstance()->PrepareTXTLOG("Function->IterProcessFiles(CreateFile()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", stFilePathtemp);
				Logger::GetLogInstance()->PrepareMySQLLOG("Error path is greater than MAX_PATH (260 symbols)", "Function->IterProcessFiles(CreateFile())", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCode, stFilePathtemp);
				if (hFile != nullptr)
					CloseHandle(hFile);
				return dwErrorCode;
			}
		}
		else
		{
			Logger::GetLogInstance()->PrepareTXTLOG("Function->IterProcessFiles(PathFileExists()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCodePExist, "; Object: ", stFilePathtemp);
			Logger::GetLogInstance()->PrepareMySQLLOG("Error path invalid or doesn't exist", "Function->IterProcessFiles(PathFileExists())", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCodePExist, stFilePathtemp);
			if (hFile != nullptr)
				CloseHandle(hFile);
			return dwErrorCode;
		}
	}
	catch (DWORD& dwErrorCode)
	{
		switch (dwErrorCode)
		{
		case ERROR_SUCCESS:
		case ERROR_NO_MORE_FILES:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->IterProcessFiles(CreateFile(DWORD dwErrorCode)): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", stFilePathtemp);
			break;
		default:
			FindClose(hFileDataFindFirst);
			if (hFile != nullptr)
				CloseHandle(hFile);
			Logger::GetLogInstance()->PrepareTXTLOG("Function->IterProcessFiles(CreateFile(DWORD dwErrorCode)): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", this->sFileInfoInst.sFileName);
			Logger::GetLogInstance()->PrepareMySQLLOG("Error in getting file attributes", "Function->IterObjects(CreateFile(DWORD dwErrorCode))", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCode, this->sFileInfoInst.sFileName);
			ErrorHandle::GetErrorHandleInst()->ErrorExit(_T("IterProcessFiles->CreateFile(DWORD dwErrorCode)"), (LPTSTR)this->sFileInfoInst.sFileName.c_str(), dwErrorCode);
		}
	}

	SetLastError(ERROR_SUCCESS);

	try 
	{
		// Get file info
		if (hFile != INVALID_HANDLE_VALUE)
		{
			this->sFileInfoInst.sFileDirName.assign(std::move(twrkDirName));// = twrkDirName;
			this->sFileInfoInst.sFileDirPath.assign(std::move(twrkDirtemp));// = ;

			this->sFileInfoInst.sFileExtension = PathFindExtension(this->sFileInfoInst.sFileName.c_str());

			dwErrorCode = this->GetFileOwnerName(hFile, this->sFileInfoInst.sFileName);
			SetLastError(ERROR_SUCCESS);
			dwErrorCode = this->GetFileSizeInst(hFile);
			dwErrorCode = this->GetFileCAWTime(hFile);
			dwErrorCode = this->ChangeFolderView();
			
			this->sFileInfoInst.iChkMask		= -1;
			this->sFileInfoInst.iChkCyrillic	= -1;
			this->sFileInfoInst.iChkProjectDB	= -1;
			this->sFileInfoInst.iChkProjectDR	= -1;
			this->sFileInfoInst.iChkÑompanyDB	= -1;
			this->sFileInfoInst.iChkRoleDB		= -1;
			this->sFileInfoInst.iChkStageDB		= -1;
			this->sFileInfoInst.iChkStageDR		= -1;
			this->sFileInfoInst.iChkSubSystemDB = -1;
			this->sFileInfoInst.iChkDataTypeDB	= -1;

			dwErrorCode = this->ChkMask();
			if (this->sFileInfoInst.iChkMask == 0)
			{
				this->sFileInfoInst.iChkProjectStatus = -1;
				throw dwErrorCode;
			}
			dwErrorCode = this->GetFileInfobyName();
			dwErrorCode = this->GetFileInfobyFolder();
			dwErrorCode = this->ChkCyrillic();
			dwErrorCode = this->ChkProjectDB();
			dwErrorCode = this->ChkProjectDR();
			dwErrorCode = this->ChkÑompanyDB();
			dwErrorCode = this->ChkRoleDB();
			dwErrorCode = this->ChkStageDB();
			dwErrorCode = this->ChkStageDR();
			dwErrorCode = this->ChkSubSystemDB();
			dwErrorCode = this->ChkDataTypeDB();

			dwErrorCode = GetLastError();
			throw dwErrorCode;
		}
	}
	catch (DWORD& dwErrorCode)
	{
		switch (dwErrorCode)
		{
		case ERROR_SUCCESS:
		case ERROR_NO_MORE_FILES:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->IterProcessFiles(GetFileInfo(DWORD dwErrorCode)): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", stFilePathtemp);
			break;
		default:
			FindClose(hFileDataFindFirst);
			if (hFile != nullptr)
				CloseHandle(hFile);
			Logger::GetLogInstance()->PrepareTXTLOG("Function->IterProcessFiles(GetFileInfo(DWORD dwErrorCode)): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", this->sFileInfoInst.sFileName);
			Logger::GetLogInstance()->PrepareMySQLLOG("Error in getting file attributes", "Function->IterObjects(GetFileInfo(DWORD dwErrorCode))", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCode, this->sFileInfoInst.sFileName);
			ErrorHandle::GetErrorHandleInst()->ErrorExit(_T("IterProcessFiles->GetFileInfo(DWORD dwErrorCode)"), (LPTSTR)this->sFileInfoInst.sFileName.c_str(), dwErrorCode);
		}
	}

	// Write to table "Folders"
	try
	{
		if (this->vstExistDirs.empty())
		{
			this->vstExistDirs.push_back(this->sFileInfoInst.sFileDirPathChngView);
			DBProcess::dbProcInstance().DBWriteFolders(this);
		}
		else
		{
			std::vector<std::basic_string<TCHAR>>::const_iterator it = std::find(this->vstExistDirs.begin(), this->vstExistDirs.end(), this->sFileInfoInst.sFileDirPathChngView.c_str());
			if (it == this->vstExistDirs.end())
			{
				this->vstExistDirs.push_back(this->sFileInfoInst.sFileDirPathChngView.c_str());
				DBProcess::dbProcInstance().DBWriteFolders(this);
			}
		}
		dwErrorCode = GetLastError();
		throw dwErrorCode;
	}
	catch (DWORD& dwErrorCode)
	{
		switch (dwErrorCode)
		{
		case ERROR_SUCCESS:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->IterProcessFiles(DBWriteFolders()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", this->sFileInfoInst.sFileDirPathChngView);
			break;
		default:
			FindClose(hFileDataFindFirst);
			if (hFile != nullptr)
				CloseHandle(hFile);
			Logger::GetLogInstance()->PrepareTXTLOG("Function->IterObjects(DBWriteFolders()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", this->sFileInfoInst.sFileDirPathChngView);
			Logger::GetLogInstance()->PrepareMySQLLOG("Error in writing to TABLE folders", "Function->IterObjects(DBWriteFolders())", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCode, this->sFileInfoInst.sFileDirPathChngView);
			ErrorHandle::GetErrorHandleInst()->ErrorExit(_T("IterObjects->DBWriteFolders()"), (LPTSTR)this->sFileInfoInst.sFileDirPathChngView.c_str(), dwErrorCode);
			break;
		}
	}

	// Write to table "Extensions"
	try
	{
		std::transform(this->sFileInfoInst.sFileExtension.begin(), this->sFileInfoInst.sFileExtension.end(), this->sFileInfoInst.sFileExtension.begin(), ::tolower);
		if (this->vstExistExtens.empty())
		{
			this->vstExistExtens.push_back(this->sFileInfoInst.sFileExtension.c_str());
			DBProcess::dbProcInstance().DBWriteExtensions(this);
		}
		else
		{
			auto result = std::find(this->vstExistExtens.begin(), this->vstExistExtens.end(), this->sFileInfoInst.sFileExtension.c_str());
			if (!(result != this->vstExistExtens.end()))
			{
					this->vstExistExtens.push_back(this->sFileInfoInst.sFileExtension.c_str());
					DBProcess::dbProcInstance().DBWriteExtensions(this);
			}
		}
		dwErrorCode = GetLastError();
		throw dwErrorCode;
	}
	catch (DWORD& dwErrorCode)
	{
		switch (dwErrorCode)
		{
		case ERROR_SUCCESS:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->IterProcessFiles(DBWriteExtensions()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", this->sFileInfoInst.sFileExtension);
			break;
		default:
			FindClose(hFileDataFindFirst);
			if (hFile != nullptr)
				CloseHandle(hFile);
			Logger::GetLogInstance()->PrepareTXTLOG("Function->IterObjects(DBWriteExtensions()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", this->sFileInfoInst.sFileExtension);
			Logger::GetLogInstance()->PrepareMySQLLOG("Error in writing to TABLE extensions", "Function->IterObjects(DBWriteExtensions())", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCode, this->sFileInfoInst.sFileExtension);
			ErrorHandle::GetErrorHandleInst()->ErrorExit(_T("IterObjects->DBWriteExtensions()"), (LPTSTR)this->sFileInfoInst.sFileExtension.c_str(), dwErrorCode);
		}
	}
	catch (std::exception& e)
	{
		std::cerr << "exception caught: " << e.what() << '\n';
	}

	// Write to table "Files"
	try
	{
		DBProcess::dbProcInstance().DBWriteFiles(this);
		dwErrorCode = GetLastError();
		throw dwErrorCode;
	}
	catch (DWORD& dwErrorCode)
	{
		switch (dwErrorCode)
		{
		case ERROR_SUCCESS:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->IterProcessFiles(DBWriteFiles()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", this->sFileInfoInst.sFileName);
			break;
		default:
			FindClose(hFileDataFindFirst);
			if (hFile != nullptr)
				CloseHandle(hFile);
			Logger::GetLogInstance()->PrepareTXTLOG("Function->IterObjects(DBWriteFiles()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", this->sFileInfoInst.sFileName);
			Logger::GetLogInstance()->PrepareMySQLLOG("Error in writing to TABLE files", "Function->IterObjects(DBWriteFiles())", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCode, this->sFileInfoInst.sFileName);
			ErrorHandle::GetErrorHandleInst()->ErrorExit(_T("IterObjects->DBWriteFiles()"), (LPTSTR)this->sFileInfoInst.sFileName.c_str(), dwErrorCode);
		}
	}
	
	if (hFile != nullptr)
		CloseHandle(hFile);

	return dwErrorCode;
};

DWORD FileProcess::GetFileOwnerName(HANDLE hFile, std::basic_string<TCHAR> sFileName)
{
	DWORD dwRtnCode = -1;
	DWORD dwErrorCode = -1;
	PSID pSidOwner = NULL;
	BOOL bRtnBool = TRUE;
	LPTSTR AcctName = NULL;
	LPTSTR DomainName = NULL;
	DWORD dwAcctName = 1, dwDomainName = 1;
	SID_NAME_USE eUse = SidTypeUnknown;
	PSECURITY_DESCRIPTOR pSD = NULL;

	// Get the owner SID of the file.
	try
	{
		dwRtnCode = GetSecurityInfo(hFile, SE_FILE_OBJECT, OWNER_SECURITY_INFORMATION, &pSidOwner, NULL, NULL, NULL, &pSD);
		dwErrorCode = GetLastError();
		throw dwErrorCode;
	}
	catch (DWORD& dwErrorCode)
	{
		if (dwErrorCode == ERROR_INVALID_PARAMETER)
		{
			SetLastError(ERROR_SUCCESS);
		}
		else if (dwErrorCode != ERROR_SUCCESS)
		{
			Logger::GetLogInstance()->PrepareTXTLOG("Function->GetFileOwnerName(GetSecurityInfo()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", sFileName);
			Logger::GetLogInstance()->PrepareMySQLLOG("Error in getting file owner", "Function->GetFileOwnerName(GetSecurityInfo())", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCode, sFileName);
			ErrorHandle::GetErrorHandleInst()->ErrorExit(_T("GetFileOwnerName->GetSecurityInfo()"), (LPTSTR)sFileName.c_str(), dwErrorCode);
		}
	}
	
	// First call to LookupAccountSid to get the buffer sizes.
	bRtnBool = LookupAccountSid(NULL, pSidOwner, AcctName, (LPDWORD)&dwAcctName, DomainName, (LPDWORD)&dwDomainName, &eUse);
	dwErrorCode = GetLastError();
	if (dwErrorCode == ERROR_INSUFFICIENT_BUFFER)
	{
		SetLastError(ERROR_SUCCESS);
		dwErrorCode = 0;
	}
	else if (dwErrorCode != ERROR_SUCCESS)
	{
		dwErrorCode = GetLastError();
		Logger::GetLogInstance()->PrepareTXTLOG("Function->GetFileOwnerName(LookupAccountSid()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", sFileName);
		Logger::GetLogInstance()->PrepareMySQLLOG("Error in getting file owner", "Function->GetFileOwnerName(LookupAccountSid())", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCode, sFileName);
	}
	// Reallocate memory for the buffers.
	HANDLE hProcHeap = HeapCreate(HEAP_CREATE_ENABLE_EXECUTE | HEAP_GENERATE_EXCEPTIONS, (size_t)dwAcctName,0);
	if (hProcHeap != nullptr)
	{
		AcctName = (LPTSTR)HeapAlloc(hProcHeap, HEAP_ZERO_MEMORY, (size_t)dwAcctName-1);
		bRtnBool = HeapValidate(hProcHeap, 0, NULL);
		//AcctName = (LPTSTR)GlobalAlloc(GMEM_FIXED, dwAcctName);
		dwErrorCode = GetLastError();
		// Check GetLastError for GlobalAlloc error condition.
		if (AcctName != NULL)
		{
			if (dwErrorCode == ERROR_INSUFFICIENT_BUFFER)
			{
				SetLastError(ERROR_SUCCESS);
				dwErrorCode = 0;
			}
			HANDLE hProcHeapT = HeapCreate(HEAP_CREATE_ENABLE_EXECUTE | HEAP_GENERATE_EXCEPTIONS, (size_t)dwDomainName, 0);
			if (hProcHeapT != nullptr)
			{
				DomainName = (LPTSTR)HeapAlloc(hProcHeapT, HEAP_GENERATE_EXCEPTIONS | HEAP_ZERO_MEMORY, (size_t)dwDomainName - 1);
				bRtnBool = HeapValidate(hProcHeapT, 0, NULL);
				//DomainName = (LPTSTR)GlobalAlloc(GMEM_FIXED, dwDomainName);
				dwErrorCode = GetLastError();
				// Check GetLastError for GlobalAlloc error condition.
				if (DomainName != NULL)
				{
					// Second call to LookupAccountSid to get the account name.
					std::basic_string<TCHAR> wsFileAcctName;
					bRtnBool = LookupAccountSid(
						NULL,                   // name of local or remote computer
						pSidOwner,              // security identifier
						AcctName,               // account name buffer
						(LPDWORD)&dwAcctName,   // size of account name buffer 
						DomainName,             // domain name
						(LPDWORD)&dwDomainName, // size of domain name buffer
						&eUse);                 // SID type

												// Check GetLastError for LookupAccountSid error condition.
					dwErrorCode = GetLastError();
					if (bRtnBool == FALSE)
					{
						if (dwErrorCode == ERROR_NONE_MAPPED)
						{
							Logger::GetLogInstance()->PrepareTXTLOG("Function->GetFileOwnerName(LookupAccountSid()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", sFileName);
							Logger::GetLogInstance()->PrepareMySQLLOG("Error in Look up Account Sid, Owner changed to \"Àäìèíèñòðàòîðû\" ", "Function->LookupAccountSid(())", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCode, sFileName);
							wsFileAcctName.append(_T("Àäìèíèñòðàòîðû"));
							this->sFileInfoInst.sFileOwnerName = wsFileAcctName;
							LocalFree(pSD);
							bRtnBool = HeapDestroy(hProcHeap);
							bRtnBool = FALSE;
							bRtnBool = HeapDestroy(hProcHeapT);
							SetLastError(ERROR_SUCCESS);
							dwErrorCode = 0;
							return dwErrorCode;
						}
						else
						{
							Logger::GetLogInstance()->PrepareTXTLOG("Function->GetFileOwnerName(LookupAccountSid()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", sFileName);
							Logger::GetLogInstance()->PrepareMySQLLOG("Error in Look up Account Sid", "Function->LookupAccountSid(())", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCode, sFileName);
							LocalFree(pSD);
							HeapDestroy(hProcHeap);
							bRtnBool = FALSE;
							HeapDestroy(hProcHeapT);
							return dwErrorCode;
						}
					}
					else
					{
						wsFileAcctName.append(AcctName);
						this->sFileInfoInst.sFileOwnerName.assign(std::move(wsFileAcctName));
						dwErrorCode = -1;
						LocalFree(pSD);
						bRtnBool = HeapDestroy(hProcHeap);
						bRtnBool = FALSE;
						bRtnBool = HeapDestroy(hProcHeapT);
						//CloseHandle(hFileLocal);
					}
				}
				else
				{
					dwErrorCode = GetLastError();
					bRtnBool = HeapDestroy(hProcHeapT);
					Logger::GetLogInstance()->PrepareTXTLOG("Function->GetFileOwnerName(HeapCreate(DomainName)): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", sFileName);
					Logger::GetLogInstance()->PrepareMySQLLOG("Error in getting file owner", "Function->GetFileOwnerName(HeapCreate())", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCode, sFileName);
					ErrorHandle::GetErrorHandleInst()->ErrorExit(_T("GetFileOwnerName->HeapCreate(DomainName)"), (LPTSTR)sFileName.c_str(), dwErrorCode);
				}
			}
		}
		else
		{
			dwErrorCode = GetLastError();
			LocalFree(pSD);
			bRtnBool = HeapDestroy(hProcHeap);
			bRtnBool = FALSE;

			Logger::GetLogInstance()->PrepareTXTLOG("Function->GetFileOwnerName(HeapCreate(AcctName)): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", sFileName);
			Logger::GetLogInstance()->PrepareMySQLLOG("Error in getting file owner", "Function->GetFileOwnerName(HeapCreate())", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCode, sFileName);
			ErrorHandle::GetErrorHandleInst()->ErrorExit(_T("GetFileOwnerName->HeapCreate(AcctName)"), (LPTSTR)sFileName.c_str(), dwErrorCode);
		}
	}
	dwErrorCode = GetLastError();
	return dwErrorCode;
};
DWORD FileProcess::GetFileSizeInst(HANDLE hFile)
{
	BOOL bStatus = FALSE;
	DWORD dwErrorCode = -1;
	try
	{
		bStatus = GetFileSizeEx(hFile, &this->sFileInfoInst.sFileSize);

		dwErrorCode = GetLastError();
		throw dwErrorCode;
	}
	catch (DWORD& dwErrorCode)
	{
		switch (dwErrorCode)
		{
		case ERROR_SUCCESS:
		case ERROR_NO_MORE_FILES:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->IterProcessFiles(GetFileSizeInst(DWORD dwErrorCode)): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", this->sFileInfoInst.sFileName);
			break;
		default:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->IterProcessFiles(GetFileSizeInst(DWORD dwErrorCode)): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ",this->sFileInfoInst.sFileName);
			Logger::GetLogInstance()->PrepareMySQLLOG("Error in getting file attributes", "Function->IterObjects(GetFileSizeInst(DWORD dwErrorCode))", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCode,this->sFileInfoInst.sFileName);
		}
	}
	
	return dwErrorCode;
};
DWORD FileProcess::GetFileCAWTime(HANDLE hFile)
{
	DWORD dwErrorCode = -1;
	BOOL bStatus = FALSE;
	//SetLastError(ERROR_SUCCESS);

	const INT64 UNIX_TIME_START = 0x019DB1DED53E8000; //January 1, 1970 (start of Unix epoch) in "ticks"
	const INT64 TICKS_PER_SECOND = 10000000; //a tick is 100ns
	LARGE_INTEGER li;
	FILETIME ftCreateUTC	= { 0 };
	FILETIME ftAccessUTC	= { 0 };
	FILETIME ftWriteUTC		= { 0 };
	FILETIME ftCreate		= { 0 };
	FILETIME ftAccess		= { 0 };
	FILETIME ftWrite		= { 0 };

	try
	{
		bStatus = GetFileTime(hFile, &ftCreateUTC, &ftAccessUTC, &ftWriteUTC);
		if(bStatus != 0)
		{
			bStatus = FileTimeToLocalFileTime(&ftCreateUTC, &ftCreate);
			bStatus = FileTimeToLocalFileTime(&ftAccessUTC, &ftAccess);
			bStatus = FileTimeToLocalFileTime(&ftWriteUTC, &ftWrite);
		}
		else
		{
			dwErrorCode = GetLastError();
			throw dwErrorCode;
		}

		li.LowPart = ftCreate.dwLowDateTime;
		li.HighPart = ftCreate.dwHighDateTime;
		this->sFileInfoInst.sFileCAWTime.i64Time_create = (li.QuadPart - UNIX_TIME_START) / TICKS_PER_SECOND;

		li.LowPart = ftAccess.dwLowDateTime;
		li.HighPart = ftAccess.dwHighDateTime;
		this->sFileInfoInst.sFileCAWTime.i64Time_access = (li.QuadPart - UNIX_TIME_START) / TICKS_PER_SECOND;

		li.LowPart = ftWrite.dwLowDateTime;
		li.HighPart = ftWrite.dwHighDateTime;
		this->sFileInfoInst.sFileCAWTime.i64Time_write = (li.QuadPart - UNIX_TIME_START) / TICKS_PER_SECOND;

		dwErrorCode = GetLastError();
		throw dwErrorCode;
	}
	catch (DWORD& dwErrorCode)
	{
		switch (dwErrorCode)
		{
		case ERROR_SUCCESS:
		case ERROR_NO_MORE_FILES:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->IterProcessFiles(GetFileCAWTime(DWORD dwErrorCode)): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ",this->sFileInfoInst.sFileName);
			break;
		default:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->IterProcessFiles(GetFileCAWTime(DWORD dwErrorCode)): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ",this->sFileInfoInst.sFileName);
			Logger::GetLogInstance()->PrepareMySQLLOG("Error in getting file attributes", "Function->IterObjects(GetFileCAWTime(DWORD dwErrorCode))", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCode,this->sFileInfoInst.sFileName);
		}
	}

	return dwErrorCode;
};
DWORD FileProcess::GetFileInfobyName()
{
	std::size_t nLoc = 0;
	std::size_t nStartFrom = 0;
	nLoc = this->sFileInfoInst.sFileName.find('.', nStartFrom);

	std::basic_string<TCHAR> sFile_Info(this->sFileInfoInst.sFileName.substr(nStartFrom, (nLoc - nStartFrom)));
	std::vector<std::basic_string<TCHAR>> vstFile_Info;
	std::wsregex_token_iterator itEnd;
	
	std::wsregex_token_iterator itwsReg_1(sFile_Info.begin(), sFile_Info.end(), stRegFileStructbyMask, -1);
	
	DWORD dwErrorCode = -1;
	try
	{
		for (; itwsReg_1 != itEnd; ++itwsReg_1)
			vstFile_Info.push_back(*itwsReg_1);
		if (vstFile_Info.size() >= 5)
		{
			this->sFileInfoInst.sFile_ProjectbyName = vstFile_Info[0];
			this->sFileInfoInst.sFile_CompanyRolebyName = vstFile_Info[1];
			this->sFileInfoInst.sFile_ProjectStagebyName = vstFile_Info[2];
			this->sFileInfoInst.sFile_SubsystembyName = vstFile_Info[3];
			this->sFileInfoInst.sFile_DatatypebyName = vstFile_Info[4];
		}
		else
		{
			dwErrorCode = 1700;
			throw dwErrorCode;
		}
		dwErrorCode = GetLastError();
		throw dwErrorCode;
	}
	catch (DWORD& dwErrorCode)
	{
		switch (dwErrorCode)
		{
		case ERROR_SUCCESS:
		case ERROR_NO_MORE_FILES:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->IterProcessFiles(GetFileInfobyName(DWORD dwErrorCode)): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ",this->sFileInfoInst.sFileName);
			break;
		default:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->IterProcessFiles(GetFileInfobyName(DWORD dwErrorCode)): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ",this->sFileInfoInst.sFileName);
			Logger::GetLogInstance()->PrepareMySQLLOG("Error in getting file attributes", "Function->IterObjects(GetFileInfobyName(DWORD dwErrorCode))", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCode,this->sFileInfoInst.sFileName);
		}
	}

	return dwErrorCode;
};
DWORD FileProcess::GetFileInfobyFolder()
{
	std::basic_string<TCHAR> sFile_Info;
	std::size_t nLoc = 0;
	DWORD dwErrorCode = -1;
	bool bFindStatus = FALSE;
	//auto itvstFileName = UI::GetUIInst().vstRootFolders.begin();
	//std::map <std::basic_string<TCHAR>, std::basic_string<TCHAR>> mRootFolders = { {_T("03-Published"), _T("P:")},{ _T("Resources"), _T("R:") },{ _T("02-Shared"), _T("S:") },{ _T("01-WorkInProgress"), _T("W:") }};
	try
	{
		sFile_Info = this->sFileInfoInst.sFileDirPath;
		//std::wcout << sFile_Info;
		//for (itvstFileName; itvstFileName != UI::GetUIInst().vstRootFolders.end(); ++itvstFileName)
		//for (UI::GetUIInst().itRootFolders = UI::GetUIInst().vstRootFolders.begin(); UI::GetUIInst().itRootFolders != UI::GetUIInst().vstRootFolders.end(); ++UI::GetUIInst().itRootFolders)
		//{
		//	nLoc = sFile_Info.find(itvstFileName->c_str(), 0);
		//	if (nLoc != std::basic_string<TCHAR>::npos)
		//	{
		//		sFile_Info.erase(0, nLoc + 1);
		//		bFindStatus = TRUE;

		//		//this->sFileInfoInst.sFileDirPath = sFile_Info;
		//		//this->sFileInfoInst.sFileDirPath.replace(this->sFileInfoInst.sFileDirPath.find(UI::GetUIInst().itRootFolders->c_str()), UI::GetUIInst().itRootFolders->length(), mRootFolders.at(UI::GetUIInst().itRootFolders->c_str()));
		//	}
		//	else
		//		continue;
		//	if (bFindStatus == FALSE)
		//		break;
		//}
		//nLoc = 0;
		//std::wcout << this->sFileInfoInst.sFileDirPath;		

		//Parse file name
		std::vector<std::basic_string<TCHAR>> vstFile_Info;
		std::size_t nStartFrom = 0;

		while (nLoc != std::basic_string<TCHAR>::npos)
		{
			nLoc = sFile_Info.find('\\', nStartFrom);
			vstFile_Info.push_back(sFile_Info.substr(nStartFrom, (nLoc - nStartFrom)));
			sFile_Info.erase(nStartFrom, nLoc + 1);
		}
		nLoc = 0;
		this->sFileInfoInst.sFile_ProjectbyPath = vstFile_Info[1];
		this->sFileInfoInst.sFile_StageforFL = vstFile_Info[3];

		dwErrorCode = GetLastError();
		throw dwErrorCode;
	}
	catch (DWORD& dwErrorCode)
	{
		switch (dwErrorCode)
		{
		case ERROR_SUCCESS:
		case ERROR_NO_MORE_FILES:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->IterProcessFiles(GetFileInfobyFolder(DWORD dwErrorCode)): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ",this->sFileInfoInst.sFileName);
			break;
		default:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->IterProcessFiles(GetFileInfobyFolder(DWORD dwErrorCode)): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ",this->sFileInfoInst.sFileName);
			Logger::GetLogInstance()->PrepareMySQLLOG("Error in getting file attributes", "Function->IterObjects(GetFileInfobyFolder(DWORD dwErrorCode))", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCode,this->sFileInfoInst.sFileName);
		}
	}

	return dwErrorCode;
};
DWORD FileProcess::ChangeFolderView()
{
	std::basic_string<TCHAR> sFile_Info = this->sFileInfoInst.sFileDirPath;
	BOOL bFindStatus = FALSE;
	std::size_t nLoc = 0;
	DWORD dwErrorCode = -1;
	int i = 0;
	std::vector<std::basic_string<TCHAR>> vstFoldersfromChng;
	vstFoldersfromChng.push_back(_T("03-Pub"));
	vstFoldersfromChng.push_back(_T("00-Res"));
	vstFoldersfromChng.push_back(_T("02-Sha"));
	vstFoldersfromChng.push_back(_T("01-WiP"));
	std::vector<std::basic_string<TCHAR>> vstFolderstoChng;
	vstFolderstoChng.push_back(_T("P:"));
	vstFolderstoChng.push_back(_T("R:"));
	vstFolderstoChng.push_back(_T("S:"));
	vstFolderstoChng.push_back(_T("W:"));
	std::vector<std::basic_string<TCHAR>>::const_iterator itvstFoldersfromChng = vstFoldersfromChng.begin();
	std::vector<std::basic_string<TCHAR>>::const_iterator itvstFolderstoChng = vstFolderstoChng.begin();

	try
	{
		for (itvstFoldersfromChng; itvstFoldersfromChng != vstFoldersfromChng.end(); ++itvstFoldersfromChng, ++itvstFolderstoChng)
		{
			nLoc = sFile_Info.find(*itvstFoldersfromChng, 0);
			if (nLoc != std::basic_string<TCHAR>::npos)
			{
				sFile_Info.replace(0, itvstFolderstoChng->length(), itvstFolderstoChng->c_str());
				this->sFileInfoInst.sFileDirPathChngView = sFile_Info;
			}
		}
		nLoc = 0;
		dwErrorCode = GetLastError();
		throw dwErrorCode;
	}
	catch (DWORD& dwErrorCode)
	{
		switch (dwErrorCode)
		{
		case ERROR_SUCCESS:
		case ERROR_NO_MORE_FILES:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->IterProcessFiles(ChangeFolderView(DWORD dwErrorCode)): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ",this->sFileInfoInst.sFileName);
			break;
		default:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->IterProcessFiles(ChangeFolderView(DWORD dwErrorCode)): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ",this->sFileInfoInst.sFileName);
			Logger::GetLogInstance()->PrepareMySQLLOG("Error in getting file attributes", "Function->IterObjects(ChangeFolderView(DWORD dwErrorCode))", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCode,this->sFileInfoInst.sFileName);
		}
	}
	//std::vector<std::basic_string<TCHAR>> vstFolderstoChng = { _T("h:\\CloudSt\\GDData\\03-Published"),  _T("h:\\Resources"), _T("h:\\CloudSt\\GDData\\02-Shared"), _T("h:\\CloudSt\\GDWiP\\01-WorkInProgress") };
	//std::vector<std::basic_string<TCHAR>> vstFoldersfromChng;
	//vstFoldersfromChng.push_back(_T("h:\\CloudSt\\GDData\\03-Published"));
	//vstFoldersfromChng.push_back(_T("h:\\Resources"));
	//vstFoldersfromChng.push_back(_T("h:\\CloudSt\\GDData\\02-Shared"));
	//vstFoldersfromChng.push_back(_T("h:\\CloudSt\\GDWiP\\01-WorkInProgress"));
	//std::vector<std::basic_string<TCHAR>> vstFoldersfromChng_NETSHARE;
	//vstFoldersfromChng_NETSHARE.push_back(_T("\\\\?\\UNC\\000-server-01\\public\\Work"));
	//std::vector<std::basic_string<TCHAR>> vstFoldersfromChng_NET;
	//vstFoldersfromChng_NET.push_back(_T("\\\\?\\"));
	//

	//std::vector<std::basic_string<TCHAR>> vstFolderstoChng_NETSHARE;
	//vstFolderstoChng_NETSHARE.push_back(_T("W:"));
	//std::vector<std::basic_string<TCHAR>> vstFolderstoChng_NET;
	//vstFolderstoChng_NET.push_back(_T(""));
	//std::vector<std::basic_string<TCHAR>>::const_iterator itvstFoldersfromChng = vstFoldersfromChng.begin();
	//std::vector<std::basic_string<TCHAR>>::const_iterator itvstFoldersfromChng_NETSHARE = vstFoldersfromChng_NETSHARE.begin();
	//std::vector<std::basic_string<TCHAR>>::const_iterator itvstFoldersfromChng_NET = vstFoldersfromChng_NET.begin();

	//std::vector<std::basic_string<TCHAR>>::const_iterator itvstFolderstoChng = vstFolderstoChng.begin();
	//std::vector<std::basic_string<TCHAR>>::const_iterator itvstFolderstoChng_NETSHARE = vstFolderstoChng_NETSHARE.begin();
	//std::vector<std::basic_string<TCHAR>>::const_iterator itvstFolderstoChng_NET = vstFolderstoChng_NET.begin();
	//std::map <std::basic_string<TCHAR>, std::basic_string<TCHAR>>::const_iterator itmRootFolders;
	//std::map <std::basic_string<TCHAR>, std::basic_string<TCHAR>> mRootFolders = {	{ vstFoldersfromChng.at(0).c_str(), vstFolderstoChng.at(0).c_str() },
	//																				{ vstFoldersfromChng.at(1).c_str(), vstFolderstoChng.at(1).c_str() },
	//																				{ vstFoldersfromChng.at(2).c_str(), vstFolderstoChng.at(2).c_str() },
	//																				{ vstFoldersfromChng.at(3).c_str(), vstFolderstoChng.at(3).c_str() } };
	//std::basic_regex<TCHAR> rgMask(_T("(h:)"));
	//std::basic_regex<TCHAR> stRegSearch_NETSHARE(_T("[UNC]"));
	//std::basic_regex<TCHAR> stRegSearch_NET(_T("\\\\\?"));
	//if (std::regex_search(sFile_Info.begin(), sFile_Info.end(), rgMask, std::regex_constants::match_not_bol))
	//{
	//	try
	//	{
	//		for (itvstFoldersfromChng; itvstFoldersfromChng != vstFoldersfromChng.end(); ++itvstFoldersfromChng, ++itvstFolderstoChng)
	//		{
	//			nLoc = sFile_Info.find(*itvstFoldersfromChng, 0);
	//			if (nLoc != std::basic_string<TCHAR>::npos)
	//			{
	//				sFile_Info.replace(0, itvstFoldersfromChng->length(), itvstFolderstoChng->c_str());
	//				this->sFileInfoInst.sFileDirPathChngView = sFile_Info;
	//			}
	//		}
	//		nLoc = 0;
	//		dwErrorCode = GetLastError();
	//		throw dwErrorCode;
	//	}
	//	catch (DWORD& dwErrorCode)
	//	{
	//		switch (dwErrorCode)
	//		{
	//		case ERROR_SUCCESS:
	//		case ERROR_NO_MORE_FILES:
	//			Logger::GetLogInstance()->PrepareTXTLOG("Function->IterProcessFiles(ChangeFolderView(DWORD dwErrorCode)): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ",this->sFileInfoInst.sFileName);
	//			break;
	//		default:
	//			Logger::GetLogInstance()->PrepareTXTLOG("Function->IterProcessFiles(ChangeFolderView(DWORD dwErrorCode)): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ",this->sFileInfoInst.sFileName);
	//			Logger::GetLogInstance()->PrepareMySQLLOG("Error in getting file attributes", "Function->IterObjects(ChangeFolderView(DWORD dwErrorCode))", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCode,this->sFileInfoInst.sFileName);
	//		}
	//	}
	//}
	//else if (std::regex_search(sFile_Info.begin(), sFile_Info.end(), stRegSearch_NET, std::regex_constants::match_not_bol))
	//{
	//	try
	//	{
	//		nLoc = sFile_Info.find(*itvstFoldersfromChng_NET, 0);
	//		if (nLoc != std::basic_string<TCHAR>::npos)
	//		{
	//			sFile_Info.replace(0, itvstFoldersfromChng_NET->length(), itvstFolderstoChng_NET->c_str());
	//			this->sFileInfoInst.sFileDirPathChngView.clear();
	//			this->sFileInfoInst.sFileDirPathChngView = sFile_Info;
	//		}
	//		nLoc = 0;
	//		dwErrorCode = GetLastError();
	//		throw dwErrorCode;
	//	}
	//	catch (DWORD& dwErrorCode)
	//	{
	//		switch (dwErrorCode)
	//		{
	//		case ERROR_SUCCESS:
	//		case ERROR_NO_MORE_FILES:
	//			Logger::GetLogInstance()->PrepareTXTLOG("Function->IterProcessFiles(ChangeFolderView(DWORD dwErrorCode)): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", this->sFileInfoInst.sFileName);
	//			break;
	//		default:
	//			Logger::GetLogInstance()->PrepareTXTLOG("Function->IterProcessFiles(ChangeFolderView(DWORD dwErrorCode)): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", this->sFileInfoInst.sFileName);
	//			Logger::GetLogInstance()->PrepareMySQLLOG("Error in getting file attributes", "Function->IterObjects(ChangeFolderView(DWORD dwErrorCode))", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCode, this->sFileInfoInst.sFileName);
	//		}
	//	}
	//}
	//else if (std::regex_search(sFile_Info.begin(), sFile_Info.end(), stRegSearch_NETSHARE, std::regex_constants::match_not_bol))
	//{
	//	try
	//	{
	//		nLoc = sFile_Info.find(*itvstFoldersfromChng_NETSHARE, 0);
	//		if (nLoc != std::basic_string<TCHAR>::npos)
	//		{
	//			sFile_Info.replace(0, itvstFoldersfromChng_NETSHARE->length(), itvstFolderstoChng_NETSHARE->c_str());
	//			this->sFileInfoInst.sFileDirPathChngView = sFile_Info;
	//		}
	//		nLoc = 0;
	//		dwErrorCode = GetLastError();
	//		throw dwErrorCode;
	//	}
	//	catch (DWORD& dwErrorCode)
	//	{
	//		switch (dwErrorCode)
	//		{
	//		case ERROR_SUCCESS:
	//		case ERROR_NO_MORE_FILES:
	//			Logger::GetLogInstance()->PrepareTXTLOG("Function->IterProcessFiles(ChangeFolderView(DWORD dwErrorCode)): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", this->sFileInfoInst.sFileName);
	//			break;
	//		default:
	//			Logger::GetLogInstance()->PrepareTXTLOG("Function->IterProcessFiles(ChangeFolderView(DWORD dwErrorCode)): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", this->sFileInfoInst.sFileName);
	//			Logger::GetLogInstance()->PrepareMySQLLOG("Error in getting file attributes", "Function->IterObjects(ChangeFolderView(DWORD dwErrorCode))", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCode, this->sFileInfoInst.sFileName);
	//		}
	//	}		
	//}
	/*else 
	{*/
		//this->sFileInfoInst.sFileDirPathChngView = sFile_Info;
		dwErrorCode = GetLastError();
	//}

	return dwErrorCode;
};

DWORD FileProcess::ChkMask()
{
	bool bmatchResult = FALSE;
	DWORD dwErrorCode = -1;
	this->sFileInfoInst.iChkMask = -1;
	try
	{
		bmatchResult = std::regex_search(this->sFileInfoInst.sFileDirPathChngView, this->regFindDStageShaPubZZ);
		if (bmatchResult == TRUE)
		{
			bmatchResult = std::regex_match(this->sFileInfoInst.sFileName, this->regMaskDStageShaPubZZ);
		}
		else
		{
			bmatchResult = std::regex_match(this->sFileInfoInst.sFileName, this->regMaskCommon);
		}
		this->sFileInfoInst.iChkMask = bmatchResult;

		dwErrorCode = GetLastError();
		throw dwErrorCode;
	}
	catch (DWORD& dwErrorCode)
	{
		switch (dwErrorCode)
		{
		case ERROR_SUCCESS:
		case ERROR_NO_MORE_FILES:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->IterProcessFiles(ChkMask(DWORD dwErrorCode)): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ",this->sFileInfoInst.sFileName);
			break;
		default:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->IterProcessFiles(ChkMask(DWORD dwErrorCode)): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ",this->sFileInfoInst.sFileName);
			Logger::GetLogInstance()->PrepareMySQLLOG("Error in getting file attributes", "Function->IterObjects(ChkMask(DWORD dwErrorCode))", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCode,this->sFileInfoInst.sFileName);
			break;
		}
	}

	return dwErrorCode;
};
DWORD FileProcess::ChkCyrillic()
{
	BOOL bmatchResult = FALSE;
	DWORD dwErrorCode = -1;
	this->sFileInfoInst.iChkCyrillic = -1;
	try
	{
		 // [U+0400–U+04FF] // /[À-ßà-ÿ¨¸¿¯³²]/u // [À-ßà-ÿ¨¸¿¯³²] // [\u0400-\u04FF]
		std::match_results<std::basic_string<TCHAR>::const_iterator> mrSearchresult;
		bmatchResult = std::regex_search(this->sFileInfoInst.sFileName, mrSearchresult, this->regMaskCyrillic);
		if (bmatchResult == FALSE)
			bmatchResult = TRUE;
		else
			bmatchResult = FALSE;
		this->sFileInfoInst.iChkCyrillic = bmatchResult;

		dwErrorCode = GetLastError();
		throw dwErrorCode;
	}
	catch (DWORD& dwErrorCode)
	{
		switch (dwErrorCode)
		{
		case ERROR_SUCCESS:
		case ERROR_NO_MORE_FILES:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->IterProcessFiles(ChkCyrillic(DWORD dwErrorCode)): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", this->sFileInfoInst.sFileName);
			break;
		default:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->IterProcessFiles(ChkCyrillic(DWORD dwErrorCode)): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", this->sFileInfoInst.sFileName);
			Logger::GetLogInstance()->PrepareMySQLLOG("Error in getting file attributes", "Function->IterObjects(ChkCyrillic(DWORD dwErrorCode))", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCode, this->sFileInfoInst.sFileName);
		}
	}

	return dwErrorCode;
};
DWORD FileProcess::ChkProjectDB()
{
	DWORD dwErrorCode = -1;
	BOOL bmatchResult = FALSE;
	this->sFileInfoInst.iChkProjectDB = -1;
	try
	{
		dwErrorCode = DBProcess::dbProcInstance().DBGetProjects(this, bmatchResult);
		this->sFileInfoInst.iChkProjectDB = bmatchResult;

		dwErrorCode = GetLastError();
		throw dwErrorCode;
	}
	catch (DWORD& dwErrorCode)
	{
		switch (dwErrorCode)
		{
		case ERROR_SUCCESS:
		case ERROR_NO_MORE_FILES:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->IterProcessFiles(ChkProjectDB(DWORD dwErrorCode)): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ",this->sFileInfoInst.sFileName);
			break;
		default:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->IterProcessFiles(ChkProjectDB(DWORD dwErrorCode)): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ",this->sFileInfoInst.sFileName);
			Logger::GetLogInstance()->PrepareMySQLLOG("Error in getting file attributes", "Function->IterObjects(ChkProjectDB(DWORD dwErrorCode))", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCode,this->sFileInfoInst.sFileName);
		}
	}

	return dwErrorCode;
};
DWORD FileProcess::ChkProjectDR()
{
	DWORD dwErrorCode = -1;
	BOOL bmatchResult = FALSE;
	this->sFileInfoInst.iChkProjectDR = -1;
	std::transform(this->sFileInfoInst.sFile_ProjectbyPath.begin(), this->sFileInfoInst.sFile_ProjectbyPath.end(), this->sFileInfoInst.sFile_ProjectbyPath.begin(), ::tolower);
	std::transform(this->sFileInfoInst.sFile_ProjectbyName.begin(), this->sFileInfoInst.sFile_ProjectbyName.end(), this->sFileInfoInst.sFile_ProjectbyName.begin(), ::tolower);
	try
	{
		if (this->sFileInfoInst.sFile_ProjectbyPath != this->sFileInfoInst.sFile_ProjectbyName)
			bmatchResult = NULL;
		else
			bmatchResult = TRUE;
		this->sFileInfoInst.iChkProjectDR = bmatchResult;

		dwErrorCode = GetLastError();
		throw dwErrorCode;
	}
	catch (DWORD& dwErrorCode)
	{
		switch (dwErrorCode)
		{
		case ERROR_SUCCESS:
		case ERROR_NO_MORE_FILES:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->IterProcessFiles(ChkProjectDR(DWORD dwErrorCode)): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ",this->sFileInfoInst.sFileName);
			break;
		default:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->IterProcessFiles(ChkProjectDR(DWORD dwErrorCode)): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ",this->sFileInfoInst.sFileName);
			Logger::GetLogInstance()->PrepareMySQLLOG("Error in getting file attributes", "Function->IterObjects(ChkProjectDR(DWORD dwErrorCode))", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCode,this->sFileInfoInst.sFileName);
		}
	}
	
	return dwErrorCode;
};
DWORD FileProcess::ChkÑompanyDB()
{
	DWORD dwErrorCode = -1;
	BOOL bmatchResult = FALSE;
	this->sFileInfoInst.iChkÑompanyDB = -1;
	try
	{
		std::match_results<std::basic_string<TCHAR>::const_iterator> mrSearchresult;
		bmatchResult = std::regex_search(this->sFileInfoInst.sFileDirPathChngView, this->regFindDStageShaPubZZ);
		if (bmatchResult == TRUE)
		{
			bmatchResult = std::regex_search(this->sFileInfoInst.sFile_CompanyRolebyName, mrSearchresult, this->regMaskDCompanyShaPubZZ);
		}
		else
		{
			bmatchResult = std::regex_search(this->sFileInfoInst.sFile_CompanyRolebyName, mrSearchresult, this->regMaskCompany);
		}

		if (mrSearchresult.ready() && bmatchResult != 0)
		{
			bmatchResult = FALSE;
			//std::wcout << mrSearchresult.str(1) << std::endl;
			this->sFileInfoInst.sFile_CompanybyName = mrSearchresult.str(1);
			dwErrorCode = DBProcess::dbProcInstance().DBGetCompany(this, bmatchResult);
		}
		else
		{
			bmatchResult = NULL;
			return ERROR_INVALID_DATA;
		}
		this->sFileInfoInst.iChkÑompanyDB = bmatchResult;

		dwErrorCode = GetLastError();
		throw dwErrorCode;
	}
	catch (DWORD& dwErrorCode)
	{
		switch (dwErrorCode)
		{
		case ERROR_SUCCESS:
		case ERROR_NO_MORE_FILES:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->IterProcessFiles(ChkÑompanyDB(DWORD dwErrorCode)): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ",this->sFileInfoInst.sFileName);
			break;
		default:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->IterProcessFiles(ChkÑompanyDB(DWORD dwErrorCode)): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ",this->sFileInfoInst.sFileName);
			Logger::GetLogInstance()->PrepareMySQLLOG("Error in getting file attributes", "Function->IterObjects(ChkÑompanyDB(DWORD dwErrorCode))", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCode,this->sFileInfoInst.sFileName);
		}
	}
	
	return dwErrorCode;
};
DWORD FileProcess::ChkRoleDB()
{
	DWORD dwErrorCode = -1;
	BOOL bmatchResult = FALSE;
	this->sFileInfoInst.iChkRoleDB = -1;
	try
	{
		std::match_results<std::basic_string<TCHAR>::const_iterator> mrSearchresult;
		std::match_results<std::basic_string<TCHAR>::const_iterator> mrSearchresult2;
		std::basic_string<TCHAR> sTMP;
		bmatchResult = std::regex_search(this->sFileInfoInst.sFileDirPathChngView, this->regFindDStageShaPubZZ);
		if (bmatchResult == TRUE)
		{
			this->sFileInfoInst.iChkRoleDB = bmatchResult;
			dwErrorCode = GetLastError();
			throw dwErrorCode;
		}
		else
		{
			bmatchResult = std::regex_search(this->sFileInfoInst.sFile_CompanyRolebyName, mrSearchresult, this->regMaskRole);
		}
		
		if (mrSearchresult.ready() && bmatchResult != 0)
		{
			bmatchResult = FALSE;
			sTMP = mrSearchresult.str(1);
			//bmatchResult = std::regex_search(this->sFileInfoInst.sFile_CompanyRolebyName,	mrSearchresult,		this->regMaskRole);
			bmatchResult = std::regex_search(sTMP, mrSearchresult2,	this->stRegRoleMask);
			this->sFileInfoInst.sFile_RolebyName = mrSearchresult2.str(1);
			dwErrorCode = DBProcess::dbProcInstance().DBGetRole(this, bmatchResult);
		}
		else
		{
			bmatchResult = FALSE;
			return ERROR_INVALID_DATA;
		}
		this->sFileInfoInst.iChkRoleDB = bmatchResult;

		dwErrorCode = GetLastError();
		throw dwErrorCode;
	}
	catch (DWORD& dwErrorCode)
	{
		switch (dwErrorCode)
		{
		case ERROR_SUCCESS:
		case ERROR_NO_MORE_FILES:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->IterProcessFiles(ChkRoleDB(DWORD dwErrorCode)): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ",this->sFileInfoInst.sFileName);
			break;
		default:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->IterProcessFiles(ChkRoleDB(DWORD dwErrorCode)): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ",this->sFileInfoInst.sFileName);
			Logger::GetLogInstance()->PrepareMySQLLOG("Error in getting file attributes", "Function->IterObjects(ChkÑompanyDB(DWORD dwErrorCode))", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCode,this->sFileInfoInst.sFileName);
		}
	}

	return dwErrorCode;
};
DWORD FileProcess::ChkStageDB()
{
	DWORD dwErrorCode = -1;
	BOOL bmatchResult = FALSE;
	this->sFileInfoInst.iChkStageDB = -1;
	//int iProjectStatus = -1;
	std::vector<std::basic_string<TCHAR>> sStageMapping_Projects	= { _T("lpk"), _T("bzh"), _T("zrc") };
	std::vector<std::basic_string<TCHAR>> sStageMapping_Stages		= { _T("C01"), _T("C00") };
	std::vector<std::basic_string<TCHAR>>::const_iterator itsStageMapping_Projects = sStageMapping_Projects.begin();
	std::basic_string<TCHAR> sStageTMP;
	try
	{
		std::match_results<std::basic_string<TCHAR>::const_iterator> mrSearchresult;
		bmatchResult = std::regex_search(this->sFileInfoInst.sFile_ProjectStagebyName, mrSearchresult, this->regMaskStage);

		if (mrSearchresult.ready() && bmatchResult != 0)
		{
			bmatchResult = FALSE;
			sStageTMP = mrSearchresult.str(1);
			for (; itsStageMapping_Projects != sStageMapping_Projects.end(); ++itsStageMapping_Projects)
			{
				if (this->sFileInfoInst.sFile_ProjectbyName == *itsStageMapping_Projects)
				{
					if (sStageTMP == sStageMapping_Stages.at(0))
					{
						sStageTMP = sStageMapping_Stages.at(1);
					}
				}
			}
			std::transform(sStageTMP.begin(), sStageTMP.end(), sStageTMP.begin(), ::tolower);
			this->sFileInfoInst.sFile_ProjectStageforDB = this->sFileInfoInst.sFile_ProjectbyName + _T('-') + sStageTMP;
			this->sFileInfoInst.sFile_StagebyName = mrSearchresult.str(1);
			dwErrorCode = DBProcess::dbProcInstance().DBGetStage(this, bmatchResult);
		}
		else
		{
			bmatchResult = FALSE;
			return ERROR_INVALID_DATA;
		}
		this->sFileInfoInst.iChkStageDB = bmatchResult;

		dwErrorCode = GetLastError();
		throw dwErrorCode;
	}
	catch (DWORD& dwErrorCode)
	{
		switch (dwErrorCode)
		{
		case ERROR_SUCCESS:
		case ERROR_NO_MORE_FILES:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->IterProcessFiles(ChkStageDB(DWORD dwErrorCode)): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ",this->sFileInfoInst.sFileName);
			break;
		default:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->IterProcessFiles(ChkStageDB(DWORD dwErrorCode)): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ",this->sFileInfoInst.sFileName);
			Logger::GetLogInstance()->PrepareMySQLLOG("Error in getting file attributes", "Function->IterObjects(ChkÑChkStageDB(DWORD dwErrorCode))", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCode,this->sFileInfoInst.sFileName);
		}
	}

	return dwErrorCode;
};
DWORD FileProcess::ChkStageDR()
{
	DWORD dwErrorCode = -1;
	BOOL bmatchResult = FALSE;
	this->sFileInfoInst.iChkStageDR = -1;
	try
	{
		std::match_results<std::basic_string<TCHAR>::const_iterator> mrSearchresult;
		bmatchResult = std::regex_search(this->sFileInfoInst.sFile_ProjectStagebyName, mrSearchresult, this->regMaskStage);
		this->sFileInfoInst.sFile_StagebyPath = mrSearchresult.str(1);

		if (this->sFileInfoInst.sFile_StagebyPath != this->sFileInfoInst.sFile_StagebyName)
			bmatchResult = NULL;
		else
			bmatchResult = TRUE;
		this->sFileInfoInst.iChkStageDR = bmatchResult;

		dwErrorCode = GetLastError();
		throw dwErrorCode;
	}
	catch (DWORD& dwErrorCode)
	{
		switch (dwErrorCode)
		{
		case ERROR_SUCCESS:
		case ERROR_NO_MORE_FILES:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->IterProcessFiles(ChkStageDR(DWORD dwErrorCode)): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ",this->sFileInfoInst.sFileName);
			break;
		default:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->IterProcessFiles(ChkStageDR(DWORD dwErrorCode)): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ",this->sFileInfoInst.sFileName);
			Logger::GetLogInstance()->PrepareMySQLLOG("Error in getting file attributes", "Function->IterObjects(ChkStageDR(DWORD dwErrorCode))", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCode,this->sFileInfoInst.sFileName);
		}
	}

	return dwErrorCode;
};
DWORD FileProcess::ChkSubSystemDB()
{
	DWORD dwErrorCode = -1;
	BOOL bmatchResult = FALSE;
	this->sFileInfoInst.iChkSubSystemDB = -1;
	try
	{
		dwErrorCode = DBProcess::dbProcInstance().DBGetSubsystem(this, bmatchResult);
		bmatchResult = TRUE; /// DELETE after adding checking logic
		this->sFileInfoInst.iChkSubSystemDB = bmatchResult;

		dwErrorCode = GetLastError();
		throw dwErrorCode;
	}
	catch (DWORD& dwErrorCode)
	{
		switch (dwErrorCode)
		{
		case ERROR_SUCCESS:
		case ERROR_NO_MORE_FILES:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->IterProcessFiles(ChkSubSystemDB(DWORD dwErrorCode)): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ",this->sFileInfoInst.sFileName);
			break;
		default:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->IterProcessFiles(ChkSubSystemDB(DWORD dwErrorCode)): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ",this->sFileInfoInst.sFileName);
			Logger::GetLogInstance()->PrepareMySQLLOG("Error in getting file attributes", "Function->IterObjects(ChkSubSystemDB(DWORD dwErrorCode))", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCode,this->sFileInfoInst.sFileName);
		}
	}

	return dwErrorCode;
};
DWORD FileProcess::ChkDataTypeDB()
{
	DWORD dwErrorCode = -1;
	BOOL bmatchResult = FALSE;
	this->sFileInfoInst.iChkDataTypeDB = -1;
	try
	{
		dwErrorCode = DBProcess::dbProcInstance().DBGetDatatype(this, bmatchResult);
		this->sFileInfoInst.iChkDataTypeDB = bmatchResult;

		dwErrorCode = GetLastError();
		throw dwErrorCode;
	}
	catch (DWORD& dwErrorCode)
	{
		switch (dwErrorCode)
		{
		case ERROR_SUCCESS:
		case ERROR_NO_MORE_FILES:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->IterProcessFiles(ChkSubSystemDB(DWORD dwErrorCode)): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ",this->sFileInfoInst.sFileName);
			break;
		default:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->IterProcessFiles(ChkSubSystemDB(DWORD dwErrorCode)): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ",this->sFileInfoInst.sFileName);
			Logger::GetLogInstance()->PrepareMySQLLOG("Error in getting file attributes", "Function->IterObjects(ChkSubSystemDB(DWORD dwErrorCode))", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCode,this->sFileInfoInst.sFileName);
		}
	}

	return dwErrorCode;
};