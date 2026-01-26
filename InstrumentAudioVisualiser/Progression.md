
# Read WAV, print info
Load the WAV file and print info:
- [x] SampleRate 

# Calculate waveform from WAV
- Waveform
	- [x] What data is needed for a waveform?
		- Wav PCM data.
	- [x] How is a waveform calculated?
		- Normalising PCM data for each sample.
- print the waveform in terminal to validate
	- [x] print the sample data and also dashes representing a sort of waveform.
		- Dash waveform looks something like (but more dashes obviously):
			```
			---|
			 --|
			  -|
			   |-
			   |--
			   |---
			   |--
			   |-
			   |
			  -|
			 --|
			---|
			```

# Draw the waveform
Research OpenGL
- [Learn opengl](https://learnopengl.com/)

Implement opengl in project
- [x] Render waveform of WAVFile only first 441 samples
- [x] Make it play the whole wavfile
- [ ] Have a play button that plays the waveform once instead of looping the whole time
	- [ ] A looping toggle?
- [ ] Have a button to toggle to loop the playback through the waveformWindow

# Your first FFT
- [ ] Implement FFT
	- [ ] Just print the values for now
	- [ ] OpenGL: Have top half for waveform and bottom half for FFT

# Implement multi threaded/real-time FFT
- Main thread - representing FFT (printing values for now) and waveform (rendering).
- Background thread - Readings input, calculates FFT, feeds main thread for rendering.
Want it to simulate real-time flow

# Rendering waveform and FFT with main thread
- [ ] Render the FFT with main thread

# FFT Bars
- [ ] 

# FFT Line Graph
- [ ] 
 
# Spectrogram
- [ ] 
 
# Real-time audio
Add option when opening application if file or live instrument
- [ ] if file then select 
- [ ] if live instrument then yeah...

# MIDI Support
- [ ] reads midi data
- [ ] Real-time auto

# [[Features/Visualisation/Overview|More Visualisations]]