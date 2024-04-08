#include "common.h"
#include <assert.h>

void display_ending_screen();
void show_HEX(int a);

void clear_HEX();

  void again_call();

   volatile unsigned int *HEX3_HEX0_ptr = (unsigned int *)0xFF200020;
    volatile unsigned int *HEX5_HEX4_ptr = (unsigned int *)0xFF200030;
  char message[] = "GOOD"; 
  char blank[] = "      "; 
   char mes2[] = "bAd";
struct videoout_t volatile *const vp = ((struct videoout_t *)0xFF203020);
struct audio_t *const audiop = ((struct audio_t *)0xff203040);
  volatile int *key_ptr = (volatile int *)0xFF20005C;


int score_value;
  


  
      void audio_playback_mono(const int *p, int n) {

            int i;

            audiop->control = 0x8; // clear the output FIFOs
            audiop->control = 0x0; // resume input conversion
            for (i = 0; i < n; i++) {
              // wait till there is space in the output FIFO
              while (audiop->warc == 0);
              audiop->ldata = p[i];
              audiop->rdata = p[i];
              
             }
       }	
      
       char display_char(char c)
   {
    char seg7_code;
    switch (c)
    {
    case 'G':
        seg7_code = 0b01101111;
        break;
    case 'O':
        seg7_code = 0b00111111;
        break;
    case 'D':
        seg7_code = 0b01011110;
        break;
    case 'b': // Change to represent 'B' pattern
        seg7_code = 0b01111100; // 'b' pattern changed to 'B'
        break;
    case 'A':
        seg7_code = 0b01110111;
        break;
    case 'd': // Define pattern for 'd'
        seg7_code = 0b01011110;
        break;
    default:
        seg7_code = 0;
        break;
    }
    return seg7_code;
} 
     

  //clear the hex
  void clear_HEX() {
    char *pmessage;
    unsigned int hex3_hex0_value = 0;
    
    pmessage = blank;
    hex3_hex0_value |= display_char(*pmessage) ;
    *HEX3_HEX0_ptr = hex3_hex0_value;
  }



// Clear the screen with black colour
void clear_screen(struct fb_t *buffer) {
    for (int i = 0; i < VGA_Y; i++) {
        for (int j = 0; j < VGA_X; j++) {
            buffer->pixels[i][j] = 0;
        }
    }
}

// Draw images with the transparent background (ignore black colour)
void draw_image(struct fb_t *buffer, const short *image, int x_array_size, int y_array_size, int x, int y) {
    int index = 0;
    for (int i = 0; i < y_array_size; i++) {
        for (int j = 0; j < x_array_size; j++) {
            if (image[index] != 0) {
                buffer->pixels[i + y][j + x] = image[index];
            }
            index++;
        }
    }
}

// Draw background images
void draw_background(struct fb_t *buffer, const short *image) {
    int index = 0;
    for (int i = 0; i < VGA_Y; i++) {
        for (int j = 0; j < VGA_X; j++) {
            buffer->pixels[i][j] = image[index];
            index++;
        }
    }
}

const short *pick_number(int score_value) {
    switch (score_value) {
        case 0:
            return zero;
        case 1:
            return one;
        case 2:
            return two;
        case 3:
            return three;
        case 4:
            return four;
        case 5:
            return five;
        case 6:
            return six;
        case 7:
            return seven;
        case 8:
            return eight;
        case 9:
            return nine;
        default:
            return zero;
    }
}

const short *pick_big_number(int score_value) {
    switch (score_value) {
        case 0:
            return zero_big;
        case 1:
            return one_big;
        case 2:
            return two_big;
        case 3:
            return three_big;
        case 4:
            return four_big;
        case 5:
            return five_big;
        case 6:
            return six_big;
        case 7:
            return seven_big;
        case 8:
            return eight_big;
        case 9:
            return nine_big;
        default:
            return zero_big;
    }
}

bool is_vsync_ready() {
    return (vp->StatusControl & 1) == 0;
}

