### SIMULATION AND EXPERIMENTATION
- [ ] Design a system for the 3 inputs
    - [ ] Design a touch analyzer with a trackpad
        - [ ] Accept input as a touch if there is a beginning and an end, and if there is a miniscule amount of movement from the touch’s origin
    - [ ] Design a scratch (slide) analyzer with a trackpad
        - [ ] Upgrade system to accept an input as a slide if there is a large amount of movement
    - [x] Design a click analyzer with a (amplitude measuring) microphone
        - [x] Design and demo an amplitude measuring system
            - [x] Accept an input if there is a sudden increase in amplitude from the baseline with an immediate decay (0.5 ms) and only lasts at most 2 ms before returning to the baseline threshold
- [x] Design a system for the output
    - [x] Screen could emulate the LCD screens with green screen and pixelated characters
- [ ] Add support buttons 
    - [ ] Add substitution buttons for each input
        - [ ] Tap button
        - [ ] Scratch button
        - [ ] Click button
    - [ ] Add space and period buttons for the output screen
        - [ ] Space button
        - [ ] Period button
    - [ ] Add a character confirm and a backspace button
        - [ ] Character confirm button
        - [ ] Backspace button
    - [ ] Add a clear screen button
    - [ ] Add an ambient amplitude button to determine baseline and threshold
    - [ ] Add a knob to increase and decrease microphone sensitivity for click recognition

### PCB DESIGN

### CASE DESIGN

### FINAL ASSEMBLY