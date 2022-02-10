var notesPerChord = 6;
var lastOptimizedChord = [];
var lastFS = [];

function strip(string) {
	return string.replace(/^\s+|\s+$/g, '');
}

function getChordn(chordNumber){
	let output = [];
	for(var i=1;i<=notesPerChord;i++){
		var cnl = document.getElementsByClassName("c" + chordNumber + "n" + i + "l");
		var cnn = document.getElementsByClassName("c" + chordNumber + "n" + i + "n");
		var letterActually = cnl[0].value + cnn[0].value;
		if(!(strip(cnl[0].value) === "" || strip(cnn[0].value) === "")){
			var cnNote = getNumberFromLetter(letterActually);
			if(!(cnNote === 0 || strip(letterActually) === "")){
				output.push(cnNote);
			}
		}
	}
	return(output);
}

function getChordnL(chordNumber){
	let output = [];
	for(var i=1;i<=notesPerChord;i++){
		var cnl = document.getElementsByClassName("c" + chordNumber + "n" + i + "l");
		var cnn = document.getElementsByClassName("c" + chordNumber + "n" + i + "n");
		var letterActually = cnl[0].value + cnn[0].value;
		if(!((strip(letterActually) === "") || strip(cnl[0].value) === "" || strip(cnn[0].value) === "")){
			output.push(strip(letterActually));
		}
	}
	return(output);
}

function setChordnL(chordNumber, notes){
	for(var i=1;i<=notesPerChord;i++){	
		var cnl = document.getElementsByClassName("c" + chordNumber + "n" + i + "l");
		cnl[0].value = "";	
		var cnn = document.getElementsByClassName("c" + chordNumber + "n" + i + "n");
		cnn[0].value = "";
	}
	for(var i=1;i<=notesPerChord&&i<=notes.length;i++){
		var cnl = document.getElementsByClassName("c" + chordNumber + "n" + i + "l");
		cnl[0].value = notes[i-1].replace(/[1234567890-]/g, "");
		var cnn = document.getElementsByClassName("c" + chordNumber + "n" + i + "n");
		cnn[0].value = notes[i-1].replace(/[^1234567890-]/g, "");
	}
}
function showOptimizedChord(index){
	var score = getOptimizedChord(index);
	var scoreVal = scoreChord(getChordn(1), score);
	var resultText = "Result: ";
	for(var i=0;i<score.length;i++){
		var note = getLetterFromNumber(score[i]);
		resultText += note;
		if(i+1<score.length){
			resultText += " ";
		}
	}
	resultText += ", Score: " + scoreVal;
	var scoreText = document.getElementsByClassName("optimizeButtonResult")[0];
	scoreText.innerHTML = resultText;
	lastOptimizedChord = score;
	document.getElementsByClassName("playOptimizedChord")[0].style.display = "";
	document.getElementsByClassName("nextChordButton")[0].style.display = "";
	document.getElementsByClassName("previousChordButton")[0].style.display = "";
	document.getElementsByClassName("previousChordButton")[0].disabled = true;
}
var currentChordIndex = 0;
var currentChordIndexMax = 0;
function optimizeChordButtonClick(){
	var chord1 = getChordn(1);
	var chord2 = getChordn(2);
	currentChordIndexMax = optimizeChord(chord1, chord2);
	currentChordIndex = 0;
	showOptimizedChord(currentChordIndex);
}
function nextChordButtonClick(){
	if(currentChordIndex < currentChordIndexMax){
		currentChordIndex++;
		showOptimizedChord(currentChordIndex);
		if(currentChordIndex+1>=currentChordIndexMax){
			document.getElementsByClassName("nextChordButton")[0].disabled = true;
		}else{
			document.getElementsByClassName("nextChordButton")[0].disabled = false;
		}
		document.getElementsByClassName("previousChordButton")[0].disabled = false;
		playOptimizedChord();
	}
}
function previousChordButtonClick(){
	if(currentChordIndex > 0){
		currentChordIndex--;
		showOptimizedChord(currentChordIndex);
		if(currentChordIndex-1<0){
			document.getElementsByClassName("previousChordButton")[0].disabled = true;
		}else{
			document.getElementsByClassName("previousChordButton")[0].disabled = false;
		}
		document.getElementsByClassName("nextChordButton")[0].disabled = false;
		playOptimizedChord();
	}
}
function playChord1(){
	var chord1 = getChordn(1);
	playNotes(chord1, 127, 1);
}

function playChord2(){
	var chord2 = getChordn(2);
	playNotes(chord2, 127, 1);
}

function buttonPlayNote(chord, note){
	var chord = getChordn(chord);
	var note = chord[note-1];
	playNote(note, 127, 1);
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
		//var c1n = document.getElementsByClassName("c1n" + i);
		//c1n[0].style.width = inputStyle;
		//var c2n = document.getElementsByClassName("c2n" + i);
		//c2n[0].style.width = inputStyle;
	}
	document.getElementsByClassName("playOptimizedChord")[0].style.display = "none";
	document.getElementsByClassName("previousChordButton")[0].style.display = "none";
	document.getElementsByClassName("nextChordButton")[0].style.display = "none";

	PlayerInit();
}

Module.onRuntimeInitialized = () => {
	onNoteChange();
}