void request_vsync() {
  assert(is_vsync_ready());
  // Set front buffer 1 to request a swap
  // Status bit becomes 0 when the front buffer finishes rendering
  vp->fbp = (struct fb_t *)1;
}


// Synchronize with VGA controller
/*void wait_for_vsync() {
  // Set front buffer 1 to request a swap
  // Status bit becomes 0 when the front buffer finishes rendering
  vp->fbp = (struct fb_t *)1;
  // Wait for current displayed frame to finish
  while ((vp->StatusControl & 1) != 0);
}

 */

    void wait_for_vsync() {
    // Save the original front frame buffer
    struct fb_t *volatile original_fbp = vp->fbp;

    // Set front buffer 1 to request a swap
    vp->fbp = (struct fb_t *)1;

    // Wait for current displayed frame to finish rendering
    while ((vp->StatusControl & 1) != 0);

    // Restore the original front frame buffer
    vp->fbp = original_fbp;
}
void draw_pitch_levels(struct fb_t *buffer, int start_x, bool draw) {
    int temp_x, colour, i, j;
    int pitch_offset = 0;
    int pixel_offset = 0;

    // If the bar hasn't arrived at x = 0, keep decrementing the starting x location
    if (start_x >= 0) {
        temp_x = start_x;
    }
    // Else, find the offsets to remove the hidden bars
    else {
        temp_x = 0;
        pitch_offset = -start_x / PITCH_BAR_LENGTH;
        pixel_offset = -start_x % PITCH_BAR_LENGTH;
    }

    // For loop to assign the next image in the back buffer
    for (i = pitch_offset; i < PITCH_ARRAY_LENGTH; i++) { // num of pitch_array
        

        if (temp_x >= VGA_X) {break;}

        for (j = 0; j < PITCH_BAR_LENGTH - pixel_offset; j++) {
            if (temp_x >= VGA_X) {break;}

            if (pitch_array[i] == 239 || !draw) {
                colour = titanic_dark[pitch_array[i] * VGA_X + temp_x]; // index of 1-D array of titanic_dark
            }
            else {
                colour = 0xFFFF;
            }

            buffer->pixels[pitch_array[i]][temp_x] = colour;
            buffer->pixels[pitch_array[i] + 1][temp_x] = colour;

            temp_x++;
        }
        // After removing the first element, set the offset to zero
        pixel_offset = 0;
    }
}


// Display the title screen at the start of the game
void display_title_screen() {
    // Draw the background image
    draw_background(vp->fbp, titanic);
  
    // Draw the title
    int index = 0;
    for (int i = 0; i < TITLE_HEIGHT; i++) {
        for (int j = 0; j < TITLE_WIDTH; j++) {
            if (title[index] != 0) {
                vp->fbp->pixels[i + 10][j + 10] = 0xffff;
            }
            index++;
        }
    }

    // Draw the instruction to start the game
    draw_image(vp->fbp, start, START_WIDTH, START_HEIGHT, (VGA_X - START_WIDTH) / 2, 220);
}

// Function to calculate Zero Crossing Rate (ZCR)
float calculate_zcr(int32_t *audio_data) {
    int zcr_count = 0;
    int i;

    // Loop through the signal within the specified range
    for (i = 1; i < CHUNK_SIZE; i++) {
        // Check for zero crossings
        if ((audio_data[i] >= 0 && audio_data[i - 1] < 0) || (audio_data[i] < 0 && audio_data[i - 1] >= 0)) {
            zcr_count++;
        }
    }

    // Calculate ZCR (zero crossings per second)
    float zcr = (float)zcr_count / (float)(CHUNK_SIZE) * SAMPLE_RATE;

    // Processing for VGA display
    if (zcr < 500) {
        zcr = 210;
    }
    else if (zcr > 2000) {
        zcr = 30;
    }
    else {
        zcr = 250 - ((zcr - 500) * (210 - 30) / (2000 - 500));
    }

    return zcr;
}


int compare_pitch(int song_pitch, int user_pitch) {
    int diff = abs(song_pitch - user_pitch);
    if (diff < 20) {
        return 1;
    }
    return 0;
}

