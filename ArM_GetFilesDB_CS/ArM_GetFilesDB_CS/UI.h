#pragma once

class UI
{
private:
	UI();
	~UI();
	UI(const UI&) = delete;                 // Prevent copy-construction
	UI& operator=(const UI&) = delete;      // Prevent assignment
protected:
public:
	friend class FileProcess;
	friend class Logger;
	friend class DBProcess;

	static UI& GetUIInst()
	{
		static UI UIInst;
		return UIInst;
	};
	//const std::basic_string<TCHAR>& GetString()	{return stFileINI_Name;	}
		
	DWORD FindExePath();
	DWORD InitbyINI();
	DWORD ConvertDateTime();
	DWORD InitReviewPeriod();

	std::basic_string<TCHAR> stWorkDir;
	std::basic_string<TCHAR> stFileINI_Name;
	std::basic_string<TCHAR> stModuleName;
	std::basic_string<TCHAR> stModulePath;
	

	SYSTEMTIME stStartChkPeriod;
	SYSTEMTIME stEndChkPeriod;
	FILETIME	tStartChkPeriod;
	FILETIME	tEndChkPeriod;

	std::vector<std::basic_string<TCHAR>> vstFolderstoParse;
	std::vector<std::basic_string<TCHAR>>::iterator itFolderstoParse;
	std::vector<std::basic_string<TCHAR>> vstFolderstoIgnore;
	std::basic_string<TCHAR> stChkStartDateTime;
	std::basic_string<TCHAR> stChkEndDateTime;
	std::vector<std::basic_string<TCHAR>> vstChkStartDateTime;
	std::vector<std::basic_string<TCHAR>> vstChkEndDateTime;
	std::vector<std::basic_string<TCHAR>> vstRootFolders;
	std::vector<std::basic_string<TCHAR>>::iterator itRootFolders;

	std::basic_string<TCHAR> vstErrorsTableName;
	std::vector<std::basic_string<TCHAR>> vstErrorsTableFields;
	std::basic_string<TCHAR> vstExtensTableName;
	std::vector<std::basic_string<TCHAR>> vstExtensTableFields;
	std::basic_string<TCHAR> vstFilesTableName;
	std::vector<std::basic_string<TCHAR>> vstFilesTableFields;
	std::basic_string<TCHAR> vstFoldersTableName;
	std::vector<std::basic_string<TCHAR>> vstFoldersTableFields;
	std::basic_string<TCHAR> vstUsersTableName;
	std::vector<std::basic_string<TCHAR>> vstUsersTableFields;
	std::basic_string<TCHAR> vstProjectsTableName;
	std::vector<std::basic_string<TCHAR>> vstProjectsTableFields;
	std::basic_string<TCHAR> vstCompaniesTableName;
	std::vector<std::basic_string<TCHAR>> vstCompaniesTableFields;
	std::basic_string<TCHAR> vstRolesTableName;
	std::vector<std::basic_string<TCHAR>> vstRolesTableFields;
	std::basic_string<TCHAR> vstSubsystemsTableName;
	std::vector<std::basic_string<TCHAR>> vstSubsystemsTableFields;
	std::basic_string<TCHAR> vstDatatypesTableName;
	std::vector<std::basic_string<TCHAR>> vstDatatypesTableFields;

	std::basic_string<TCHAR> vstMySQl_Hostname;
	std::basic_string<TCHAR> vstMySQl_Login;
	std::basic_string<TCHAR> vstMySQl_Password;
	std::basic_string<TCHAR> vstMySQl_DefSchema;
	std::basic_string<TCHAR> vstChk_FileMaskCommon;
	std::basic_string<TCHAR> vstChk_Cyrillic;
	std::basic_string<TCHAR> vstChk_Company;
	std::basic_string<TCHAR> vstChk_Role;
	std::basic_string<TCHAR> vstChk_Stage;
	std::basic_string<TCHAR> vstChk_FindDStageShaPubZZ;
	std::basic_string<TCHAR> vstChk_FileMaskDStageShaPubZZ;
};