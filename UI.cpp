#include "Score.hpp"
#include "letterConvert.hpp"
#include <cstring>
#include "WebCpp-Interaction-Lib/DOM.hpp"
#include <emscripten.h>
#include "Player.hpp"
#include "UI.hpp"
#include "ChordTable.hpp"
#include <thread>

extern "C" {
	void EMSCRIPTEN_KEEPALIVE setup();
	void EMSCRIPTEN_KEEPALIVE optimizeChordButtonClick();
	void EMSCRIPTEN_KEEPALIVE nextChordButtonClick();
	void EMSCRIPTEN_KEEPALIVE previousChordButtonClick();
	void EMSCRIPTEN_KEEPALIVE onNoteChange();
	void EMSCRIPTEN_KEEPALIVE playOptimizedChord();
	void EMSCRIPTEN_KEEPALIVE sortChord1();
	void EMSCRIPTEN_KEEPALIVE sortChord2();
	void EMSCRIPTEN_KEEPALIVE parallelFithWeightChange();
	void EMSCRIPTEN_KEEPALIVE parallelOctiveWeightChange();
	void EMSCRIPTEN_KEEPALIVE arpegiateChordChange();
	void EMSCRIPTEN_KEEPALIVE memorizeOptimizedChord();
	void EMSCRIPTEN_KEEPALIVE memorizeChord1();
	void EMSCRIPTEN_KEEPALIVE memorizeChord2();
	void EMSCRIPTEN_KEEPALIVE clearMemory();
	void EMSCRIPTEN_KEEPALIVE eraseMemory();
}

std::string strip(std::string in){
	std::string output;
	for(int i=0;i<in.length();i++){
		if(!isspace(in[i])){
			output+=in[i];
		}
	}
	return(output);
}

std::vector<uint8_t> getChordn(unsigned int chordNumber){
	std::vector<uint8_t> output;
	for(unsigned int i=1;i<=notesPerChord;i++){
		auto cnl = Element::getByClassName("c" + std::to_string(chordNumber) + "n" + std::to_string(i) + "l");
		auto cnn = Element::getByClassName("c" + std::to_string(chordNumber) + "n" + std::to_string(i) + "n");
		std::string letterActually = (std::string)cnl[0]->dom_value + (std::string)cnn[0]->dom_value;
		//std::cout<<"Letteractually: "<<letterActually<<std::endl;
		if(!(strip(cnl[0]->dom_value) == "" || strip(cnn[0]->dom_value) == "")){
			auto cnNote = noteFromLetter(letterActually);
			if(!(!cnNote || strip(letterActually) == "")){
				output.push_back(cnNote.value());
				//std::cout<<" cnNote: "<<(unsigned int)cnNote.value()<<std::endl;
			}
		}
	}
	return(output);
}

std::vector<uint8_t> getMemoryn(unsigned int chordNumber){
	std::vector<uint8_t> output;
	for(unsigned int i=0;i<chordMemories;i++){
		auto cn = Element::getByClassName("ChordMemory" + std::to_string(chordNumber) + "c" + std::to_string(6-i));
		std::string letterActually = (std::string)cn[0]->dom_innerHTML;
		//std::cout<<"Letteractually: "<<letterActually<<std::endl;
		if(!(strip(letterActually) == "")){
			auto cnNote = noteFromLetter(letterActually);
			if(!(!cnNote || strip(letterActually) == "")){
				output.push_back(cnNote.value());
				//std::cout<<" cnNote: "<<(unsigned int)cnNote.value()<<std::endl;
			}
		}
	}
	return(output);
}

