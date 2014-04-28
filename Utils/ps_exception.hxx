/*******************************************************************************
File Name  : ps_tcexception.h
Type       : Teamcenter exception type class.

Author     : Mattias Jonsson

Change History

Who  | Date       | Description
=======|============|=========================================================
MJ   | 22/01/2013 | Initial
******************************************************************************/

#pragma once

#include <exception>
#include <string>

using namespace std;

namespace ps
{

	class tcexception: public exception
	{
	private:
		int			_stat;
		string		_what;

	public:
		tcexception(const string& what) { _what = what; }
		tcexception(int stat, const string& what) { _stat = stat; _what = what; }
		~tcexception() { }
		virtual const char* what() const throw() { return _what.c_str(); }
		bool ifstat(int stat) {	if (_stat == stat) return true;	return false; }
		int getstat() { return _stat; }
	};

	class psexception: public exception
	{
	private:
		string		_what;

	public:
		psexception(const string& what) { _what = what; }
		~psexception() { }
		virtual const char* what() const throw() { return _what.c_str(); }
	};

}