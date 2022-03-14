#include "CSV.hpp"
#include <sstream>

std::vector<std::string> SplitString(std::string str,std::string delimeter){
    std::vector<std::string> splittedStrings = {};
    size_t pos = 0;

    while ((pos = str.find(delimeter)) != std::string::npos)
    {
        std::string token = str.substr(0, pos);
        if (token.length() > 0)
            splittedStrings.push_back(token);
        str.erase(0, pos + delimeter.length());
    }

    if (str.length() > 0)
        splittedStrings.push_back(str);
    return splittedStrings;
}

std::vector<std::vector<std::string>> CSV::fromString(const std::string& src){
	std::istringstream iss(src);
	std::vector<std::vector<std::string>> output;
	std::string line;
	while(std::getline(iss, line)){
		if(line[line.length()] == '\n'){
			line = line.substr(0, line.length()-1);
		}
		std::vector<std::string> values = SplitString(line, ",");
		output.push_back(values);
	}
	return(output);
}
