// CarPi - Raspberry Pi car entertainment system
// Copyright (c) 2013, David H. Hovemeyer <david.hovemeyer@gmail.com>

// This file is part of CarPi.
// 
//     CarPi is free software: you can redistribute it and/or modify
//     it under the terms of the GNU General Public License as published by
//     the Free Software Foundation, either version 3 of the License, or
//     (at your option) any later version.
// 
//     CarPi is distributed in the hope that it will be useful,
//     but WITHOUT ANY WARRANTY; without even the implied warranty of
//     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//     GNU General Public License for more details.
// 
//     You should have received a copy of the GNU General Public License
//     along with CarPi.  If not, see <http://www.gnu.org/licenses/>.

#ifndef STRING_UTIL_H
#define STRING_UTIL_H

#include <cctype>
#include <string>
#include <algorithm>

namespace StringUtil {
	inline bool endsWith(const std::string &s, const std::string &suffix)
	{
		// See: http://stackoverflow.com/questions/874134/find-if-string-endswith-another-string-in-c
		if (s.size() < suffix.size()) { return false; }
		return std::mismatch(suffix.rbegin(), suffix.rend(), s.rbegin()).first == suffix.rend();
	}
	
	inline bool startsWith(const std::string &s, const std::string &prefix)
	{
		// Same idea as endsWith
		if (s.size() < prefix.size()) { return false; }
		return std::mismatch(prefix.begin(), prefix.end(), s.begin()).first == prefix.end();
	}
	
	inline std::string trimToSize(const std::string &s, size_t n)
	{
		return (s.size() <= n) ? s : s.substr(0, n);
	}
	
	inline std::string padWithSpaces(const std::string &s, size_t n)
	{
		std::string result;
		if (result.size() > n) {
			result = trimToSize(s, n);
		} else {
			result = s;
		}
		while (result.size() < n) {
			result += ' ';
		}
		return result;
	}
	
	// Trim leading and trailing space characters from given string
	inline std::string trimSpaces(const std::string &s)
	{
		if (s.empty()) { return s; }
		
		// Skip leading space characters
		size_t n = 0;
		while (n < s.size() && isspace(s[n])) {
			n++;
		}
		
		// Skip trailing space characters
		size_t e = s.size();
		while (e > n && isspace(s[e-1])) {
			e--;
		}
		
		return s.substr(n, s.size() - (n+(s.size()-e)));
	}
};

#endif // STRING_UTIL_H