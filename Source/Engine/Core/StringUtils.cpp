#include "StringUtils.h"  
#include <iostream>  

namespace nc {  

	std::string StringUtils::ToUpper(std::string stringy)  
	{
		std::string result;  // Create an empty string for the result.
		for (char& c : stringy) {  // Iterate through each character in the input string.
			c = std::toupper(c);  // Convert the character to uppercase.
			result += c;  // Append the uppercase character to the result string.
		}
		return result;  
	}

	std::string StringUtils::ToLower(std::string stringy)  
	{
		std::string result;  // Create an empty string for the result.
		for (char& c : stringy) {  // Iterate through each character in the input string.
			c = std::tolower(c);  // Convert the character to lowercase.
			result += c;  // Append the lowercase character to the result string.
		};
		return result;  
	}

	bool StringUtils::IsEqualIgnoreCase(std::string stringy1, std::string stringy2)  
	{
		std::string result = ToLower(stringy1);  // Convert the first string to lowercase.
		std::string result2 = ToLower(stringy2);  // Convert the second string to lowercase.
		if (result == result2) {  // Compare the resulting lowercase strings.
			return true;  // The strings are equal when case is ignored.
		}
		else {

			return false; 
		}
	}

	std::string StringUtils::CreateUnique(const std::string& str)
	{
		static uint32_t unique = 0;  // Create a static counter to keep track of the integer.
		return str + std::to_string(unique++);  // Append the incremented integer to the input string and return the result.
	}
}