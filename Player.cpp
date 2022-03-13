#include <iostream>
#include "WebCpp-Interaction-Lib/DOM.hpp"
#include "UI.hpp"
#include "Player.hpp"
#include <emscripten.h>

extern "C" {
	void EMSCRIPTEN_KEEPALIVE playChord1();
	void EMSCRIPTEN_KEEPALIVE playChord2();
	void EMSCRIPTEN_KEEPALIVE arpegChord1();
	void EMSCRIPTEN_KEEPALIVE arpegChord2();
	void EMSCRIPTEN_KEEPALIVE buttonPlayNote(unsigned int c, unsigned int n);
}

EM_JS(void, Player_playNote, (uint8_t note, uint8_t velocity, float length, float time), {
	playNote(note, velocity, length, time);
});

void playNote(uint8_t note, uint8_t velocity, float length, float time){
	Player_playNote(note, velocity, length, time);
}

void playNotes(std::vector<uint8_t>& notes, uint8_t velocity, float length, float stagger){
	for(unsigned int i=0;i<notes.size();i++){
		playNote(notes[i], velocity, length, i*stagger);
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

void arpegChord1(){
	auto chord1 = getChordn(1);
	playNotes(chord1, 127, 0.25, 0.25);
}

void arpegChord2(){
	auto chord2 = getChordn(2);
	playNotes(chord2, 127, 0.25, 0.25);
}

void buttonPlayNote(unsigned int c, unsigned int n){
	auto chord = getChordn(c);
	auto note = chord[n-1];
	playNote(note, 127, 1);
}


