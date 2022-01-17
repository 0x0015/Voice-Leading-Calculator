#include <iostream>
#include <sstream>
#include "Score.hpp"
#include "letterConvert.hpp"
#include "ArguementHandler/ArgHandle.hpp"

std::vector<std::string> strSplit(std::string& base, char delim){
	std::vector<std::string> output;
	std::stringstream ss(base);
	std::string number;
	while(getline(ss,number, delim)){
		output.push_back(number);
	}
	return(output);
}

int main(int argc, char** argv){
	ArguementHandler args(argc, argv);
	std::vector<uint8_t> chord1;// = {noteFromLetter("G3"), noteFromLetter("B3")};
	std::vector<uint8_t> chord2;// = {noteFromLetter("G#3"), noteFromLetter("C5")};
	if(args.hasArg("-c1")){
		auto c1Arg = strSplit(args.findArgs("-c1")[0].value, ',');
		for(int i=0;i<c1Arg.size();i++){
			chord1.push_back(noteFromLetter(c1Arg[i]));
		}
	}
	if(args.hasArg("-c2")){
		auto c2Arg = strSplit(args.findArgs("-c2")[0].value, ',');
		for(int i=0;i<c2Arg.size();i++){
			chord2.push_back(noteFromLetter(c2Arg[i]));
		}
	}
	std::cout<<"Range score: "<<Score::scoreRanges(chord1, chord2)<<std::endl;
	std::vector<uint8_t> bestChord = Score::optimizeScore(chord1, chord2);
	std::cout<<"Best score: "<<Score::scoreRanges(chord1, bestChord)<<" with ";
	Score::printChord(bestChord);
	std::cout<<".  (chord1 = ";
	Score::printChord(chord1);
	std::cout<<")"<<std::endl;
	return(0);
}
