// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"

Logger::Logger()
{
	this->hFile = nullptr;
	this->sLogFile.sFileName = _T("\\#Res-[ArM]1IM-P-ZZ-TD-GetFilesAttribsLOG.txt");
};
Logger::~Logger() 
{
	
};
Logger * Logger::GetLogInstance()
{
	static Logger logProcInst;
	return &logProcInst;
};
DWORD Logger::CreateTXTLog()
{
	DWORD dwErrorCode = -1;
	this->sLogFile.sFileRoot = UI::GetUIInst().stModulePath;
	this->sLogFile.sFilePath.append(this->sLogFile.sFileRoot.c_str());// =  + this->sLogFile.sFileName;
	this->sLogFile.sFilePath.append(this->sLogFile.sFileName.c_str());
	this->hFile = CreateFile(this->sLogFile.sFilePath.c_str(), GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); //READ_CONTROL
	dwErrorCode = GetLastError();
	if (this->hFile == INVALID_HANDLE_VALUE)
	{
		dwErrorCode = GetLastError();
		CloseHandle(this->hFile);
		Logger::GetLogInstance()->PrepareTXTLOG("Function->CreateTXTLog(CreateFile()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", this->sLogFile.sFilePath);
		ErrorHandle::GetErrorHandleInst()->ErrorExit(_T("CreateLogFile()->CreateFile()"), _T("."), dwErrorCode);
		return dwErrorCode;
	}
	if (dwErrorCode == ERROR_SUCCESS || dwErrorCode == ERROR_ALREADY_EXISTS)
	{
		Logger::GetLogInstance()->PrepareTXTLOG("Function->CreateTXTLog(CreateFile()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; File: ", this->sLogFile.sFilePath);
		SetLastError(ERROR_SUCCESS);
		dwErrorCode = 0;
	}
		
	return dwErrorCode;
};
DWORD Logger::CreateMySQLLog()
{
	DWORD dwErrorCode = -1;
	try
	{
		std::shared_ptr<sql::Statement> stmt;
		//sql::Statement* stmt;
		std::string stMySQLST_Truncate("TRUNCATE TABLE ");
		stMySQLST_Truncate.append(DBProcess::dbProcInstance().stMySQLTableErrors.stMySQLTable_name);
		stMySQLST_Truncate.append(";");
		stmt.reset(DBProcess::dbProcInstance().con->createStatement());
		stmt->execute(stMySQLST_Truncate.c_str());
		stmt->close();

		dwErrorCode = GetLastError();
		throw dwErrorCode;
	}
	catch (sql::SQLException &e)
	{
		Logger::GetLogInstance()->PrepareTXTLOG("Function->CreateMySQLLog(TRUNCATE TABLE errors) -> MySQL error: ", e.what(), "MySQL error code: ", e.getErrorCode(), "SQLState: ", e.getSQLState());
	}
	catch (DWORD Err_code)
	{
		switch (Err_code)
		{
		case ERROR_SUCCESS:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->CreateMySQLLog(TRUNCATE TABLE errors): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", "MySQL Connection");
			break;
		default:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->CreateMySQLLog(TRUNCATE TABLE errors): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", "MySQL Connection");
			ErrorHandle::GetErrorHandleInst()->ErrorExit(_T("CreateMySQLLog(TRUNCATE TABLE errors) "), _T("."), dwErrorCode);
		}
	}
	dwErrorCode = GetLastError();
	return dwErrorCode;
};
DWORD Logger::PrepareTXTLOG(char* cAddString1, const std::string &sState1, char* cAddString2, int iErrorCode, char* cAddString3, const std::string &sState2)
{
	DWORD dwErrorCode = -1;
	this->sBuff.insert(0, cAddString1);
	this->sBuff.append(sState1.c_str());
	this->sBuff.append(" ");
	this->sBuff.append(cAddString2);
	std::string sBuffTemp = std::to_string(iErrorCode);
	this->sBuff.append(sBuffTemp.c_str());
	this->sBuff.append(" ");
	this->sBuff.append(cAddString3);
	this->sBuff.append(sState2.c_str());
	this->sBuff.append("\r\n");
	this->WriteToTXTLog();

	this->sBuff.clear();
	return dwErrorCode;
};
DWORD Logger::PrepareTXTLOG(char* cAddString1, const std::basic_string<TCHAR> &sState1, char* cAddString2, int iErrorCode, char* cAddString3, const std::string &sState2)
{
	DWORD dwErrorCode = -1;
	std::string sFileTMP = "";
	dwErrorCode = ConvertStrings::GetConvStrInst()->UnicodeStringToAnsiString(sState1, sFileTMP);

	this->sBuff.insert(0,cAddString1);
	this->sBuff.append(sFileTMP);
	this->sBuff.append(" ");
	this->sBuff.append(cAddString2);
	std::string sBuffTemp = std::to_string(iErrorCode);
	this->sBuff.append(sBuffTemp.c_str());
	this->sBuff.append(" ");
	this->sBuff.append(cAddString3);
	this->sBuff.append(sState2.c_str());
	this->sBuff.append("\r\n");
	this->WriteToTXTLog();

	this->sBuff.clear();
	return dwErrorCode;
};
DWORD Logger::PrepareTXTLOG(char* cAddString1, const std::basic_string<TCHAR> &sState1, char* cAddString2, int iErrorCode, char* cAddString3, const std::basic_string<TCHAR> &sState2)
{
	DWORD dwErrorCode = -1;
	std::string sFuncTMP = "";
	dwErrorCode = ConvertStrings::GetConvStrInst()->UnicodeStringToAnsiString(sState1, sFuncTMP);
	if (dwErrorCode == 0)
	{
		dwErrorCode = -1;
		std::string sFileTMP = "";
		dwErrorCode = ConvertStrings::GetConvStrInst()->UnicodeStringToAnsiString(sState2, sFileTMP);
		if ( dwErrorCode == 0)
		{
			this->sBuff.insert(0, cAddString1);
			this->sBuff.append(sFuncTMP);
			this->sBuff.append(" ");
			this->sBuff.append(cAddString2);
			std::string sBuffTemp = std::to_string(iErrorCode);
			this->sBuff.append(sBuffTemp.c_str());
			this->sBuff.append(" ");
			this->sBuff.append(cAddString3);
			this->sBuff.append(sFileTMP);
			this->sBuff.append("\r\n");
			this->WriteToTXTLog();

			this->sBuff.clear();
		}
	}
	else
	{
		return dwErrorCode;
	}

	return dwErrorCode;
};
DWORD Logger::PrepareMySQLLOG(std::string stSubject, std::string stFunc, std::string sState1, int iErrorCode, std::string sState2)
{
	DWORD dwErrorCode = -1;
	this->stSubjectBuff.insert(0, stSubject);
	this->sBuff.insert(0, stFunc);
	this->sBuff.append(sState1);
	this->sBuff.append(" Error code: ");
	std::string sBuffTemp = std::to_string(iErrorCode);
	this->sBuff.append(sBuffTemp.c_str());
	this->sBuff.append(" Object: ");
	this->sBuff.append(sState2);
	this->sBuff.append("\r\n");

	this->WriteToMySQLLog();

	this->stSubjectBuff.clear();
	this->sBuff.clear();
	return dwErrorCode;
};
DWORD Logger::PrepareMySQLLOG(std::string stSubject, std::string stFunc, std::basic_string<TCHAR> sState1, int iErrorCode, std::basic_string<TCHAR> sState2)
{
	DWORD dwErrorCode = -1;
	std::string sFuncTMP = "";
	dwErrorCode = ConvertStrings::GetConvStrInst()->UnicodeStringToAnsiString(sState1, sFuncTMP);
	if (dwErrorCode == 0)
	{
		dwErrorCode = -1;
		std::string sFileTMP = "";
		dwErrorCode = ConvertStrings::GetConvStrInst()->UnicodeStringToAnsiString(sState2, sFileTMP);
		if (dwErrorCode == 0)
		{
			this->stSubjectBuff.insert(0, stSubject);
			this->sBuff.insert(0, stFunc);
			this->sBuff.append(sFuncTMP);
			this->sBuff.append(" Error code: ");
			std::string sBuffTemp = std::to_string(iErrorCode);
			this->sBuff.append(sBuffTemp.c_str());
			this->sBuff.append(" Object: ");
			this->sBuff.append(sFileTMP);
			this->sBuff.append("\r\n");

			this->WriteToMySQLLog();

			this->stSubjectBuff.clear();
			this->sBuff.clear();
		}
	}
	else
		return dwErrorCode;

	return dwErrorCode;
};
DWORD Logger::PrepareMySQLLOG(std::string stSubject, std::string stFunc, std::string sState1, int iErrorCode, std::basic_string<TCHAR> sState2)
{
	DWORD dwErrorCode = -1;
	std::string sFileTMP = "";
	dwErrorCode = ConvertStrings::GetConvStrInst()->UnicodeStringToAnsiString(sState2, sFileTMP);

	this->stSubjectBuff.insert(0, stSubject);
	this->sBuff.insert(0, stFunc);
	this->sBuff.append(sState1);
	this->sBuff.append(" Error code: ");
	std::string sBuffTemp = std::to_string(iErrorCode);
	this->sBuff.append(sBuffTemp.c_str());
	this->sBuff.append(" Object: ");
	this->sBuff.append(sFileTMP);
	this->sBuff.append("\r\n");

	this->WriteToMySQLLog();

	this->stSubjectBuff.clear();
	this->sBuff.clear();
	return dwErrorCode;
};
DWORD Logger::WriteToTXTLog()
{
	BOOL bErrorFlag = FALSE;
	DWORD dwBytesToWrite = (DWORD)this->sBuff.size();
	DWORD dwBytesWritten = 0;
	DWORD dwErrorCode = -1;
	LARGE_INTEGER iDist = {0};
	LARGE_INTEGER iPos = { 0 };
	this->hFile = CreateFile(this->sLogFile.sFilePath.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); //READ_CONTROL
	bErrorFlag = SetFilePointerEx(this->hFile, iDist, &iPos, FILE_END);
	//DWORD dwPos = SetFilePointer(this->hFile, 0, NULL, FILE_END);
	bErrorFlag = FALSE;
	LockFile(this->hFile, iPos.LowPart, 0, dwBytesToWrite, 0);
	bErrorFlag = WriteFile(this->hFile, this->sBuff.c_str()/*DataBuffer */, dwBytesToWrite, &dwBytesWritten, NULL);
	UnlockFile(this->hFile, iPos.LowPart, 0, dwBytesToWrite, 0);
	if (bErrorFlag == FALSE)
	{
		dwErrorCode = GetLastError();
		CloseHandle(this->hFile);
		ErrorHandle::GetErrorHandleInst()->ErrorExit(_T("WriteToTXTLog()->WriteFile()"), _T("."), dwErrorCode);
		return dwErrorCode;
	}
	else
	{
		if (dwBytesWritten != dwBytesToWrite)
		{
			// This is an error because a synchronous write that results in
			// success (WriteFile returns TRUE) should write all data as
			// requested. This would not necessarily be the case for
			// asynchronous writes.
			CloseHandle(this->hFile);
			ErrorHandle::GetErrorHandleInst()->ErrorExit(_T("WriteToTXTLog()->WriteFile(asynchronous write)"), _T("."), dwErrorCode);
			return dwErrorCode;
		}
		//else this->sBuff.clear();
		CloseHandle(this->hFile);
	}
	dwErrorCode = GetLastError();
	return dwErrorCode;
};
DWORD Logger::WriteToMySQLLog()
{
	DWORD dwErrorCode = -1;
	int Result = 0;
	std::shared_ptr<sql::PreparedStatement> pstmtWrite;

	// INSERT INTO portal.extensions
	try
	{
		std::string stMySQLST_Insert("INSERT INTO tablename (subject, description) VALUES (?,?);");		
		stMySQLST_Insert.replace(stMySQLST_Insert.find("tablename"),	std::string("tablename").length(),	DBProcess::dbProcInstance().stMySQLTableErrors.stMySQLTable_name.c_str());
		stMySQLST_Insert.replace(stMySQLST_Insert.find("subject"),		std::string("subject").length(),	DBProcess::dbProcInstance().stMySQLTableErrors.stMySQLTable_fields.at(0).c_str());
		stMySQLST_Insert.replace(stMySQLST_Insert.find("description"),	std::string("description").length(),DBProcess::dbProcInstance().stMySQLTableErrors.stMySQLTable_fields.at(1).c_str());
		pstmtWrite.reset(DBProcess::dbProcInstance().con->prepareStatement(stMySQLST_Insert.c_str()));// extensions extensions_bak
		pstmtWrite->setString(1, this->stSubjectBuff.c_str());
		pstmtWrite->setString(2, this->sBuff.c_str());
		pstmtWrite->execute();
		pstmtWrite->close();

		dwErrorCode = GetLastError();
		throw dwErrorCode;
	}
	catch (sql::SQLException &e)
	{
		Logger::GetLogInstance()->PrepareTXTLOG("Function->WriteToMySQLLog(INSERT INTO errors) -> MySQL error: ", e.what(), "MySQL error code: ", e.getErrorCode(), "SQLState: ", e.getSQLState());
	}
	catch (DWORD Err_code)
	{
		switch (Err_code)
		{
		case ERROR_SUCCESS:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->WriteToMySQLLog(INSERT INTO errors): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", "MySQL connection");
			break;
		default:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->WriteToMySQLLog(INSERT INTO errors): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", "MySQL connection");
			ErrorHandle::GetErrorHandleInst()->ErrorExit(_T("IterObjects->WriteToMySQLLog(INSERT INTO errors)"), _T("MySQL connection"), dwErrorCode);
		}
	}
	return dwErrorCode;
};