#include "stdafx.h"
#include "Switcher.h"

Switcher::Switcher()
{
	idle_ = PowerPlan::get(idle_name_);
	active_ = PowerPlan::get(active_name_);
	set_check_delay_active(60);
	set_check_delay_idle(5);
	set_idle_time(300);
	current_check_delay_ = get_check_delay_active();
}

Switcher::~Switcher()
{
}

void Switcher::run()
{
	while (!is_stopped_)
	{
		if (get_last_input_time() > get_idle_time())
		{
			if (!idle_->is_active() && !ProcessList::any_of_process_is_running(exclusion_processes_))
			{
				idle_->activate();
				current_check_delay_ = get_check_delay_idle() * 1000;
			}
		}
		else
		{
			if (!active_->is_active())
			{
				active_->activate();
				current_check_delay_ = get_check_delay_active() * 1000;
			}
		}
		Sleep(current_check_delay_);
	}
}

void Switcher::stop()
{
	is_stopped_ = true;
}

wstring Switcher::get_idle_name()
{
	return idle_name_;
}

void Switcher::set_idle_name(const wstring & name)
{
	idle_name_ = name;
}

wstring Switcher::get_active_name()
{
	return active_name_;
}

void Switcher::set_active_name(const wstring & name)
{
	active_name_ = name;
}

unsigned int Switcher::get_idle_time()
{
	return idle_time_;
}

void Switcher::set_idle_time(unsigned int time)
{
	idle_time_ = time;
}

unsigned int Switcher::get_check_delay_idle()
{
	return check_delay_idle_;
}

void Switcher::set_check_delay_idle(unsigned int delay)
{
	check_delay_idle_ = delay;
}

unsigned int Switcher::get_check_delay_active()
{
	return check_delay_active_;
}

void Switcher::set_check_delay_active(unsigned int delay)
{
	check_delay_active_ = delay;
}

list<wstring> Switcher::get_exclude_processes()
{
	return exclusion_processes_;
}

void Switcher::add_exclusion_process(const wstring & process_name)
{
	exclusion_processes_.push_back(process_name);
}

void Switcher::remove_exclusion_process(const wstring & process_name)
{
	exclusion_processes_.remove(process_name);
}

DWORD Switcher::get_last_input_time()
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
