// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"

DBProcess::DBProcess() 
{
	MySQLConnectionInst.stTCIP.assign(				ConvertStrings::GetConvStrInst()->UnicodeStringToAnsiString(UI::GetUIInst()->vstMySQl_Hostname));
	MySQLConnectionInst.stLogin.assign(				ConvertStrings::GetConvStrInst()->UnicodeStringToAnsiString(UI::GetUIInst()->vstMySQl_Login));
	MySQLConnectionInst.stPassword.assign(			ConvertStrings::GetConvStrInst()->UnicodeStringToAnsiString(UI::GetUIInst()->vstMySQl_Password));
	MySQLConnectionInst.stSchemaName.assign(		ConvertStrings::GetConvStrInst()->UnicodeStringToAnsiString(UI::GetUIInst()->vstMySQl_DefSchema));

	stMySQLTableErrors.stMySQLTable_name.assign(	ConvertStrings::GetConvStrInst()->UnicodeStringToAnsiString(UI::GetUIInst()->vstErrorsTableName));
	stMySQLTableErrors.stMySQLTable_fields =		ConvertStrings::GetConvStrInst()->UnicodeVectorToAnsiVector(UI::GetUIInst()->vstErrorsTableFields);

	stMySQLTableExtens.stMySQLTable_name.assign(	ConvertStrings::GetConvStrInst()->UnicodeStringToAnsiString(UI::GetUIInst()->vstExtensTableName));
	stMySQLTableExtens.stMySQLTable_fields =		ConvertStrings::GetConvStrInst()->UnicodeVectorToAnsiVector(UI::GetUIInst()->vstExtensTableFields);

	stMySQLTableFiles.stMySQLTable_name.assign(		ConvertStrings::GetConvStrInst()->UnicodeStringToAnsiString(UI::GetUIInst()->vstFilesTableName));
	stMySQLTableFiles.stMySQLTable_fields =			ConvertStrings::GetConvStrInst()->UnicodeVectorToAnsiVector(UI::GetUIInst()->vstFilesTableFields);

	stMySQLTableFolders.stMySQLTable_name.assign(	ConvertStrings::GetConvStrInst()->UnicodeStringToAnsiString(UI::GetUIInst()->vstFoldersTableName));
	stMySQLTableFolders.stMySQLTable_fields =		ConvertStrings::GetConvStrInst()->UnicodeVectorToAnsiVector(UI::GetUIInst()->vstFoldersTableFields);

	stMySQLTableUsers.stMySQLTable_name.assign(		ConvertStrings::GetConvStrInst()->UnicodeStringToAnsiString(UI::GetUIInst()->vstUsersTableName));
	stMySQLTableUsers.stMySQLTable_fields =			ConvertStrings::GetConvStrInst()->UnicodeVectorToAnsiVector(UI::GetUIInst()->vstUsersTableFields);

	stMySQLTableProjects.stMySQLTable_name.assign(	ConvertStrings::GetConvStrInst()->UnicodeStringToAnsiString(UI::GetUIInst()->vstProjectsTableName));
	stMySQLTableProjects.stMySQLTable_fields =		ConvertStrings::GetConvStrInst()->UnicodeVectorToAnsiVector(UI::GetUIInst()->vstProjectsTableFields);

	stMySQLTableCompanies.stMySQLTable_name.assign(	ConvertStrings::GetConvStrInst()->UnicodeStringToAnsiString(UI::GetUIInst()->vstCompaniesTableName));
	stMySQLTableCompanies.stMySQLTable_fields =		ConvertStrings::GetConvStrInst()->UnicodeVectorToAnsiVector(UI::GetUIInst()->vstCompaniesTableFields);

	stMySQLTableRole.stMySQLTable_name.assign(		ConvertStrings::GetConvStrInst()->UnicodeStringToAnsiString(UI::GetUIInst()->vstRolesTableName));
	stMySQLTableRole.stMySQLTable_fields =			ConvertStrings::GetConvStrInst()->UnicodeVectorToAnsiVector(UI::GetUIInst()->vstRolesTableFields);

	stMySQLTableSubsystem.stMySQLTable_name.assign(	ConvertStrings::GetConvStrInst()->UnicodeStringToAnsiString(UI::GetUIInst()->vstSubsystemsTableName));
	stMySQLTableSubsystem.stMySQLTable_fields =		ConvertStrings::GetConvStrInst()->UnicodeVectorToAnsiVector(UI::GetUIInst()->vstSubsystemsTableFields);

	stMySQLTableDatatype.stMySQLTable_name.assign(	ConvertStrings::GetConvStrInst()->UnicodeStringToAnsiString(UI::GetUIInst()->vstDatatypesTableName));
	stMySQLTableDatatype.stMySQLTable_fields =		ConvertStrings::GetConvStrInst()->UnicodeVectorToAnsiVector(UI::GetUIInst()->vstDatatypesTableFields);
};
DBProcess::~DBProcess() {};
DBProcess * DBProcess::dbProcInstance()
{
	static DBProcess dbProcInst;
	return &dbProcInst;
};
DWORD DBProcess::DBCreateMYSQLConnection()
{
	DWORD dwErrorCode = -1;
	try
	{
		std::shared_ptr<sql::Statement> stmt;

		driver = sql::mysql::get_mysql_driver_instance();
		con.reset(driver->connect(this->MySQLConnectionInst.stTCIP.c_str(), this->MySQLConnectionInst.stLogin.c_str(), this->MySQLConnectionInst.stPassword.c_str() ));
		con->setSchema(this->MySQLConnectionInst.stSchemaName.c_str());
		std::string stMySQL_UseSchema;
		stMySQL_UseSchema = "USE `" + this->MySQLConnectionInst.stSchemaName + "`;";
		stmt.reset(con->createStatement());
		stmt->execute(stMySQL_UseSchema.c_str());

		stmt.reset();
		dwErrorCode = GetLastError();
		throw dwErrorCode;
	}
	catch (sql::SQLException &e)
	{
		Logger::GetLogInstance()->PrepareMySQLLOG("Error connect to MySQL driver", "Function->DBCreateMYSQLConnection()", e.what(), e.getErrorCode(), e.getSQLState());
		Logger::GetLogInstance()->PrepareTXTLOG("Function->DBCreateMYSQLConnection() -> MySQL error: ", e.what(), "MySQL error code: ", e.getErrorCode(), "SQLState: ", e.getSQLState());
	}
	catch (DWORD dwErrorCode)
	{
		switch (dwErrorCode)
		{
		case ERROR_SUCCESS:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->DBCreateMYSQLConnection(): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", "MySQL connection");
			break;
		default:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->DBCreateMYSQLConnection(): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", "MySQL connection");
			ErrorHandle::GetErrorHandleInst()->ErrorExit(_T("IterObjects->DBCreateMYSQLConnection()"), _T("MySQL connection"), dwErrorCode);
		}
	}
	return dwErrorCode;
}
DWORD DBProcess::DBInitTables()
{
	DWORD dwErrorCode = -1;
	try
	{
		//pstmtInit = con->prepareStatement("CREATE TABLE fileInfo (dbName VARCHAR(255) NOT NULL, dbFileRoot VARCHAR(255) NOT NULL, dbFileOwnerName VARCHAR(255) NOT NULL, dbFileSize VARCHAR(255) NOT NULL, dbCreateUNIXTime VARCHAR(255) NOT NULL, dbAccessUNIXTime VARCHAR(255) NOT NULL, dbWriteUNIXTime VARCHAR(255) NOT NULL, dbRoot_Name VARCHAR(255) NOT NULL);");
		//pstmtInit = con->prepareStatement("CREATE TABLE fileInfo (dbName VARCHAR(255) NOT NULL, dbFileRoot VARCHAR(255) NOT NULL, dbFileOwnerName VARCHAR(255) NOT NULL, dbFileSize INT NOT NULL, dbCreateDate DATE DEFAULT'1000-01-01' NOT NULL, dbCreateTime TIME DEFAULT'-838:59:59' NOT NULL, dbAccessDate DATE DEFAULT'1000-01-01' NOT NULL, dbAccessTime TIME DEFAULT'-838:59:59' NOT NULL, dbWriteDate DATE DEFAULT'1000-01-01' NOT NULL, dbWriteTime TIME DEFAULT'-838:59:59' NOT NULL);");
		std::shared_ptr<sql::Statement> stmt;
		std::string stMySQLST_Truncate("TRUNCATE TABLE ");

		std::string stMySQLST_TruncateTable = stMySQLST_Truncate + stMySQLTableExtens.stMySQLTable_name + ";";
		stmt.reset(con->createStatement());
		stmt->execute(stMySQLST_TruncateTable);
		stMySQLST_TruncateTable.clear();
		stmt->close();
		stmt.reset();

		stMySQLST_TruncateTable = stMySQLST_Truncate + stMySQLTableFiles.stMySQLTable_name + ";";
		stmt.reset(con->createStatement());
		stmt->execute(stMySQLST_TruncateTable);
		stMySQLST_TruncateTable.clear();
		stmt->close();
		stmt.reset();

		stMySQLST_TruncateTable = stMySQLST_Truncate + stMySQLTableFolders.stMySQLTable_name + ";";
		stmt.reset(con->createStatement());
		stmt->execute(stMySQLST_TruncateTable);
		stMySQLST_TruncateTable.clear();
		stmt->close();
		stmt.reset();

		dwErrorCode = GetLastError();
		throw dwErrorCode;
	}
	catch (sql::SQLException &e)
	{
		Logger::GetLogInstance()->PrepareMySQLLOG("Error truncate MySQL tables", "Function->DBInitTables()", e.what(), e.getErrorCode(), e.getSQLState());
		Logger::GetLogInstance()->PrepareTXTLOG("Function->DBInitTables(TRUNCATE TABLES) -> MySQL error: ", e.what(), "MySQL error code: ", e.getErrorCode(), "SQLState: ", e.getSQLState());
	}
	catch (DWORD dwErrorCode)
	{
		switch (dwErrorCode)
		{
		case ERROR_SUCCESS:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->DBInitTables(TRUNCATE TABLES): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", "MySQL connection");
			break;
		default:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->DBInitTables(TRUNCATE TABLES): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", "MySQL connection");
			ErrorHandle::GetErrorHandleInst()->ErrorExit(_T("IterObjects->DBInitTables(TRUNCATE TABLES)"), _T("MySQL connection"), dwErrorCode);
		}
	}
	return dwErrorCode;
};
DWORD DBProcess::DBWriteFolders(FileProcess *fpInst)
{
	DWORD dwErrorCode = -1;
	std::shared_ptr<sql::PreparedStatement> pstmtWrite;

	//INSERT INTO portal.folders
	try
	{
		std::string sFileDirPathTMP = ConvertStrings::GetConvStrInst()->UnicodeStringToAnsiString(fpInst->sFileInfoInst.sFileDirPathChngView);

		std::string stMySQLST_Insert("INSERT INTO tablename (field) VALUES (?);");
		stMySQLST_Insert.replace(stMySQLST_Insert.find("tablename"),std::string("tablename").length(),	stMySQLTableFolders.stMySQLTable_name);
		stMySQLST_Insert.replace(stMySQLST_Insert.find("field"),	std::string("field").length(),		stMySQLTableFolders.stMySQLTable_fields.at(0));
		pstmtWrite.reset(con->prepareStatement(stMySQLST_Insert));//folders_bak folders
		pstmtWrite->setString(1, sFileDirPathTMP);	
		pstmtWrite->execute();
		pstmtWrite->close();

		pstmtWrite.reset();
		dwErrorCode = GetLastError();
		throw dwErrorCode;
	}
	catch (sql::SQLException &e)
	{
		Logger::GetLogInstance()->PrepareMySQLLOG("Error write to table folders", "Function->DBWriteFolders()", e.what(), e.getErrorCode(), e.getSQLState());
		Logger::GetLogInstance()->PrepareTXTLOG("Function->DBWriteFolders(INSERT INTO folders) -> MySQL error: ", e.what(), "MySQL error code: ", e.getErrorCode(), "SQLState: ", e.getSQLState());
	}
	catch (DWORD Err_code)
	{
		switch (Err_code)
		{
		case ERROR_SUCCESS:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->DBWriteFolders(INSERT INTO folders): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", fpInst->sFileInfoInst.sFileDirPath);
			break;
		default:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->DBWriteFolders(INSERT INTO folders): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", fpInst->sFileInfoInst.sFileDirPath);
			Logger::GetLogInstance()->PrepareMySQLLOG("Error write to table folders", "Function->DBWriteFolders(INSERT INTO folders)", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCode, fpInst->sFileInfoInst.sFileDirPath);
			ErrorHandle::GetErrorHandleInst()->ErrorExit(_T("IterObjects->DBWriteFolders(INSERT INTO folders)"), (LPTSTR)&fpInst->sFileInfoInst.sFileDirPath, dwErrorCode);
		}
	}
	return dwErrorCode;
};
DWORD DBProcess::DBWriteExtensions(FileProcess *fpInst)
{
	DWORD dwErrorCode = -1;
	std::shared_ptr<sql::PreparedStatement> pstmtWrite;

	// INSERT INTO portal.extensions
	try
	{
		std::string sFileExtensionTMP = ConvertStrings::GetConvStrInst()->UnicodeStringToAnsiString(fpInst->sFileInfoInst.sFileExtension);

		std::string stMySQLST_Insert("INSERT INTO tablename (field) VALUES (?);");
		stMySQLST_Insert.replace(stMySQLST_Insert.find("tablename"),std::string("tablename").length(),	stMySQLTableExtens.stMySQLTable_name);
		stMySQLST_Insert.replace(stMySQLST_Insert.find("field"),	std::string("field").length(),		stMySQLTableExtens.stMySQLTable_fields.at(0));
		pstmtWrite.reset(con->prepareStatement(stMySQLST_Insert));// extensions extensions_bak
		pstmtWrite->setString(1, sFileExtensionTMP);
		pstmtWrite->execute();
		pstmtWrite->close();

		pstmtWrite.reset();

		dwErrorCode = GetLastError();
		throw dwErrorCode;
	}
	catch (sql::SQLException &e)
	{
		Logger::GetLogInstance()->PrepareMySQLLOG("Error write to table extensions", e.what(), "MySQL error code: ", e.getErrorCode(), fpInst->sFileInfoInst.sFileExtension);
		Logger::GetLogInstance()->PrepareTXTLOG("Function->DBWriteExtensions(INSERT INTO extensions) -> MySQL error: ", e.what(), "MySQL error code: ", e.getErrorCode(), "SQLState: ", e.getSQLState());
	}
	catch (DWORD Err_code)
	{
		switch (Err_code)
		{
		case ERROR_SUCCESS:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->DBWriteExtensions(INSERT INTO extensions): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", fpInst->sFileInfoInst.sFileExtension);
			break;
		default:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->DBWriteExtensions(INSERT INTO extensions): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", fpInst->sFileInfoInst.sFileExtension);
			Logger::GetLogInstance()->PrepareMySQLLOG("Error write to table extensions", "Function->DBWriteExtensions(INSERT INTO extensions)", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCode, fpInst->sFileInfoInst.sFileExtension);
			ErrorHandle::GetErrorHandleInst()->ErrorExit(_T("IterObjects->DBWriteExtensions(INSERT INTO extensions)"), (LPTSTR)&fpInst->sFileInfoInst.sFileExtension, dwErrorCode);
		}
	}
	return dwErrorCode;
};
DWORD DBProcess::DBWriteFiles(FileProcess *fpInst)
{
	DWORD dwErrorCode = -1;
	std::shared_ptr<sql::PreparedStatement> pstmtGet;
	std::shared_ptr<sql::ResultSet> resGet;
	int Result = 0;
	int iExtension_ID = 0;
	int iFolder_ID = 0;
	int iUser_ID = 0;
	// Get from portal.files
	try
	{
		std::string sFileExtensionTMP = ConvertStrings::GetConvStrInst()->UnicodeStringToAnsiString(fpInst->sFileInfoInst.sFileExtension);

		std::string stMySQLST_Select("SELECT id FROM tablename WHERE field = (?);");
		stMySQLST_Select.replace(stMySQLST_Select.find("tablename"),std::string("tablename").length(),	stMySQLTableExtens.stMySQLTable_name);
		stMySQLST_Select.replace(stMySQLST_Select.find("field"),	std::string("field").length(),		stMySQLTableExtens.stMySQLTable_fields.at(0));
		pstmtGet.reset(con->prepareStatement(stMySQLST_Select));// extensions_bak extensions
		pstmtGet->setString(1, sFileExtensionTMP);
		resGet.reset( pstmtGet->executeQuery() );
		pstmtGet->close();
		while (resGet->next()) { iExtension_ID = resGet->getInt(1); };

		std::string sFileFolderTMP = ConvertStrings::GetConvStrInst()->UnicodeStringToAnsiString(fpInst->sFileInfoInst.sFileDirPathChngView);

		stMySQLST_Select.assign("SELECT id FROM tablename WHERE field = (?);");
		stMySQLST_Select.replace(stMySQLST_Select.find("tablename"),std::string("tablename").length(),	stMySQLTableFolders.stMySQLTable_name);
		stMySQLST_Select.replace(stMySQLST_Select.find("field"),	std::string("field").length(),		stMySQLTableFolders.stMySQLTable_fields.at(0));
		pstmtGet.reset(con->prepareStatement(stMySQLST_Select));// folders_bak folders
		pstmtGet->setString(1, sFileFolderTMP);
		resGet.reset(pstmtGet->executeQuery());
		pstmtGet->close();
		while (resGet->next()) { iFolder_ID = resGet->getInt(1); };

		std::string ssFileUserTMP = ConvertStrings::GetConvStrInst()->UnicodeStringToAnsiString(fpInst->sFileInfoInst.sFileOwnerName);

		stMySQLST_Select.assign("SELECT id FROM tablename WHERE field = (?);");
		stMySQLST_Select.replace(stMySQLST_Select.find("tablename"),std::string("tablename").length(),	stMySQLTableUsers.stMySQLTable_name);
		stMySQLST_Select.replace(stMySQLST_Select.find("field"),	std::string("field").length(),		stMySQLTableUsers.stMySQLTable_fields.at(0));
		pstmtGet.reset(con->prepareStatement(stMySQLST_Select));
		pstmtGet->setString(1, ssFileUserTMP);
		resGet.reset(pstmtGet->executeQuery());
		pstmtGet->close();
		while (resGet->next()) { iUser_ID = resGet->getInt(1); };

		resGet.reset();
		pstmtGet.reset();

		dwErrorCode = GetLastError();
		throw dwErrorCode;
	}
	catch (sql::SQLException &e)
	{
		Logger::GetLogInstance()->PrepareMySQLLOG("Error SELECT id FROM TABLES", "Function->DBWriteFiles(SELECT id FROM TABLES)", e.what(), e.getErrorCode(), e.getSQLState());
		Logger::GetLogInstance()->PrepareTXTLOG("Function->DBWriteFiles(SELECT id FROM TABLES) -> MySQL error: ", e.what(), "MySQL error code: ", e.getErrorCode(), "SQLState: ", e.getSQLState());
	}
	catch (DWORD Err_code)
	{
		switch (Err_code)
		{
		case ERROR_SUCCESS:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->DBWriteFiles(SELECT id FROM TABLES): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", fpInst->sFileInfoInst.sFileName);
			break;
		default:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->DBWriteFiles(SELECT id FROM TABLES): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", fpInst->sFileInfoInst.sFileName);
			Logger::GetLogInstance()->PrepareMySQLLOG("Error in getting file attributes", "Function->DBWriteFiles(SELECT id FROM TABLES)", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCode, fpInst->sFileInfoInst.sFileName);
			ErrorHandle::GetErrorHandleInst()->ErrorExit(_T("IterObjects->DBWriteFiles(SELECT id FROM TABLES)"), (LPTSTR)&fpInst->sFileInfoInst.sFileName, dwErrorCode);
		}
	}

	// INSERT INTO portal.files
	std::shared_ptr<sql::PreparedStatement> pstmtWrite;
	try
	{
		std::string sFileSize, sTime_create, sTime_write;
		sFileSize.insert	(0, std::to_string(fpInst->sFileInfoInst.sFileSize.QuadPart));
		sTime_create.insert	(0, std::to_string(fpInst->sFileInfoInst.sFileCAWTime.i64Time_create));
		sTime_write.insert	(0, std::to_string(fpInst->sFileInfoInst.sFileCAWTime.i64Time_write));

		//std::basic_string<TCHAR> stPathFilename;
		//stPathFilename.insert(0, fpInst->sFileInfoInst.sFileDirPath);
		//stPathFilename.append(fpInst->sFileInfoInst.sFileName);
		//std::string sFileDirPathTMP = ConvertStrings::GetConvStrInst()->UnicodeStringToAnsiString(stPathFilename);
		std::string sFileNameTMP = ConvertStrings::GetConvStrInst()->UnicodeStringToAnsiString(fpInst->sFileInfoInst.sFileName);

		std::string stMySQLST_Insert ("INSERT INTO tablename (field_0, field_1, field_2, field_3, field_4, field_5, field_6, field_7, field_8, field_9, field_10, field_11, field_12, field_13, field_14, field_15, field_16) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?);");
		stMySQLST_Insert.replace(stMySQLST_Insert.find("tablename"), std::string("tablename").length(), stMySQLTableFiles.stMySQLTable_name);
		stMySQLST_Insert.replace(stMySQLST_Insert.find("field_0"), std::string("field_0").length(), stMySQLTableFiles.stMySQLTable_fields.at(0));
		stMySQLST_Insert.replace(stMySQLST_Insert.find("field_1"), std::string("field_1").length(), stMySQLTableFiles.stMySQLTable_fields.at(1));
		stMySQLST_Insert.replace(stMySQLST_Insert.find("field_2"), std::string("field_2").length(), stMySQLTableFiles.stMySQLTable_fields.at(2));
		stMySQLST_Insert.replace(stMySQLST_Insert.find("field_3"), std::string("field_3").length(), stMySQLTableFiles.stMySQLTable_fields.at(3));
		stMySQLST_Insert.replace(stMySQLST_Insert.find("field_4"), std::string("field_4").length(), stMySQLTableFiles.stMySQLTable_fields.at(4));
		stMySQLST_Insert.replace(stMySQLST_Insert.find("field_5"), std::string("field_5").length(), stMySQLTableFiles.stMySQLTable_fields.at(5));
		stMySQLST_Insert.replace(stMySQLST_Insert.find("field_6"), std::string("field_6").length(), stMySQLTableFiles.stMySQLTable_fields.at(6));

		stMySQLST_Insert.replace(stMySQLST_Insert.find("field_7"), std::string("field_7").length(), stMySQLTableFiles.stMySQLTable_fields.at(7));
		stMySQLST_Insert.replace(stMySQLST_Insert.find("field_8"), std::string("field_8").length(), stMySQLTableFiles.stMySQLTable_fields.at(8));
		stMySQLST_Insert.replace(stMySQLST_Insert.find("field_9"), std::string("field_9").length(), stMySQLTableFiles.stMySQLTable_fields.at(9));
		stMySQLST_Insert.replace(stMySQLST_Insert.find("field_10"), std::string("field_10").length(), stMySQLTableFiles.stMySQLTable_fields.at(10));
		stMySQLST_Insert.replace(stMySQLST_Insert.find("field_11"), std::string("field_11").length(), stMySQLTableFiles.stMySQLTable_fields.at(11));
		stMySQLST_Insert.replace(stMySQLST_Insert.find("field_12"), std::string("field_12").length(), stMySQLTableFiles.stMySQLTable_fields.at(12));
		stMySQLST_Insert.replace(stMySQLST_Insert.find("field_13"), std::string("field_13").length(), stMySQLTableFiles.stMySQLTable_fields.at(13));
		stMySQLST_Insert.replace(stMySQLST_Insert.find("field_14"), std::string("field_14").length(), stMySQLTableFiles.stMySQLTable_fields.at(14));
		stMySQLST_Insert.replace(stMySQLST_Insert.find("field_15"), std::string("field_15").length(), stMySQLTableFiles.stMySQLTable_fields.at(15));
		stMySQLST_Insert.replace(stMySQLST_Insert.find("field_16"), std::string("field_16").length(), stMySQLTableFiles.stMySQLTable_fields.at(16));

		pstmtWrite.reset(con->prepareStatement(stMySQLST_Insert));
		pstmtWrite->setString	(1, sFileNameTMP);
		pstmtWrite->setInt		(2, (int32_t)iExtension_ID);
		pstmtWrite->setInt		(3, (int32_t)iFolder_ID);
		pstmtWrite->setInt		(4, (int32_t)iUser_ID);
		pstmtWrite->setString	(5, sFileSize);
		
		pstmtWrite->setInt		(6, fpInst->sFileInfoInst.iChkMask);
		pstmtWrite->setInt		(7, fpInst->sFileInfoInst.iChkCyrillic);
		pstmtWrite->setInt		(8, fpInst->sFileInfoInst.iChkProjectDB);
		pstmtWrite->setInt		(9, fpInst->sFileInfoInst.iChkProjectDR);
		pstmtWrite->setInt		(10, fpInst->sFileInfoInst.iChkÑompanyDB);
		pstmtWrite->setInt		(11, fpInst->sFileInfoInst.iChkRoleDB);
		pstmtWrite->setInt		(12, fpInst->sFileInfoInst.iChkStageDB);
		pstmtWrite->setInt		(13, fpInst->sFileInfoInst.iChkStageDR);
		pstmtWrite->setInt		(14, fpInst->sFileInfoInst.iChkSubSystemDB);
		pstmtWrite->setInt		(15, fpInst->sFileInfoInst.iChkDataTypeDB);

		pstmtWrite->setInt64	(16, fpInst->sFileInfoInst.sFileCAWTime.i64Time_create);
		pstmtWrite->setInt64	(17, fpInst->sFileInfoInst.sFileCAWTime.i64Time_write);

		pstmtWrite->execute();
		pstmtWrite->close();

		pstmtWrite.reset();

		dwErrorCode = GetLastError();
		throw dwErrorCode;
	}
	catch (sql::SQLException &e)
	{
		Logger::GetLogInstance()->PrepareMySQLLOG("Error INSERT INTO files", "Function->DBWriteFiles(INSERT INTO files)", e.what(), e.getErrorCode(), e.getSQLState());
		Logger::GetLogInstance()->PrepareTXTLOG("Function->DBWriteFiles(INSERT INTO files) -> MySQL error: ", e.what(), "MySQL error code: ", e.getErrorCode(), "SQLState: ", e.getSQLState());
	}
	catch (DWORD Err_code)
	{
		switch (Err_code)
		{
		case ERROR_SUCCESS:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->DBWriteFiles(INSERT INTO files): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", fpInst->sFileInfoInst.sFileName);
			break;
		default:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->DBWriteFiles(INSERT INTO files): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", fpInst->sFileInfoInst.sFileName);
			Logger::GetLogInstance()->PrepareMySQLLOG("Error in getting file attributes", "Function->DBWriteFiles(INSERT INTO files)", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCode, fpInst->sFileInfoInst.sFileName);
			ErrorHandle::GetErrorHandleInst()->ErrorExit(_T("IterObjects->DBWriteFiles(INSERT INTO files)"), (LPTSTR)&fpInst->sFileInfoInst.sFileName, dwErrorCode);
		}
	}
	return dwErrorCode;
};
DWORD DBProcess::DBGetProjects(FileProcess *fpInst, BOOL &bStatus)
{
	DWORD dwErrorCode = -1;
	std::shared_ptr<sql::PreparedStatement> pstmtGet;
	std::shared_ptr<sql::ResultSet> resGet;
	int Result = 0;
	int iProjectID = 0;

	// Get from portal.files
	try
	{
		std::string sFile_ProjectTMP = ConvertStrings::GetConvStrInst()->UnicodeStringToAnsiString(fpInst->sFileInfoInst.sFile_ProjectbyName);
		sFile_ProjectTMP.append("%");
		std::string stMySQLST_Select("SELECT * FROM tablename WHERE field LIKE (?);");
		stMySQLST_Select.replace(stMySQLST_Select.find("tablename"), std::string("tablename").length(), stMySQLTableProjects.stMySQLTable_name);
		stMySQLST_Select.replace(stMySQLST_Select.find("field"), std::string("field").length(), stMySQLTableProjects.stMySQLTable_fields.at(0));
		pstmtGet.reset(con->prepareStatement(stMySQLST_Select));// extensions_bak extensions
		pstmtGet->setString(1, sFile_ProjectTMP);
		resGet.reset(pstmtGet->executeQuery());
		pstmtGet->close();
		while (resGet->next())
		{ 
			bStatus = TRUE;
			iProjectID = resGet->getInt(1);
		};
		resGet.reset();
		pstmtGet.reset();

		dwErrorCode = GetLastError();
		throw dwErrorCode;
	}
	catch (sql::SQLException &e)
	{
		Logger::GetLogInstance()->PrepareMySQLLOG("Error SELECT id FROM TABLES", "Function->DBWriteFiles(SELECT id FROM TABLES)", e.what(), e.getErrorCode(), e.getSQLState());
		Logger::GetLogInstance()->PrepareTXTLOG("Function->DBWriteFiles(SELECT id FROM TABLES) -> MySQL error: ", e.what(), "MySQL error code: ", e.getErrorCode(), "SQLState: ", e.getSQLState());
	}
	catch (DWORD Err_code)
	{
		switch (Err_code)
		{
		case ERROR_SUCCESS:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->DBWriteFiles(SELECT id FROM TABLES): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", fpInst->sFileInfoInst.sFileName);
			break;
		default:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->DBWriteFiles(SELECT id FROM TABLES): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", fpInst->sFileInfoInst.sFileName);
			Logger::GetLogInstance()->PrepareMySQLLOG("Error in getting file attributes", "Function->DBWriteFiles(SELECT id FROM TABLES)", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCode, fpInst->sFileInfoInst.sFileName);
			ErrorHandle::GetErrorHandleInst()->ErrorExit(_T("IterObjects->DBWriteFiles(SELECT id FROM TABLES)"), (LPTSTR)&fpInst->sFileInfoInst.sFileName, dwErrorCode);
		}
	}
};
DWORD DBProcess::DBGetCompany(FileProcess *fpInst, BOOL &bStatus)
{
	DWORD dwErrorCode = -1;
	std::shared_ptr<sql::PreparedStatement> pstmtGet;
	std::shared_ptr<sql::ResultSet> resGet;
	int Result = 0;
	int iCompanyID = 0;

	// Get from portal.files
	try
	{
		std::string sFile_CompanyTMP = ConvertStrings::GetConvStrInst()->UnicodeStringToAnsiString(fpInst->sFileInfoInst.sFile_CompanybyName);
		std::string stMySQLST_Select("SELECT * FROM tablename WHERE field = (?);");
		stMySQLST_Select.replace(stMySQLST_Select.find("tablename"), std::string("tablename").length(), stMySQLTableCompanies.stMySQLTable_name);
		stMySQLST_Select.replace(stMySQLST_Select.find("field"), std::string("field").length(), stMySQLTableCompanies.stMySQLTable_fields.at(0));
		pstmtGet.reset(con->prepareStatement(stMySQLST_Select));// extensions_bak extensions
		pstmtGet->setString(1, sFile_CompanyTMP);
		resGet.reset(pstmtGet->executeQuery());
		pstmtGet->close();
		while (resGet->next())
		{
			bStatus = TRUE;
			iCompanyID = resGet->getInt(1);
		};
		resGet.reset();
		pstmtGet.reset();

		dwErrorCode = GetLastError();
		throw dwErrorCode;
	}
	catch (sql::SQLException &e)
	{
		Logger::GetLogInstance()->PrepareMySQLLOG("Error SELECT id FROM TABLES", "Function->DBGetCompany(SELECT * FROM TABLES)", e.what(), e.getErrorCode(), e.getSQLState());
		Logger::GetLogInstance()->PrepareTXTLOG("Function->DBGetCompany(SELECT * FROM TABLE) -> MySQL error: ", e.what(), "MySQL error code: ", e.getErrorCode(), "SQLState: ", e.getSQLState());
	}
	catch (DWORD Err_code)
	{
		switch (Err_code)
		{
		case ERROR_SUCCESS:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->DBGetCompany(SELECT * FROM TABLE): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", fpInst->sFileInfoInst.sFileName);
			break;
		default:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->DBWriteFiles(SELECT id FROM TABLES): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", fpInst->sFileInfoInst.sFileName);
			Logger::GetLogInstance()->PrepareMySQLLOG("Error in getting file attributes", "Function->DBWriteFiles(SELECT id FROM TABLES)", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCode, fpInst->sFileInfoInst.sFileName);
			ErrorHandle::GetErrorHandleInst()->ErrorExit(_T("IterObjects->DBWriteFiles(SELECT id FROM TABLES)"), (LPTSTR)&fpInst->sFileInfoInst.sFileName, dwErrorCode);
		}
	}
};
DWORD DBProcess::DBGetRole(FileProcess *fpInst, BOOL &bStatus)
{
	DWORD dwErrorCode = -1;
	std::shared_ptr<sql::PreparedStatement> pstmtGet;
	std::shared_ptr<sql::ResultSet> resGet;
	int Result = 0;
	int iRoleID = 0;

	// Get from portal.files
	try
	{
		std::string sFile_RoleTMP = ConvertStrings::GetConvStrInst()->UnicodeStringToAnsiString(fpInst->sFileInfoInst.sFile_RolebyName);
		std::string stMySQLST_Select("SELECT * FROM tablename WHERE field = (?);");
		stMySQLST_Select.replace(stMySQLST_Select.find("tablename"), std::string("tablename").length(), stMySQLTableRole.stMySQLTable_name);
		stMySQLST_Select.replace(stMySQLST_Select.find("field"), std::string("field").length(), stMySQLTableRole.stMySQLTable_fields.at(0));
		pstmtGet.reset(con->prepareStatement(stMySQLST_Select));// extensions_bak extensions
		pstmtGet->setString(1, sFile_RoleTMP);
		resGet.reset(pstmtGet->executeQuery());
		pstmtGet->close();
		while (resGet->next())
		{
			bStatus = TRUE;
			iRoleID = resGet->getInt(1);
		};
		resGet.reset();
		pstmtGet.reset();

		dwErrorCode = GetLastError();
		throw dwErrorCode;
	}
	catch (sql::SQLException &e)
	{
		Logger::GetLogInstance()->PrepareMySQLLOG("Error SELECT id FROM TABLES", "Function->DBGetRole(SELECT * FROM TABLES)", e.what(), e.getErrorCode(), e.getSQLState());
		Logger::GetLogInstance()->PrepareTXTLOG("Function->DBGetRole(SELECT * FROM TABLE) -> MySQL error: ", e.what(), "MySQL error code: ", e.getErrorCode(), "SQLState: ", e.getSQLState());
	}
	catch (DWORD Err_code)
	{
		switch (Err_code)
		{
		case ERROR_SUCCESS:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->DBGetRole(SELECT * FROM TABLE): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", fpInst->sFileInfoInst.sFileName);
			break;
		default:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->ChkRoleDB(SELECT * FROM TABLE): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", fpInst->sFileInfoInst.sFileName);
			Logger::GetLogInstance()->PrepareMySQLLOG("Error in getting file attributes", "Function->ChkRoleDB(SELECT * FROM TABLE)", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCode, fpInst->sFileInfoInst.sFileName);
			ErrorHandle::GetErrorHandleInst()->ErrorExit(_T("IterObjects->ChkRoleDB(SELECT * FROM TABLE)"), (LPTSTR)&fpInst->sFileInfoInst.sFileName, dwErrorCode);
		}
	}
};
DWORD DBProcess::DBGetStage(FileProcess *fpInst, BOOL &bStatus)
{
	DWORD dwErrorCode = -1;
	std::shared_ptr<sql::PreparedStatement> pstmtGet;
	std::shared_ptr<sql::ResultSet> resGet;
	int Result = 0;
	int iStageID = 0;

	// Get from portal.files
	try
	{
		std::string sFile_StageTMP = ConvertStrings::GetConvStrInst()->UnicodeStringToAnsiString(fpInst->sFileInfoInst.sFile_ProjectStageforDB);
		std::string stMySQLST_Select("SELECT * FROM tablename WHERE field = (?);");
		stMySQLST_Select.replace(stMySQLST_Select.find("tablename"), std::string("tablename").length(), stMySQLTableProjects.stMySQLTable_name);
		stMySQLST_Select.replace(stMySQLST_Select.find("field"), std::string("field").length(), stMySQLTableProjects.stMySQLTable_fields.at(0));
		pstmtGet.reset(con->prepareStatement(stMySQLST_Select));// extensions_bak extensions
		pstmtGet->setString(1, sFile_StageTMP);
		resGet.reset(pstmtGet->executeQuery());
		pstmtGet->close();
		while (resGet->next())
		{
			bStatus = TRUE;
			iStageID = resGet->getInt(1);
		};
		resGet.reset();
		pstmtGet.reset();

		dwErrorCode = GetLastError();
		throw dwErrorCode;
	}
	catch (sql::SQLException &e)
	{
		Logger::GetLogInstance()->PrepareMySQLLOG("Error SELECT * FROM TABLE", "Function->DBGetStage(SELECT * FROM TABLE)", e.what(), e.getErrorCode(), e.getSQLState());
		Logger::GetLogInstance()->PrepareTXTLOG("Function->DBGetStage(SELECT * FROM TABLE) -> MySQL error: ", e.what(), "MySQL error code: ", e.getErrorCode(), "SQLState: ", e.getSQLState());
	}
	catch (DWORD Err_code)
	{
		switch (Err_code)
		{
		case ERROR_SUCCESS:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->DBGetStage(SELECT * FROM TABLE): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", fpInst->sFileInfoInst.sFileName);
			break;
		default:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->DBGetStage(SELECT * FROM TABLE): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", fpInst->sFileInfoInst.sFileName);
			Logger::GetLogInstance()->PrepareMySQLLOG("Error in getting file attributes", "Function->DBGetStage(SELECT * FROM TABLE)", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCode, fpInst->sFileInfoInst.sFileName);
			ErrorHandle::GetErrorHandleInst()->ErrorExit(_T("IterObjects->DBWriteFiles(SELECT * FROM TABLE)"), (LPTSTR)&fpInst->sFileInfoInst.sFileName, dwErrorCode);
		}
	}
};
DWORD DBProcess::DBGetSubsystem(FileProcess *fpInst, BOOL &bStatus)
{
	DWORD dwErrorCode = -1;
	std::shared_ptr<sql::PreparedStatement> pstmtGet;
	std::shared_ptr<sql::ResultSet> resGet;
	int Result = 0;
	int iSubsystemID = 0;

	// Get from portal.files
	try
	{
		std::string sFile_SubsystemTMP = ConvertStrings::GetConvStrInst()->UnicodeStringToAnsiString(fpInst->sFileInfoInst.sFile_SubsystembyName);
		std::string stMySQLST_Select("SELECT * FROM tablename WHERE field = (?);");
		stMySQLST_Select.replace(stMySQLST_Select.find("tablename"), std::string("tablename").length(), stMySQLTableSubsystem.stMySQLTable_name);
		stMySQLST_Select.replace(stMySQLST_Select.find("field"), std::string("field").length(), stMySQLTableSubsystem.stMySQLTable_fields.at(0));
		pstmtGet.reset(con->prepareStatement(stMySQLST_Select));// extensions_bak extensions
		pstmtGet->setString(1, sFile_SubsystemTMP);
		resGet.reset(pstmtGet->executeQuery());
		pstmtGet->close();
		while (resGet->next())
		{
			bStatus = TRUE;
			iSubsystemID = resGet->getInt(1);
		};
		resGet.reset();
		pstmtGet.reset();

		dwErrorCode = GetLastError();
		throw dwErrorCode;
	}
	catch (sql::SQLException &e)
	{
		Logger::GetLogInstance()->PrepareMySQLLOG("Error SELECT id FROM TABLES", "Function->DBWriteFiles(SELECT id FROM TABLES)", e.what(), e.getErrorCode(), e.getSQLState());
		Logger::GetLogInstance()->PrepareTXTLOG("Function->DBWriteFiles(SELECT id FROM TABLES) -> MySQL error: ", e.what(), "MySQL error code: ", e.getErrorCode(), "SQLState: ", e.getSQLState());
	}
	catch (DWORD Err_code)
	{
		switch (Err_code)
		{
		case ERROR_SUCCESS:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->DBWriteFiles(SELECT id FROM TABLES): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", fpInst->sFileInfoInst.sFileName);
			break;
		default:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->DBWriteFiles(SELECT id FROM TABLES): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", fpInst->sFileInfoInst.sFileName);
			Logger::GetLogInstance()->PrepareMySQLLOG("Error in getting file attributes", "Function->DBWriteFiles(SELECT id FROM TABLES)", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCode, fpInst->sFileInfoInst.sFileName);
			ErrorHandle::GetErrorHandleInst()->ErrorExit(_T("IterObjects->DBWriteFiles(SELECT id FROM TABLES)"), (LPTSTR)&fpInst->sFileInfoInst.sFileName, dwErrorCode);
		}
	}
};
DWORD DBProcess::DBGetDatatype(FileProcess *fpInst, BOOL &bStatus)
{
	DWORD dwErrorCode = -1;
	std::shared_ptr<sql::PreparedStatement> pstmtGet;
	std::shared_ptr<sql::ResultSet> resGet;
	int Result = 0;
	int iDatatypeID = 0;

	// Get from portal.files
	try
	{
		std::string sFile_DatatypeTMP = ConvertStrings::GetConvStrInst()->UnicodeStringToAnsiString(fpInst->sFileInfoInst.sFile_DatatypebyName);
		std::string stMySQLST_Select("SELECT * FROM tablename WHERE field = (?);");
		stMySQLST_Select.replace(stMySQLST_Select.find("tablename"), std::string("tablename").length(), stMySQLTableDatatype.stMySQLTable_name);
		stMySQLST_Select.replace(stMySQLST_Select.find("field"), std::string("field").length(), stMySQLTableDatatype.stMySQLTable_fields.at(0));
		pstmtGet.reset(con->prepareStatement(stMySQLST_Select));// extensions_bak extensions
		pstmtGet->setString(1, sFile_DatatypeTMP);
		resGet.reset(pstmtGet->executeQuery());
		pstmtGet->close();
		while (resGet->next())
		{
			bStatus = TRUE;
			iDatatypeID = resGet->getInt(1);
		};
		resGet.reset();
		pstmtGet.reset();

		dwErrorCode = GetLastError();
		throw dwErrorCode;
	}
	catch (sql::SQLException &e)
	{
		Logger::GetLogInstance()->PrepareMySQLLOG("Error SELECT id FROM TABLES", "Function->DBWriteFiles(SELECT id FROM TABLES)", e.what(), e.getErrorCode(), e.getSQLState());
		Logger::GetLogInstance()->PrepareTXTLOG("Function->DBWriteFiles(SELECT id FROM TABLES) -> MySQL error: ", e.what(), "MySQL error code: ", e.getErrorCode(), "SQLState: ", e.getSQLState());
	}
	catch (DWORD Err_code)
	{
		switch (Err_code)
		{
		case ERROR_SUCCESS:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->DBWriteFiles(SELECT id FROM TABLES): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", fpInst->sFileInfoInst.sFileName);
			break;
		default:
			Logger::GetLogInstance()->PrepareTXTLOG("Function->DBWriteFiles(SELECT id FROM TABLES): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", fpInst->sFileInfoInst.sFileName);
			Logger::GetLogInstance()->PrepareMySQLLOG("Error in getting file attributes", "Function->DBWriteFiles(SELECT id FROM TABLES)", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), dwErrorCode, fpInst->sFileInfoInst.sFileName);
			ErrorHandle::GetErrorHandleInst()->ErrorExit(_T("IterObjects->DBWriteFiles(SELECT id FROM TABLES)"), (LPTSTR)&fpInst->sFileInfoInst.sFileName, dwErrorCode);
		}
	}
};
DWORD DBProcess::DBCloseMYSQLConnection()
{
	//Here shared_ptr auto delete;
	con.reset();

	DWORD dwErrorCode = GetLastError();
	return dwErrorCode;
};