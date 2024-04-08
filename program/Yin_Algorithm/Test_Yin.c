#include <stdio.h>
#include <math.h>
#include "voice_only.h"
#include "Yin.h"

// 8000 Hz = 1 sec
// 4000 Hz = 0.5 sec = 1 chunk
// 20 sec silience = 8000 / 4000 * 20 = 40 chuncks
#define CHUNK_SIZE 4000
#define SAMPLING_RATE 8000 


int main() {
    const int32_t *audio_data = voice_only; // Pointer to the start of the array
    Yin yin;
    int prev_pitch = 0;

    int pitch_array_length = voice_only_n / CHUNK_SIZE;

    // Open the file in write mode
    FILE *file = fopen("pitch.c", "w");

    fprintf(file, "#define PITCH_ARRAY_LENGTH %d\n\n", pitch_array_length);
    fprintf(file, "int pitch_array[] = {\n");

    for (int i = 0; i < voice_only_n; i += CHUNK_SIZE) {
        // Process the current chunk:
        const int32_t *chunk_start = audio_data + i;  // Pointer to the start of the chunk

        Yin_init(&yin, CHUNK_SIZE, YIN_DEFAULT_THRESHOLD);
        double estimated_pitch = Yin_getPitch(&yin, chunk_start);
        //fprintf(file, "%d: %f\n", i / CHUNK_SIZE, estimated_pitch);

        // Processing for VGA display
        if (i < 42 * CHUNK_SIZE || i > 202 * CHUNK_SIZE) {
          fprintf(file, "%d,\n", 239);
        }
        else {
          if (estimated_pitch == -1) {
            estimated_pitch = prev_pitch;
          }
          prev_pitch = estimated_pitch;
        
          // original pitch range from 200 to 500
          // processed pitch range from 30 to 200
          estimated_pitch = 210 - ((estimated_pitch - 200) * (200 - 30) / (500 - 200));
          fprintf(file, "%d,\n", (int)(estimated_pitch));
        }
    }

    fprintf(file, "};");

    fclose(file);

  return 0;
}