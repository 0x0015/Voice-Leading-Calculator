#include "ChordTable.hpp"
#include "ChordTable.h"
#include <algorithm>

void chordTable::load(){
	std::string csv(reinterpret_cast<char*>(ChordTable), ChordTable_len);
	table = CSV::fromString(csv);
	for(int i=0;i<table.size();i++){
		if(table[i].size() != 4){
			std::cout<<"Malformed entry... line: "<<i<<std::endl;
		}
	}
}

std::string strip(std::string in);

bool isSwizzle(const std::string& str1, const std::string& str2){
	std::string tempStr = str1;
	for(int i=0;i<str1.length();i++){
		if(tempStr == str2){
			return(true);
		}
		tempStr = tempStr.substr(1, tempStr.length()-1) + tempStr[0];
	}
	return(false);
}

std::optional<std::pair<std::string, std::string>> chordTable::lookup(const std::vector<uint8_t>& intervals){
	std::string lookupString;
	for(int i=0;i<intervals.size();i++){
		switch(intervals[i]){//could be optimized(askii codes, etc...), but good enough
		case 0:
			lookupString += "0";
			break;
		case 1:
			lookupString += "1";
			break;
		case 2:
			lookupString += "2";
			break;
		case 3:
			lookupString += "3";
			break;
		case 4:
			lookupString += "4";
			break;
		case 5:
			lookupString += "5";
			break;
		case 6:
			lookupString += "6";
			break;
		case 7:
			lookupString += "7";
			break;
		case 8:
			lookupString += "8";
			break;
		case 9:
			lookupString += "9";
			break;
		case 10:
			lookupString += "A";
			break;
		case 11:
			lookupString += "B";
			break;
		case 12:
			lookupString += "C";
			break;
		}
	}
	std::cout<<"Lookup string: "<<lookupString<<std::endl;
	for(auto& entry : table){
		if(entry.size() != 4){
			continue;
		}
		if(std::stoi(entry[0]) == intervals.size()){
			if(isSwizzle(entry[1], lookupString)){
				std::pair<std::string, std::string> output = std::make_pair(entry[2], entry[3]);
				return(output);
			}
		}
	}
	return(std::nullopt);
}


std::vector<uint8_t> chordTable::getIntervals(const std::vector<uint8_t>& c){
	std::vector<uint8_t> chord;
	for(int i=0;i<c.size();i++){
		chord.push_back(c[i]%12);
	}
	std::sort(chord.begin(), chord.end());
	std::vector<uint8_t> output;
	if(chord.size() < 2){
		return(output);
	}
	uint8_t lastVal = chord[0];
	for(int i=1;i<chord.size();i++){
		uint8_t difference = (uint8_t)(std::abs((int)chord[i]-(int)lastVal));
		if(difference != 0){
			output.push_back(difference);
		}
		lastVal = chord[i];
	}
	uint8_t sum = 0;
	for(int i=0;i<output.size();i++){
		sum += output[i];
	}
	output.push_back((uint8_t)(std::abs(12-(int)sum)));
	return(output);
}
