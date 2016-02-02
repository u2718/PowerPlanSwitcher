#pragma once
#include "PowerPlan.h"

class Switcher
{
private:
	const wstring idle_name = L"�������� �������";
	const wstring active_name = L"����������������";

	shared_ptr<PowerPlan> idle;
	shared_ptr<PowerPlan> active;
public:
	Switcher();
	~Switcher();
	void run();
private:
	DWORD getIdleTime();
};

