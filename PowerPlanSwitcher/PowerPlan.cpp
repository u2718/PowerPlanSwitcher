#include "stdafx.h"
#include "PowerPlan.h"
#include "WMI.h"

PowerPlan::PowerPlan(const wstring& name, const wstring& id)
{
	name_ = name;
	id_ = id;
}


PowerPlan::~PowerPlan()
{
}

bool PowerPlan::activate()
{
	auto query_id = id_.substr(0, id_.find('\\')) + L"\\\\" + id_.substr(id_.find('\\') + 1, id_.length() - id_.find('\\') - 1);
	return WMI::call(L"ROOT\\CIMV2\\power", L"SELECT * FROM Win32_PowerPlan WHERE InstanceID='" + query_id + L"'", L"Activate");;
}

list<PowerPlan> PowerPlan::get_list()
{
	auto power_plans = WMI::execute(L"ROOT\\CIMV2\\power", L"SELECT * FROM Win32_PowerPlan", make_tuple(wstring(L"ElementName"), wstring(L"InstanceID")));
	list<PowerPlan> result;
	for each (auto plan in power_plans)
	{
		result.push_back(PowerPlan(get<0>(plan), get<1>(plan)));
	}
	return result;
}