std::vector<std::string> getChordnL(unsigned int chordNumber){
	std::vector<std::string> output;
	for(unsigned int i=1;i<=notesPerChord;i++){
		auto cnl = Element::getByClassName("c" + std::to_string(chordNumber) + "n" + std::to_string(i) + "l");
		auto cnn = Element::getByClassName("c" + std::to_string(chordNumber) + "n" + std::to_string(i) + "n");
		std::string letterActually = (std::string)cnl[0]->dom_value + (std::string)cnn[0]->dom_value;
		if(!((strip(letterActually) == "") || strip(cnl[0]->dom_value)== "" || strip(cnn[0]->dom_value) == "")){
			output.push_back(strip(letterActually));
		}
	}
	return(output);
}

bool digOrMinus(char c){
	if(std::isdigit(c)){
		return(true);
	}else if(c == '-'){
		return(true);
	}
	return(false);
}

std::string removeDigOrMinus(std::string& str, bool inv){
	std::string output;
	for(int i=0;i<str.length();i++){
		if(inv){
			if(!digOrMinus(str[i])){
				output += str[i];
			}
		}else{
			if(digOrMinus(str[i])){
				output += str[i];
			}
		}
	}
	return(output);
}

void setChordnL(unsigned int chordNumber, std::vector<std::string>& notes){
	for(unsigned int i=1;i<=notesPerChord;i++){	
		auto cnl = Element::getByClassName("c" + std::to_string(chordNumber) + "n" + std::to_string(i) + "l");
		auto cnn = Element::getByClassName("c" + std::to_string(chordNumber) + "n" + std::to_string(i) + "n");
		cnn[0]->dom_value = (std::string)" ";
		cnl[0]->dom_value = (std::string)" ";
	}
	//std::cout<<"Cleared notes"<<std::endl;
	for(unsigned int i=1;i<=notesPerChord;i++){	
		auto cnl = Element::getByClassName("c" + std::to_string(chordNumber) + "n" + std::to_string(i) + "l");
		auto cnn = Element::getByClassName("c" + std::to_string(chordNumber) + "n" + std::to_string(i) + "n");
		std::string cnnVal = removeDigOrMinus(notes[i-1], false);
		std::string cnlVal = removeDigOrMinus(notes[i-1], true);
		cnn[0]->dom_value = (std::string)cnnVal;
		cnl[0]->dom_value = (std::string)cnlVal;
	}
}

void setChordMemory(unsigned int chordNumber, std::vector<std::string>& notes){
	for(unsigned int i=0;i<notesPerChord;i++){
		auto cn = Element::getByClassName("ChordMemory" + std::to_string(chordNumber) + "c" + std::to_string(notesPerChord-i));
		cn[0]->dom_innerHTML = std::string("");
	}
	//std::cout<<"Cleared notes"<<std::endl;
	for(unsigned int i=0;i<notesPerChord&&i<notes.size();i++){
		auto cn = Element::getByClassName("ChordMemory" + std::to_string(chordNumber) + "c" + std::to_string(notesPerChord-i));
		cn[0]->dom_innerHTML = (std::string)notes[i];
	}
}

std::vector<std::vector<uint8_t>> lastOptimizedChord;

template<uint8_t dif> std::vector<std::pair<uint8_t, uint8_t>> getNumStepsRecur(std::vector<uint8_t>& chord){//O(n!)
	if(chord.size() == 0){
		return(std::vector<std::pair<uint8_t, uint8_t>>());
	}
	std::vector<std::pair<uint8_t, uint8_t>> found;
	for(int ln = 0;ln<chord.size();ln++){
		uint8_t lastNote = chord[ln];
		for(int i=ln;i<chord.size();i++){
			unsigned int noteDiference = (unsigned int)(std::abs((int)chord[i]-(int)lastNote));
			if(noteDiference == dif){
				uint8_t n1 = chord[i];
				uint8_t n2 = lastNote;
				if(n1<=n2){
					found.push_back(std::make_pair(n1, n2));
				}else{
					found.push_back(std::make_pair(n2, n1));
				}
			}
		}
	}
	return(found);
}

