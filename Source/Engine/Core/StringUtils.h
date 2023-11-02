#pragma once
#include <string>

namespace nc {
	class StringUtils {
	public:
		//Initialize all the functions
		static std::string ToUpper(std::string stringy);
		static std::string ToLower(std::string stringy);
		static bool IsEqualIgnoreCase(std::string stringy1, std::string stringy2);
		static std::string CreateUnique(const std::string& str);
	};
}