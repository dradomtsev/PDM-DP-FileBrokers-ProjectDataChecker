// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"

ConvertStrings::ConvertStrings() {}
ConvertStrings::~ConvertStrings() {}
ConvertStrings * ConvertStrings::GetConvStrInst()
{
	static ConvertStrings ConvStrInst;
	return &ConvStrInst;
};
DWORD AnsiStringToUnicodeString(const std::string &stIn, std::basic_string<TCHAR> &wstOut)
{
	//std::basic_string<TCHAR> wstOut;
	DWORD dwErrorCode = -1;
	SetLastError(ERROR_SUCCESS);
	setlocale(LC_ALL, "");
	try
	{
		std::size_t iCharSize = MultiByteToWideChar(CP_UTF8, WC_ERR_INVALID_CHARS, stIn.c_str(), -1, nullptr, 0);
		//std::size_t iCharSize = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, stIn.c_str(), -1, nullptr, 0, NULL, NULL);
		if (iCharSize == 0)
		{
			dwErrorCode = GetLastError();
			throw dwErrorCode;
		}
		else
		{
			wstOut.resize(iCharSize);
			iCharSize = MultiByteToWideChar(		CP_UTF8, WC_ERR_INVALID_CHARS, stIn.c_str(), -1, &wstOut[0], (int)iCharSize);
			//iCharSize = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, stIn.c_str(), -1, &wstOut[0], (int)iCharSize, NULL, NULL);
			if (iCharSize == 0)
			{
				dwErrorCode = GetLastError();
				throw dwErrorCode;
			}
			else
			{
				wstOut.resize(iCharSize - 1);
			}
			dwErrorCode = GetLastError();
			throw dwErrorCode;
		}

	}
	catch (DWORD& dwErrorCode)
	{
		switch (dwErrorCode)
		{
		case ERROR_SUCCESS:
			break;
		case ERROR_INSUFFICIENT_BUFFER:
		case ERROR_INVALID_FLAGS:
		case ERROR_INVALID_PARAMETER:
		case ERROR_NO_UNICODE_TRANSLATION:
		{
			Logger::GetLogInstance()->PrepareTXTLOG("Function->AnsiStringToUnicodeString(WideCharToMultiByte()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", stIn);
			ErrorHandle::GetErrorHandleInst()->ErrorExit(_T("AnsiStringToUnicodeString->WideCharToMultiByte()"), (LPTSTR)stIn.c_str(), dwErrorCode);
			break;
		}
		}
	}
	return dwErrorCode;
};
DWORD ConvertStrings::UnicodeStringToAnsiString(const std::basic_string<TCHAR> &wstIn, std::string &stOut)
{
	//std::string stOut;
	DWORD dwErrorCode = -1;
	SetLastError(ERROR_SUCCESS);
	setlocale(LC_ALL, "");
	try 
	{
		std::size_t iCharSize = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, wstIn.c_str(), -1, nullptr, 0, NULL, NULL);
		if (iCharSize == 0)
		{
			dwErrorCode = GetLastError();
			throw dwErrorCode;
		}
		else
		{
			stOut.resize(iCharSize);
			iCharSize = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, wstIn.c_str(), -1, &stOut[0], (int)iCharSize, NULL, NULL);
			if (iCharSize == 0)
			{
				dwErrorCode = GetLastError();
				throw dwErrorCode;
			}
			else
			{
				stOut.resize(iCharSize - 1);
			}
			dwErrorCode = GetLastError();
			throw dwErrorCode;
		}

	}
	catch (DWORD& dwErrorCode)
	{
		switch (dwErrorCode)
		{
			case ERROR_SUCCESS:
				break;
			case ERROR_INSUFFICIENT_BUFFER:
			case ERROR_INVALID_FLAGS:
			case ERROR_INVALID_PARAMETER:
			case ERROR_NO_UNICODE_TRANSLATION:
			{
				Logger::GetLogInstance()->PrepareTXTLOG("Function->UnicodeStringToAnsiString(WideCharToMultiByte()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", wstIn);
				ErrorHandle::GetErrorHandleInst()->ErrorExit(_T("UnicodeStringToAnsiString->WideCharToMultiByte()"), (LPTSTR)wstIn.c_str(), dwErrorCode);
				break;
			}
		}
	}
	return dwErrorCode;
};
DWORD ConvertStrings::UnicodeVectorToAnsiVector(const std::vector<std::basic_string<TCHAR>> &wstIn, std::vector<std::string> &vstOut)
{
	//std::vector<std::string> stOut(wstIn.size());
	vstOut.resize(wstIn.size());
	DWORD dwErrorCode = -1;
	SetLastError(ERROR_SUCCESS);
	std::size_t i = 0;
	try 
	{
		for (std::vector<std::basic_string<TCHAR>>::const_iterator it = wstIn.begin(); it != wstIn.end(); ++it, ++i)
		{
			std::size_t iCharSize = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, it->c_str(), -1, nullptr, 0, NULL, NULL);
			if (iCharSize == 0)
			{
				dwErrorCode = GetLastError();
				throw dwErrorCode;
			}
			else
			{
				//vstOut.at(i).assign(" ");
				vstOut.at(i).resize(iCharSize);
				iCharSize = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, it->c_str(), -1, &vstOut.at(i)[0], (int)iCharSize, NULL, NULL);
				if (iCharSize == 0)
				{
					dwErrorCode = GetLastError();
					throw dwErrorCode;
				}
				else
				{
					vstOut.at(i).resize(iCharSize - 1);
				}
			}
		}
		dwErrorCode = GetLastError();
		throw dwErrorCode;
	}
	catch (DWORD& dwErrorCode)
	{
		switch (dwErrorCode)
		{
			case ERROR_SUCCESS:
				break;
			case ERROR_INSUFFICIENT_BUFFER:
			case ERROR_INVALID_FLAGS:
			case ERROR_INVALID_PARAMETER:
			case ERROR_NO_UNICODE_TRANSLATION:
			{
				Logger::GetLogInstance()->PrepareTXTLOG("Function->UnicodeStringToAnsiString(WideCharToMultiByte()): ", ErrorHandle::GetErrorHandleInst()->GetErrorDescription(dwErrorCode), "Error code: ", dwErrorCode, "; Object: ", wstIn.at(i));
				ErrorHandle::GetErrorHandleInst()->ErrorExit(_T("UnicodeStringToAnsiString->WideCharToMultiByte()"), (LPTSTR)wstIn.at(i).c_str(), dwErrorCode);
				break;
			}
		}
	}
	return dwErrorCode;
};