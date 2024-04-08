#ifndef COMMON_H
#define COMMON_H

// Headers
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

// Constants
#define VGA_X 320
#define VGA_Y 240

#define SCORE_SMALL_HEIGHT 10
#define SCORE_SMALL_WIDTH 10
#define SCORE_WIDTH 55
#define SCORE_HEIGHT 10

#define ZERO_BIG_HEIGHT 60
#define ZERO_BIG_WIDTH 60

#define SCORE_X 230
#define SCORE_Y 10
#define SCORE1_X 290
#define SCORE2_X 300

#define SCORE_BIG_WIDTH 60
#define SCORE_BIG_HEIGHT 60
#define SCORE_BIG_X 160 // middle of x-coordinates
#define SCORE_BIG_Y 100

#define TITLE_HEIGHT 73
#define TITLE_WIDTH 242

#define START_HEIGHT 10
#define START_WIDTH 178

#define LYRICS_Y 220
#define LYRICS_HEIGHT 10
#define LYRICS_1_WIDTH 212
#define LYRICS_2_WIDTH 176
#define LYRICS_3_WIDTH 242
#define LYRICS_4_WIDTH 212
#define LYRICS_5_WIDTH 194
#define LYRICS_6_WIDTH 278
#define LYRICS_7_WIDTH 245
#define LYRICS_8_WIDTH 312
#define LYRICS_9_WIDTH 252
#define LYRICS_10_WIDTH 242
#define LYRICS_11_WIDTH 262

#define SAMPLES_N 845745
#define VOICE_ONLY_N 837716
#define SAMPLE_RATE 8000
#define end_sample 21340

// 8000 Hz = 1 sec
// 1000 Hz = 0.125 sec = 1 chunk
// 20 sec silience = 8000 / 1000 * 20 = 160 chunks
#define CHUNK_SIZE 4000
#define SAMPLING_RATE 8000

#define PITCH_ARRAY_LENGTH 200
#define PITCH_BAR_LENGTH 27

#define PITCH_LINE_X 40

#define BLACK 0x0000
#define WHITE 0xffff
#define CYAN 0x977e
#define GREY 0xa514

// Structs
// Frame buffer
struct fb_t {
    unsigned short volatile pixels[256][512];
};

// Pixel buffer controller
struct videoout_t {
    struct fb_t *volatile fbp;         // front frame buffer
    struct fb_t *volatile bfbp;        // back frame buffer
    unsigned int volatile resolution;  // resolution two fields of 16b each, packed into a 32b word
    unsigned int volatile StatusControl;
};

// Audio
struct audio_t {
	volatile unsigned int control;
	volatile unsigned char rarc;
	volatile unsigned char ralc;
	volatile unsigned char warc;
	volatile unsigned char walc;
    volatile unsigned int ldata;
	volatile unsigned int rdata;
};

// Audio
extern const int32_t samples[];
extern const int32_t voice_only[];
extern int pitch_array[];
extern int const ends[];

// Images
extern const short titanic[];
extern const short titanic_dark[];
extern const short score[];

extern const short zero[];
extern const short one[];
extern const short two[];
extern const short three[];
extern const short four[];
extern const short five[];
extern const short six[];
extern const short seven[];
extern const short eight[];
extern const short nine[];

extern const short zero_big[];
extern const short one_big[];
extern const short two_big[];
extern const short three_big[];
extern const short four_big[];
extern const short five_big[];
extern const short six_big[];
extern const short seven_big[];
extern const short eight_big[];
extern const short nine_big[];

extern const short title[];
extern const short start[];

extern const short lyrics_1[];
extern const short lyrics_2[];
extern const short lyrics_3[];
extern const short lyrics_4[];
extern const short lyrics_5[];
extern const short lyrics_6[];
extern const short lyrics_7[];
extern const short lyrics_8[];
extern const short lyrics_9[];
extern const short lyrics_10[];
extern const short lyrics_11[];

#endif