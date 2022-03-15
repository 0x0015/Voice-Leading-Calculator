#include "Score.hpp"
#include "letterConvert.hpp"
#include <limits>
#include <optional>
#include <algorithm>


std::vector<uint8_t> Score::sortChord(std::vector<uint8_t>& chord){
	std::vector<uint8_t> output = chord;
	std::sort(output.begin(), output.end());
	return(output);
}

unsigned int Score::scoreRanges(std::vector<uint8_t>& chord1, std::vector<uint8_t>& chord2){
	if(chord1.size() != chord2.size()){
		std::cout<<"Comparing ranges of scores of unequal size.  Unable to match notes."<<std::endl;
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
	std::vector<uint8_t> chord1S = sortChord(chord1);
	std::vector<uint8_t> chord2S = sortChord(chord2);
	if(chord1S.size() != chord2S.size()){
		unsigned int output = scoreRanges(chord1S, chord2S);
		return(output);
	}
	unsigned int output = 0;
	for(int i=0;i<chord1S.size();i++){
		output += (unsigned int)(std::abs(((int)chord1S[i])-((int)chord2S[i])));

	}
	return(output);
}

template<uint8_t dif> unsigned int findNumSteps(std::vector<uint8_t>& chord){//O(n)
	if(chord.size() == 0){
		return(0);
	}
	unsigned int found = 0;
	uint8_t lastNote = chord[0];
	for(int i=1;i<chord.size();i++){
		unsigned int noteDiference = (unsigned int)(std::abs((int)chord[i]-(int)lastNote));
		if(noteDiference == dif){
			found++;
		}
		lastNote = chord[i];
	}
	return(found);
}

template<uint8_t dif> unsigned int findNumStepsRecur(std::vector<uint8_t>& chord){//O(n!)
	if(chord.size() == 0){
		return(0);
	}
	unsigned int found = 0;
	for(int ln = 0;ln<chord.size();ln++){
		uint8_t lastNote = chord[ln];
		for(int i=ln;i<chord.size();i++){
			unsigned int noteDiference = (unsigned int)(std::abs((int)chord[i]-(int)lastNote));
			if(noteDiference == dif){
				found++;
			}
		}
	}
	return(found);
}



unsigned int Score::scoreParallelFiths(std::vector<uint8_t>& chord1, std::vector<uint8_t>& chord2){
	unsigned int chord1F = findNumStepsRecur<7>(chord1);
	if(chord1F > 0){
		unsigned int chord2F = findNumStepsRecur<7>(chord2);
		return(std::min(chord1F, chord2F));
	}
	return(0);
}

unsigned int Score::scoreParallelOctives(std::vector<uint8_t>& chord1, std::vector<uint8_t>& chord2){
	unsigned int chord1F = findNumStepsRecur<12>(chord1);
	if(chord1F > 0){
		unsigned int chord2F = findNumStepsRecur<12>(chord2);
		return(std::min(chord1F, chord2F));
	}
	return(0);
}

unsigned int Score::scoreTwo(std::pair<uint8_t, uint8_t> pair1, std::pair<uint8_t, uint8_t> pair2){
	unsigned int output = 0;
	
	return(output);
}


unsigned int Score::scoreAll(std::vector<uint8_t>& chord1, std::vector<uint8_t>& chord2){
	if(chord1.size() == 0 || chord2.size() == 0){
		return(std::numeric_limits<unsigned int>::max());
	}
	if(findNumSteps<0>(chord1) > 0 || findNumSteps<0>(chord2) > 0){
		return(std::numeric_limits<unsigned int>::max());
	}
	unsigned int numNotes = std::min(chord1.size(), chord2.size());
	unsigned int output = 0;
	noteDifferenceScore = scoreRanges(chord1, chord2);
	output += noteDifferenceScore;
	parallelFithsScore = scoreParallelFiths(chord1, chord2) * parallelFithsWeight * numNotes;
	output += parallelFithsScore;
	parallelOctivesScore = scoreParallelOctives(chord1, chord2) * parallelOctivesWeight * numNotes;
	output += parallelOctivesScore;
	return(output);
}

void Score::printChord(std::vector<uint8_t>& chord){
	for(int i=0;i<chord.size();i++){
		std::cout<<letterFromNote(chord[i])<<"("<<(unsigned int)chord[i]<<")";
		if(i+1<chord.size()){
			std::cout<<", ";
		}
	}
}

void sortVVectors(std::vector<std::pair<unsigned int, std::vector<uint8_t>>>& vec){
	for(int i=0;i<vec.size();i++){
		std::sort(vec[i].second.begin(), vec[i].second.end());
	}
	std::sort(vec.begin(), vec.end(), [](const std::pair<unsigned int, std::vector<uint8_t>>& obj1, const std::pair<unsigned int, std::vector<uint8_t>>& obj2){
		if(obj1.first == obj2.first){
			int i=0;
			for(;i<obj1.second.size()&&i<obj2.second.size();i++){
				if(obj1.second[i] != obj2.second[i]){
					return(obj1.second[i] < obj2.second[i]); 
				}
			}
			if(i >= obj1.second.size() && i < obj2.second.size()){
				return(true);
			}
			return(false);
		}else{	
			return(obj1.first < obj2.first);
		}
	});
	auto last = std::unique(vec.begin(), vec.end(), [](const std::pair<unsigned int, std::vector<uint8_t>>& obj1, const std::pair<unsigned int, std::vector<uint8_t>>& obj2){
		return(obj1.second == obj2.second);
	});
	vec.erase(last, vec.end());
}

void Score::recursePossibleOctives(std::vector<std::vector<uint8_t>>& possibleOctives, std::vector<uint8_t>& chord1, int i, std::vector<uint8_t> accum, std::vector<std::pair<unsigned int, std::vector<uint8_t>>>& bestChord, unsigned int& computeCount){
	if(i == possibleOctives.size()){
		//here accum is a possible combo
		unsigned int currentScore = scoreAll(chord1, accum);
		if(bestChord.size() < keepScores){
			bestChord.push_back(std::pair<unsigned int, std::vector<uint8_t>>(currentScore, accum));
		}else{
			if(currentScore <= bestChord.back().first){
				bestChord.push_back(std::pair<unsigned int, std::vector<uint8_t>>(currentScore, accum));
				sortVVectors(bestChord);
				if(bestChord.size() >= keepScores){
					bestChord.pop_back();
				}
			}
		}
		computeCount++;
	}else{
		std::vector<uint8_t> row = possibleOctives[i];
		for(int j=0;j<row.size();j++){
			std::vector<uint8_t> tmp(accum);
			tmp.push_back(row[j]);
			recursePossibleOctives(possibleOctives, chord1, i+1, tmp, bestChord, computeCount);
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
	while(currentNote <=max){
		if(currentNote >= min){
			output.push_back((uint8_t)currentNote);
		}
		currentNote+=12;
	}
	return(output);
}

std::vector<std::vector<uint8_t>> Score::optimizeScore(std::vector<uint8_t>& chord1, std::vector<uint8_t>& chord2){
	std::vector<std::pair<unsigned int, std::vector<uint8_t>>> bestChord;
	bestChord.reserve(keepScores);
	std::vector<std::vector<uint8_t>> possibleOctives;
	uint8_t lowestSearch = clampToUI8((int)getLowestNote(chord1)-12);
	uint8_t highestSearch = clampToUI8((int)getHighestNote(chord1)+12);
	for(int i=0;i<chord2.size();i++){
		possibleOctives.push_back(getOctives(chord2[i], highestSearch, lowestSearch));
	}
	unsigned int computeCount = 0;
	recursePossibleOctives(possibleOctives, chord1, 0, {}, bestChord, computeCount);
	std::cout<<"Optimized score in "<<computeCount<<" iterations"<<std::endl;
	bestChord.shrink_to_fit();
	sortVVectors(bestChord);
	std::vector<std::vector<uint8_t>> outputVec;
	outputVec.reserve(bestChord.size());
	for(int i=0;i<bestChord.size();i++){
		outputVec.push_back(sortChord(bestChord[i].second));
	}
	outputVec.shrink_to_fit();
	return(outputVec);
}
