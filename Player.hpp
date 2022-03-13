#pragma once
#include <iostream>
#include <vector>

inline bool arpegiate = false;

void playNotes(std::vector<uint8_t>& notes, uint8_t velocity, float length, float stagger = 0);
void playNote(uint8_t note, uint8_t velocity, float length, float time = 0);
