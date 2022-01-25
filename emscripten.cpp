#include "Score.hpp"
#include "letterConvert.hpp"
#include <cstring>
#include <emscripten.h>

extern "C" {
	uint8_t* EMSCRIPTEN_KEEPALIVE optimizeChord(const uint8_t* chord1, uint32_t chord1len, const uint8_t* chord2, uint32_t chord2len);
	uint32_t EMSCRIPTEN_KEEPALIVE scoreChord(const uint8_t* chord1, uint32_t chord1len, const uint8_t* chord2, uint32_t chord2len);
	uint8_t EMSCRIPTEN_KEEPALIVE getCListValue(const uint8_t* list, uint32_t index);
	uint8_t EMSCRIPTEN_KEEPALIVE getNumberFromLetter(const char* string);
	char* EMSCRIPTEN_KEEPALIVE getLetterFromNumber(uint8_t note);
	uint32_t EMSCRIPTEN_KEEPALIVE getLetterFromNumber_Len(uint8_t note);
	uint32_t EMSCRIPTEN_KEEPALIVE strlen_r(const char* str);
	void EMSCRIPTEN_KEEPALIVE freeArr(char* array);
	uint8_t EMSCRIPTEN_KEEPALIVE noteFlat(const char* str);
}

void freeArr(char* array){
	delete[] array;
}

uint32_t strlen_r(const char* str){
	uint32_t i = 0;
	while(str[i] != 0){
		i++;
	}
	return(i);
}

char* getLetterFromNumber(uint8_t note){
	std::string text = letterFromNote(note);
	char* textG = new char[text.length()+1];
	std::memcpy(textG, text.c_str(), text.length());
	textG[text.length()] = 0;
	return(textG);
}

uint32_t getLetterFromNumber_Len(uint8_t note){
	std::string text = letterFromNote(note);
	return(text.length());
}

uint8_t getNumberFromLetter(const char* s){
	std::string cppstring(s);
	return(noteFromLetter(cppstring));
}

uint8_t noteFlat(const char* s){
	std::string cppstring(s);
	return(isNoteFlat(cppstring));
}

uint8_t getCListValue(const uint8_t* list, uint32_t index){
	return(list[index]);
}

uint8_t* optimizeChord(const uint8_t* chord1, uint32_t chord1len, const uint8_t* chord2, uint32_t chord2len){
		std::vector<uint8_t> c1(chord1, chord1+chord1len);
		std::vector<uint8_t> c2(chord2, chord2+chord2len);
		std::vector<uint8_t> output = Score::optimizeScore(c1, c2);
		uint8_t* array = new uint8_t[output.size()];
		std::memcpy(array, output.data(), output.size());
		return(array);
	}
	unsigned int scoreChord(const uint8_t* chord1, uint32_t chord1len, const uint8_t* chord2, uint32_t chord2len){
		std::vector<uint8_t> c1(chord1, chord1+chord1len);
		std::vector<uint8_t> c2(chord2, chord2+chord2len);
		return(Score::scoreAll(c1, c2));
	}
