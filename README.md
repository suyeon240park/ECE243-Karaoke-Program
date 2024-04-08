# ECE243 Karaoke Program
This is a karaoke program that allows users to sing along to the song "My Heart Will Go On" by Celine Dion. It is written in C and operated by the Intel NIOS II Processor in FPGA.


## Features
- Song Playback: Plays the instrumental version of "My Heart Will Go On" for users to sing along with.
- Pitch Detection: Utilizes the YIN algorithm to accurately detect the pitch of the acapella version of the song and preprocesses the pitch levels to fit into the VGA screen dimensions before running the program.
- Voice Input Analysis: Analyzes user voice input using the Zero-Crossing Rate method to adjust the speed of the program, optimizing performance for FPGA operation, while being less accurate than the YIN algorithm. Displays and animates the user pitch icon on the screen.
- Animation: Displays the original pitch levels on the screen and animates them across the screen as the song progresses.
- Score Calculation: Calculates the score based on the similarity of the user input and original song pitches and displays during the play and at the ending screen.
- Lyrics: Displays lyrics at the right timing.
- KEY Operations: Use keys to start or restart the program.
- HEX Operations: Shows how well the user input pitches match with the original song pitches.

## Demo
Demo video:
[]

## Usage
1. Ensure the FPGA hardware is properly connected and configured.
2. Run the program on the Intel NIOS II Processor.
3. Press KEY0 to start the karaoke program.
4. Sing along with the song, aiming to match the pitches for higher scores.
5. View the final score at the end of the performance.
6. To restart the program, press KEY3.

## Acknowledgments
The YIN algorithm used for pitch detection was provided by [https://github.com/ashokfernandez/Yin-Pitch-Tracking/tree/master].
