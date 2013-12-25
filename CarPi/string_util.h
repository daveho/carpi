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

#include <string>
#include <algorithm>

namespace StringUtil {
	inline bool endsWith(const std::string &s, const std::string &suffix)
	{
		// See: http://stackoverflow.com/questions/874134/find-if-string-endswith-another-string-in-c
		return std::mismatch(suffix.rbegin(), suffix.rend(), s.rbegin()).first == suffix.rend();
	}
	
	inline std::string trimToSize(const std::string &s, size_t n)
	{
		return (s.size() <= n) ? s : s.substr(0, n);
	}
};

#endif // STRING_UTIL_H