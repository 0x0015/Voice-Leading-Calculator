#include "Score.hpp"
#include "letterConvert.hpp"
#include <limits>
#include <optional>



std::vector<uint8_t> sortChord(std::vector<uint8_t>& chord){
	std::vector<uint8_t> output;
	while(output.size() < chord.size()){
		uint8_t noteToAdd = std::numeric_limits<uint8_t>::max();
		for(int i=0;i<chord.size();i++){
			if(output.size() == 0){
				if(chord[i] < noteToAdd){
					noteToAdd = chord[i];
				}
			}else{
				if(chord[i] < noteToAdd && chord[i] > output.back()){
					noteToAdd = chord[i];
				}
			}
		}
		output.push_back(noteToAdd);
	}
	if(output.size() != chord.size()){
		std::cout<<"Warning: sorted chord does not have the same number of notes as input"<<std::endl;
	}
	return(output);
}

unsigned int Score::scoreRanges(std::vector<uint8_t>& chord1, std::vector<uint8_t>& chord2){
	if(chord1.size() != chord2.size()){
		std::cout<<"comparing ranges of scores of unequal size"<<std::endl;
		return(0);
	}
	unsigned int output = 0;
	for(int i=0;i<chord2.size();i++){
		unsigned int chord2Min = std::numeric_limits<unsigned int>::max();
		for(int j=0;j<chord1.size();j++){
			unsigned int newDistence = (unsigned int)(std::abs(((int)chord1[j])-((int)chord2[i])));
			if(newDistence<chord2Min){
				chord2Min = newDistence;
			}
		}
		output+=chord2Min;
	}
	return(output);
}

unsigned int Score::scoreTwo(std::pair<uint8_t, uint8_t> pair1, std::pair<uint8_t, uint8_t> pair2){
	unsigned int output = 0;
	
	return(output);
}

unsigned int Score::scoreAll(std::vector<uint8_t>& chord1, std::vector<uint8_t>& chord2){	
	unsigned int output = scoreRanges(chord1, chord2);
	return(output);
}

void Score::printChord(std::vector<uint8_t>& chord){
	for(int i=0;i<chord.size();i++){
		std::cout<<letterFromNote(chord[i])<<"("<<std::to_string(chord[i])<<")";
		if(i+1<chord.size()){
			std::cout<<", ";
		}
	}
}

void Score::recursePossibleOctives(std::vector<std::vector<uint8_t>>& possibleOctives, std::vector<uint8_t>& chord1, int i, std::vector<uint8_t> accum, unsigned int& bestScore, std::vector<uint8_t>& bestChord, unsigned int& computeCount){
	if(i == possibleOctives.size()){
		//here accum is a possible combo
		unsigned int currentScore = scoreAll(chord1, accum);
		if(currentScore < bestScore){
			bestScore = currentScore;
			bestChord = accum;
			std::cout<<"New best score: "<<bestScore<<" with chord: ";
			printChord(bestChord);
			std::cout<<std::endl;
		}
		computeCount++;
	}else{
		std::vector<uint8_t> row = possibleOctives[i];
		for(int j=0;j<row.size();j++){
			std::vector<uint8_t> tmp(accum);
			tmp.push_back(row[j]);
			recursePossibleOctives(possibleOctives, chord1, i+1, tmp, bestScore, bestChord, computeCount);
		}
	}
}

uint8_t getHighestNote(std::vector<uint8_t>& chord){
	uint8_t output = 0;
	for(int i=0;i<chord.size();i++){
		if(output<chord[i]){
			output=chord[i];
		}
	}
	return(output);
}

uint8_t getLowestNote(std::vector<uint8_t>& chord){
	uint8_t output = std::numeric_limits<uint8_t>::max();
	for(int i=0;i<chord.size();i++){
		if(chord[i]<output){
			output=chord[i];
		}
	}
	return(output);
}

uint8_t clampToUI8(int note){
	if(note<0){
		return(0);
	}
	if(note>std::numeric_limits<uint8_t>::max()){
		return(std::numeric_limits<uint8_t>::max());
	}
	return((uint8_t)note);
}

std::vector<uint8_t> getOctives(uint8_t note, uint8_t max, uint8_t min){
	std::vector<uint8_t> output;
	unsigned int currentNote = (unsigned int)(note%12);
	while(currentNote < std::numeric_limits<uint8_t>::max()){
		if(currentNote >= min && currentNote <= max){
			output.push_back((uint8_t)currentNote);
		}
		currentNote+=12;
	}
	return(output);
}

std::vector<uint8_t> Score::optimizeScore(std::vector<uint8_t>& chord1, std::vector<uint8_t>& chord2){
	unsigned int bestScore = std::numeric_limits<unsigned int>::max();
	std::vector<uint8_t> bestChord;
	std::vector<std::vector<uint8_t>> possibleOctives;
	uint8_t lowestSearch = clampToUI8((int)getLowestNote(chord1)-12);
	uint8_t highestSearch = clampToUI8((int)getHighestNote(chord1)+12);
	for(int i=0;i<chord2.size();i++){
		possibleOctives.push_back(getOctives(chord2[i], highestSearch, lowestSearch));
	}
	unsigned int computeCount = 0;
	recursePossibleOctives(possibleOctives, chord1, 0, {}, bestScore, bestChord, computeCount);
	std::cout<<"Optimized score in "<<computeCount<<" iterations"<<std::endl;
	return(sortChord(bestChord));
}
