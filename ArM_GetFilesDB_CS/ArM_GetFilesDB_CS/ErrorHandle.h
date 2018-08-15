#pragma once
class ErrorHandle
{
private:
protected:
	ErrorHandle(const ErrorHandle&);                 // Prevent copy-construction
	ErrorHandle& operator=(const ErrorHandle&);      // Prevent assignment
public:
	ErrorHandle();
	~ErrorHandle();

	static ErrorHandle* GetErrorHandleInst();
	void ErrorExit(LPTSTR lpszFunction, LPTSTR lpszAddText, DWORD dwErrorCode);
	std::basic_string<TCHAR> ErrorHandle::GetErrorDescription(DWORD dwErrorCode);
};