std::string getAnalysisText(unsigned int scoreVal, std::vector<uint8_t> chord1 = {}, std::vector<uint8_t> chord2 = {}){
	std::string scoreAnalysis = "Overall Score: ";
	scoreAnalysis += std::to_string(scoreVal);
	if(chord1.size() > 0 && chord2.size() > 0){
		auto chord1intervals = chordTable::getIntervals(chord1);
		auto chord2intervals = chordTable::getIntervals(chord2);
		auto chord1T = chordTable::lookup(chord1intervals);
		auto chord2T = chordTable::lookup(chord2intervals);
		scoreAnalysis += " (";
		if(chord1T && chord1T.value().second != "" && strip(chord1T.value().second) != ""){
			scoreAnalysis += chord1T.value().second + " [" + chord1T.value().first + "]";
		}else{
			scoreAnalysis += "Unknown <";
			for(auto& n : chord1intervals){
				scoreAnalysis += std::to_string((int)n);
			}
			scoreAnalysis += ">";
		}
		scoreAnalysis += " - ";
		if(chord2T && chord2T.value().second != "" && strip(chord2T.value().second) != ""){
			scoreAnalysis += chord2T.value().second + " [" + chord2T.value().first + "]";
		}else{
			scoreAnalysis += "Unknown <";
			for(auto& n : chord2intervals){
				scoreAnalysis += std::to_string((int)n);
			}
			scoreAnalysis += ">";
		}
		scoreAnalysis += ")";
	}
	scoreAnalysis += "<br><br>";
	scoreAnalysis += "Note Difference Score: " + std::to_string(Score::noteDifferenceScore) + "<br>";
	scoreAnalysis += "Parallel Fifths Score: " + std::to_string(Score::parallelFithsScore);
	if(chord1.size() > 0 && chord2.size() > 0){
		auto chord1Fifths = getNumStepsRecur<7>(chord1);
		auto chord2Fifths = getNumStepsRecur<7>(chord2);
		if(chord1Fifths.size() > 0 && chord2Fifths.size() > 0){
			scoreAnalysis += " (";
			for(auto& o : chord1Fifths){
				scoreAnalysis += letterFromNote(o.first) + ":" + letterFromNote(o.second);
			}
			scoreAnalysis += " - ";
			for(auto& o : chord2Fifths){
				scoreAnalysis += letterFromNote(o.first) + ":" + letterFromNote(o.second);
			}
			scoreAnalysis += ")";
		}
	}
	scoreAnalysis += "<br>";
	scoreAnalysis += "Parallel Octives Score: " + std::to_string(Score::parallelOctivesScore);
	if(chord1.size() > 0 && chord2.size() > 0){
		auto chord1Octives = getNumStepsRecur<12>(chord1);
		auto chord2Octives = getNumStepsRecur<12>(chord2);
		if(chord1Octives.size() > 0 && chord2Octives.size() > 0){
			scoreAnalysis += " (";
			for(auto& o : chord1Octives){
				scoreAnalysis += letterFromNote(o.first) + ":" + letterFromNote(o.second);
			}
			scoreAnalysis += " - ";
			for(auto& o : chord2Octives){
				scoreAnalysis += letterFromNote(o.first) + ":" + letterFromNote(o.second);
			}
			scoreAnalysis += ")";
		}
	}
	return(scoreAnalysis);
}

