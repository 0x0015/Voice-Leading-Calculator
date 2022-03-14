#pragma once
#include <iostream>
#include <vector>

class CSV{
public:
	static std::vector<std::vector<std::string>> fromString(const std::string& src);
};
