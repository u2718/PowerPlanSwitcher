#pragma once
class PowerPlan
{
private:
	wstring name_;
	wstring id_;
	bool is_active_;
	static map<wstring, shared_ptr<PowerPlan>> instances_;
	
	PowerPlan(const wstring& name, const wstring& id, const bool is_active);
	static void init();
public:
	
	~PowerPlan();
	bool activate();
	static list<shared_ptr<PowerPlan>> get_list();
	static shared_ptr<PowerPlan> get(const wstring& name);
	bool is_active();
	wstring get_name();
};

