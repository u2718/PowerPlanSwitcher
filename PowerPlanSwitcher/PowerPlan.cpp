#include "stdafx.h"
#include "PowerPlan.h"
#include "WMI.h"

PowerPlan::PowerPlan()
{
}


PowerPlan::~PowerPlan()
{
}

list<wstring> PowerPlan::getList()
{
	return WMI::execute(L"ROOT\\CIMV2\\power", L"SELECT * FROM Win32_PowerPlan", L"InstanceID");;
}
