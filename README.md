# ECE243 Final Project: Karaoke Program
This karaoke program enables users to sing along with Celine Dion's "My Heart Will Go On." Developed in C, it is powered by the Intel NIOS II Processor within an FPGA.


## Features
- **Song Playback**: Plays the instrumental version of "My Heart Will Go On" for users to sing along with.
- **Pitch Detection**: Utilizes the **YIN algorithm** to accurately detect the pitch of the acapella version of the song and preprocesses the pitch levels to fit into the VGA screen dimensions before running the program.
- **Animation**: Displays the original pitch levels on the screen and animates them across the screen as the song progresses.
- **Voice Input Analysis**: Analyzes user voice input using the **Zero-Crossing Rate** method. The YIN algorithm detects pitch precisely but carries a high computational load, which can slow down processing and real-time animation of pitch levels. The analyzed pitch level is visually represented by animated horizontal bars.
- **Score Calculation**: Calculates the score based on the similarity of the user input and original song pitches and displays during the play and at the ending screen.
- **Lyrics**: Displays lyrics at the right timing.
- **KEY Operations**: Use keys to start or restart the program.
- **HEX Operations**: Shows how well the user input pitches match with the original song pitches on FPGA Hex.


## Demo
> https://youtu.be/x1BUx8fDzpE

![start screen](src/images/background/start_screen.jpg)


## Usage
1. Ensure the FPGA hardware is properly connected and configured.
2. Run the program on the Intel NIOS II Processor.
3. Press KEY0 to start the karaoke program.
4. Sing along with the song, aiming to match the pitches for higher scores.
5. View the final score at the end of the performance.
6. To restart the program, press KEY3.


## Acknowledgments
The YIN algorithm used for pitch detection was referenced from the [YIN algorithm paper](http://audition.ens.fr/adc/pdf/2002_JASA_YIN.pdf) and repository [ashokfernandez/Yin-Pitch-Tracking](https://github.com/ashokfernandez/Yin-Pitch-Tracking/tree/master).
