// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"

UI::UI() : 
	stWorkDir(), 
	stFileINI_Name(_T("\\config.json")),//(_T("\\#Res-[ArM]1IM-P-ZZ-SP-GetFilesAttribsINI.ini")),
	stModulePath(),
	stStartChkPeriod(),
	stEndChkPeriod(),
	tStartChkPeriod(),
	tEndChkPeriod(),
	vstFolderstoParse(),
	vstFolderstoIgnore(),
	stChkStartDateTime(),
	stChkEndDateTime(),
	vstChkStartDateTime(),
	vstChkEndDateTime(),
	vstRootFolders(),
	vstErrorsTableName(),
	vstErrorsTableFields(),
	vstExtensTableName(),
	vstExtensTableFields(),
	vstFilesTableName(),
	vstFilesTableFields(),
	vstFoldersTableName(),
	vstFoldersTableFields(),
	vstUsersTableName(),
	vstUsersTableFields(),
	vstProjectsTableName(),
	vstProjectsTableFields(),
	vstCompaniesTableName(),
	vstCompaniesTableFields(),
	vstRolesTableName(),
	vstRolesTableFields(),
	vstSubsystemsTableName(),
	vstSubsystemsTableFields(),
	vstDatatypesTableName(),
	vstDatatypesTableFields(),
	stMySQl_Hostname(),
	stMySQl_Login(),
	stMySQl_Password(),
	stMySQl_DefSchema(),
	vstChk_FileMaskCommon_fns2(),
	vstChk_Cyrillic_fns2(),
	vstChk_Company_fns2(),
	vstChk_Role_fns2(),
	vstChk_Stage_fns2(),
	vstChk_FindDStageShaPubZZ_fns2(),
	vstChk_FileMaskDStageShaPubZZ_fns2(),

	vstChk_FileMaskCommon_fns3(),
	vstChk_Cyrillic_fns3(),
	vstChk_Company_fns3(),
	vstChk_Role_fns3(),
	vstChk_Stage_fns3(),
	vstChk_FindDStageShaPubZZ_fns3(),
	vstChk_FileMaskDStageShaPubZZ_fns3()
{
	//stModuleName.resize(MAX_PATH);
};
UI::~UI() {};

