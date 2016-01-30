#pragma once
class WMI
{
public:
	static list<wstring> execute(const wstring& root, const wstring& query, const wstring& name);
};

