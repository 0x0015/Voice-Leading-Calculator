#include "Score.hpp"
#include "letterConvert.hpp"
#include <cstring>
#include "WebCpp-Interaction-Lib/DOM.hpp"
#include <emscripten.h>
#include "Player.hpp"

extern "C" {
	void EMSCRIPTEN_KEEPALIVE setup();
	void EMSCRIPTEN_KEEPALIVE optimizeChordButtonClick();
	void EMSCRIPTEN_KEEPALIVE nextChordButtonClick();
	void EMSCRIPTEN_KEEPALIVE previousChordButtonClick();
	void EMSCRIPTEN_KEEPALIVE onNoteChange();
	void EMSCRIPTEN_KEEPALIVE playOptimizedChord();
	void EMSCRIPTEN_KEEPALIVE arpegOptimizedChord();
	void EMSCRIPTEN_KEEPALIVE sortChord1();
	void EMSCRIPTEN_KEEPALIVE sortChord2();
	void EMSCRIPTEN_KEEPALIVE parallelFithWeightChange();
	void EMSCRIPTEN_KEEPALIVE parallelOctiveWeightChange();
}

int notesPerChord = 6;

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
		std::cout<<"Letteractually: "<<letterActually<<std::endl;
		if(!(strip(cnl[0]->dom_value) == "" || strip(cnn[0]->dom_value) == "")){
			auto cnNote = noteFromLetter(letterActually);
			if(!(!cnNote || strip(letterActually) == "")){
				output.push_back(cnNote.value());
				std::cout<<" cnNote: "<<(unsigned int)cnNote.value()<<std::endl;
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
	std::cout<<"Cleared notes"<<std::endl;
	for(unsigned int i=1;i<=notesPerChord;i++){	
		auto cnl = Element::getByClassName("c" + std::to_string(chordNumber) + "n" + std::to_string(i) + "l");
		auto cnn = Element::getByClassName("c" + std::to_string(chordNumber) + "n" + std::to_string(i) + "n");
		std::string cnnVal = removeDigOrMinus(notes[i-1], false);
		std::string cnlVal = removeDigOrMinus(notes[i-1], true);
		cnn[0]->dom_value = (std::string)cnnVal;
		cnl[0]->dom_value = (std::string)cnlVal;
	}
}

std::vector<std::vector<uint8_t>> lastOptimizedChord;

std::string getAnalysisText(unsigned int scoreVal){
	std::string scoreAnalysis = "Overall Score: ";
	scoreAnalysis += std::to_string(scoreVal) + "<br><br>";
	scoreAnalysis += "Note Difference Score: " + std::to_string(Score::noteDifferenceScore) + "<br>";
	scoreAnalysis += "Parallel Fifths Score: " + std::to_string(Score::parallelFithsScore) + "<br>";
	scoreAnalysis += "Parallel Octives Score: " + std::to_string(Score::parallelOctivesScore);
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
	analysisText->dom_innerHTML = getAnalysisText(scoreVal);
	Element::getByClassName("playOptimizedChord")[0]->dom_style["display"] = "";
	Element::getByClassName("arpegOptimizedChord")[0]->dom_style["display"] = "";
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
		std::cout<<currentChordIndex<<std::endl;
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
		scoreText->dom_innerHTML = getAnalysisText(score);
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

void setup(){
	Element::getByClassName("playOptimizedChord")[0]->dom_style["display"] = "none";
	Element::getByClassName("arpegOptimizedChord")[0]->dom_style["display"] = "none";
	Element::getByClassName("previousChordButton")[0]->dom_style["display"] = "none";
	Element::getByClassName("nextChordButton")[0]->dom_style["display"] = "none";
	Element::getByClassName("optimizeButtonResultDisplay")[0]->dom_style["display"] = "none";
	Element::getByClassName("optimizeButtonResultAnalysis")[0]->dom_style["display"] = "none";
}

int main(){
	setup();
	onNoteChange();
	std::string initPlayer = "PlayerInit();";
	GLOBAL_ACCESS->evalJS(initPlayer);
	return(0);
}

void playOptimizedChord(){
	playNotes(lastOptimizedChord[currentChordIndex], 127, 1);
}

void arpegOptimizedChord(){
	playNotes(lastOptimizedChord[currentChordIndex], 127, 0.25, 0.25);
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
