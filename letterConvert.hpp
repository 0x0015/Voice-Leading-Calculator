#pragma once
#include <iostream>

uint8_t noteFromLetter(std::string letter);
std::string letterFromNote(uint8_t note, bool preferFlat = false);
uint8_t isNoteFlat(std::string letter);//0 = neither, 2 = flat, 1 = sharp
