// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define _CRTDBG_MAP_ALLOC  
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <locale>
#include <TimeApi.h>
#include "Shlwapi.h"
#include <stdio.h>
#include <sstream>
#include <strsafe.h>
#include <vector>
#include <algorithm>
#include <crtdbg.h>
#include <iostream>
#include <regex>
#include <string.h>
//#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

//#include <atlbase.h>
//#include <atlstr.h>

// TODO: reference additional headers your program requires here
#include "accctrl.h"
#include "aclapi.h"
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "User32.lib")
#pragma comment(lib, "netapi32.lib")

//#Include MySQL headers
#include <mysql_connection.h>
#include <mysql_driver.h>

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

#include "UI.h"
#include "FileProcess.h"
#include "DBProcess.h"
#include "Logger.h"
#include "ErrorHandle.h"
#include "ConvertStrings.h"

#include <assert.h>
#include <lm.h>

#define _SECOND ((_int64) 10000000)
#define _MINUTE (60 * _SECOND)
#define _HOUR   (60 * _MINUTE)
#define _DAY    (24 * _HOUR)
//#define _PERIOD ((_int64) 32)

// TODO: reference additional headers your program requires here
