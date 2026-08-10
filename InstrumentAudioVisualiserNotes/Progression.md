
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
- Damn ending up being a whole rendering system/object
- [ ] Render the FFT and waveform with main thread
	- [ ] Normalise fft output from 0 to 1
	- FFT block based only for now

# Polishing/Optimising
- [ ] Windowing stuff
	- [ ] Correctly adjusts size when changing window size (maybe need to change away from using constansts::SCR_HEIGHT/WIDTH all the time, maybe only use for initialisation)
		- [ ] window_manager onResize actually working with glfwGetUserPointer and glfwSetUserPointer
		- [ ] Fix button projection for window resizing
			- [ ] Including changing the button positions alongside the renders
	- [ ] window_manager onDrag is now actually when dragging near the split point instead of just clicking
		- [ ] Needs logic in the glfw callback to only call onDrag when initial mouse click was near the split point or something like that
- [ ] Better UI
	- [ ] Thing like a renderer to clean up main
	- [ ] UI Bug
	- [ ] Use Nativefiledialog-extended to choose the .wav file themselves.
		- [ ] Button to choose the wav file.
	- [ ] button icons and effect when hovering mouse over
- [ ] Do something about WaveformUtils and FFTUtils to make nicer
- [ ] Change so instead of writing and reading 10s of thousands of elements per frame to a lot less.
	- [ ] I assume changing sampleAmount to only new samples (which is a couple hundred a frame)
	- [ ] Make background thread only gather new samples and calculate fft and main thread will generate the extra x and z verticies per sample for the gpu.
	- [ ] Make main thread use glbuffersubdata to again reduce the total amount of data moved per frame.
		- [ ] Maybe can be used to reduce the buffer capacity as it won't need to hold as much data if the gpu will update the samples/vertex data instead.
- [ ] Can change the amount of FFT bins at runtime
	- [ ] Text box or slider
	- [ ] Can choose whether there are spaces between each bar or not
- [ ] Play the audio file
	- [ ] Play the actual audio from the file
	- [ ] Change the waveform "animation" to a vertical line going through so its easier to track
		- [ ] If waveform is longer than the window then when the line in middle make it stay in middle until the end of the waveform can reach the right of the window

# More FFT visuals
- [ ] Have option of block based or sliding window
- [ ] FFT Line Graph
- [ ] Spectrogram
- [ ] Have UI option to switch between the 3
- This is more so for more practice working with graphics and UI
 
# Real-time audio
Add option when opening application if file or live instrument
- [ ] if file then select 
- [ ] if live instrument then yeah...

# Final Polishing 
Mostly a combination of a bunch of little stuff
- [ ] Peak holds on fft line/bar graph
	- [ ] Button to reset peak holds
- [ ] faint grid lines on all relevant graphs to make look nice


# Project end
- [ ] Finish