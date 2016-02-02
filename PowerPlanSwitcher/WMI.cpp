#include "stdafx.h"
#include "WMI.h"

bool WMI::is_security_initialized_ = false;

void WMI::init_security()
{
	auto hres = CoInitializeSecurity(
		NULL,
		-1,                          // COM authentication
		NULL,                        // Authentication services
		NULL,                        // Reserved
		RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication 
		RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation  
		NULL,                        // Authentication info
		EOAC_NONE,                   // Additional capabilities 
		NULL                         // Reserved
		);


	if (FAILED(hres))
	{
		CoUninitialize();
		throw exception();           // Program has failed.
	}

	is_security_initialized_ = true;
}

//https://msdn.microsoft.com/ru-ru/library/windows/desktop/aa390423%28v=vs.85%29.asp
list<tuple<wstring, wstring, bool>> WMI::execute(const wstring& root, const wstring& query, const tuple<const wstring&, const wstring&, const wstring&>& names)
{
	HRESULT hres;
	// Step 1: --------------------------------------------------
	// Initialize COM. ------------------------------------------

	hres = CoInitializeEx(0, COINIT_MULTITHREADED);
	if (FAILED(hres))
	{
		throw exception();                  // Program has failed.
	}

	// Step 2: --------------------------------------------------
	// Set general COM security levels --------------------------

	if (!is_security_initialized_)
	{
		init_security();
	}

	// Step 3: ---------------------------------------------------
	// Obtain the initial locator to WMI -------------------------

	IWbemLocator *pLoc = NULL;

	hres = CoCreateInstance(
		CLSID_WbemLocator,
		0,
		CLSCTX_INPROC_SERVER,
		IID_IWbemLocator, (LPVOID *)&pLoc);

	if (FAILED(hres))
	{
		CoUninitialize();
		throw exception();         // Program has failed.
	}

	// Step 4: -----------------------------------------------------
	// Connect to WMI through the IWbemLocator::ConnectServer method

	IWbemServices *pSvc = NULL;

	// Connect to the root\cimv2 namespace with
	// the current user and obtain pointer pSvc
	// to make IWbemServices calls.
	hres = pLoc->ConnectServer(
		_bstr_t(root.c_str()), // Object path of WMI namespace
		NULL,                    // User name. NULL = current user
		NULL,                    // User password. NULL = current
		0,                       // Locale. NULL indicates current
		NULL,                    // Security flags.
		0,                       // Authority (for example, Kerberos)
		0,                       // Context object 
		&pSvc                    // pointer to IWbemServices proxy
		);

	if (FAILED(hres))
	{
		pLoc->Release();
		CoUninitialize();
		throw exception();                // Program has failed.
	}


	// Step 5: --------------------------------------------------
	// Set security levels on the proxy -------------------------

	hres = CoSetProxyBlanket(
		pSvc,                        // Indicates the proxy to set
		RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx
		RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
		NULL,                        // Server principal name 
		RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx 
		RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
		NULL,                        // client identity
		EOAC_NONE                    // proxy capabilities 
		);

	if (FAILED(hres))
	{
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		throw exception();           // Program has failed.
	}

	// Step 6: --------------------------------------------------
	// Use the IWbemServices pointer to make requests of WMI ----

	// For example, get the name of the operating system
	IEnumWbemClassObject* pEnumerator = NULL;
	hres = pSvc->ExecQuery(
		bstr_t("WQL"),
		bstr_t(query.c_str()),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
		NULL,
		&pEnumerator);

	if (FAILED(hres))
	{
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		throw exception();               // Program has failed.
	}

	// Step 7: -------------------------------------------------
	// Get the data from the query in step 6 -------------------

	IWbemClassObject *pclsObj = NULL;
	ULONG uReturn = 0;

	list<tuple<wstring, wstring, bool>> result;
	while (pEnumerator)
	{
		HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1,
			&pclsObj, &uReturn);

		if (0 == uReturn)
		{
			break;
		}

		VARIANT vtProp;

		hr = pclsObj->Get(get<0>(names).c_str(), 0, &vtProp, 0, 0);
		auto t0 = wstring(vtProp.bstrVal, SysStringLen(vtProp.bstrVal));
		hr = pclsObj->Get(get<1>(names).c_str(), 0, &vtProp, 0, 0);
		auto t1 = wstring(vtProp.bstrVal, SysStringLen(vtProp.bstrVal));
		hr = pclsObj->Get(get<2>(names).c_str(), 0, &vtProp, 0, 0);
		auto t2 = vtProp.boolVal;

		result.push_back(make_tuple(t0, t1, t2));
		VariantClear(&vtProp);

		pclsObj->Release();
	}

	// Cleanup
	// ========

	pSvc->Release();
	pLoc->Release();
	pEnumerator->Release();
	CoUninitialize();

	return result;
}

