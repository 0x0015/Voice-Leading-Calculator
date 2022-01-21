var loadedMidi = false;

function PlayerInit(){
	MIDI.loadPlugin({
		soundfontUrl: "https://gleitz.github.io/midi-js-soundfonts/FluidR3_GM/",
		instrument: "acoustic_grand_piano",
		onprogress: function(state, progress){
			console.log(state, progress);
		},
		onsuccess: function(){
			loadedMidi = true;
			MIDI.setVolume(0, 127);
		}
	});
}

function playNote(note, velocity, length){
	if(loadedMidi){
		MIDI.noteOn(0, note, velocity, 0);
		MIDI.noteOff(0, note, length);
	}
}

function playNotes(notes, velocity, length){
	for(var i=0;i<notes.length;i++){
		playNote(notes[i], velocity, length);
	}
}