DWORD UI::FindExePath()
{
	//0. Get EXE Path for futher searching of INI-file
	DWORD dwErrorCode = -1;
	DWORD dStatus = -1;
	SetLastError(ERROR_SUCCESS);
	int iSize = 0;
	std::size_t nLocCRLF = 0;
	//std::vector<TCHAR> tModulePath;
	//tModulePath.push_back(0);
	//std::wstring tModulePath;
	TCHAR tModulePath[MAX_PATH];
	//TCHAR tModuleName[MAX_PATH];
	try
	{
		iSize = GetModuleFileName(NULL, tModulePath, MAX_PATH); //UI::GetUIInst().stModuleName
		dwErrorCode = GetLastError();
		if (iSize == 0)
			ErrorHandle::GetErrorHandleInst()->ErrorExit(_T("FindExePath->GetModuleFileName()"), _T("."), dwErrorCode);
		throw dwErrorCode;
	}
	catch (DWORD& dwErrorCode)
	{
		switch (dwErrorCode)
		{
		case ERROR_SUCCESS:
		case ERROR_INSUFFICIENT_BUFFER:
			break;
		default:
			ErrorHandle::GetErrorHandleInst()->ErrorExit(_T("FindExePath->GetModuleFileName()"),_T("."), dwErrorCode);
		}
	}
	
	stModuleName = tModulePath;
	nLocCRLF = stModuleName.find_last_of('\\');
	stModuleName.erase(0, nLocCRLF+1);

	try
	{
		dStatus = PathRemoveFileSpec(tModulePath);
		stModulePath = tModulePath;
		if (dStatus == 0)
		{
			dStatus = FALSE;
		}
		else
			dStatus = TRUE;
		dwErrorCode = GetLastError();
		throw dwErrorCode;
	}
	catch (DWORD& dwErrorCode)
	{
		switch (dwErrorCode)
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

		std::string sJSONPath;
		ConvertStrings::GetConvStrInst()->ws2s(stCurDirBuff, sJSONPath);
		boost::property_tree::wiptree wpt;
		boost::property_tree::read_json(sJSONPath, wpt);

		for (const boost::property_tree::wiptree::value_type &ptVV : wpt.get_child	(_T("DataStructure.FoldersToRead")))
			vstFolderstoParse.push_back(ptVV.second.data());
		for (const boost::property_tree::wiptree::value_type &ptVV : wpt.get_child	(_T("DataStructure.FoldersIgnore")))
			vstFolderstoIgnore.push_back(ptVV.second.data());
		for (const boost::property_tree::wiptree::value_type &ptVV : wpt.get_child	(_T("DataStructure.RootFolders")))
			vstRootFolders.push_back(ptVV.second.data());
		stChkStartDateTime.	append(wpt.get<std::basic_string<TCHAR>>				(_T("Timing.ChkStartDateTime")));
		stChkEndDateTime.	append(wpt.get<std::basic_string<TCHAR>>				(_T("Timing.ChkStartDateTime")));
		
		stMySQl_Hostname.	append(wpt.get<std::basic_string<TCHAR>>				(_T("DataBase.Connection.HostnamePort")));
		stMySQl_Login.		append(wpt.get<std::basic_string<TCHAR>>				(_T("DataBase.Connection.Login")));
		stMySQl_Password.	append(wpt.get<std::basic_string<TCHAR>>				(_T("DataBase.Connection.Password")));
		stMySQl_DefSchema.	append(wpt.get<std::basic_string<TCHAR>>				(_T("DataBase.Connection.DefaultSchema")));

		vstErrorsTableName.append(wpt.get<std::basic_string<TCHAR>>					(_T("DataBase.Tables.Errors.Name")));
		for (const boost::property_tree::wiptree::value_type &ptVV : wpt.get_child	(_T("DataBase.Tables.Errors.Fields")))
			vstErrorsTableFields.push_back(ptVV.second.data());
		vstExtensTableName.append(wpt.get<std::basic_string<TCHAR>>					(_T("DataBase.Tables.FileExtensions.Name")));
		for (const boost::property_tree::wiptree::value_type &ptVV : wpt.get_child	(_T("DataBase.Tables.FileExtensions.Fields")))
			vstExtensTableFields.push_back(ptVV.second.data());
		vstFilesTableName.append(wpt.get<std::basic_string<TCHAR>>					(_T("DataBase.Tables.Files.Name")));
		for (const boost::property_tree::wiptree::value_type &ptVV : wpt.get_child	(_T("DataBase.Tables.Files.Fields")))
			vstFilesTableFields.push_back(ptVV.second.data());
		vstFoldersTableName.append(wpt.get<std::basic_string<TCHAR>>				(_T("DataBase.Tables.Folders.Name")));
		for (const boost::property_tree::wiptree::value_type &ptVV : wpt.get_child	(_T("DataBase.Tables.Folders.Fields")))
			vstFoldersTableFields.push_back(ptVV.second.data());
		vstUsersTableName.append(wpt.get<std::basic_string<TCHAR>>					(_T("DataBase.Tables.Users.Name")));
		for (const boost::property_tree::wiptree::value_type &ptVV : wpt.get_child	(_T("DataBase.Tables.Users.Fields")))
			vstUsersTableFields.push_back(ptVV.second.data());
		vstProjectsTableName.append(wpt.get<std::basic_string<TCHAR>>				(_T("DataBase.Tables.Projects.Name")));
		for (const boost::property_tree::wiptree::value_type &ptVV : wpt.get_child	(_T("DataBase.Tables.Projects.Fields")))
			vstProjectsTableFields.push_back(ptVV.second.data());
		vstCompaniesTableName.append(wpt.get<std::basic_string<TCHAR>>				(_T("DataBase.Tables.Companies.Name")));
		for (const boost::property_tree::wiptree::value_type &ptVV : wpt.get_child	(_T("DataBase.Tables.Companies.Fields")))
			vstCompaniesTableFields.push_back(ptVV.second.data());
		vstRolesTableName.append(wpt.get<std::basic_string<TCHAR>>					(_T("DataBase.Tables.Roles.Name")));
		for (const boost::property_tree::wiptree::value_type &ptVV : wpt.get_child	(_T("DataBase.Tables.Roles.Fields")))
			vstRolesTableFields.push_back(ptVV.second.data());
		vstSubsystemsTableName.append(wpt.get<std::basic_string<TCHAR>>				(_T("DataBase.Tables.Subsystems.Name")));
		for (const boost::property_tree::wiptree::value_type &ptVV : wpt.get_child	(_T("DataBase.Tables.Subsystems.Fields")))
			vstSubsystemsTableFields.push_back(ptVV.second.data());
		vstDatatypesTableName.append(wpt.get<std::basic_string<TCHAR>>				(_T("DataBase.Tables.DataTypes.Name")));
		for (const boost::property_tree::wiptree::value_type &ptVV : wpt.get_child	(_T("DataBase.Tables.DataTypes.Fields")))
			vstDatatypesTableFields.push_back(ptVV.second.data());
		
		for (const boost::property_tree::wiptree::value_type &ptVV : wpt.get_child(_T("FileNamingSystem.FNS2.Projects.Include")))
			vstProjectsInclude_fns2.push_back(ptVV.second.data());
		for (const boost::property_tree::wiptree::value_type &ptVV : wpt.get_child(_T("FileNamingSystem.FNS2.Projects.Exclude")))
			vstProjectsExclude_fns2.push_back(ptVV.second.data());
		vstChk_FileMaskCommon_fns2.append(wpt.get<std::basic_string<TCHAR>>			(_T("FileNamingSystem.FNS2.Rules.Common.FileMask")));
		vstChk_Cyrillic_fns2.append(wpt.get<std::basic_string<TCHAR>>				(_T("FileNamingSystem.FNS2.Rules.Common.Cyrillic")));
		vstChk_Company_fns2.append(wpt.get<std::basic_string<TCHAR>>				(_T("FileNamingSystem.FNS2.Rules.Common.Company")));
		vstChk_Role_fns2.append(wpt.get<std::basic_string<TCHAR>>					(_T("FileNamingSystem.FNS2.Rules.Common.Role")));
		vstChk_Stage_fns2.append(wpt.get<std::basic_string<TCHAR>>					(_T("FileNamingSystem.FNS2.Rules.Common.Stage")));
		vstChk_BuildingSection_fns2.append(wpt.get<std::basic_string<TCHAR>>		(_T("FileNamingSystem.FNS2.Rules.Common.BuildingSection")));
		vstChk_SubSystem_fns2.append(wpt.get<std::basic_string<TCHAR>>				(_T("FileNamingSystem.FNS2.Rules.Common.SubSystem")));
		vstChk_FindDStageShaPubZZ_fns2.append(wpt.get<std::basic_string<TCHAR>>		(_T("FileNamingSystem.FNS2.Rules.Specific.FindDStageShaPubZZ")));
		vstChk_FileMaskDStageShaPubZZ_fns2.append(wpt.get<std::basic_string<TCHAR>>	(_T("FileNamingSystem.FNS2.Rules.Specific.FileMaskDStageShaPubZZ")));
		vstChk_MaskDCompanyShaPubZZ_fns2.append(wpt.get<std::basic_string<TCHAR>>	(_T("FileNamingSystem.FNS2.Rules.Specific.MaskDCompanyShaPubZZ")));
		
		for (const boost::property_tree::wiptree::value_type &ptVV : wpt.get_child(_T("FileNamingSystem.FNS3.Projects.Include")))
			vstProjectsInclude_fns3.push_back(ptVV.second.data());
		for (const boost::property_tree::wiptree::value_type &ptVV : wpt.get_child(_T("FileNamingSystem.FNS3.Projects.Exclude")))
			vstProjectsExclude_fns3.push_back(ptVV.second.data());
		vstChk_FileMaskCommon_fns3.append(wpt.get<std::basic_string<TCHAR>>			(_T("FileNamingSystem.FNS3.Rules.Common.FileMask")));
		vstChk_Cyrillic_fns3.append(wpt.get<std::basic_string<TCHAR>>				(_T("FileNamingSystem.FNS3.Rules.Common.Cyrillic")));
		vstChk_Company_fns3.append(wpt.get<std::basic_string<TCHAR>>				(_T("FileNamingSystem.FNS3.Rules.Common.Company")));
		vstChk_Role_fns3.append(wpt.get<std::basic_string<TCHAR>>					(_T("FileNamingSystem.FNS3.Rules.Common.Role")));
		vstChk_Stage_fns3.append(wpt.get<std::basic_string<TCHAR>>					(_T("FileNamingSystem.FNS3.Rules.Common.Stage")));
		vstChk_BuildingSection_fns3.append(wpt.get<std::basic_string<TCHAR>>		(_T("FileNamingSystem.FNS3.Rules.Common.BuildingSection")));
		vstChk_SubSystem_fns3.append(wpt.get<std::basic_string<TCHAR>>				(_T("FileNamingSystem.FNS3.Rules.Common.SubSystem")));
		vstChk_FindDStageShaPubZZ_fns3.append(wpt.get<std::basic_string<TCHAR>>		(_T("FileNamingSystem.FNS3.Rules.Specific.FindDStageShaPubZZ")));
		vstChk_FileMaskDStageShaPubZZ_fns3.append(wpt.get<std::basic_string<TCHAR>>	(_T("FileNamingSystem.FNS3.Rules.Specific.FileMaskDStageShaPubZZ")));
		vstChk_MaskDCompanyShaPubZZ_fns3.append(wpt.get<std::basic_string<TCHAR>>	(_T("FileNamingSystem.FNS3.Rules.Specific.MaskDCompanyShaPubZZ")));
	}
	catch (boost::property_tree::json_parser_error &bjsErr_code)
	{
		dwErrorCode = ERROR_READ_FAULT;
		Logger::GetLogInstance()->PrepareTXTLOG("Function->InitbyINI(SetCurrentDirectory()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", stCurDirBuff);
		ErrorHandle::GetErrorHandleInst()->ErrorExit(_T("InitDirbyINI->read_json()"), _T("."), (DWORD)_T("Error reading JSON-INI"));
	}
	//SetLastError(ERROR_SUCCESS);
	//try
	//{
	//	stCurDirBuff.assign(this->stModulePath);
	//	stCurDirBuff.append(this->stFileINI_Name);
	//	hFile = CreateFile( stCurDirBuff.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,NULL);
	//	dwErrorCode = GetLastError();
	//	if (hFile == INVALID_HANDLE_VALUE) 
	//	{
	//		CloseHandle(hFile);
	//		Logger::GetLogInstance()->PrepareTXTLOG("Function->InitbyINI(CreateFile()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", stCurDirBuff);
	//		ErrorHandle::GetErrorHandleInst()->ErrorExit(_T("InitDirbyINI->CreateFile()"), _T("."), dwErrorCode);
	//	}
	//	throw dwErrorCode;
	//}
	//catch(DWORD Err_code)
	//{
	//	switch (Err_code)
	//	{
	//	case ERROR_SUCCESS:
	//		Logger::GetLogInstance()->PrepareTXTLOG("Function->InitbyINI(CreateFile()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", stCurDirBuff);
	//		break;
	//	default:
	//		CloseHandle(hFile);
	//		Logger::GetLogInstance()->PrepareTXTLOG("Function->InitbyINI(CreateFile()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", stCurDirBuff);
	//		ErrorHandle::GetErrorHandleInst()->ErrorExit(_T("InitDirbyINI->CreateFile()"), _T("."), dwErrorCode);
	//	}
	//}
	////3. Get INI-file handle Information
	//SetLastError(ERROR_SUCCESS);
	//try
	//{
	//	bStatus = GetHandleInformation(hFile,&lpdwFlags);
	//	dwErrorCode = GetLastError();
	//	if (bStatus == 0)
	//	{
	//		CloseHandle(hFile);
	//		Logger::GetLogInstance()->PrepareTXTLOG("Function->InitbyINI(GetHandleInformation()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", stCurDirBuff);
	//		ErrorHandle::GetErrorHandleInst()->ErrorExit(_T("InitDirbyINI->GetHandleInformation()"), _T("."), dwErrorCode);
	//	}
	//	throw dwErrorCode;
	//}
	//catch(DWORD Err_code)
	//{
	//	switch (Err_code)
	//	{
	//	case ERROR_SUCCESS:
	//		Logger::GetLogInstance()->PrepareTXTLOG("Function->InitbyINI(GetHandleInformation()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", stCurDirBuff);
	//		break;
	//	default:
	//		Logger::GetLogInstance()->PrepareTXTLOG("Function->InitbyINI(GetHandleInformation()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", stCurDirBuff);
	//		ErrorHandle::GetErrorHandleInst()->ErrorExit(_T("InitDirbyINI->GetHandleInformation"), _T("."), dwErrorCode);
	//	}
	//}
	//// 4. Get INI-file size
	//SetLastError(ERROR_SUCCESS);
	//try
	//{
	//	liFileSize = GetFileSize(hFile,NULL);
	//	dwErrorCode = GetLastError();
	//	if (bStatus == 0)
	//	{
	//		CloseHandle(hFile);
	//		Logger::GetLogInstance()->PrepareTXTLOG("Function->InitbyINI(GetFileSizeEx()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", stCurDirBuff);
	//		ErrorHandle::GetErrorHandleInst()->ErrorExit(_T("InitDirbyINI->GetFileSizeEx()"), _T("."), dwErrorCode);
	//		return FALSE;
	//	}
	//	throw dwErrorCode;
	//}
	//catch(DWORD Err_code)
	//{
	//	switch (Err_code)
	//	{
	//	case ERROR_SUCCESS:
	//		Logger::GetLogInstance()->PrepareTXTLOG("Function->InitbyINI(GetFileSizeEx()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", stCurDirBuff);
	//		break;
	//	default:
	//		Logger::GetLogInstance()->PrepareTXTLOG("Function->InitbyINI(GetFileSizeEx()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", stCurDirBuff);
	//		ErrorHandle::GetErrorHandleInst()->ErrorExit(_T("InitDirbyINI->GetFileSizeEx()"), _T("."), dwErrorCode);
	//		return FALSE;
	//	}
	//}
	////5. Read INI-file
	//SetLastError(ERROR_SUCCESS);
	//DWORD nBytesToRead = liFileSize;
	////std::vector<std::basic_string<TCHAR>> vstINIbuffer(nBytesToRead);
	//std::basic_string<TCHAR> stINIbuffer(nBytesToRead,0);
	////TCHAR *InPointDirBuff = new TCHAR[nBytesToRead];
	////InPointDirBuff[nBytesToRead/2] = 0;
	//DWORD nBytesReaded;
	//LARGE_INTEGER iDist = { 0 };
	//LARGE_INTEGER iPos = { 0 };
	//BOOL bErrorFlag = FALSE;
	//try
	//{	
	//	bErrorFlag = SetFilePointerEx(hFile, iDist, &iPos, FILE_BEGIN);
	//	dwErrorCode = GetLastError();
	//	if (dStatus == INVALID_SET_FILE_POINTER)
	//	{
	//		CloseHandle(hFile);
	//		Logger::GetLogInstance()->PrepareTXTLOG("Function->InitbyINI(SetFilePointerEx()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", stCurDirBuff);
	//		ErrorHandle::GetErrorHandleInst()->ErrorExit(_T("InitDirbyINI->SetFilePointerEx()"), _T("."), dwErrorCode);
	//		return FALSE;
	//	}
	//	throw dwErrorCode;
	//}
	//catch (DWORD Err_code)
	//{
	//	switch (Err_code)
	//	{
	//	case ERROR_SUCCESS:
	//		Logger::GetLogInstance()->PrepareTXTLOG("Function->InitbyINI(SetFilePointerEx()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", stCurDirBuff);
	//		break;
	//	default:
	//		CloseHandle(hFile);
	//		Logger::GetLogInstance()->PrepareTXTLOG("Function->InitbyINI(SetFilePointerEx()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", stCurDirBuff);
	//		ErrorHandle::GetErrorHandleInst()->ErrorExit(_T("InitDirbyINI->SetFilePointerEx()"), _T("."), dwErrorCode);
	//	}
	//}
	//SetLastError(ERROR_SUCCESS);
	//try
	//{
	//	if (dStatus != INVALID_SET_FILE_POINTER)
	//		bStatus = ReadFile(hFile, &stINIbuffer[0], nBytesToRead, &nBytesReaded, NULL);	// read all data from INI-file
	//	dwErrorCode = GetLastError();
	//	throw dwErrorCode;
	//}
	//catch (DWORD Err_code)
	//{
	//	switch (Err_code)
	//	{
	//	case ERROR_SUCCESS:
	//		Logger::GetLogInstance()->PrepareTXTLOG("Function->InitbyINI(ReadFile()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", stCurDirBuff);
	//		break;
	//	default:
	//		CloseHandle(hFile);
	//		Logger::GetLogInstance()->PrepareTXTLOG("Function->InitbyINI(ReadFile()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", stCurDirBuff);
	//		ErrorHandle::GetErrorHandleInst()->ErrorExit(_T("InitDirbyINI->+ReadFile()"), _T("."), dwErrorCode);
	//	}
	//}
	////std::vector<std::basic_string<TCHAR>> vstINI;
	//std::size_t nStartFrom = 0;
	//std::size_t nLocCRLF = 0;
	////std::basic_string<TCHAR> stCRLF = _T("\r\n");
	////std::basic_string<TCHAR> stTir = _T("-");
	////std::basic_string<TCHAR> stComa = _T(";");
	//std::size_t nLoc = 0;
	//
	//SetLastError(ERROR_SUCCESS);
	//bool bmatchResult = FALSE;
	//std::basic_regex<TCHAR> stRegSearch_1(_T("\r\n"));
	//std::basic_regex<TCHAR> stRegSearch_2(_T("\\[(.*?)\\]"));
	//std::basic_regex<TCHAR> stRegSearch_3(_T(";"));
	//std::wsmatch stSubMatch;
	//std::wsregex_token_iterator itEnd;
	//std::wsregex_token_iterator itEndIn;
	//std::basic_string<TCHAR> wsRegresult_1;
	//std::basic_string<TCHAR> wsRegresult_2;
	//std::basic_string<TCHAR> wsRegresult_3;
	////std::basic_string<TCHAR> wsRegresult_4;
	//std::vector<std::basic_string<TCHAR>> wsINI = { _T("[FoldersToRead]"),_T("[FoldersIgnore]"),_T("[ChkStartDateTime]"),_T("[ChkEndDateTime]"),_T("[RootFolders]"),_T("[ErrorsTableName]"),_T("[ErrorsTableFields]"),_T("[ExtensTableName]"),_T("[ExtensTableFields]"),_T("[FilesTableName]"),_T("[FilesTableFields]"),_T("[FoldersTableName]"),_T("[FoldersTableFields]"),_T("[UsersTableName]"),_T("[UsersTableFields]"),_T("[ProjectsTableName]"),_T("[ProjectsTableFields]"),_T("[CompaniesTableName]"),_T("[CompaniesTableFields]"),_T("[RolesTableName]"),_T("[RolesTableFields]"),_T("[SubsystemsTableName]"),_T("[SubsystemsTableFields]"),_T("[DatatypesTableName]"),_T("[DatatypesTableFields]"),_T("[MySQLHostnamePort]"),_T("[MySQLLogin]"),_T("[MySQLPassword]"),_T("[MySQLDefaultSchema]"),_T("[ChkFileMaskCommon]"),_T("[ChkCyrillic]"),_T("[ChkCompany]"),_T("[ChkRole]"),_T("[ChkStage]"),_T("[ChkFindDStageShaPubZZ]"),_T("[ChkFileMaskDStageShaPubZZ]") };
	////std::vector<std::basic_string<TCHAR>> vwsSubMatch;
	////wsStringsINI ewsStringsINI;
	//try
	//{
	//	std::wsregex_token_iterator itwsReg_1(stINIbuffer.begin(), stINIbuffer.end(), stRegSearch_1, -1);
	//	for ( ; itwsReg_1 != itEnd; ++itwsReg_1)
	//	{
	//		wsRegresult_1 = *itwsReg_1;
	//		bmatchResult = std::regex_search(wsRegresult_1,stSubMatch, stRegSearch_2);
	//		if (bmatchResult != 0)
	//		{
	//			wsRegresult_2 = stSubMatch._At(0);
	//			wsRegresult_3 = wsRegresult_1.substr(wsRegresult_1.find_first_of('-', nStartFrom)+1);
	//			std::wsregex_token_iterator itwsReg_2(wsRegresult_3.begin(), wsRegresult_3.end(), stRegSearch_3, -1);
	//			// Init Folders
	//			if (wsINI.at(0) == wsRegresult_2)
	//			{
	//				for (; itwsReg_2 != itEndIn; ++itwsReg_2)
	//					vstFolderstoParse.push_back(*itwsReg_2);
	//				continue;
	//			}
	//			if (wsINI.at(1) == wsRegresult_2)
	//			{
	//				for (; itwsReg_2 != itEndIn; ++itwsReg_2)
	//					vstFolderstoIgnore.push_back(*itwsReg_2);
	//				continue;
	//			}
	//			//Init Time
	//			if (wsINI.at(2) == wsRegresult_2)
	//			{
	//				for (; itwsReg_2 != itEndIn; ++itwsReg_2)
	//					stChkStartDateTime.append(*itwsReg_2);
	//				continue;
	//			}
	//			if (wsINI.at(3) == wsRegresult_2)
	//			{
	//				for (; itwsReg_2 != itEndIn; ++itwsReg_2)
	//					stChkEndDateTime.append(*itwsReg_2);
	//				continue;
	//			}
	//			//Root Folders
	//			if (wsINI.at(4) == wsRegresult_2)
	//			{
	//				for (; itwsReg_2 != itEndIn; ++itwsReg_2)
	//					vstRootFolders.push_back(*itwsReg_2);
	//				continue;
	//			}
	//			//Init Tables
	//			if (wsINI.at(5) == wsRegresult_2)
	//			{
	//				for (; itwsReg_2 != itEndIn; ++itwsReg_2)
	//					vstErrorsTableName.append(*itwsReg_2);
	//				continue;
	//			}
	//			if (wsINI.at(6) == wsRegresult_2)
	//			{
	//				for (; itwsReg_2 != itEndIn; ++itwsReg_2)
	//					vstErrorsTableFields.push_back(*itwsReg_2);
	//				continue;
	//			}
	//			if (wsINI.at(7) == wsRegresult_2)
	//			{
	//				for (; itwsReg_2 != itEndIn; ++itwsReg_2)
	//					vstExtensTableName.append(*itwsReg_2);
	//				continue;
	//			}
	//			if (wsINI.at(8) == wsRegresult_2)
	//			{
	//				for (; itwsReg_2 != itEndIn; ++itwsReg_2)
	//					vstExtensTableFields.push_back(*itwsReg_2);
	//				continue;
	//			}
	//			if (wsINI.at(9) == wsRegresult_2)
	//			{
	//				for (; itwsReg_2 != itEndIn; ++itwsReg_2)
	//					vstFilesTableName.append(*itwsReg_2);
	//				continue;
	//			}
	//			if (wsINI.at(10) == wsRegresult_2)
	//			{
	//				for (; itwsReg_2 != itEndIn; ++itwsReg_2)
	//					vstFilesTableFields.push_back(*itwsReg_2);
	//				continue;
	//			}
	//			if (wsINI.at(11) == wsRegresult_2)
	//			{
	//				for (; itwsReg_2 != itEndIn; ++itwsReg_2)
	//					vstFoldersTableName.append(*itwsReg_2);
	//				continue;
	//			}
	//			if (wsINI.at(12) == wsRegresult_2)
	//			{
	//				for (; itwsReg_2 != itEndIn; ++itwsReg_2)
	//					vstFoldersTableFields.push_back(*itwsReg_2);
	//				continue;
	//			}
	//			if (wsINI.at(13) == wsRegresult_2)
	//			{
	//				for (; itwsReg_2 != itEndIn; ++itwsReg_2)
	//					vstUsersTableName.append(*itwsReg_2);
	//				continue;
	//			}
	//			if (wsINI.at(14) == wsRegresult_2)
	//			{
	//				for (; itwsReg_2 != itEndIn; ++itwsReg_2)
	//					vstUsersTableFields.push_back(*itwsReg_2);
	//				continue;
	//			}
	//			if (wsINI.at(15) == wsRegresult_2)
	//			{
	//				for (; itwsReg_2 != itEndIn; ++itwsReg_2)
	//					vstProjectsTableName.append(*itwsReg_2);
	//				continue;
	//			}
	//			if (wsINI.at(16) == wsRegresult_2)
	//			{
	//				for (; itwsReg_2 != itEndIn; ++itwsReg_2)
	//					vstProjectsTableFields.push_back(*itwsReg_2);
	//				continue;
	//			}
	//			if (wsINI.at(17) == wsRegresult_2)
	//			{
	//				for (; itwsReg_2 != itEndIn; ++itwsReg_2)
	//					vstCompaniesTableName.append(*itwsReg_2);
	//				continue;
	//			}
	//			if (wsINI.at(18) == wsRegresult_2)
	//			{
	//				for (; itwsReg_2 != itEndIn; ++itwsReg_2)
	//					vstCompaniesTableFields.push_back(*itwsReg_2);
	//				continue;
	//			}
	//			if (wsINI.at(19) == wsRegresult_2)
	//			{
	//				for (; itwsReg_2 != itEndIn; ++itwsReg_2)
	//					vstRolesTableName.append(*itwsReg_2);
	//				continue;
	//			}					
	//			if (wsINI.at(20) == wsRegresult_2)
	//			{
	//				for (; itwsReg_2 != itEndIn; ++itwsReg_2)
	//					vstRolesTableFields.push_back(*itwsReg_2);
	//				continue;
	//			}
	//			if (wsINI.at(21) == wsRegresult_2)
	//			{
	//				for (; itwsReg_2 != itEndIn; ++itwsReg_2)
	//					vstSubsystemsTableName.append(*itwsReg_2);
	//				continue;
	//			}
	//			if (wsINI.at(22) == wsRegresult_2)
	//			{
	//				for (; itwsReg_2 != itEndIn; ++itwsReg_2)
	//					vstSubsystemsTableFields.push_back(*itwsReg_2);
	//				continue;
	//			}
	//			if (wsINI.at(23) == wsRegresult_2)
	//			{
	//				for (; itwsReg_2 != itEndIn; ++itwsReg_2)
	//					vstDatatypesTableName.append(*itwsReg_2);
	//				continue;
	//			}
	//			if (wsINI.at(24) == wsRegresult_2)
	//			{
	//				for (; itwsReg_2 != itEndIn; ++itwsReg_2)
	//					vstDatatypesTableFields.push_back(*itwsReg_2);
	//				continue;
	//			}
	//			//Init MySQl tables
	//			if (wsINI.at(25) == wsRegresult_2)
	//			{
	//				for (; itwsReg_2 != itEndIn; ++itwsReg_2)
	//					stMySQl_Hostname.append(*itwsReg_2);
	//				continue;
	//			}
	//			if (wsINI.at(26) == wsRegresult_2)
	//			{
	//				for (; itwsReg_2 != itEndIn; ++itwsReg_2)
	//					stMySQl_Login.append(*itwsReg_2);
	//				continue;
	//			}
	//			if (wsINI.at(27) == wsRegresult_2)
	//			{
	//				for (; itwsReg_2 != itEndIn; ++itwsReg_2)
	//					stMySQl_Password.append(*itwsReg_2);
	//				continue;
	//			}
	//			if (wsINI.at(28) == wsRegresult_2)
	//			{
	//				for (; itwsReg_2 != itEndIn; ++itwsReg_2)
	//					stMySQl_DefSchema.append(*itwsReg_2);
	//				continue;
	//			}					
	//			//Init REGEX masks
	//			if (wsINI.at(29) == wsRegresult_2)
	//			{
	//				for (; itwsReg_2 != itEndIn; ++itwsReg_2)
	//					vstChk_FileMaskCommon.append(*itwsReg_2);
	//				continue;
	//			}
	//			if (wsINI.at(30) == wsRegresult_2)
	//			{
	//				for (; itwsReg_2 != itEndIn; ++itwsReg_2)
	//					vstChk_Cyrillic.append(*itwsReg_2);
	//				continue;
	//			}
	//			if (wsINI.at(31) == wsRegresult_2)
	//			{
	//				for (; itwsReg_2 != itEndIn; ++itwsReg_2)
	//					vstChk_Company.append(*itwsReg_2);
	//				continue;
	//			}
	//			if (wsINI.at(32) == wsRegresult_2)
	//			{
	//				for (; itwsReg_2 != itEndIn; ++itwsReg_2)
	//					vstChk_Role.append(*itwsReg_2);
	//				continue;
	//			}
	//			if (wsINI.at(33) == wsRegresult_2)
	//			{
	//				for (; itwsReg_2 != itEndIn; ++itwsReg_2)
	//					vstChk_Stage.append(*itwsReg_2);
	//				continue;
	//			}
	//			if (wsINI.at(34) == wsRegresult_2)
	//			{
	//				for (; itwsReg_2 != itEndIn; ++itwsReg_2)
	//					vstChk_FindDStageShaPubZZ.append(*itwsReg_2);
	//				continue;
	//			}
	//			if (wsINI.at(35) == wsRegresult_2)
	//			{
	//				for (; itwsReg_2 != itEndIn; ++itwsReg_2)
	//					vstChk_FileMaskDStageShaPubZZ.append(*itwsReg_2);
	//				continue;
	//			}
	//		}
	//		else
	//		{
	//			dwErrorCode = ERROR_FILE_INVALID;
	//			throw dwErrorCode;
	//		}
	//	}
	//	dwErrorCode = GetLastError();
	//	throw dwErrorCode;
	//}
	//catch (DWORD Err_code)
	//{
	//	switch (Err_code)
	//	{
	//	case ERROR_SUCCESS:
	//		Logger::GetLogInstance()->PrepareTXTLOG("Function->InitbyINI(ParseINI()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", stCurDirBuff);
	//		break;
	//	default:
	//		CloseHandle(hFile);
	//		Logger::GetLogInstance()->PrepareTXTLOG("Function->InitbyINI(ParseINI()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", stCurDirBuff);
	//		ErrorHandle::GetErrorHandleInst()->ErrorExit(_T("InitDirbyINI->ParseINI()"), _T("."), dwErrorCode);
	//	}
	//}
	//CloseHandle(hFile);
	return dwErrorCode;
}
DWORD UI::ConvertDateTime()
{
	DWORD dwErrorCode = -1;
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

	dwErrorCode = ERROR_SUCCESS;
	return dwErrorCode;
};
DWORD UI::InitReviewPeriod()
{
	DWORD dwErrorCode = -1;
	BOOL bStatus = FALSE;
	//ULONGLONG qwResult;
	//std::basic_string<TCHAR> stNone = _T(" ");
	SetLastError(ERROR_SUCCESS);
	try
	{
		bStatus = SystemTimeToFileTime(&UI::GetUIInst().stStartChkPeriod, &this->tStartChkPeriod);
		bStatus = FALSE;
		bStatus = SystemTimeToFileTime(&UI::GetUIInst().stEndChkPeriod, &this->tEndChkPeriod);
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