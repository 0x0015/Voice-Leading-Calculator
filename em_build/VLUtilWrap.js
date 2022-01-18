scoreChordWrap = Module.cwrap('scoreChord', 'number', ['array', 'number', 'array', 'number']);
optimizeChordWrap = Module.cwrap('optimizeChord', '[number]', ['array', 'number', 'array', 'number']);
getCListValueWrap = Module.cwrap('getCListValue', 'number', ['number', 'number']);
getNumberFromLetterWrap = Module.cwrap('getNumberFromLetter', 'number', ['array']);
strlenWrap = Module.cwrap('strlen_r', 'number', 'number');
getLetterFromNumberWrap = Module.cwrap('getLetterFromNumber', '[number]', ['number']);
getLetterFromNumber_LenWrap = Module.cwrap('getLetterFromNumber_Len', 'number', ['number']);
freeWrap = Module.cwrap('freeArr', '', 'number');

var notesPerChord = 6;

function scoreChord(chord1, chord2){
	return(scoreChordWrap(chord1, chord1.length, chord2, chord2.length));
}
function optimizeChord(chord1, chord2){
	var listPtr = optimizeChordWrap(chord1, chord1.length, chord2, chord2.length);
	
	var myTypedArray = new Uint8Array(chord2.length);
	for(var i=0;i<chord2.length;i++){
		myTypedArray[i] = getCListValueWrap(listPtr, i);
	}
	freeWrap(listPtr);
	return(myTypedArray);
}
function setup(){
	var inputStyle = "5ch";
	for(var i=1;i<=notesPerChord;i++){
		var c1n = document.getElementsByClassName("c1n" + i);
		c1n[0].style.width = inputStyle;
		var c2n = document.getElementsByClassName("c2n" + i);
		c2n[0].style.width = inputStyle;
	}
}
function string2cstring(inputStr){
	let output = [];
	for(var i=0;i<inputStr.length;i++){
		output.push(inputStr.charCodeAt(i));
	}
	output.push(0);
	return(output);
}
function cstring2string(inputCStr, strLength = 0){
	if(length === 0){
		var strLength = strlenWrap(inputCStr);
	}
	var myTypedArray = new Uint8Array(strLength);
	for(var i=0;i<strLength;i++){
		myTypedArray[i] = getCListValueWrap(inputCStr, i);
	}
	var output = String.fromCharCode(...myTypedArray);
	delete myTypedArray;
	return(output);
}
function getLetterFromNumber(inputNum){
	var strPtr = getLetterFromNumberWrap(inputNum);
	var strLength = getLetterFromNumber_LenWrap(inputNum);
	var output = cstring2string(strPtr, strLength);
	freeWrap(strPtr);
	return(output);
}
function getChordn(chordNumber){
	let output = [];
	for(var i=1;i<=notesPerChord;i++){
		var cn = document.getElementsByClassName("c" + chordNumber + "n" + i);
		var cnNote = getNumberFromLetterWrap(string2cstring(cn[0].value));
		if(!(cnNote === 0 || cn[0].value === "")){
			output.push(cnNote);
		}
	}
	return(output);
}

function scoreChordButtonClick(){
	var scoreText = document.getElementsByClassName("scoreButtonResult")[0];
	var chord1 = getChordn(1);
	var chord2 = getChordn(2);
	var score = scoreChord(chord1, chord2);
	scoreText.innerHTML = "Result: " + score;
}
function optimizeChordButtonClick(){
	var scoreText = document.getElementsByClassName("optimizeButtonResult")[0];
	var chord1 = getChordn(1);
	var chord2 = getChordn(2);
	var score = optimizeChord(chord1, chord2);
	var resultText = "Result: ";
	for(var i=0;i<score.length;i++){
		var note = getLetterFromNumber(score[i]);
		resultText += note;
		if(i+1<score.length){
			resultText += " ";
		}
	}
	scoreText.innerHTML = resultText;
}
