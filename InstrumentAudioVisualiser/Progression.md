
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
	- [ ] print the sample data and also dashes representing a sort of waveform.
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
Research:
- OpenGL
	- [ ] **Data Format**: What format does OpenGL expect vertex data in?
	- [ ] **Buffer Objects**: How do you get your vertex data from CPU memory into GPU memory? (You'll see terms like *VBO*, *VAO*)
		- [ ] What is *VBO*?
		- [ ] What is *VAO*?
	- [ ] **Shaders**: What's the minimum shader code needed to draw lines?
	- [ ] **Drawing**: What OpenGL function actually triggers the code?

- [ ] Render waveform of WAVFile

# Your first FFT
- [ ] Implement FFT
	- [ ] Just print the values

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

# MIDI Support
- [ ] reads midi data
- [ ] Real-time rendering

# [[Features/Visualisation/Overview|More Visualisations]]