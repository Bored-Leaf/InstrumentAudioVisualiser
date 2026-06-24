
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
- [x] Have a play button that plays the waveform once instead of looping the whole time
	- [x] A looping toggle?
- [x] Have a button to toggle to loop the playback through the waveformWindow
- [x] Refactor to be cleaner and clearer as too many non-const globals.

# Your first FFT
- [x] Implement FFT
	- [x] Just print the values for now

# Implement multi threaded/real-time FFT
- [x] Implement CircularBuffer for use in multi-threading

- [x] Main thread - representing FFT (printing values for now) and waveform (rendering).
- [x] Background thread - Readings input, calculates FFT, feeds main thread for rendering.
Want it to simulate real-time flow

# Rendering waveform *and* FFT with main thread and make look nice
- [ ] Render the FFT with main thread

# Light Polishing/Optimising
- [ ] Matrix stuff for buttons and coordinates for window resizing and keeping aspect ratio of buttons.
- [ ] Better button icons and effect when hovering mouse over
- [ ] Play the audio file
	- [ ] Play the actual audio from the file
	- [ ] Change the waveform "animation" to a vertical line going through so its easier to track
- [ ] Change so instead of writing and reading 10s of thousands of elements per frame to only a couple hundred.
	- [ ] I assume changing sampleAmount to only new samples (which is a couple hundred a frame)
	- [ ] Make background thread only gather new samples and calculate fft and main thread will generate the extra x and z verticies per sample for the gpu.
	- [ ] Make main thread use glbuffersubdata to again reduce the total amount of data moved per frame.
		- [ ] Maybe can be used to reduce the buffer capacity as it won't need to hold as much data if the gpu will update the samples/vertex data instead.

# More FFT visuals
- [ ] FFT Line Graph
- [ ] Spectrogram
- [ ] Have UI option to switch between the 3
- This is more so for more practice working with graphics
 
# Real-time audio
Add option when opening application if file or live instrument
- [ ] if file then select 
- [ ] if live instrument then yeah...

# Final Polish
Mostly a combination of a bunch of little stuff
- [ ] Peak holds on fft line/bar graph
	- [ ] Button to reset peak holds
- [ ] faint grid lines on all relevant graphs to make look nice
