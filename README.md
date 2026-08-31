# ECE243 Final Project: FPGA Karaoke System

This project is an FPGA-based karaoke system that lets users sing along to Celine Dion's "My Heart Will Go On." The program is written in C and runs on the Intel Nios II processor implemented on an FPGA.

The system plays the instrumental track, displays synchronized lyrics and reference pitch information on a VGA display, analyzes the user's vocal input in real time, and calculates a score based on how closely the user's pitch matches the reference vocals.

## Features

- Song Playback: Plays the instrumental version of "My Heart Will Go On" for the user to sing along to.

- Reference Pitch Extraction: Uses the YIN pitch detection algorithm to estimate pitch from an isolated recording of Celine Dion's vocals. The extracted pitch values are preprocessed and scaled to the VGA display dimensions before runtime.

- Real-Time Pitch Visualization: Displays the reference pitch contour on the VGA screen and animates it as the song progresses, allowing the user to visually follow the target melody.

- Voice Input Analysis: Captures and analyzes the user's vocal input in real time using the Zero-Crossing Rate (ZCR) method. Although YIN provides more robust pitch estimation, it is computationally expensive for real-time execution on the Nios II processor. ZCR was therefore used for live user input to reduce computational overhead and maintain responsive real-time visualization.

- Pitch Comparison and Scoring: Compares the user's detected pitch with the reference pitch and calculates a score based on how closely they match. The score is displayed throughout the performance and on the final results screen.

- Synchronized Lyrics: Displays lyrics in sync with the song as it plays.

- Button Controls: Uses the FPGA push buttons to control the program:
  - "KEY0" starts the karaoke session.
  - "KEY3" restarts the program.

- HEX Display Feedback: Uses the FPGA's seven-segment HEX displays to provide additional feedback on how closely the user's detected pitch matches the reference pitch.

## Demo
https://youtu.be/x1BUx8fDzpE

"Start Screen" (src/images/background/start_screen.jpg)

## Usage

1. Ensure that the FPGA hardware is properly connected and configured.
2. Load and run the program on the Intel Nios II processor.
3. Press "KEY0" to start the karaoke session.
4. Sing along with the instrumental track while following the lyrics and reference pitch visualization.
5. Try to match the reference pitch as closely as possible to achieve a higher score.
6. View the final score on the results screen after the song ends.
7. Press "KEY3" to restart the program.

## Acknowledgments
The YIN algorithm used for pitch detection was referenced from the [YIN algorithm paper](http://audition.ens.fr/adc/pdf/2002_JASA_YIN.pdf) and repository [ashokfernandez/Yin-Pitch-Tracking](https://github.com/ashokfernandez/Yin-Pitch-Tracking/tree/master).