void display_lyrics(int pitch_index) {
    if (pitch_index == 50 || pitch_index == 60 || pitch_index == 79 || pitch_index == 88 || 
        pitch_index == 99 || pitch_index == 118 || pitch_index == 134 || pitch_index == 156 || 
        pitch_index == 173 || pitch_index == 182) {
        for (int i = LYRICS_Y; i < (LYRICS_Y + LYRICS_HEIGHT); i++) {
            for (int j = 0; j < VGA_X; j++) {
                vp->bfbp->pixels[i][j] = 0x0000;
            }
        }
    }

    switch (pitch_index) {
        case 40:
            draw_image(vp->bfbp, lyrics_1, LYRICS_1_WIDTH, LYRICS_HEIGHT, VGA_X / 2 - LYRICS_1_WIDTH / 2, LYRICS_Y);
            break;
        case 50:
            draw_image(vp->bfbp, lyrics_2, LYRICS_2_WIDTH, LYRICS_HEIGHT, VGA_X / 2 - LYRICS_2_WIDTH / 2, LYRICS_Y);
            break;
        case 60:
            draw_image(vp->bfbp, lyrics_3, LYRICS_3_WIDTH, LYRICS_HEIGHT, VGA_X / 2 - LYRICS_3_WIDTH / 2, LYRICS_Y);
            break;
        case 79:
            draw_image(vp->bfbp, lyrics_4, LYRICS_4_WIDTH, LYRICS_HEIGHT, VGA_X / 2 - LYRICS_4_WIDTH / 2, LYRICS_Y);
            break;
        case 88:
            draw_image(vp->bfbp, lyrics_5, LYRICS_5_WIDTH, LYRICS_HEIGHT, VGA_X / 2 - LYRICS_5_WIDTH / 2, LYRICS_Y);
            break;
        case 99:
            draw_image(vp->bfbp, lyrics_6, LYRICS_6_WIDTH, LYRICS_HEIGHT, VGA_X / 2 - LYRICS_6_WIDTH / 2, LYRICS_Y);
            break;
        case 118:
            draw_image(vp->bfbp, lyrics_7, LYRICS_7_WIDTH, LYRICS_HEIGHT, VGA_X / 2 - LYRICS_7_WIDTH / 2, LYRICS_Y);
            break;
        case 134:
            draw_image(vp->bfbp, lyrics_8, LYRICS_8_WIDTH, LYRICS_HEIGHT, VGA_X / 2 - LYRICS_8_WIDTH / 2, LYRICS_Y);
            break;
        case 156:
            draw_image(vp->bfbp, lyrics_9, LYRICS_9_WIDTH, LYRICS_HEIGHT, VGA_X / 2 - LYRICS_9_WIDTH / 2, LYRICS_Y);
            break;
        case 173:
            draw_image(vp->bfbp, lyrics_10, LYRICS_10_WIDTH, LYRICS_HEIGHT, VGA_X / 2 - LYRICS_10_WIDTH / 2, LYRICS_Y);
            break;
        case 182:
            draw_image(vp->bfbp, lyrics_11, LYRICS_11_WIDTH, LYRICS_HEIGHT, VGA_X / 2 - LYRICS_11_WIDTH / 2, LYRICS_Y);
            break;
    }
}


     





       
    int play_game () {
        while(1) {


             if (*key_ptr & 0x2) { 
            printf("key is pressed");
            clear_screen(vp->fbp);
            display_ending_screen();
            printf("loop ends");
            printf("dont do");
            audio_playback_mono(ends, end_sample);
            show_HEX(score_value);
            // again_call(); // Uncomment this line if you want to call again_call() function
             break; // Exit the function if key2 is pressed
        }
    audiop->control = 0x8; // clear the output FIFOs
    audiop->control = 0x0; // resume input conversion
    
    draw_background(vp->fbp, titanic_dark);
    
    draw_image(vp->fbp, score, SCORE_WIDTH, SCORE_HEIGHT, SCORE_X, SCORE_Y);
        
    // Swap the front buffer and back buffer
    struct fb_t *volatile temp = vp->fbp;
    *vp->fbp = *vp->bfbp;
  
    // Initialize the location of the pitch levels
    int start_x = VGA_X - 1;
    score_value = 0; // arbitrary number for testing
    int index = 0;
    int pitch_index = 0;
    int left, right;
    int user_pitch = 0;
    int score_change = 0;
    int32_t inputAudio[CHUNK_SIZE] = {0};

    draw_image(vp->fbp, pick_number(score_value), SCORE_SMALL_WIDTH, SCORE_SMALL_HEIGHT, SCORE2_X, SCORE_Y);

    draw_pitch_levels(vp->fbp, start_x, true);
        *key_ptr = 0xF;
    for (int i = 0; i < SAMPLES_N; i++) {
        // wait till there is space in the output FIFO

         
        while (audiop->warc == 0);

        left = audiop->ldata;
        right = audiop->rdata;

        audiop->ldata = left;
        audiop->rdata = samples[i];

        inputAudio[index] = left;
        index++;

        if (!is_vsync_ready()) continue;


         
        // Swap the front buffer and back buffer
        struct fb_t *volatile temp = vp->fbp;
        *vp->fbp = *vp->bfbp;
        *vp->bfbp = *temp;

        
       
        // Delete the pitch lines
        draw_pitch_levels(vp->bfbp, start_x, false);

    

        // Update the x location of the lines (moving to the left)
        start_x = VGA_X - 1 - (3 * i/450);
        draw_pitch_levels(vp->bfbp, start_x, true);

       
        // Update the user pitch icon
        if (index >= CHUNK_SIZE) {
            // Delete the user pitch icon
            if (pitch_index != 0) {
                for (int i = 37; i < 44; i++) {
                    for (int j = user_pitch - 3; j < (user_pitch + 4); j++) {
                        vp->bfbp->pixels[j][i] = titanic_dark[i + VGA_X * j];
                    }
                }
            }

            index = 0;
            user_pitch = (int)calculate_zcr(inputAudio);
            //printf("%d\n", user_pitch); 
            

            // Compare user pitch with song pitch
            score_change = compare_pitch(pitch_array[pitch_index], user_pitch);
            if (score_change == 1) {
                score_value += 1;
                user_pitch = pitch_array[pitch_index];

                // Delete the score by overwriting this portion with the background image
                for (int i = SCORE_Y; i < (SCORE_Y + SCORE_SMALL_HEIGHT); i++) {
                    for (int j = SCORE1_X; j < (SCORE1_X + SCORE_SMALL_WIDTH * 2); j++) {
                        vp->bfbp->pixels[i][j] = 0x0000;
                    }
                }

                // Draw a lighting effect when the pitches match
                for (int i = PITCH_LINE_X - 3; i <= PITCH_LINE_X + 3; i++) {
                    for (int j = user_pitch - 3; j <= user_pitch + 3; j++) {
                        vp->bfbp->pixels[j][i] = GREY;
                    }
                }

                // Update the score
                if (score_value < 10) {
                    draw_image(vp->bfbp, pick_number(score_value), SCORE_SMALL_WIDTH, SCORE_SMALL_HEIGHT, SCORE1_X, SCORE_Y);
                }
                else {
                    draw_image(vp->bfbp, pick_number(score_value / 10), SCORE_SMALL_WIDTH, SCORE_SMALL_HEIGHT, SCORE1_X, SCORE_Y);
                    draw_image(vp->bfbp, pick_number(score_value % 10), SCORE_SMALL_WIDTH, SCORE_SMALL_HEIGHT, SCORE2_X, SCORE_Y);
                }
            }
             //Draw user pitch lines
            for (int i = 0; i < VGA_Y; i++) {
                vp->bfbp->pixels[i][40] = 0x977e;
            }
               
            // Draw a new user pitch icon
            for (int i = 38; i <= 42; i++) {
                for (int j = user_pitch - 2; j <= user_pitch + 2; j++) {
                    vp->bfbp->pixels[j][i] = 0xffff;
                }
            }

            pitch_index++;
            display_lyrics(pitch_index);
        }
                
        request_vsync();
    }
       clear_screen(vp->fbp);
       display_ending_screen();
     printf("loop ends");
     printf("dont do");
    audio_playback_mono(ends, end_sample);
    show_HEX(score_value);
    //again_call();
      
        }
    return -1;
        }
     


 void again_call() {
    printf("NOOOOOO");
     int val;
     *key_ptr = 0xF;
     /*/ while (1) {
        val = *key_ptr;
        if (val == 0b1000) {
            break;
        }
    }
     display_title_screen();

    */
      while (1) {
        val = *key_ptr;
         if ((val & 1) == 1) {
            break;
         }
       }
        play_game();


 } 



