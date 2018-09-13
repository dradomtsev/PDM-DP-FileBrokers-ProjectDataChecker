#pragma once
class ConvertStrings
{
private:
	ConvertStrings();
	~ConvertStrings();
	ConvertStrings(const ConvertStrings&);                 // Prevent copy-construction
	ConvertStrings& operator=(const ConvertStrings&);      // Prevent assignment
protected:
public:
	friend class Looger;
	friend class DBProcess;
	static ConvertStrings* GetConvStrInst();
	DWORD AnsiStringToUnicodeString(const std::string &stIn, std::basic_string<TCHAR> &wstOut);
	DWORD UnicodeStringToAnsiString(const std::basic_string<TCHAR> &wstIn, std::string &stOut);
	DWORD UnicodeVectorToAnsiVector(const std::vector<std::basic_string<TCHAR>> &wstIn, std::vector<std::string> &vstOut);
};