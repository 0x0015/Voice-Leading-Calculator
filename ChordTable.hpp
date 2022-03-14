#pragma once
#include <iostream>
#include <vector>
#include <optional>
#include "CSV.hpp"

class chordTable{
public:
	static inline std::vector<std::vector<std::string>> table;
	static void load();
	static std::optional<std::pair<std::string, std::string>> lookup(const std::vector<uint8_t>& intervals);
	static std::vector<uint8_t> getIntervals(const std::vector<uint8_t>& chord);
};