void showOptimizedChord(unsigned int index){
	if(lastOptimizedChord.size() <= index){
		return;
	}
	std::vector<uint8_t> score = lastOptimizedChord[index];
	auto chord1 = getChordn(1);
	unsigned int scoreVal = Score::scoreAll(chord1, score);
	if(scoreVal == std::numeric_limits<unsigned int>::max()){
		return;
	}
	std::string resultText = "Result: ";
	for(unsigned int i=1;i<=6;i++){
		auto noteScoreText = Element::getByClassName("optimizeButtonResult" + std::to_string(i))[0];
		noteScoreText->dom_style["display"] = "none";
	}
	for(unsigned int i=0;i<score.size();i++){
		auto noteScoreText = Element::getByClassName("optimizeButtonResult" + std::to_string(6-i))[0];
		noteScoreText->dom_innerHTML = letterFromNote(score[i]);
		noteScoreText->dom_style["display"] = "";
	}
	auto scoreText = Element::getByClassName("optimizeButtonResult")[0];
	scoreText->dom_innerHTML = (std::string)resultText;
	auto analysisText = Element::getByClassName("optimizeButtonResultAnalysis")[0];
	analysisText->dom_innerHTML = getAnalysisText(scoreVal, chord1, lastOptimizedChord[index]);
	Element::getByClassName("playOptimizedChord")[0]->dom_style["display"] = "";
	Element::getByClassName("memorizeOptimizedChord")[0]->dom_style["display"] = "";
	Element::getByClassName("nextChordButton")[0]->dom_style["display"] = "";
	Element::getByClassName("previousChordButton")[0]->dom_style["display"] = "";
	Element::getByClassName("optimizeButtonResultDisplay")[0]->dom_style["display"] = "";
	Element::getByClassName("optimizeButtonResultAnalysis")[0]->dom_style["display"] = "";
	Element::getByClassName("previousChordButton")[0]->dom_disabled = true;
}

unsigned int currentChordIndex = 0;

void optimizeChordButtonClick(){
	auto chord1 = getChordn(1);
	auto chord2 = getChordn(2);
	lastOptimizedChord = Score::optimizeScore(chord1, chord2);
	currentChordIndex = 0;
	showOptimizedChord(currentChordIndex);
}

void nextChordButtonClick(){
	if(currentChordIndex < lastOptimizedChord.size()){
		currentChordIndex++;
		showOptimizedChord(currentChordIndex);
		if(currentChordIndex+1>=lastOptimizedChord.size()){
			Element::getByClassName("nextChordButton")[0]->dom_disabled = true;
		}else{
			Element::getByClassName("nextChordButton")[0]->dom_disabled = false;
		}
		Element::getByClassName("previousChordButton")[0]->dom_disabled = false;
		playOptimizedChord();
	}
}

void previousChordButtonClick(){
	if(currentChordIndex > 0){
		currentChordIndex--;
		showOptimizedChord(currentChordIndex);
		//std::cout<<currentChordIndex<<std::endl;
		if(currentChordIndex < 1){
			Element::getByClassName("previousChordButton")[0]->dom_disabled = true;
		}else{
			Element::getByClassName("previousChordButton")[0]->dom_disabled = false;
		}
		Element::getByClassName("nextChordButton")[0]->dom_disabled = false;
		playOptimizedChord();
	}
}

void onNoteChange(){
	auto scoreText = Element::getByClassName("scoreButtonResult")[0];
	auto chord1 = getChordn(1);
	auto chord2 = getChordn(2);
	unsigned int score = Score::scoreAll(chord1, chord2);
	if(score == std::numeric_limits<unsigned int>::max()){
		scoreText->dom_innerHTML = (std::string)"Score: N/A";
	}else{
		//scoreText->dom_innerHTML = "Score: " + std::to_string(score);
		scoreText->dom_innerHTML = getAnalysisText(score, chord1, chord2);
	}
}

void parallelOctiveWeightChange(){
	Score::parallelOctivesWeight = std::stoi(Element::getByClassName("parallelOctiveWeightInput")[0]->dom_value);
	onNoteChange();
}

void parallelFithWeightChange(){
	Score::parallelFithsWeight = std::stoi(Element::getByClassName("parallelFithWeightInput")[0]->dom_value);
	onNoteChange();
}

void memorizeOptimizedChord(){
	auto chordi = lastOptimizedChord[currentChordIndex];
	std::vector<std::string> chord;
	for(int i=0;i<chordi.size();i++){
		std::string note = letterFromNote(chordi[i]);
		chord.push_back(note);
	}
	setChordMemory(memorizedChords+1, chord);
	memorizedChords++;
	if(memorizedChords+1>chordMemories){	
		Element::getByClassName("memorizeOptimizedChord")[0]->dom_disabled = true;
		Element::getByClassName("memorizeChord1")[0]->dom_disabled = true;
		Element::getByClassName("memorizeChord2")[0]->dom_disabled = true;
	}
}

