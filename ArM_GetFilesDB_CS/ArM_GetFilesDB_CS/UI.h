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
	std::vector<std::basic_string<TCHAR>> vstFilestoIgnore;
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

	std::basic_string<TCHAR> stMySQl_Hostname;
	std::basic_string<TCHAR> stMySQl_Login;
	std::basic_string<TCHAR> stMySQl_Password;
	std::basic_string<TCHAR> stMySQl_DefSchema;

	std::vector<std::basic_string<TCHAR>> vstProjectsInclude_fns2;
	std::vector<std::basic_string<TCHAR>> vstProjectsExclude_fns2;
	std::basic_string<TCHAR> vstChk_FileMaskCommon_fns2;
	std::basic_string<TCHAR> vstChk_Cyrillic_fns2;
	std::basic_string<TCHAR> vstChk_Company_fns2;
	std::basic_string<TCHAR> vstChk_Role_fns2;
	std::basic_string<TCHAR> vstChk_Stage_fns2;
	std::basic_string<TCHAR> vstChk_BuildingSection_fns2;
	std::basic_string<TCHAR> vstChk_SubSystem_fns2;

	std::basic_string<TCHAR> vstChk_FindDStageShaPubZZ_fns2;
	std::basic_string<TCHAR> vstChk_FileMaskDStageShaPubZZ_fns2;
	std::basic_string<TCHAR> vstChk_MaskDCompanyShaPubZZ_fns2;

	std::vector<std::basic_string<TCHAR>> vstProjectsInclude_fns3;
	std::vector<std::basic_string<TCHAR>> vstProjectsExclude_fns3;
	std::basic_string<TCHAR> vstChk_FileMaskCommon_fns3;
	std::basic_string<TCHAR> vstChk_Cyrillic_fns3;
	std::basic_string<TCHAR> vstChk_Company_fns3;
	std::basic_string<TCHAR> vstChk_Role_fns3;
	std::basic_string<TCHAR> vstChk_Stage_fns3;
	std::basic_string<TCHAR> vstChk_BuildingSection_fns3;
	std::basic_string<TCHAR> vstChk_SubSystem_fns3;

	std::basic_string<TCHAR> vstChk_FindDStageShaPubZZ_fns3;
	std::basic_string<TCHAR> vstChk_FileMaskDStageShaPubZZ_fns3;
	std::basic_string<TCHAR> vstChk_MaskDCompanyShaPubZZ_fns3;
};