// CarPi - Raspberry Pi car entertainment system
// Copyright (c) 2013,2014 David H. Hovemeyer <david.hovemeyer@gmail.com>

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

#include <cstdio>
#include <cctype>
#include <string>
#include <vector>
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
		std::string result(s);
		if (result.size() > n) {
			result = trimToSize(s, n);
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
	
	// Source:
	//   http://stackoverflow.com/questions/10051679/c-tokenize-string
	// I changed the function name and indentation style, but otherwise
	// it's copied verbatim.
	inline std::vector<std::string> tokenize(const std::string &source, const char *delimiter = " ", bool keepEmpty = false)
	{
		std::vector<std::string> results;

		size_t prev = 0;
		size_t next = 0;

		while ((next = source.find_first_of(delimiter, prev)) != std::string::npos) {
			if (keepEmpty || (next - prev != 0)) {
				results.push_back(source.substr(prev, next - prev));
			}
			prev = next + 1;
		}

		if (prev < source.size()) {
			results.push_back(source.substr(prev));
		}

		return results;
	}

	// Read a line of text from given file handle.
	// Result is assigned to the string parameter.
	// Returns false if EOF is reached without reading
	// any characters.
	inline bool readLine(FILE *f, std::string &line)
	{
		line.clear();
		for (;;) {
			int c = fgetc(f);
			if (c == EOF) { return !line.empty(); }
			if (c == '\n') { break; }
			line += char(c);
		}
		return true;
	}

	// Read one line of text from named file
	inline bool readOneLine(const std::string &fileName, std::string &line)
	{
		FILE *fh = fopen(fileName.c_str(), "r");
		if (!fh) {
			return false;
		}
		bool gotLine = readLine(fh, line);
		fclose(fh);
		return gotLine;
	}
};

#endif // STRING_UTIL_H
