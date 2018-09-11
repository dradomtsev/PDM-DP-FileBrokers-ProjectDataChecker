#pragma once
class UI;

class FileProcess
{
private:

	SYSTEMTIME	curSysTime;
	FILETIME	curFileTime;
	FILETIME	tStartChkPeriod;
	FILETIME	tEndChkPeriod;
	struct FileCAWTime
	{
		INT64 i64Time_create, i64Time_access, i64Time_write;
	};
	struct FileInfo
	{
		std::basic_string<TCHAR> sFileName;
		std::basic_string<TCHAR> sFileExtension;
		std::basic_string<TCHAR> sFileOwnerName;
		std::basic_string<TCHAR> sFileDirName;
		std::basic_string<TCHAR> sFileDirPath;
		std::basic_string<TCHAR> sFileDirPathChngView;
		std::basic_string<TCHAR> sFile_ProjectbyName;
		std::basic_string<TCHAR> sFile_ProjectbyPath;
		std::basic_string<TCHAR> sFile_CompanyRolebyName;
		std::basic_string<TCHAR> sFile_CompanybyName;
		std::basic_string<TCHAR> sFile_RolebyName;
		std::basic_string<TCHAR> sFile_ProjectStagebyName;
		std::basic_string<TCHAR> sFile_ProjectStageforDB;
		std::basic_string<TCHAR> sFile_StageforFL;
		std::basic_string<TCHAR> sFile_StagebyName;
		std::basic_string<TCHAR> sFile_StagebyPath;
		std::basic_string<TCHAR> sFile_SubsystembyName;
		std::basic_string<TCHAR> sFile_DatatypebyName;
		INT iChkMask		= -1;
		INT iChkCyrillic	= -1;
		INT iChkProjectDB	= -1;
		INT iChkProjectDR	= -1;
		INT iChk—ompanyDB	= -1;
		INT iChkRoleDB		= -1;
		INT iChkStageDB		= -1;
		INT iChkStageDR		= -1;
		INT iChkSubSystemDB = -1;
		INT iChkDataTypeDB	= -1;
		LARGE_INTEGER sFileSize		= { 0 };
		//std::string sNumberofLinks;
		FileCAWTime sFileCAWTime	= { 0 };
	} sFileInfoInst;
	std::basic_string <char>::size_type iIndex;
	std::basic_string <char>::size_type npos;
	std::vector<std::basic_string<TCHAR>> vstExistExtens;
	std::vector<std::basic_string<TCHAR>> vstExistDirs;
	//std::vector<std::basic_string<TCHAR>> vstExistDirsRoot;
	//std::basic_string<TCHAR> stDirPathforNameCheck;
protected:
public:
	//friend class UI;
	friend class DBProcess;

	FileProcess();
	~FileProcess();

	DWORD InitReviewPeriod		();

	DWORD StartMySQLConect		();
	DWORD InitMySQLConect		();
	DWORD CloseMySQLConect		();
	
	DWORD IterDirs				(std::basic_string<TCHAR> stWorkDir);
	DWORD IterObjects			(std::basic_string<TCHAR> twrkDir,const std::basic_string<TCHAR> &twrkDirName, int iCounter);
	DWORD IterProcessFiles		(HANDLE hFileDataFindFirst, std::basic_string<TCHAR> twrkDirtemp, std::basic_string<TCHAR> twrkDirName);

	DWORD GetFileOwnerName		(HANDLE hFile, std::basic_string<TCHAR> sFileName);
	DWORD GetFileSizeInst		(HANDLE hFile);
	DWORD GetFileCAWTime		(HANDLE hFile);
	DWORD GetFileInfobyName		();
	DWORD GetFileInfobyFolder	();
	DWORD ChangeFolderView		();
	DWORD ChkMask				(const std::basic_string<TCHAR> &sFileName);
	DWORD ChkCyrillic			(const std::basic_string<TCHAR> &sFileName);
	DWORD ChkProjectDB			();
	DWORD ChkProjectDR			();
	DWORD Chk—ompanyDB			();
	DWORD ChkRoleDB				();
	DWORD ChkStageDB			();
	DWORD ChkStageDR			();
	DWORD ChkSubSystemDB		();
	DWORD ChkDataTypeDB			();
};
