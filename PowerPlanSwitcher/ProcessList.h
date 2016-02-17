#pragma once
class ProcessList
{
public:
	ProcessList();
	~ProcessList();

	static bool any_of_process_is_running(const list<wstring>& processes);
private:
	static list<wstring> get_processes();
};

