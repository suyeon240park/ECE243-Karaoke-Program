#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct {
    uint32_t chunkID;
    uint32_t chunkSize;
    uint32_t format;
    uint32_t subchunk1ID;
    uint32_t subchunk1Size;
    uint16_t audioFormat;
    uint16_t numChannels;
    uint32_t sampleRate;
    uint32_t byteRate;
    uint16_t blockAlign;
    uint16_t bitsPerSample;
    uint32_t subchunk2ID;
    uint32_t subchunk2Size;
} WavHeader;

int main() {
    FILE *input_file = fopen("song.wav", "rb");
    if (input_file == NULL) {
        printf("Error: Unable to open input file.\n");
        return 1;
    }

    WavHeader header;
    fread(&header, sizeof(header), 1, input_file);

    if (header.audioFormat != 1) {
        printf("Error: Unsupported audio format.\n");
        fclose(input_file);
        return 1;
    }

    if (header.bitsPerSample != 32) {
        printf("Error: Unsupported bit depth. Only 32-bit audio is supported.\n");
        fclose(input_file);
        return 1;
    }

    int32_t *audio_data = (int32_t *)malloc(header.subchunk2Size);
    if (audio_data == NULL) {
        printf("Error: Memory allocation failed.\n");
        fclose(input_file);
        return 1;
    }

    fread(audio_data, sizeof(int32_t), header.subchunk2Size / sizeof(int32_t), input_file);
    fclose(input_file);

    FILE *output_file = fopen("song_sample.c", "w");
    if (output_file == NULL) {
        printf("Error: Unable to open output file.\n");
        free(audio_data);
        return 1;
    }

    // Write C code to output file
    fprintf(output_file, "#include <stdint.h>\n\n");
    fprintf(output_file, "const int32_t audio_data[] = {\n");
    fprintf(output_file, "    %d", audio_data[0]);
    for (int i = 1; i < header.subchunk2Size / sizeof(int32_t); i++) {
        if (i % 6 == 0) {
            fprintf(output_file, ",\n    %d", audio_data[i]);
        } else {
            fprintf(output_file, ", %d", audio_data[i]);
        }
    }
    fprintf(output_file, "\n};\n");
    fprintf(output_file, "const int audio_data_size = %d;\n", header.subchunk2Size / sizeof(int32_t));
    fclose(output_file);

    free(audio_data);

    printf("C code generated successfully.\n");

    return 0;
}
