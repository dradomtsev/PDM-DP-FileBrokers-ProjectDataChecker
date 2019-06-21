// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// ArM_GetFilesDB_CS.cpp : Defines the entry point for the console application.

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	// Get current flag  
	int tmpFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	// Turn on leak-checking bit.  
	tmpFlag |= _CRTDBG_ALLOC_MEM_DF; 
	tmpFlag |= _CRTDBG_CHECK_ALWAYS_DF;
	tmpFlag |= _CRTDBG_LEAK_CHECK_DF;
	// Set flag to the new value.  
	_CrtSetDbgFlag(tmpFlag);
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
	{
		DWORD dwErrorCode = -1;
		
		SYSTEMTIME stlocalTime;
		std::basic_string<TCHAR> stFormatStr(MAX_PATH, 0);

		setlocale(LC_ALL, "");
		GetLocalTime(&stlocalTime);

		UI::GetUIInst().FindExePath();
		wprintf_s(_T("%s started at: %d:%d:%d %d:%d:%d\r\n"), UI::GetUIInst().stModuleName.c_str(), stlocalTime.wYear, stlocalTime.wMonth, stlocalTime.wDay, stlocalTime.wHour, stlocalTime.wMinute, stlocalTime.wSecond);

		Logger::GetLogInstance()->CreateTXTLog();
		stFormatStr = _T("Log file created");
		std::wcout << std::left << stFormatStr << std::endl;
		stFormatStr.clear();

		UI::GetUIInst().InitbyINI(); // Get dirs fro review, dirs to ignore and review period
		stFormatStr = _T("Process initialized by *INI file");
		std::wcout << std::left << stFormatStr << std::endl;
		stFormatStr.clear();

		UI::GetUIInst().ConvertDateTime();
		UI::GetUIInst().InitReviewPeriod(); // Init review period
		stFormatStr = _T("Check period initialized");
		std::wcout << std::left << stFormatStr << std::endl;
		stFormatStr.clear();
		
		// Init MySQL Engine
		dwErrorCode = -1;
		dwErrorCode = DBProcess::dbProcInstance().DBCreateMYSQLConnection();
		stFormatStr = _T("MySQL DB connection strarted");
		std::wcout << std::left << stFormatStr << std::endl;
		stFormatStr.clear();

		Logger::GetLogInstance()->CreateMySQLLog();
		stFormatStr = _T("MySQL error log initialized");
		std::wcout << std::left << stFormatStr << std::endl;
		stFormatStr.clear();

		dwErrorCode = -1;
		dwErrorCode = DBProcess::dbProcInstance().DBInitTables();// InitMySQLConect();
		stFormatStr = _T("MySQL DB connection initialized");
		std::wcout << std::left << stFormatStr << std::endl;
		stFormatStr.clear();

		FileProcess* hFpInst = new FileProcess();
		for (UI::GetUIInst().itFolderstoParse = UI::GetUIInst().vstFolderstoParse.begin(); UI::GetUIInst().itFolderstoParse != UI::GetUIInst().vstFolderstoParse.end(); ++UI::GetUIInst().itFolderstoParse)
		{
			wprintf_s(_T("Working in main directory: %s\r\n"), UI::GetUIInst().itFolderstoParse->c_str());
			stFormatStr.clear();

			dwErrorCode = hFpInst->IterDirs(UI::GetUIInst().itFolderstoParse->c_str()); // Process dirs for review & ignore
			if (dwErrorCode == ERROR_NO_MORE_FILES)
				SetLastError(ERROR_SUCCESS);
		}
		stFormatStr = _T("Getting of files attributes done!");
		std::wcout << std::left << stFormatStr << std::endl;
		stFormatStr.clear();

		// Delete MySQL Engine instance
		DBProcess::dbProcInstance().DBCloseMYSQLConnection();// CloseMySQLConect();
		stFormatStr = _T("MySQL DB connection closed");
		std::wcout << std::left << stFormatStr << std::endl;
		stFormatStr.clear();

		wprintf_s(_T("%s started at: %d:%d:%d %d:%d:%d\r\n"), UI::GetUIInst().stModuleName.c_str(), stlocalTime.wYear, stlocalTime.wMonth, stlocalTime.wDay, stlocalTime.wHour, stlocalTime.wMinute, stlocalTime.wSecond);
		GetLocalTime(&stlocalTime);
		wprintf_s(_T("%s ended at: %d:%d:%d %d:%d:%d\r\n"), UI::GetUIInst().stModuleName.c_str(), stlocalTime.wYear, stlocalTime.wMonth, stlocalTime.wDay, stlocalTime.wHour, stlocalTime.wMinute, stlocalTime.wSecond);
		//system("pause");
		delete hFpInst;
	}

	_CrtDumpMemoryLeaks();
	return 0;
}