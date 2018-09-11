#pragma once
class Logger
{
private:
	HANDLE hFile;
protected:
	Logger(const Logger&);                 // Prevent copy-construction
	Logger& operator=(const Logger&);      // Prevent assignment
public:
	Logger();
	~Logger();

	static  Logger* GetLogInstance();
	DWORD Logger::CreateTXTLog();
	DWORD Logger::CreateMySQLLog();
	DWORD Logger::PrepareTXTLOG(char* cAddString1, const std::string &sState1,				char* cAddString2, int iErrorCode, char* cAddString3, const std::string &sState2);
	DWORD Logger::PrepareTXTLOG(char* cAddString1, const std::basic_string<TCHAR> &sState1, char* cAddString2, int iErrorCode, char* cAddString3, const std::string &sState2);
	DWORD Logger::PrepareTXTLOG(char* cAddString1, const std::basic_string<TCHAR> &sState1, char* cAddString2, int iErrorCode, char* cAddString3, const std::basic_string<TCHAR> &sState2);
	DWORD Logger::PrepareMySQLLOG(std::string stSubject, std::string stFunc, std::string sState1, int iErrorCode, std::string sState2);
	DWORD Logger::PrepareMySQLLOG(std::string stSubject, std::string stFunc, std::string sState1, int iErrorCode, std::basic_string<TCHAR> sState2);
	DWORD Logger::PrepareMySQLLOG(std::string stSubject, std::string stFunc, std::basic_string<TCHAR> sState1, int iErrorCode, std::basic_string<TCHAR> sState2);
	DWORD Logger::WriteToTXTLog();
	DWORD Logger::WriteToMySQLLog();
	struct LogFileInfo
	{
		std::basic_string<TCHAR> sFileName;
		std::basic_string<TCHAR> sFileRoot;
		std::basic_string<TCHAR> sFilePath;
	} sLogFile;
	std::string sBuff;
	std::string stSubjectBuff;
};