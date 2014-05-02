/*!
 * \file ps_exception.hxx
 * \date 2014/04/30 20:40
 *
 * \author Mattias Jonsson (jonssonm)
 * Contact: jonsson.mattias@siemens.com
 *
 * \brief Exception classes defined for handling Teamcenter server errors and general errors.
 *
*/

#pragma once

using namespace std;

namespace ps
{
	/*!
	 *  \brief Use in combination with ITK macro, which will throw this exception whenever an ITK error occurs.
	 *  
	 *  \sa ps_macro.hxx
	 */
	class tcexception: public exception
	{
	private:
		int			_stat;
		string		_what;

	public:
		//! Constructor for initializing exception with error message \a what.
		tcexception(const string& what) { _what = what; }
		//! Constructor for initializing exception with error code \a stat and error message \a what
		tcexception(int stat, const string& what) { _stat = stat; _what = what; }
		~tcexception() { }
		//! Returns a c-string containing the error message.
		virtual const char* what() const throw() { return _what.c_str(); }
		//! Checks if the error code equals \a stat.
		bool ifstat(int stat) {	if (_stat == stat) return true;	return false; }
		//! Returns the error code.
		int getstat() { return _stat; }
	};

	/*!
	 *  \brief General exception thrown when an error happens in user implemented ITK code.
	 *
	 */
	class psexception: public exception
	{
	private:
		string		_what;

	public:
		//! Constructor for initializing exception with error message \a what.
		psexception(const string& what) { _what = what; }
		~psexception() { }
		//! Returns a c-string containing the error message.
		virtual const char* what() const throw() { return _what.c_str(); }
	};

}