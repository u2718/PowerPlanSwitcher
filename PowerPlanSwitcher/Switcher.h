#pragma once
#include "PowerPlan.h"
#include "ProcessList.h"

class Switcher
{
private:
	wstring idle_name_ = L"Экономия энергии";
	wstring active_name_ = L"Сбалансированная";

	unsigned int idle_time_;
	unsigned int check_delay_idle_;
	unsigned int check_delay_active_;
	unsigned int current_check_delay_;

	bool is_stopped_ = false;

	shared_ptr<PowerPlan> idle_;
	shared_ptr<PowerPlan> active_;

	list<wstring> exclusion_processes_;
public:
	Switcher();
	~Switcher();
	void run();
	void stop();

	wstring get_idle_name();
	void set_idle_name(const wstring &name);

	wstring get_active_name();
	void set_active_name(const wstring &name);

	unsigned int get_idle_time();
	void set_idle_time(unsigned int time);

	unsigned int get_check_delay_idle();
	void set_check_delay_idle(unsigned int delay);

	unsigned int get_check_delay_active();
	void set_check_delay_active(unsigned int delay);

	list<wstring> get_exclude_processes();
	void add_exclusion_process(const wstring &process_name);
	void remove_exclusion_process(const wstring &process_name);
private:
	DWORD get_last_input_time();
};

