// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"


ErrorHandle::ErrorHandle(){}
ErrorHandle::~ErrorHandle(){}
ErrorHandle * ErrorHandle::GetErrorHandleInst()
{
	static ErrorHandle ErrHandInst;
	return &ErrHandInst;
};
void ErrorHandle::ErrorExit(LPTSTR lpszFunction, LPTSTR lpszAddText, DWORD dwErrorCode)
{
	// Retrieve the system error message for the last-error code

	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	//DWORD dwErrorCode = GetLastError();
	HRESULT hsResult;
	std::size_t pszDest_Len, lpszFunctionTemp_Len, lpszAddTextTemp_Len, lpMsgBuf_Len;
	std::size_t isSize;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dwErrorCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);

	// Display the error message and exit the process
	LPTSTR lpszFunctionTemp = lpszFunction;
	LPTSTR lpszAddTextTemp = lpszAddText;
	
	TCHAR pszDest[] = _T(" ");
	hsResult = StringCchLength(pszDest, STRSAFE_MAX_CCH - 1, &pszDest_Len);
	hsResult = StringCchLength(lpszFunctionTemp, STRSAFE_MAX_CCH - 1, &lpszFunctionTemp_Len);
	isSize = pszDest_Len + lpszFunctionTemp_Len + 1;
	hsResult = StringCchCat(pszDest, isSize, lpszFunctionTemp);
	dwErrorCode = -1;
	dwErrorCode = GetLastError();

	hsResult = StringCchLength(pszDest, STRSAFE_MAX_CCH - 1, &pszDest_Len);
	hsResult = StringCchLength(lpszAddTextTemp, STRSAFE_MAX_CCH - 1, &lpszAddTextTemp_Len);
	isSize = pszDest_Len + lpszAddTextTemp_Len + 1;
	hsResult = StringCchCat(pszDest, isSize, lpszAddTextTemp);
	dwErrorCode = -1;
	dwErrorCode = GetLastError();

	hsResult = StringCchLength((LPTSTR)lpMsgBuf, STRSAFE_MAX_CCH - 1, &lpMsgBuf_Len);
	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, (lpMsgBuf_Len + lpszFunctionTemp_Len + lpszAddTextTemp_Len + 40) * sizeof(TCHAR));
	if (lpDisplayBuf != 0)
	{
		hsResult = StringCchPrintf((LPTSTR)lpDisplayBuf, LocalSize(lpDisplayBuf) / sizeof(TCHAR), TEXT("%s failed with error %u: %s"), pszDest, dwErrorCode, (LPTSTR)lpMsgBuf);
		MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);
	}
	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
	ExitProcess(dwErrorCode);
};
std::basic_string<TCHAR> ErrorHandle::GetErrorDescription(DWORD dwErrorCode)
{
	//DWORD dwErrorCode = GetLastError();
	LPTSTR messageBuffer = nullptr;
	
	size_t size = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dwErrorCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&messageBuffer,
		0, NULL);
	std::basic_string<TCHAR> lpMsgBuf(messageBuffer, size);
	if (!lpMsgBuf.empty())
		lpMsgBuf.erase(lpMsgBuf.size()-2,2);
	LocalFree(messageBuffer);
	return lpMsgBuf;
};