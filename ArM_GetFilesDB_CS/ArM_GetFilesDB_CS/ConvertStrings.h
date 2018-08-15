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
	std::basic_string<TCHAR> AnsiStringToUnicodeString(std::string stIn);
	std::string UnicodeStringToAnsiString(std::basic_string<TCHAR> wstIn);
	std::vector<std::string> UnicodeVectorToAnsiVector(std::vector<std::basic_string<TCHAR>> wstIn);
};