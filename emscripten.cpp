#include "Score.hpp"
#include <emscripten.h>

extern "C" {
	uint8_t* EMSCRIPTEN_KEEPALIVE optimizeChord(const uint8_t* chord1, uint32_t chord1len, const uint8_t* chord2, uint32_t chord2len);
	uint32_t EMSCRIPTEN_KEEPALIVE scoreChord(const uint8_t* chord1, uint32_t chord1len, const uint8_t* chord2, uint32_t chord2len);
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
