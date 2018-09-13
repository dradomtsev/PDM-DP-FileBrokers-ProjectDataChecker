// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"

FileProcess::FileProcess() : sFileInfoInst()
{
	iIndex			= 0;
	npos			= -1;
	//vstExistExtens	= { _T("") };
	//vstExistDirs	= { _T("") };
	curSysTime		= { 0 };
	curFileTime		= { 0 };
	tStartChkPeriod = { 0 };
	tEndChkPeriod	= { 0 };
	//sFileInfoInst	= { 0 };
};
FileProcess::~FileProcess() {};
DWORD FileProcess::InitReviewPeriod()
{
	DWORD dwErrorCode = -1;
	BOOL bStatus = FALSE;
	//ULONGLONG qwResult;
	//std::basic_string<TCHAR> stNone = _T("");
	SetLastError(ERROR_SUCCESS);
	try
	{
		bStatus = SystemTimeToFileTime(&UI::GetUIInst()->stStartChkPeriod, &this->tStartChkPeriod);
		bStatus = FALSE;
		//std::cout << bStatus;
		bStatus = SystemTimeToFileTime(&UI::GetUIInst()->stEndChkPeriod, &this->tEndChkPeriod);
		//GetSystemTimeAsFileTime(&this->curFileTime); //Time point to check file age from
		//if (vstDays.compare(stNone) == 0)
		//	return dwErrorCode;
		//qwResult = (((ULONGLONG)this->curFileTime.dwHighDateTime) << 32) + this->curFileTime.dwLowDateTime;
		//std::string::size_type sz = 0;
		//_int64 i64Period = std::stoll(vstDays, &sz, 0);
		//qwResult -= i64Period * _DAY; //(_int64)vstDays.c_str()
		//this->curFileTime.dwLowDateTime = (DWORD)(qwResult & 0xFFFFFFFF);
		//this->curFileTime.dwHighDateTime = (DWORD)(qwResult >> 32);

		dwErrorCode = GetLastError();
		if (dwErrorCode == ERROR_INVALID_PARAMETER)
		{
			SetLastError(ERROR_SUCCESS);
			dwErrorCode = ERROR_SUCCESS;
		}
		throw dwErrorCode;
	}
	catch (DWORD Err_code)
	{
		switch (Err_code)
		{
		case ERROR_SUCCESS:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->InitReviewPeriod(): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", "Current System time");
			break;
		default:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->InitReviewPeriod(): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", "Current System time");
			ErrorHandle::GetErrorHandleInst()->ErrorExit(_T("InitDirbyINI->InitReviewPeriod()"), _T("."), dwErrorCode);
		}
	}
	return dwErrorCode;
};
DWORD FileProcess::StartMySQLConect()
{
	DWORD dwErrorCode = -1;
	SetLastError(ERROR_SUCCESS);
	//2. Init MYSQL driver & create connection
	try
	{
		dwErrorCode = DBProcess::dbProcInstance()->DBCreateMYSQLConnection();
		throw  dwErrorCode;
	}
	catch (DWORD dwErrorCode)
	{
		switch (dwErrorCode)
		{
		case ERROR_SUCCESS:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->StartMySQLConect(DBCreateMYSQLConnection()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", "MySQL Connection");
			break;
		default:
			DBProcess::dbProcInstance()->DBCloseMYSQLConnection();
			Logger::GetLogInstance()->PrepareTXTLOG("Function->StartMySQLConect(DBCreateMYSQLConnection()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", "MySQL Connection");
			ErrorHandle::GetErrorHandleInst()->ErrorExit(_T("StartMySQLConect(DBCreateMYSQLConnection())"), _T("."), dwErrorCode);
		}
	}
	return dwErrorCode;
};
DWORD FileProcess::InitMySQLConect()
{
	DWORD dwErrorCode = -1;
	SetLastError(ERROR_SUCCESS);
	//3. Init MySQL tables by trancation
	try
	{
		dwErrorCode = DBProcess::dbProcInstance()->DBInitTables();
		throw  dwErrorCode;
	}
	catch (DWORD dwErrorCode)
	{
		switch (dwErrorCode)
		{
		case ERROR_SUCCESS:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->InitMySQLConect(DBInitTables()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", "MySQL tables");
			break;
		default:
			DBProcess::dbProcInstance()->DBCloseMYSQLConnection();
			Logger::GetLogInstance()->PrepareTXTLOG("Function->InitMySQLConect(DBInitTables()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", "MySQL tables");
			ErrorHandle::GetErrorHandleInst()->ErrorExit(_T("InitMySQLConect(DBInitTables())"), _T("."), dwErrorCode);
		}
	}
	return dwErrorCode;
};
DWORD FileProcess::CloseMySQLConect()
{
	DWORD dwErrorCode = -1;
	SetLastError(ERROR_SUCCESS);
	//5. Close MYSQL connection
	try
	{
		dwErrorCode = DBProcess::dbProcInstance()->DBCloseMYSQLConnection();
		throw  dwErrorCode;
	}
	catch (DWORD dwErrorCode)
	{
		switch (dwErrorCode)
		{
		case ERROR_SUCCESS:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->CloseMySQLConect(DBCloseMYSQLConnection()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", "MySQL tables");
			break;
		default:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->CloseMySQLConect(DBCloseMYSQLConnection()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", "MySQL tables");
			ErrorHandle::GetErrorHandleInst()->ErrorExit(_T("DBCloseMYSQLConnection(DBCloseMYSQLConnection())"), _T("."), dwErrorCode);
		}
	}
	return dwErrorCode;
};
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
	catch (DWORD dwErrorCode)
	{
		switch (dwErrorCode)
		{
		case ERROR_SUCCESS:
		case ERROR_NO_MORE_FILES:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->IterDirs(IterObjects()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", "MySQL tables");
			break;
		default:
			DBProcess::dbProcInstance()->DBCloseMYSQLConnection();
			Logger::GetLogInstance()->PrepareTXTLOG("Function->IterDirs(IterObjects()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", "MySQL tables");
			ErrorHandle::GetErrorHandleInst()->ErrorExit(_T("IterDirs(IterObjects())"), (LPTSTR)stWorkDir.c_str(), dwErrorCode);
		}
	}
	return  dwErrorCode;
}
DWORD FileProcess::IterObjects(std::basic_string<TCHAR> twrkDir,const std::basic_string<TCHAR> &twrkDirName, int iCounter)
{
	WIN32_FIND_DATA fdFileData = { 0 };
	HANDLE hFileDataFindFirst = nullptr;
	HANDLE hFileReparse = nullptr;
	//BY_HANDLE_FILE_INFORMATION hfiFileReparse;
	SetLastError(ERROR_SUCCESS);
	DWORD dwErrorCode = -1;
	BOOL bStatus = FALSE;
	//0. Set current working directory
	std::basic_string<TCHAR> twrkDirtemp = twrkDir + twrkDirName;

	wprintf_s(_T("Working in %s\r\n"), twrkDirtemp.c_str());

	twrkDirtemp.insert(0, _T("\\\\?\\"));
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
			/*else if (fdFileData.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM)
				continue;*/
			//else if (fdFileData.dwFileAttributes & FILE_ATTRIBUTE_TEMPORARY)
				//break;
			else if (fdFileData.dwFileAttributes & FILE_ATTRIBUTE_VIRTUAL)
				continue;
			else if (fdFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				// Examine dirs names for "." and dirs for ignore
				this->iIndex = this->sFileInfoInst.sFileName.find_last_of('.',0);
				if (this->iIndex != this->npos)
					if (this->iIndex == 0 || this->iIndex == 1)
						continue;
				int iDirStatus = 0;
				for (std::vector<std::basic_string<TCHAR>>::iterator it = UI::GetUIInst()->vstFolderstoIgnore.begin(); it != UI::GetUIInst()->vstFolderstoIgnore.end(); ++it)
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
				for (std::vector<std::basic_string<TCHAR>>::iterator it = UI::GetUIInst()->vstFolderstoIgnore.begin(); it != UI::GetUIInst()->vstFolderstoIgnore.end(); ++it)
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
					//if (UI::GetUIInst()->vstDaysToParse.compare(_T("")) == 0)
						//this->IterProcessFiles(hFileDataFindFirst, twrkDirtemp, twrkDirName);
					//else
					//{
					int iMaxTimeFlag	= CompareFileTime(&fdFileData.ftCreationTime, &fdFileData.ftLastWriteTime);
					switch (iMaxTimeFlag)
					{
					case -1:
						this->curFileTime = fdFileData.ftLastWriteTime;
					case 0:
						this->curFileTime = fdFileData.ftCreationTime;
					case 1:
						this->curFileTime = fdFileData.ftCreationTime;
					}
					int iStartLater		= CompareFileTime(&this->tStartChkPeriod, &this->curFileTime);
					int iEndEarlier		= CompareFileTime(&this->tEndChkPeriod, &this->curFileTime);
					//std::cout << std::endl << iStartLater << "________" << iEndEarlier;
					if (iStartLater == -1 && iEndEarlier == 1) //CompareFileTime(&this->curFileTime, &fdFileData.ftCreationTime) && CompareFileTime(&this->curFileTime, &fdFileData.ftLastWriteTime)
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
			ErrorHandle::GetErrorHandleInst()->ErrorExit(_T("IterObjects->Main \"do-while loop\""), (LPTSTR)twrkDirtemp.c_str(), dwErrorCode);
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
			hFile = CreateFile(stFilePathtemp.c_str(), READ_CONTROL, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); //READ_CONTROL
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
	catch (DWORD dwErrorCode)
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
	catch (...)
	{
		FindClose(hFileDataFindFirst);
		if (hFile != nullptr)
			CloseHandle(hFile);
		Logger::GetLogInstance()->PrepareTXTLOG("Function->IterProcessFiles(CreateFile(...)): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", this->sFileInfoInst.sFileName);
		Logger::GetLogInstance()->PrepareMySQLLOG("Error in getting file attributes", "Function->IterObjects(CreateFile(...))", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCode, this->sFileInfoInst.sFileName);
		ErrorHandle::GetErrorHandleInst()->ErrorExit(_T("IterProcessFiles->CreateFile(...)"), (LPTSTR)this->sFileInfoInst.sFileName.c_str(), dwErrorCode);
	}
	SetLastError(ERROR_SUCCESS);

	try 
	{
		// Get file info
		if (hFile != INVALID_HANDLE_VALUE)
		{
			this->sFileInfoInst.sFileDirName = twrkDirName;
			this->sFileInfoInst.sFileDirPath = twrkDirtemp;

			this->sFileInfoInst.sFileExtension = PathFindExtension(this->sFileInfoInst.sFileName.c_str());

			dwErrorCode = this->GetFileOwnerName(hFile, this->sFileInfoInst.sFileName);
			SetLastError(ERROR_SUCCESS);
			dwErrorCode = this->GetFileSizeInst(hFile);
			dwErrorCode = this->GetFileCAWTime(hFile);
			dwErrorCode = this->ChangeFolderView();


			dwErrorCode = this->ChkMask();
			if (this->sFileInfoInst.iChkMask == 0)
				throw dwErrorCode;
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
	catch (DWORD dwErrorCode)
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
	catch (...)
	{
		FindClose(hFileDataFindFirst);
		if (hFile != nullptr)
			CloseHandle(hFile);
		Logger::GetLogInstance()->PrepareTXTLOG("Function->IterProcessFiles(GetFileInfo(...)): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", this->sFileInfoInst.sFileName);
		Logger::GetLogInstance()->PrepareMySQLLOG("Error in getting file attributes", "Function->IterObjects(GetFileInfo(...))", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCode, this->sFileInfoInst.sFileName);
		ErrorHandle::GetErrorHandleInst()->ErrorExit(_T("IterProcessFiles->GetFileInfo(...)"), (LPTSTR)this->sFileInfoInst.sFileName.c_str(), dwErrorCode);
	}

	// Write to table "Folders"
	//try
	//{
		if (this->vstExistDirs.empty())
		{
			this->vstExistDirs.push_back(this->sFileInfoInst.sFileDirPath);
			DBProcess::dbProcInstance()->DBWriteFolders(this);
		}
		else
		{
			std::vector<std::basic_string<TCHAR>>::const_iterator it = std::find(this->vstExistDirs.begin(), this->vstExistDirs.end(), this->sFileInfoInst.sFileDirPath);
			if (it == this->vstExistDirs.end())
			{
				this->vstExistDirs.push_back(this->sFileInfoInst.sFileDirPath);
				DBProcess::dbProcInstance()->DBWriteFolders(this);
			}
		}
		//dwErrorCode = GetLastError();
		//throw dwErrorCode;
	//}
	//catch (DWORD dwErrorCode)
	//{
	//	switch (dwErrorCode)
	//	{
	//	case ERROR_SUCCESS:
	//		//Logger::GetLogInstance()->PrepareTXTLOG("Function->IterProcessFiles(DBWriteFolders()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", this->sFileInfoInst.sFileDirPathChngView);
	//		break;
	//	//default:
	//		//FindClose(hFileDataFindFirst);
	//		//if (hFile != nullptr)
	//			//CloseHandle(hFile);
	//		//Logger::GetLogInstance()->PrepareTXTLOG("Function->IterObjects(DBWriteFolders()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", this->sFileInfoInst.sFileDirPathChngView);
	//		//Logger::GetLogInstance()->PrepareMySQLLOG("Error in writing to TABLE folders", "Function->IterObjects(DBWriteFolders())", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCode, this->sFileInfoInst.sFileDirPathChngView);
	//		//ErrorHandle::GetErrorHandleInst()->ErrorExit(_T("IterObjects->DBWriteFolders()"), (LPTSTR)this->sFileInfoInst.sFileDirPath.c_str(), dwErrorCode);
	//		//break;
	//	}
	//}

	// Write to table "Extensions"
	try
	{
		//std::transform(this->sFileInfoInst.sFileExtension.begin(), this->sFileInfoInst.sFileExtension.end(), this->sFileInfoInst.sFileExtension.begin(), ::tolower);
		if (this->vstExistExtens.empty())
		{
			this->vstExistExtens.push_back(this->sFileInfoInst.sFileExtension);
			DBProcess::dbProcInstance()->DBWriteExtensions(this);
		}
		else
		{
			auto result = std::find(this->vstExistExtens.begin(), this->vstExistExtens.end(), this->sFileInfoInst.sFileExtension);
			if (result == this->vstExistExtens.end())
			{
					this->vstExistExtens.push_back(this->sFileInfoInst.sFileExtension.c_str());
					DBProcess::dbProcInstance()->DBWriteExtensions(this);
			}
		}
		dwErrorCode = GetLastError();
		throw dwErrorCode;
	}
	catch (DWORD dwErrorCode)
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

	// Write to table "Files"
	try
	{
		DBProcess::dbProcInstance()->DBWriteFiles(this);
		dwErrorCode = GetLastError();
		throw dwErrorCode;
	}
	catch (DWORD dwErrorCode)
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
	
	//if (hFile != nullptr)
	//	CloseHandle(hFile);

	return dwErrorCode;
};

DWORD FileProcess::GetFileOwnerName(HANDLE hFile, std::basic_string<TCHAR> sFileName)
{
	DWORD dwRtnCode;
	DWORD dwErrorCode = -1;
	PSID pSidOwner = NULL;
	BOOL bRtnBool = TRUE;
	LPTSTR AcctName = NULL;
	LPTSTR DomainName = NULL;
	DWORD dwAcctName = 1, dwDomainName = 1;
	SID_NAME_USE eUse = SidTypeUnknown;
	PSECURITY_DESCRIPTOR pSD = NULL;

	//// Get the handle of the file object.
	//hFile = CreateFile(
	//	TEXT("AAAAmyfile.txt"),
	//	GENERIC_READ,
	//	FILE_SHARE_READ,
	//	NULL,
	//	OPEN_EXISTING,
	//	FILE_ATTRIBUTE_NORMAL,
	//	NULL);

	 //Check GetLastError for CreateFile error code.
	//if (hFile == INVALID_HANDLE_VALUE) 
	//{
	//	DWORD dwErrorCode = 0;
	//	dwErrorCode = GetLastError();
	//	_tprintf(TEXT("GetFileOwnerName()->CreateFile error = %d\n"), dwErrorCode);
	//	return -1;
	//}

	// Get the owner SID of the file.
	try
	{
		//dwRtnCode = GetNamedSecurityInfo(sFileName.c_str(), SE_FILE_OBJECT, OWNER_SECURITY_INFORMATION, &pSidOwner, NULL, NULL, NULL, &pSD);
		dwRtnCode = GetSecurityInfo(hFile, SE_FILE_OBJECT, OWNER_SECURITY_INFORMATION, &pSidOwner, NULL, NULL, NULL, &pSD);
		dwErrorCode = GetLastError();
		throw dwErrorCode;
	}
	catch (DWORD dwErrorCode)
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
			//if (dwErrorCode == ERROR_INSUFFICIENT_BUFFER)
			//{
			//	SetLastError(ERROR_SUCCESS);
			//	dwErrorCode = 0;
			//}
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
							Logger::GetLogInstance()->PrepareMySQLLOG("Error in Look up Account Sid, Owner changed to \"Àäìèíèñòðàòîðû\"", "Function->LookupAccountSid(())", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCode, sFileName);
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
							//bRtnBool = FALSE;
							HeapDestroy(hProcHeapT);
							return dwErrorCode;
						}
					}
					else
					{
						wsFileAcctName.append(AcctName);
						this->sFileInfoInst.sFileOwnerName = wsFileAcctName;
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
			bRtnBool = HeapDestroy(hProcHeap);
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
	DWORD dwErrorCode;
	try
	{
		bStatus = GetFileSizeEx(hFile, &this->sFileInfoInst.sFileSize);

		dwErrorCode = GetLastError();
		throw dwErrorCode;
	}
	catch (DWORD dwErrorCode)
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
	catch (...)
	{
		Logger::GetLogInstance()->PrepareTXTLOG("Function->IterProcessFiles(GetFileSizeInst(...)): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ",this->sFileInfoInst.sFileName);
		Logger::GetLogInstance()->PrepareMySQLLOG("Error in getting file attributes", "Function->IterObjects(GetFileSizeInst(...))", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCode,this->sFileInfoInst.sFileName);
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
	catch (DWORD dwErrorCode)
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
	catch (...)
	{
		Logger::GetLogInstance()->PrepareTXTLOG("Function->IterProcessFiles(GetFileCAWTime(...)): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ",this->sFileInfoInst.sFileName);
		Logger::GetLogInstance()->PrepareMySQLLOG("Error in getting file attributes", "Function->IterObjects(GetFileCAWTime(...))", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCode,this->sFileInfoInst.sFileName);
	}

	return dwErrorCode;
};
DWORD FileProcess::GetFileInfobyName()
{
	std::basic_string<TCHAR> sFile_Info(this->sFileInfoInst.sFileName);
	std::vector<std::basic_string<TCHAR>> vstFile_Info;
	std::vector<std::basic_string<TCHAR>> vstFile_TMP;
	std::size_t nStartFrom = 0;
	std::size_t nLoc = 0;

	DWORD dwErrorCode = -1;
	try
	{
		while (nLoc != std::basic_string<TCHAR>::npos)
		{
			nLoc = sFile_Info.find('-', nStartFrom);
			vstFile_Info.push_back(sFile_Info.substr(nStartFrom, (nLoc - nStartFrom)));
			sFile_Info.erase(nStartFrom, nLoc + 1);
		}
		nLoc = 0;

		this->sFileInfoInst.sFile_ProjectbyName = vstFile_Info[0];
		this->sFileInfoInst.sFile_CompanyRolebyName = vstFile_Info[1];
		this->sFileInfoInst.sFile_ProjectStagebyName = vstFile_Info[2];
		this->sFileInfoInst.sFile_SubsystembyName = vstFile_Info[3];
		this->sFileInfoInst.sFile_DatatypebyName = vstFile_Info[4];

		while (nLoc != std::basic_string<TCHAR>::npos)
		{
			nLoc = this->sFileInfoInst.sFile_DatatypebyName.find('.', nStartFrom);
			vstFile_TMP.push_back(this->sFileInfoInst.sFile_DatatypebyName.substr(nStartFrom, (nLoc - nStartFrom)));
			this->sFileInfoInst.sFile_DatatypebyName.erase(nStartFrom, nLoc + 1);
		}
		this->sFileInfoInst.sFile_DatatypebyName = vstFile_TMP[0];

		dwErrorCode = GetLastError();
		throw dwErrorCode;
	}
	catch (DWORD dwErrorCode)
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
	catch (...)
	{
		Logger::GetLogInstance()->PrepareTXTLOG("Function->IterProcessFiles(GetFileInfobyName(...)): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ",this->sFileInfoInst.sFileName);
		Logger::GetLogInstance()->PrepareMySQLLOG("Error in getting file attributes", "Function->IterObjects(GetFileInfobyName(...))", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCode,this->sFileInfoInst.sFileName);
	}

	return dwErrorCode;
};
DWORD FileProcess::GetFileInfobyFolder()
{
	std::basic_string<TCHAR> sFile_Info;
	std::size_t nLoc = 0;
	DWORD dwErrorCode;
	bool bFindStatus = FALSE;
	//auto itvstFileName = UI::GetUIInst()->vstRootFolders.begin();
	//std::map <std::basic_string<TCHAR>, std::basic_string<TCHAR>> mRootFolders = { {_T("03-Published"), _T("P:")},{ _T("Resources"), _T("R:") },{ _T("02-Shared"), _T("S:") },{ _T("01-WorkInProgress"), _T("W:") }};
	try
	{
		sFile_Info = this->sFileInfoInst.sFileDirPath;
		//std::wcout << sFile_Info;
		//for (itvstFileName; itvstFileName != UI::GetUIInst()->vstRootFolders.end(); ++itvstFileName)
		//for (UI::GetUIInst()->itRootFolders = UI::GetUIInst()->vstRootFolders.begin(); UI::GetUIInst()->itRootFolders != UI::GetUIInst()->vstRootFolders.end(); ++UI::GetUIInst()->itRootFolders)
		//{
		//	nLoc = sFile_Info.find(itvstFileName->c_str(), 0);
		//	if (nLoc != std::basic_string<TCHAR>::npos)
		//	{
		//		sFile_Info.erase(0, nLoc + 1);
		//		bFindStatus = TRUE;

		//		//this->sFileInfoInst.sFileDirPath = sFile_Info;
		//		//this->sFileInfoInst.sFileDirPath.replace(this->sFileInfoInst.sFileDirPath.find(UI::GetUIInst()->itRootFolders->c_str()), UI::GetUIInst()->itRootFolders->length(), mRootFolders.at(UI::GetUIInst()->itRootFolders->c_str()));
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
	catch (DWORD dwErrorCode)
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
	catch (...)
	{
		Logger::GetLogInstance()->PrepareTXTLOG("Function->IterProcessFiles(GetFileInfobyFolder(...)): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ",this->sFileInfoInst.sFileName);
		Logger::GetLogInstance()->PrepareMySQLLOG("Error in getting file attributes", "Function->IterObjects(GetFileInfobyFolder(...))", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCode,this->sFileInfoInst.sFileName);
	}

	return dwErrorCode;
};
DWORD FileProcess::ChangeFolderView()
{
	std::basic_string<TCHAR> sFile_Info = this->sFileInfoInst.sFileDirPath;
	BOOL bFindStatus = FALSE;
	std::size_t nLoc = 0;
	DWORD dwErrorCode;
	int i = 0;

	//std::vector<std::basic_string<TCHAR>> vstFolderstoChng = { _T("h:\\CloudSt\\GDData\\03-Published"),  _T("h:\\Resources"), _T("h:\\CloudSt\\GDData\\02-Shared"), _T("h:\\CloudSt\\GDWiP\\01-WorkInProgress") };
	std::vector<std::basic_string<TCHAR>> vstFoldersfromChng;
	vstFoldersfromChng.push_back(_T("h:\\CloudSt\\GDData\\03-Published"));
	vstFoldersfromChng.push_back(_T("h:\\Resources"));
	vstFoldersfromChng.push_back(_T("h:\\CloudSt\\GDData\\02-Shared"));
	vstFoldersfromChng.push_back(_T("h:\\CloudSt\\GDWiP\\01-WorkInProgress"));
	//std::vector<std::basic_string<TCHAR>> vstFoldersfromChng	= { _T("P:"), _T("R:"), _T("S:"), _T("W:") };
	std::vector<std::basic_string<TCHAR>> vstFolderstoChng;
	vstFolderstoChng.push_back(_T("P:"));
	vstFolderstoChng.push_back(_T("R:"));
	vstFolderstoChng.push_back(_T("S:"));
	vstFolderstoChng.push_back(_T("W:"));
	std::vector<std::basic_string<TCHAR>>::const_iterator itvstFoldersfromChng = vstFoldersfromChng.begin();
	std::vector<std::basic_string<TCHAR>>::const_iterator itvstFolderstoChng = vstFolderstoChng.begin();
	//std::map <std::basic_string<TCHAR>, std::basic_string<TCHAR>>::const_iterator itmRootFolders;
	//std::map <std::basic_string<TCHAR>, std::basic_string<TCHAR>> mRootFolders = {	{ vstFoldersfromChng.at(0).c_str(), vstFolderstoChng.at(0).c_str() },
	//																				{ vstFoldersfromChng.at(1).c_str(), vstFolderstoChng.at(1).c_str() },
	//																				{ vstFoldersfromChng.at(2).c_str(), vstFolderstoChng.at(2).c_str() },
	//																				{ vstFoldersfromChng.at(3).c_str(), vstFolderstoChng.at(3).c_str() } };
	std::regex rgMask("(h:)");
	if (std::regex_match(sFile_Info.begin(), sFile_Info.end(), rgMask, std::regex_constants::match_not_bol))
	{
		try
		{
			for (itvstFoldersfromChng; itvstFoldersfromChng != vstFoldersfromChng.end(); ++itvstFoldersfromChng, ++itvstFolderstoChng)
			{
				nLoc = sFile_Info.find(*itvstFoldersfromChng, 0);
				if (nLoc != std::basic_string<TCHAR>::npos)
				{
					sFile_Info.replace(0, itvstFoldersfromChng->length(), itvstFolderstoChng->c_str());
					this->sFileInfoInst.sFileDirPathChngView = sFile_Info;
				}
			}
			nLoc = 0;
			dwErrorCode = GetLastError();
			throw dwErrorCode;
		}
		catch (DWORD dwErrorCode)
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
		catch (...)
		{
			Logger::GetLogInstance()->PrepareTXTLOG("Function->IterProcessFiles(ChangeFolderView(...)): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ",this->sFileInfoInst.sFileName);
			Logger::GetLogInstance()->PrepareMySQLLOG("Error in getting file attributes", "Function->IterObjects(ChangeFolderView(...))", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCode,this->sFileInfoInst.sFileName);
		}
	}
	else 
	{
		this->sFileInfoInst.sFileDirPathChngView = sFile_Info;
		dwErrorCode = GetLastError();
	}

	return dwErrorCode;
};

DWORD FileProcess::ChkMask()
{
	BOOL bmatchResult = FALSE;
	DWORD dwErrorCode;
	this->sFileInfoInst.iChkMask = -1;
	try
	{
		std::basic_regex<TCHAR> regMaskCommon(UI::GetUIInst()->vstChk_FileMaskCommon.c_str());
		bmatchResult = std::regex_match(this->sFileInfoInst.sFileName, regMaskCommon);
		this->sFileInfoInst.iChkMask = bmatchResult;

		dwErrorCode = GetLastError();
		throw dwErrorCode;
	}
	catch (DWORD dwErrorCode)
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
		}
	}
	catch (...)
	{
		Logger::GetLogInstance()->PrepareTXTLOG("Function->IterProcessFiles(ChkMask(...)): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ",this->sFileInfoInst.sFileName);
		Logger::GetLogInstance()->PrepareMySQLLOG("Error in getting file attributes", "Function->IterObjects(ChkMask(...))", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCode,this->sFileInfoInst.sFileName);
	}

	return dwErrorCode;
};
DWORD FileProcess::ChkCyrillic()
{
	BOOL bmatchResult = NULL;
	DWORD dwErrorCode;
	this->sFileInfoInst.iChkCyrillic = -1;
	try
	{
		std::basic_regex<TCHAR> regMaskCommon(UI::GetUIInst()->vstChk_Cyrillic.c_str()); // [U+0400–U+04FF] // /[À-ßà-ÿ¨¸¿¯³²]/u // [À-ßà-ÿ¨¸¿¯³²] // [\u0400-\u04FF]
		std::match_results<std::basic_string<TCHAR>::const_iterator> mrSearchresult;
		bmatchResult = std::regex_search(this->sFileInfoInst.sFileName, mrSearchresult, regMaskCommon);
		if (bmatchResult == FALSE)
			bmatchResult = TRUE;
		else
			bmatchResult = FALSE;
		this->sFileInfoInst.iChkCyrillic = bmatchResult;

		dwErrorCode = GetLastError();
		throw dwErrorCode;
	}
	catch (DWORD dwErrorCode)
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
	catch (...)
	{
		Logger::GetLogInstance()->PrepareTXTLOG("Function->IterProcessFiles(ChkCyrillic(...)): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ",this->sFileInfoInst.sFileName);
		Logger::GetLogInstance()->PrepareMySQLLOG("Error in getting file attributes", "Function->IterObjects(ChkCyrillic(...))", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCode,this->sFileInfoInst.sFileName);
	}

	return dwErrorCode;
};
DWORD FileProcess::ChkProjectDB()
{
	DWORD dwErrorCode = -1;
	BOOL bmatchResult = NULL;
	this->sFileInfoInst.iChkProjectDB = -1;
	try
	{
		dwErrorCode = DBProcess::dbProcInstance()->DBGetProjects(this, bmatchResult);
		this->sFileInfoInst.iChkProjectDB = bmatchResult;

		dwErrorCode = GetLastError();
		throw dwErrorCode;
	}
	catch (DWORD dwErrorCode)
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
	catch (...)
	{
		Logger::GetLogInstance()->PrepareTXTLOG("Function->IterProcessFiles(ChkProjectDB(...)): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ",this->sFileInfoInst.sFileName);
		Logger::GetLogInstance()->PrepareMySQLLOG("Error in getting file attributes", "Function->IterObjects(ChkProjectDB(...))", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCode,this->sFileInfoInst.sFileName);
	}

	return dwErrorCode;
};
DWORD FileProcess::ChkProjectDR()
{
	DWORD dwErrorCode = -1;
	BOOL bmatchResult = NULL;
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
	catch (DWORD dwErrorCode)
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
	catch (...)
	{
		Logger::GetLogInstance()->PrepareTXTLOG("Function->IterProcessFiles(ChkProjectDR(...)): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ",this->sFileInfoInst.sFileName);
		Logger::GetLogInstance()->PrepareMySQLLOG("Error in getting file attributes", "Function->IterObjects(ChkProjectDR(...))", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCode,this->sFileInfoInst.sFileName);
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
		std::basic_regex<TCHAR> regMaskCommon(UI::GetUIInst()->vstChk_Company.c_str());
		std::match_results<std::basic_string<TCHAR>::const_iterator> mrSearchresult;
		bmatchResult = std::regex_search(this->sFileInfoInst.sFile_CompanyRolebyName, mrSearchresult, regMaskCommon);

		if (mrSearchresult.ready() && bmatchResult != 0)
		{
			bmatchResult = FALSE;
			//std::wcout << mrSearchresult.str(1) << std::endl;
			this->sFileInfoInst.sFile_CompanybyName = mrSearchresult.str(1);
			dwErrorCode = DBProcess::dbProcInstance()->DBGetCompany(this, bmatchResult);
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
	catch (DWORD dwErrorCode)
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
	catch (...)
	{
		Logger::GetLogInstance()->PrepareTXTLOG("Function->IterProcessFiles(ChkÑompanyDB(...)): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ",this->sFileInfoInst.sFileName);
		Logger::GetLogInstance()->PrepareMySQLLOG("Error in getting file attributes", "Function->IterObjects(ChkÑompanyDB(...))", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCode,this->sFileInfoInst.sFileName);
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
		std::basic_regex<TCHAR> regMaskCommon(UI::GetUIInst()->vstChk_Role.c_str());
		std::match_results<std::basic_string<TCHAR>::const_iterator> mrSearchresult;
		bmatchResult = std::regex_search(this->sFileInfoInst.sFile_CompanyRolebyName, mrSearchresult, regMaskCommon);

		if (mrSearchresult.ready() && bmatchResult != 0)
		{
			bmatchResult = FALSE;
			//std::wcout << mrSearchresult.str(1) << std::endl;
			this->sFileInfoInst.sFile_RolebyName = mrSearchresult.str(1);
			dwErrorCode = DBProcess::dbProcInstance()->DBGetRole(this, bmatchResult);
		}
		else
		{
			bmatchResult = NULL;
			return ERROR_INVALID_DATA;
		}
		this->sFileInfoInst.iChkRoleDB = bmatchResult;

		dwErrorCode = GetLastError();
		throw dwErrorCode;
	}
	catch (DWORD dwErrorCode)
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
	catch (...)
	{
		Logger::GetLogInstance()->PrepareTXTLOG("Function->IterProcessFiles(ChkRoleDB(...)): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ",this->sFileInfoInst.sFileName);
		Logger::GetLogInstance()->PrepareMySQLLOG("Error in getting file attributes", "Function->IterObjects(ChkRoleDB(...))", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCode,this->sFileInfoInst.sFileName);
	}

	return dwErrorCode;
};
DWORD FileProcess::ChkStageDB()
{
	DWORD dwErrorCode = -1;
	BOOL bmatchResult = NULL;
	this->sFileInfoInst.iChkStageDB = -1;
	try
	{
		std::basic_regex<TCHAR> regMaskCommon(UI::GetUIInst()->vstChk_Stage.c_str());
		std::match_results<std::basic_string<TCHAR>::const_iterator> mrSearchresult;
		bmatchResult = std::regex_search(this->sFileInfoInst.sFile_ProjectStagebyName, mrSearchresult, regMaskCommon);

		if (mrSearchresult.ready() && bmatchResult != 0)
		{
			bmatchResult = FALSE;
			//std::wcout << mrSearchresult.str(1) << std::endl;
			this->sFileInfoInst.sFile_ProjectStageforDB = this->sFileInfoInst.sFile_ProjectbyName + _T('-') + mrSearchresult.str(1);
			this->sFileInfoInst.sFile_StagebyName = mrSearchresult.str(1);
			dwErrorCode = DBProcess::dbProcInstance()->DBGetStage(this, bmatchResult);
		}
		else
		{
			bmatchResult = NULL;
			return ERROR_INVALID_DATA;
		}
		this->sFileInfoInst.iChkStageDB = bmatchResult;

		dwErrorCode = GetLastError();
		throw dwErrorCode;
	}
	catch (DWORD dwErrorCode)
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
	catch (...)
	{
		Logger::GetLogInstance()->PrepareTXTLOG("Function->IterProcessFiles(ChkStageDB(...)): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ",this->sFileInfoInst.sFileName);
		Logger::GetLogInstance()->PrepareMySQLLOG("Error in getting file attributes", "Function->IterObjects(ChkStageDB(...))", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCode,this->sFileInfoInst.sFileName);
	}

	return dwErrorCode;
};
DWORD FileProcess::ChkStageDR()
{
	DWORD dwErrorCode = -1;
	BOOL bmatchResult = NULL;
	this->sFileInfoInst.iChkStageDR = -1;
	try
	{
		std::basic_regex<TCHAR> regMaskCommon(UI::GetUIInst()->vstChk_Stage.c_str());
		std::match_results<std::basic_string<TCHAR>::const_iterator> mrSearchresult;
		bmatchResult = std::regex_search(this->sFileInfoInst.sFile_ProjectStagebyName, mrSearchresult, regMaskCommon);
		this->sFileInfoInst.sFile_StagebyPath = mrSearchresult.str(1);

		if (this->sFileInfoInst.sFile_StagebyPath != this->sFileInfoInst.sFile_StagebyName)
			bmatchResult = NULL;
		else
			bmatchResult = TRUE;
		this->sFileInfoInst.iChkStageDR = bmatchResult;

		dwErrorCode = GetLastError();
		throw dwErrorCode;
	}
	catch (DWORD dwErrorCode)
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
	catch (...)
	{
		Logger::GetLogInstance()->PrepareTXTLOG("Function->IterProcessFiles(ChkStageDR(...)): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ",this->sFileInfoInst.sFileName);
		Logger::GetLogInstance()->PrepareMySQLLOG("Error in getting file attributes", "Function->IterObjects(ChkStageDR(...))", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCode,this->sFileInfoInst.sFileName);
	}

	return dwErrorCode;
};
DWORD FileProcess::ChkSubSystemDB()
{
	DWORD dwErrorCode = -1;
	BOOL bmatchResult = NULL;
	this->sFileInfoInst.iChkSubSystemDB = -1;
	try
	{
		dwErrorCode = DBProcess::dbProcInstance()->DBGetSubsystem(this, bmatchResult);
		bmatchResult = TRUE; /// DELETE after adding checking logic
		this->sFileInfoInst.iChkSubSystemDB = bmatchResult;

		dwErrorCode = GetLastError();
		throw dwErrorCode;
	}
	catch (DWORD dwErrorCode)
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
	catch (...)
	{
		Logger::GetLogInstance()->PrepareTXTLOG("Function->IterProcessFiles(ChkSubSystemDB(...)): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ",this->sFileInfoInst.sFileName);
		Logger::GetLogInstance()->PrepareMySQLLOG("Error in getting file attributes", "Function->IterObjects(ChkSubSystemDB(...))", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCode,this->sFileInfoInst.sFileName);
	}

	return dwErrorCode;
};
DWORD FileProcess::ChkDataTypeDB()
{
	DWORD dwErrorCode = -1;
	BOOL bmatchResult = NULL;
	this->sFileInfoInst.iChkDataTypeDB = -1;
	try
	{
		dwErrorCode = DBProcess::dbProcInstance()->DBGetDatatype(this, bmatchResult);
		this->sFileInfoInst.iChkDataTypeDB = bmatchResult;

		dwErrorCode = GetLastError();
		throw dwErrorCode;
	}
	catch (DWORD dwErrorCode)
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
	catch (...)
	{
		Logger::GetLogInstance()->PrepareTXTLOG("Function->IterProcessFiles(ChkSubSystemDB(...)): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ",this->sFileInfoInst.sFileName);
		Logger::GetLogInstance()->PrepareMySQLLOG("Error in getting file attributes", "Function->IterObjects(ChkSubSystemDB(...))", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCode,this->sFileInfoInst.sFileName);
	}

	return dwErrorCode;
};