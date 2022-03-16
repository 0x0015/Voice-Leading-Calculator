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
	void EMSCRIPTEN_KEEPALIVE memorizeOptimizedChordStart();
	void EMSCRIPTEN_KEEPALIVE memorizeChord1Start();
	void EMSCRIPTEN_KEEPALIVE memorizeChord2Start();
	void EMSCRIPTEN_KEEPALIVE clearMemory();
	void EMSCRIPTEN_KEEPALIVE eraseMemory();
	void EMSCRIPTEN_KEEPALIVE killUnisonsChange();
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
	for(unsigned int i=0;i<notesPerChord;i++){
		auto cn = Element::getByClassName("ChordMemory" + std::to_string(chordNumber) + "c" + std::to_string(notesPerChord-i));
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

std::string getStringFromInterval(const std::vector<uint8_t>& intervals){
	std::string output = "<";	
	for(auto& n : intervals){
		output += std::to_string((int)n);
	}
	output += ">";
	return(output);
}

std::string getAnalysisText(std::vector<uint8_t>& chord1, std::vector<uint8_t>& chord2, std::optional<std::pair<unsigned int, unsigned int>> chordsWithScore = std::nullopt){
	std::string scoreAnalysis = "Overall Score: ";
	unsigned int scoreVal = Score::scoreAll(chord1, chord2);
	scoreAnalysis += std::to_string(scoreVal);
	if(chordsWithScore){
		scoreAnalysis += " (" + std::to_string(chordsWithScore.value().first) + "/" + std::to_string(chordsWithScore.value().second) + ") ";
	}
	if(chord1.size() > 0 && chord2.size() > 0){
		auto chord1intervals = chordTable::getIntervals(chord1);
		auto chord2intervals = chordTable::getIntervals(chord2);
		auto chord1T = chordTable::lookup(chord1intervals);
		auto chord2T = chordTable::lookup(chord2intervals);
		scoreAnalysis += "<br>";
		if(chord1T){
			if(chord1T.value().second != "" && strip(chord1T.value().second) != ""){
				scoreAnalysis += getStringFromInterval(chord1intervals) + " " + chord1T.value().second + " [" + chord1T.value().first + "]";
			}else{
				scoreAnalysis += getStringFromInterval(chord1intervals) + " Unknown [" + chord1T.value().first + "]";
			}
		}
		scoreAnalysis += " - ";
		if(chord2T){
			if(chord2T.value().second != "" && strip(chord2T.value().second) != ""){
				scoreAnalysis += getStringFromInterval(chord2intervals) + " " + chord2T.value().second + " [" + chord2T.value().first + "]";
			}else{
				scoreAnalysis += getStringFromInterval(chord2intervals) + " Unknown [" + chord2T.value().first + "]";
			}
		}
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

int showOptimizedChord(unsigned int index){
	if(lastOptimizedChord.size() <= index){
		std::cerr<<"invalid index of optimized chord"<<std::endl;
		return -1;
	}
	std::vector<uint8_t> score = lastOptimizedChord[index];
	auto chord1 = getChordn(1);
	unsigned int scoreVal = Score::scoreAll(chord1, score);
	if(scoreVal == std::numeric_limits<unsigned int>::max()){
		std::cerr<<"invalid score value"<<std::endl;
		return -1;
	}
	std::string resultText = "Result: ";
	for(unsigned int i=1;i<=notesPerChord;i++){
		auto noteScoreText = Element::getByClassName("optimizeButtonResult" + std::to_string(i))[0];
		noteScoreText->dom_style["display"] = "none";
	}
	for(unsigned int i=0;i<score.size();i++){
		auto noteScoreText = Element::getByClassName("optimizeButtonResult" + std::to_string(notesPerChord-i))[0];
		noteScoreText->dom_innerHTML = letterFromNote(score[i]);
		noteScoreText->dom_style["display"] = "";
	}
	int chordsWithScore = -1;
	int cws_index = 0;
	for(unsigned int i=index;i>0;i--){
		if(scoreVal == Score::scoreAll(chord1, lastOptimizedChord[i])){
			chordsWithScore++;
			cws_index++;
		}else{
			break;
		}
	}
	for(unsigned int i=index;i<lastOptimizedChord.size();i++){
		if(scoreVal == Score::scoreAll(chord1, lastOptimizedChord[i])){
			chordsWithScore++;
		}else{
			break;
		}
	}
	if(cws_index == 0)
		cws_index++;
	if(chordsWithScore == 0)
		chordsWithScore++;

	auto scoreText = Element::getByClassName("optimizeButtonResult")[0];
	scoreText->dom_innerHTML = (std::string)resultText;
	auto analysisText = Element::getByClassName("optimizeButtonResultAnalysis")[0];
	std::string analysisTextIH = getAnalysisText(chord1, lastOptimizedChord[index], std::make_pair(cws_index, chordsWithScore));
	analysisText->dom_innerHTML = (std::string)analysisTextIH;
	Element::getByClassName("playOptimizedChord")[0]->dom_style["display"] = "";
	Element::getByClassName("memorizeOptimizedChord")[0]->dom_style["display"] = "";
	Element::getByClassName("memorizeOptimizedChordStart")[0]->dom_style["display"] = "";
	Element::getByClassName("nextChordButton")[0]->dom_style["display"] = "";
	Element::getByClassName("previousChordButton")[0]->dom_style["display"] = "";
	Element::getByClassName("optimizeButtonResultDisplay")[0]->dom_style["display"] = "";
	Element::getByClassName("optimizeButtonResultAnalysis")[0]->dom_style["display"] = "";
	Element::getByClassName("previousChordButton")[0]->dom_disabled = true;
	return(0);
}

unsigned int currentChordIndex = 0;

void optimizeChordButtonClick(){
	auto chord1 = getChordn(1);
	auto chord2 = getChordn(2);
	lastOptimizedChord = Score::optimizeScore(chord1, chord2);
	std::sort(lastOptimizedChord.begin(), lastOptimizedChord.end(), [&](auto& obj1, auto& obj2){
		return(Score::scoreAll(chord1, obj1) < Score::scoreAll(chord1, obj2));
	});
	currentChordIndex = 0;
	showOptimizedChord(currentChordIndex);
}

int hasOptimizedChord(unsigned int index){
	if(lastOptimizedChord.size() <= index){
		std::cerr<<"invalid index of optimized chord"<<std::endl;
		return -1;
	}
	std::vector<uint8_t> score = lastOptimizedChord[index];
	auto chord1 = getChordn(1);
	unsigned int scoreVal = Score::scoreAll(chord1, score);
	if(scoreVal == std::numeric_limits<unsigned int>::max()){
		std::cerr<<"invalid score value"<<std::endl;
		return -1;
	}
	return(0);
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
		if(currentChordIndex+1 < lastOptimizedChord.size() && hasOptimizedChord(currentChordIndex+1) != 0){
			Element::getByClassName("nextChordButton")[0]->dom_disabled = true;
			return;
		}
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
		scoreText->dom_innerHTML = getAnalysisText(chord1, chord2);
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

void memorizeOptimizedChordStart(){
	auto chordi = lastOptimizedChord[currentChordIndex];
	std::vector<std::string> chord;
	for(int i=0;i<chordi.size();i++){
		std::string note = letterFromNote(chordi[i]);
		chord.push_back(note);
	}
	setChordnL(1, chord);
	onNoteChange();
}

void memorizeChord1Start(){
	auto chord = getChordnL(1);
	setChordnL(2, chord);
	onNoteChange();
}

void memorizeChord2Start(){
	auto chord = getChordnL(2);
	setChordnL(1, chord);
	onNoteChange();
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
	Element::getByClassName("memorizeOptimizedChordStart")[0]->dom_style["display"] = "none";
	Element::getByClassName("previousChordButton")[0]->dom_style["display"] = "none";
	Element::getByClassName("nextChordButton")[0]->dom_style["display"] = "none";
	Element::getByClassName("optimizeButtonResultDisplay")[0]->dom_style["display"] = "none";
	Element::getByClassName("optimizeButtonResultAnalysis")[0]->dom_style["display"] = "none";
}

int main(){
	std::cout<<"Voice Leading Calculator:  v0x06"<<std::endl;
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

void killUnisonsChange(){
	Score::killUnisons = Element::getByClassName("removeUnison")[0]->dom_checked;
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