bool WMI::call(const wstring& root, const wstring& query, const wstring& method)
{
	HRESULT hres;

	// Step 1: --------------------------------------------------
	// Initialize COM. ------------------------------------------

	hres = CoInitializeEx(0, COINIT_MULTITHREADED);
	if (FAILED(hres))
	{
		throw exception();                   // Program has failed.
	}

	// Step 2: --------------------------------------------------
	// Set general COM security levels --------------------------
	if (!is_security_initialized_)
	{
		init_security();
	}

	// Step 3: ---------------------------------------------------
	// Obtain the initial locator to WMI -------------------------

	IWbemLocator *pLoc = NULL;

	hres = CoCreateInstance(
		CLSID_WbemLocator,
		0,
		CLSCTX_INPROC_SERVER,
		IID_IWbemLocator, (LPVOID *)&pLoc);

	if (FAILED(hres))
	{
		CoUninitialize();
		throw exception();             // Program has failed.
	}

	// Step 4: ---------------------------------------------------
	// Connect to WMI through the IWbemLocator::ConnectServer method

	IWbemServices *pSvc = NULL;

	// Connect to the local root\cimv2 namespace
	// and obtain pointer pSvc to make IWbemServices calls.
	hres = pLoc->ConnectServer(
		_bstr_t(root.c_str()),
		NULL,
		NULL,
		0,
		NULL,
		0,
		0,
		&pSvc
		);

	if (FAILED(hres))
	{
		pLoc->Release();
		CoUninitialize();
		throw exception();                 // Program has failed.
	}

	// Step 5: --------------------------------------------------
	// Set security levels for the proxy ------------------------

	hres = CoSetProxyBlanket(
		pSvc,                        // Indicates the proxy to set
		RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx 
		RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx 
		NULL,                        // Server principal name 
		RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx 
		RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
		NULL,                        // client identity
		EOAC_NONE                    // proxy capabilities 
		);

	if (FAILED(hres))
	{
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		throw exception();                // Program has failed.
	}

	// Step 6: --------------------------------------------------
	// Use the IWbemServices pointer to make requests of WMI ----

	BSTR MethodName = SysAllocString(method.c_str());

	IEnumWbemClassObject* pEnumerator = NULL;
	hres = pSvc->ExecQuery(
		bstr_t("WQL"),
		bstr_t(query.c_str()),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
		NULL,
		&pEnumerator);

	if (FAILED(hres))
	{
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		throw exception();               // Program has failed.
	}

	// Step 7: -------------------------------------------------
	// Get the data from the query in step 6 -------------------

	IWbemClassObject *pClass = NULL;
	ULONG uReturn = 0;

	bool result;
	while (pEnumerator)
	{
		HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1,
			&pClass, &uReturn);

		if (uReturn == 0)
		{
			break;
		}

		VARIANT vtPath;
		VariantInit(&vtPath);
		if (FAILED(pClass->Get(L"__Path", 0, &vtPath, NULL, NULL)))
		{
			pClass->Release();
		}

		IWbemClassObject* pResult = NULL;
		if (FAILED(hres = pSvc->ExecMethod(vtPath.bstrVal, MethodName, 0, NULL, NULL, &pResult, NULL)))
		{
			SysFreeString(MethodName);
			pClass->Release();
			pSvc->Release();
			pLoc->Release();
			CoUninitialize();
		}
		else
		{
			VARIANT vtRet;
			VariantInit(&vtRet);
			if (FAILED(hres = pResult->Get(L"ReturnValue", 0, &vtRet, NULL, 0)))
			{
				VariantClear(&vtRet);
				SysFreeString(MethodName);
				pClass->Release();
				pSvc->Release();
				pLoc->Release();
				CoUninitialize();
			}
			else
			{
				result = vtRet.boolVal;
				VariantClear(&vtRet);
				pResult->Release();
				break;
			}

			VariantClear(&vtRet);
			pResult->Release();
		}

		if (FAILED(hres))
		{
			SysFreeString(MethodName);
			pClass->Release();
			pSvc->Release();
			pLoc->Release();
			CoUninitialize();
			throw exception();               // Program has failed.
		}
	}

	// Clean up
	//--------------------------
	SysFreeString(MethodName);
	pClass->Release();
	pLoc->Release();
	pSvc->Release();
	CoUninitialize();
	return result;
}
