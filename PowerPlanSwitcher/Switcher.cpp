#include "stdafx.h"
#include "Switcher.h"

Switcher::Switcher()
{
	for each (auto plan in PowerPlan::get_list())
	{
		if (plan->get_name() == idle_name)
		{
			idle = plan;
		}
		else if (plan->get_name() == active_name)
		{
			active = plan;
		}
	}
}

Switcher::~Switcher()
{
}

void Switcher::run()
{
	while (true)
	{
		if (getIdleTime() > 10)
		{
			if (!idle->is_active())
			{
				idle->activate();
			}
		}
		else
		{
			if (!active->is_active())
			{
				active->activate();
			}
		}
		Sleep(5000);
	}
}

DWORD Switcher::getIdleTime()
{
	DWORD idleTime = 0;
	LASTINPUTINFO lastInputInfo;
	lastInputInfo.cbSize = sizeof(LASTINPUTINFO);
	lastInputInfo.dwTime = 0;
	DWORD envTicks = GetTickCount();

	if (GetLastInputInfo(&lastInputInfo))
	{
		idleTime = envTicks - lastInputInfo.dwTime;
	}

	return abs((int)idleTime) / 1000;
}
