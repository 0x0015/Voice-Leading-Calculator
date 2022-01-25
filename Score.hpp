#pragma once
#include <iostream>
#include <vector>

class Score{
public:
	static inline bool debug = false;
	//section C.  give 1 point if there is exactly one of these errors
	static inline bool uncharacteristicRisingUnequalFiths = true;
	static inline bool octivesOrFithsOfOuterVoices = true;
	static inline bool overlappingVoices = true;
	static inline bool motionLeadingToCrossedVoices = true;
	static inline bool chordalSeventhApproachedByDescendingLeap = true;
	//section D.  give 0 points for any of these errors
	static inline bool parallelOctives = true;
	static inline bool parallelFiths = true;
	static inline bool parallelUnisons = true;
	static inline bool uncharacteristicLeaps = true;
	static inline bool chordalSeventhsUnresolved = true;
	static inline bool leadingToneIsOuterVoiceUnresolved = true;
	static inline bool SixthOrFourthOfCadential54Unresolved = true;
	static inline bool fewerThan4Voices = true;
	static inline bool incorrectPreperationOrResolutionOfSuspension = true;
	static inline bool multipleSectionCErrors = true;
	static unsigned int scoreTwo(std::pair<uint8_t, uint8_t> pair1, std::pair<uint8_t, uint8_t> pair2);
	static unsigned int scoreAll(std::vector<uint8_t>& chord1, std::vector<uint8_t>& chord2);
	static unsigned int scoreRanges(std::vector<uint8_t>& chord1, std::vector<uint8_t>& chord2);
	static std::vector<uint8_t> optimizeScore(std::vector<uint8_t>& chord1, std::vector<uint8_t>& chord2);
	static void printChord(std::vector<uint8_t>& chord);
	static std::vector<uint8_t> sortChord(std::vector<uint8_t>& chord);
private:
	static void recursePossibleOctives(std::vector<std::vector<uint8_t>>& possibleOctives, std::vector<uint8_t>& chord1, int i, std::vector<uint8_t> accum, unsigned int& bestScore, std::vector<uint8_t>& bestChord, unsigned int& computeCount);
};
