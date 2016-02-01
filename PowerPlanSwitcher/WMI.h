#pragma once
class WMI
{
public:
	static list<tuple<wstring, wstring, bool>> execute(const wstring& root, const wstring& query, const tuple<const wstring&, const wstring&, const wstring&>& names);
	static bool call(const wstring& root, const wstring& query, const wstring& method);
};

