#include <iostream>
#include "WebCpp-Interaction-Lib/DOM.hpp"
#include "UI.hpp"
#include <emscripten.h>

extern "C" {
	void EMSCRIPTEN_KEEPALIVE playChord1();
	void EMSCRIPTEN_KEEPALIVE playChord2();
	void EMSCRIPTEN_KEEPALIVE buttonPlayNote(unsigned int c, unsigned int n);
}

EM_JS(void, Player_playNote, (uint8_t note, uint8_t velocity, uint8_t length), {
	playNote(note, velocity, length);
});

void playNote(uint8_t note, uint8_t velocity, uint8_t length){
	Player_playNote(note, velocity, length);
}

void playNotes(std::vector<uint8_t>& notes, uint8_t velocity, uint8_t length){
	for(unsigned int i=0;i<notes.size();i++){
		playNote(notes[i], velocity, length);
	}
}

void playChord1(){
	auto chord1 = getChordn(1);
	playNotes(chord1, 127, 1);
}

void playChord2(){
	auto chord2 = getChordn(2);
	playNotes(chord2, 127, 1);
}

void buttonPlayNote(unsigned int c, unsigned int n){
	auto chord = getChordn(c);
	auto note = chord[n-1];
	playNote(note, 127, 1);
}


