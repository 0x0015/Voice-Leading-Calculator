var notesPerChord = 6;
var lastOptimizedChord = [];
var lastFS = [];

function strip(string) {
	return string.replace(/^\s+|\s+$/g, '');
}

function getChordn(chordNumber){
	let output = [];
	for(var i=1;i<=notesPerChord;i++){
		var cn = document.getElementsByClassName("c" + chordNumber + "n" + i);
		var cnNote = getNumberFromLetter(cn[0].value);
		if(!(cnNote === 0 || strip(cn[0].value) === "")){
			output.push(cnNote);
		}
	}
	return(output);
}

function getChordnL(chordNumber){
	let output = [];
	for(var i=1;i<=notesPerChord;i++){
		var cn = document.getElementsByClassName("c" + chordNumber + "n" + i);
		if(!(strip(cn[0].value) === "")){
			output.push(strip(cn[0].value));
		}
	}
	return(output);
}

function setChordnL(chordNumber, notes){
	for(var i=1;i<=notesPerChord;i++){	
		var cn = document.getElementsByClassName("c" + chordNumber + "n" + i);
		cn[0].value = "";
	}
	for(var i=1;i<=notesPerChord&&i<=notes.length;i++){
		var cn = document.getElementsByClassName("c" + chordNumber + "n" + i);
		cn[0].value = notes[i-1];
	}
}

function optimizeChordButtonClick(){
	var scoreText = document.getElementsByClassName("optimizeButtonResult")[0];
	var chord1 = getChordn(1);
	var chord2 = getChordn(2);
	var score = optimizeChord(chord1, chord2);
	var scoreVal = scoreChord(chord1, score);
	var resultText = "Result: ";
	for(var i=0;i<score.length;i++){
		var note = getLetterFromNumber(score[i]);
		resultText += note;
		if(i+1<score.length){
			resultText += " ";
		}
	}
	resultText += ", Score: " + scoreVal;
	scoreText.innerHTML = resultText;
	lastOptimizedChord = score;
	document.getElementsByClassName("playOptimizedChord")[0].style.display = "";
}

function playChord1(){
	var chord1 = getChordn(1);
	playNotes(chord1, 127, 1);
}

function playChord2(){
	var chord2 = getChordn(2);
	playNotes(chord2, 127, 1);
}

function sortChord(chord){//super rudimentary sort.  O=n^2
	var output = [];
	var lastVal = -1;
	var chordVals = [];
	for(var i=0;i<chord.length;i++){
		chordVals.push(getNumberFromLetter(chord[i]));
	}
	while(output.length < chord.length){
		var currentBestScore = 256;//uint8_t max
		var currentBestNote = "";
		for(var i=0;i<chord.length;i++){
			var currentVal = chordVals[i];
			if(currentVal > lastVal && currentVal < currentBestScore){
				currentBestScore = currentVal;
				currentBestNote = chord[i];
			}
		}
		if(currentBestScore === 256){
			//no more notes could be added
			return(output);
		}
		lastVal = currentBestScore;
		output.push(currentBestNote);
	}
	return(output);
}

function sortChord1(){
	var chord = getChordnL(1);
	var sorted = sortChord(chord);
	setChordnL(1, sorted);
}

function sortChord2(){	
	var chord = getChordnL(2);
	var sorted = sortChord(chord);
	setChordnL(2, sorted);
}

function playOptimizedChord(){
	playNotes(lastOptimizedChord, 127, 1);
}

function onNoteChange(){
	var scoreText = document.getElementsByClassName("scoreButtonResult")[0];
	var chord1 = getChordn(1);
	var chord2 = getChordn(2);
	var score = scoreChord(chord1, chord2);
	if(score === ""){
		score = "N/A";
	}
	scoreText.innerHTML = "Score: " + score;
}


function setup(){
	var inputStyle = "5ch";
	for(var i=1;i<=notesPerChord;i++){
		var c1n = document.getElementsByClassName("c1n" + i);
		c1n[0].style.width = inputStyle;
		var c2n = document.getElementsByClassName("c2n" + i);
		c2n[0].style.width = inputStyle;
	}
	document.getElementsByClassName("playOptimizedChord")[0].style.display = "none";

	
	PlayerInit();
}

Module.onRuntimeInitialized = () => {
	onNoteChange();
}
