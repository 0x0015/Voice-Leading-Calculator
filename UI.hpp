#pragma once
#include <iostream>
#include <vector>

std::vector<uint8_t> getChordn(unsigned int chordNumber);
std::vector<uint8_t> getMemoryn(unsigned int chordNumber);
inline int notesPerChord = 6;
inline int chordMemories = 6;
inline int memorizedChords = 0;
