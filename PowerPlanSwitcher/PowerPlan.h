#pragma once
class PowerPlan
{
private:
	wstring name_;
	wstring id_;
public:
	PowerPlan(const wstring& name, const wstring& id);
	~PowerPlan();
	bool activate();
	static list<PowerPlan> get_list();
};

