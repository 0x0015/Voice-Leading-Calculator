#pragma once
#include <iostream>

uint8_t noteFromLetter(std::string letter);
std::string letterFromNote(uint8_t note, bool preferFlat = false);
