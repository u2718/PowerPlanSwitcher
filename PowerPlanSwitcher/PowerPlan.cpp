#include "stdafx.h"
#include "PowerPlan.h"
#include "WMI.h"

map<wstring, shared_ptr<PowerPlan>> PowerPlan::instances_;

PowerPlan::PowerPlan(const wstring & name, const wstring & id, const bool is_active)
{
	name_ = name;
	id_ = id;
	is_active_ = is_active;

	instances_[name] = make_shared<PowerPlan>(*this);
}

void PowerPlan::init()
{
	auto power_plans = WMI::execute(L"ROOT\\CIMV2\\power", L"SELECT * FROM Win32_PowerPlan", make_tuple(wstring(L"ElementName"), wstring(L"InstanceID"), wstring(L"IsActive")));
	list<PowerPlan> result;
	for each (auto plan in power_plans)
	{
		instances_[std::get<0>(plan)] = make_shared<PowerPlan>(PowerPlan(std::get<0>(plan), std::get<1>(plan), std::get<2>(plan)));
	}
}

PowerPlan::~PowerPlan()
{
}

bool PowerPlan::activate()
{
	for_each(instances_.begin(), instances_.end(), [](pair<wstring, shared_ptr<PowerPlan>> val) {val.second->is_active_ = false; });

	auto query_id = id_.substr(0, id_.find('\\')) + L"\\\\" + id_.substr(id_.find('\\') + 1, id_.length() - id_.find('\\') - 1);
	is_active_ = WMI::call(L"ROOT\\CIMV2\\power", L"SELECT * FROM Win32_PowerPlan WHERE InstanceID='" + query_id + L"'", L"Activate");
	return is_active_;
}

list<shared_ptr<PowerPlan>> PowerPlan::get_list()
{
	if (instances_.empty())
	{
		init();
	}

	list<shared_ptr<PowerPlan>> result;
	transform(instances_.begin(), instances_.end(), back_inserter(result), [](pair<wstring, shared_ptr<PowerPlan>> val) {return val.second; });
	return result;
}

shared_ptr<PowerPlan> PowerPlan::get(const wstring & name)
{
	if (instances_.empty())
	{
		init();
	}

	return instances_[name];
}

bool PowerPlan::is_active()
{
	return is_active_;
}

wstring PowerPlan::get_name()
{
	return name_;
}
