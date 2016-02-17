#include "stdafx.h"
#include "ProcessList.h"


ProcessList::ProcessList()
{
}


ProcessList::~ProcessList()
{
}

bool ProcessList::any_of_process_is_running(const list<wstring> & processes)
{
	auto running_processes = get_processes();

	return any_of(processes.cbegin(), processes.cend(),
		[&](auto process) { return find(running_processes.cbegin(), running_processes.cend(), process) != running_processes.cend(); });
}

list<wstring> ProcessList::get_processes()
{
	HANDLE hProcessSnap;
	HANDLE hProcess;
	PROCESSENTRY32 pe32;

	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		throw exception();
	}
	
	pe32.dwSize = sizeof(PROCESSENTRY32);
	if (!Process32First(hProcessSnap, &pe32))
	{
		CloseHandle(hProcessSnap);
		throw exception();
	}

	list<wstring> processes;
	do
	{
		processes.push_back(pe32.szExeFile);
	} while (Process32Next(hProcessSnap, &pe32));

	CloseHandle(hProcessSnap);
	return processes;
}
