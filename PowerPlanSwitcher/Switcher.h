#pragma once
#include "PowerPlan.h"

class Switcher
{
private:
	const wstring idle_name = L"Экономия энергии";
	const wstring active_name = L"Сбалансированная";

	shared_ptr<PowerPlan> idle;
	shared_ptr<PowerPlan> active;
public:
	Switcher();
	~Switcher();
	void run();
private:
	DWORD getIdleTime();
};