void display_ending_screen() {
    clear_screen(vp->fbp);
    
    draw_background(vp->fbp, titanic_dark);

    // Ending screen
    draw_image(vp->fbp, score, SCORE_WIDTH, SCORE_HEIGHT, SCORE_BIG_X - SCORE_BIG_WIDTH / 2, SCORE_BIG_Y - 50);

    if (score_value == 0) {
        draw_image(vp->fbp, zero, SCORE_BIG_WIDTH, SCORE_BIG_HEIGHT, SCORE_BIG_X, SCORE_BIG_Y);
    }
    else if (score_value < 10) {
        draw_image(vp->fbp, pick_number(score_value), SCORE_BIG_WIDTH, SCORE_BIG_HEIGHT, SCORE_BIG_X, SCORE_BIG_Y);
    }
    else {
        draw_image(vp->fbp, pick_big_number(score_value / 10), SCORE_BIG_WIDTH, SCORE_BIG_HEIGHT, SCORE_BIG_X - SCORE_BIG_WIDTH, SCORE_BIG_Y);
        draw_image(vp->fbp, pick_big_number(score_value % 10), SCORE_BIG_WIDTH, SCORE_BIG_HEIGHT, SCORE_BIG_X, SCORE_BIG_Y);
    }
    
}


    void show_HEX(int a) {
     
    char *pmessage;
    int display = 1, count = 0;
    // Set address pointers to the I/O ports

      

    while (1) {
        // Toggle display based on score
        
        // Display messageif (score >= 5) {
             

             if (score_value >= 50) {
            if (display) 
                pmessage = message; 
            else 
                pmessage = blank; 
        } else {
            if (display) 
                pmessage = mes2; 
            else 
                pmessage = blank; 
        }
        unsigned int hex3_hex0_value = 0; // Initialize the value to be written to HEX3_HEX0_ptr

        hex3_hex0_value |= display_char(*pmessage) << 24;
        hex3_hex0_value |= display_char(*(pmessage + 1)) << 16;
        hex3_hex0_value |= display_char(*(pmessage + 2)) << 8;
        hex3_hex0_value |= display_char(*(pmessage + 3));

        *HEX3_HEX0_ptr = hex3_hex0_value; // Set the value of HEX3_HEX0_ptr register

        for (count = 0; count < 10000000; ++count)
            ;
        display ^= 1; // toggle

        // Add delay for flashing effect
        for (count = 0; count < 10000000; ++count)
            ;
            if (*key_ptr & 0x8) { 
                  display_title_screen();
                  clear_HEX();
            again_call(); // Call the function again_call if key3 is pressed
            return; // Exit the current function
        }
            }
          

    }



   int main(void) {
    int val;
    clear_screen(vp->fbp);
    clear_HEX();
    *key_ptr = 0b1111;

    // Display title screen
    display_title_screen();
     

    // Wait for key 0 press to start the game
    while (1) {
        val = *key_ptr;
        if (val ==1) {
            break;
        }
    }

    *key_ptr = 0b1111; // Reset the edge capture for key 0

    // Start the game play
    int finalScore = play_game();

    // If the game has ended, display the ending screen
    if (finalScore == -1) {
        display_ending_screen();
        wait_for_vsync();
    }

    return 0;
}