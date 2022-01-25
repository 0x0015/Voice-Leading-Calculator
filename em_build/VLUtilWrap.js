scoreChordWrap = Module.cwrap('scoreChord', 'number', ['array', 'number', 'array', 'number']);
optimizeChordWrap = Module.cwrap('optimizeChord', '[number]', ['array', 'number', 'array', 'number']);
getCListValueWrap = Module.cwrap('getCListValue', 'number', ['number', 'number']);
getNumberFromLetterWrap = Module.cwrap('getNumberFromLetter', 'number', ['array']);
strlenWrap = Module.cwrap('strlen_r', 'number', 'number');
getLetterFromNumberWrap = Module.cwrap('getLetterFromNumber', '[number]', ['number']);
getLetterFromNumber_LenWrap = Module.cwrap('getLetterFromNumber_Len', 'number', ['number']);
freeWrap = Module.cwrap('freeArr', '', 'number');
noteFlatWrap = Module.cwrap('noteFlat', 'number', ['array']);

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
function getNumberFromLetter(inputLet){
	return(getNumberFromLetterWrap(string2cstring(inputLet)));
}
function noteFlat(note){
	return(noteFlatWrap(string2cstring(note)));
}
