#pragma once
class DBProcess
{
private:
	DBProcess();
	~DBProcess();
	DBProcess(const DBProcess&);                 // Prevent copy-construction
	DBProcess& operator=(const DBProcess&);      // Prevent assignment
	struct MySQLConnection
	{
		std::string stTCIP;
		std::string stLogin;
		std::string stPassword;
		std::string stSchemaName;
	}MySQLConnectionInst;
	struct stMySQLTable
	{
		std::string stMySQLTable_name;
		std::vector<std::string> stMySQLTable_fields;
	}stMySQLTableErrors, stMySQLTableExtens, stMySQLTableFiles, stMySQLTableFolders, stMySQLTableUsers, stMySQLTableProjects, stMySQLTableCompanies, stMySQLTableRole, stMySQLTableSubsystem, stMySQLTableDatatype;
protected:

	sql::Driver *driver;
	std::shared_ptr<sql::Connection> con;
public:
	friend class FileProcess;
	friend class Logger;

	static  DBProcess* dbProcInstance	();
	DWORD DBCreateMYSQLConnection		();
	DWORD DBInitTables					();
	DWORD DBWriteFolders				(FileProcess *fpInst);
	DWORD DBWriteExtensions				(FileProcess *fpInst);
	DWORD DBWriteFiles					(FileProcess *fpInst);
	DWORD DBGetProjects					(FileProcess *fpInst, BOOL &bStatus);
	DWORD DBGetCompany					(FileProcess *fpInst, BOOL &bStatus);
	DWORD DBGetRole						(FileProcess *fpInst, BOOL &bStatus);
	DWORD DBGetStage					(FileProcess *fpInst, BOOL &bStatus);
	DWORD DBGetSubsystem				(FileProcess *fpInst, BOOL &bStatus);
	DWORD DBGetDatatype					(FileProcess *fpInst, BOOL &bStatus);
	DWORD DBCloseMYSQLConnection		();

};