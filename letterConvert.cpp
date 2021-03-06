#include "letterConvert.hpp"

constexpr unsigned int str2int(const char* str, int h = 0)
{
    return !str[h] ? 5381 : (str2int(str, h+1) * 33) ^ str[h];
}

std::string setFirstLetter(std::string base, std::string letter){
	std::string output1 = base.substr(1, base.length());
	std::string output = letter;
	output.append(output1);
	return(output);
}

std::string sanatizeFirstNote(std::string letter){
	if(letter.length() == 0){
		return("");
	}
	switch(letter[0]){
		case 'c':
			return(setFirstLetter(letter, "C"));
		case 'd':
			return(setFirstLetter(letter, "D"));
		case 'e':
			return(setFirstLetter(letter, "E"));
		case 'f':
			return(setFirstLetter(letter, "F"));
		case 'g':
			return(setFirstLetter(letter, "G"));
		case 'a':
			return(setFirstLetter(letter, "A"));
		case 'b':
			return(setFirstLetter(letter, "B"));
		default:
			return(letter);
	}
	return(letter);
}

std::optional<uint8_t> noteFromLetter(std::string letterUnsanitized){
	std::string letter = sanatizeFirstNote(letterUnsanitized);
	int letterOnly;
	int numberFrom = 1;
	bool hasMinus = false;
	if(letter.find("-") != std::string::npos){
		numberFrom = 2;
		hasMinus = true;
	}
	std::string letterOnlyNote = letter.substr(0, letter.length()-numberFrom);
	switch(str2int(letterOnlyNote.c_str())){
		case str2int("C"):  
			letterOnly = 0;
			break;
		case str2int("C#"):  
			letterOnly = 1;
			break;
		case str2int("Db"):  
			letterOnly = 1;
			break;
		case str2int("D"):  
			letterOnly = 2;
			break;
		case str2int("D#"):  
			letterOnly = 3;
			break;
		case str2int("Eb"):  
			letterOnly = 3;
			break;
		case str2int("E"):  
			letterOnly = 4;
			break;
		case str2int("F"):  
			letterOnly = 5;
			break;
		case str2int("F#"):  
			letterOnly = 6;
			break;
		case str2int("Gb"):  
			letterOnly = 6;
			break;
		case str2int("G"):  
			letterOnly = 7;
			break;
		case str2int("G#"):  
			letterOnly = 8;
			break;
		case str2int("Ab"):  
			letterOnly = 8;
			break;
		case str2int("A"):  
			letterOnly = 9;
			break;
		case str2int("A#"):  
			letterOnly = 10;
			break;
		case str2int("Bb"):  
			letterOnly = 10;
			break;
		case str2int("B"):  
			letterOnly = 11;
			break;
		default:
			return(std::nullopt);
	}
	int octive;
	try{
		if(hasMinus){
			octive = std::stoi(letter.substr(letter.length()-2, 2));
		}else{
			octive = std::stoi(letter.substr(letter.length()-1, 1));
		}
	}catch(std::invalid_argument& e){
		std::cout<<"Letterconvert found invalid arguement"<<std::endl;
		return(std::nullopt);
	}
	int noteNum = letterOnly + (12 * (octive + 1));
	return((uint8_t)noteNum);
}

std::string letterFromNote(uint8_t note, bool preferFlat){
	uint8_t baseNote = note%12;
	int octive = (int)((note/12)-1);
	std::string octiveStr = std::to_string(octive);
	switch(baseNote){
		case 0:
			return("C" + octiveStr);
		case 1:
			if(preferFlat){
				return("Db" + octiveStr);
			}else{
				return("C#" + octiveStr);
			}
		case 2:
			return("D" + octiveStr);
		case 3:
			if(preferFlat){
				return("Eb" + octiveStr);
			}else{
				return("D#" + octiveStr);
			}
		case 4:
			return("E" + octiveStr);
		case 5:
			return("F" + octiveStr);
		case 6:
			if(preferFlat){
				return("Gb" + octiveStr);
			}else{
				return("F#" + octiveStr);
			}
		case 7:
			return("G" + octiveStr);
		case 8:
			if(preferFlat){
				return("Ab" + octiveStr);
			}else{
				return("G#" + octiveStr);
			}
		case 9:
			return("A" + octiveStr);
		case 10:
			if(preferFlat){
				return("Bb" + octiveStr);
			}else{
				return("A#" + octiveStr);
			}
		case 11:
			return("B" + octiveStr);
		default:
			std::cout<<"Unknown base note number(should never happen)"<<std::endl;
			return("");
	}
}

uint8_t isNoteFlat(std::string note){
	if(note.find("b") != std::string::npos){
		return(2);
	}else if(note.find("#") != std::string::npos){
		return(1);
	}else{
		return(0);
	}
}
