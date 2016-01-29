#include "stdafx.h"
#include "Switcher.h"


Switcher::Switcher()
{
}


Switcher::~Switcher()
{
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
