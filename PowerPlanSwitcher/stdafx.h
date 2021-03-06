// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <tlhelp32.h>
#include <shellapi.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: reference additional headers your program requires here
#include <comdef.h>
#include <Wbemidl.h>
#include <string>
#include <list>
#include <tuple>
#include <thread>
#include <map>
#include <algorithm>
#include <iterator>
using namespace std;
# pragma comment(lib, "wbemuuid.lib")