void memorizeChord1(){
	auto chord = getChordnL(1);
	setChordMemory(memorizedChords+1, chord);
	memorizedChords++;
	if(memorizedChords+1>chordMemories){	
		Element::getByClassName("memorizeOptimizedChord")[0]->dom_disabled = true;
		Element::getByClassName("memorizeChord1")[0]->dom_disabled = true;
		Element::getByClassName("memorizeChord2")[0]->dom_disabled = true;
	}
}

void memorizeChord2(){
	auto chord = getChordnL(2);
	setChordMemory(memorizedChords+1, chord);
	memorizedChords++;
	if(memorizedChords+1>chordMemories){	
		Element::getByClassName("memorizeOptimizedChord")[0]->dom_disabled = true;
		Element::getByClassName("memorizeChord1")[0]->dom_disabled = true;
		Element::getByClassName("memorizeChord2")[0]->dom_disabled = true;
	}
}

void clearMemory(){
	std::vector<std::string> chord = {};
	for(int i=1;i<=chordMemories;i++){
		setChordMemory(i, chord);
	}
	memorizedChords = 0;
	Element::getByClassName("memorizeOptimizedChord")[0]->dom_disabled = false;
	Element::getByClassName("memorizeChord1")[0]->dom_disabled = false;
	Element::getByClassName("memorizeChord2")[0]->dom_disabled = false;
}

void eraseMemory(){
	std::vector<std::string> chord = {};
	setChordMemory(memorizedChords, chord);
	memorizedChords--;
	Element::getByClassName("memorizeOptimizedChord")[0]->dom_disabled = false;
	Element::getByClassName("memorizeChord1")[0]->dom_disabled = false;
	Element::getByClassName("memorizeChord2")[0]->dom_disabled = false;
}

void setup(){
	Element::getByClassName("playOptimizedChord")[0]->dom_style["display"] = "none";
	Element::getByClassName("memorizeOptimizedChord")[0]->dom_style["display"] = "none";
	Element::getByClassName("previousChordButton")[0]->dom_style["display"] = "none";
	Element::getByClassName("nextChordButton")[0]->dom_style["display"] = "none";
	Element::getByClassName("optimizeButtonResultDisplay")[0]->dom_style["display"] = "none";
	Element::getByClassName("optimizeButtonResultAnalysis")[0]->dom_style["display"] = "none";
}

int main(){
	std::cout<<"Voice Leading Calculator:  v0x04"<<std::endl;
	setup();
	std::string initPlayer = "PlayerInit();";
	GLOBAL_ACCESS->evalJS(initPlayer);
	chordTable::load();
	onNoteChange();
	return(0);
}

void playOptimizedChord(){
	if(!arpegiate){
		playNotes(lastOptimizedChord[currentChordIndex], 127, 1);
	}else{
		playNotes(lastOptimizedChord[currentChordIndex], 127, 0.25, 0.25);
	}
}

void arpegiateChordChange(){
	arpegiate = Element::getByClassName("arpegiateChord")[0]->dom_checked;

}

std::vector<std::string> sortChord(std::vector<std::string>& chord){
	std::vector<std::string> output = chord;
	std::sort(output.begin(), output.end(), [](const std::string& obj1, const std::string& obj2){
		return(noteFromLetter(obj1) < noteFromLetter(obj2));
		});
	return(output);
}

void sortChord1(){
	auto chord = getChordnL(1);
	auto sorted = sortChord(chord);
	setChordnL(1, sorted);
}

void sortChord2(){
	auto chord = getChordnL(2);
	auto sorted = sortChord(chord);
	setChordnL(2, sorted);
}
