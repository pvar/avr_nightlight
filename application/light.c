/*
 * 
 */ 

#include <util/delay.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "ws2812.h"

// actual colour values on LEDs
struct cRGB leds[18];

// preset colours values
struct cRGB cols[8];

// value to reach
struct cRGB tmp_col;

uint8_t init;


/*
 * Get next window-light
 */
uint8_t next_win (uint8_t win)
{
        if (win >= 16)
                win = 0;
        else
                win += 2;

        return win;
}

/*
 * Get a random window-light
 */
uint8_t rand_win (uint8_t win)
{
        uint8_t tmp = win;
        while (tmp == win)
                win = 2 * (rand() % 9);

        return win;
}

/*
 * Turn window-lights on, in sequential or random order
 */
void windows (void)
{
        static uint8_t win_step = 0;
        static uint8_t win_mode = 0;
        static uint8_t cnt = 0;
        static uint8_t win = 0;
        static uint8_t mode = 0;
        uint8_t tmp;

        // initialize colours -- execute only once
        if (init) {
                // turn all LEDs off
                for (tmp = 0; tmp < 18; tmp++) {
                        leds[tmp].r = 0;
                        leds[tmp].g = 0;
                        leds[tmp].b = 0;
                }
                init = 0;
        }

        // check if fading has finished
        if (cnt == 0) {
                // check if time to switch windows
                if (mode == 0) {
                        // increase window counter/step
                        win_step++;
                        // check if time to change mode
                        if (win_step == 16) {
                                win_step = 0;
                                win_mode = 1 - win_mode;
                        }
                        // pick another window
                        if (win_mode == 0)
                                win = rand_win(win);
                        else
                                win = next_win(win);
                        // turn specific light on with random colour
                        tmp = rand() % 8;
                        tmp_col = cols[tmp];
                }
                // reset fade counter
                cnt = 130;
                // toggle mode
                mode = 1 - mode;
        } else {
                if (mode) {
                        if (leds[win].r > tmp_col.r)
                                leds[win].r -= 1;
                        else
                                leds[win].r += 1;

                        if (leds[win].g > tmp_col.g)
                                leds[win].g -= 1;
                        else
                                leds[win].g += 1;

                        if (leds[win].b > tmp_col.b)
                                leds[win].b -= 1;
                        else
                                leds[win].b += 1;

                        leds[win + 1] = leds[win];
                } else {
                        if (leds[win].r > 0)
                                leds[win].r -= 1;
                        if (leds[win].g > 0)
                                leds[win].g -= 1;
                        if (leds[win].b > 0)
                                leds[win].b -= 1;

                        leds[win + 1] = leds[win];
                }
        }

        // decrease fade counter
        cnt--;
        // insert fading delay
        _delay_ms(32);
}

/*
 * Rotate rainbow-like colours on all LEDs
 */
void rotate (void)
{
        static uint8_t cnt = 0;
        static uint8_t col = 0;
        uint8_t i, tmp1, tmp2;

        // initialize colours -- execute only once
        if (init) {
                cnt = 254;
                for(i = 0; i < 9; i++) {
                        tmp1 = 2 * i;
                        tmp2 = (i + col) % 8;
                        leds[tmp1] = cols[tmp2];
                        leds[tmp1 + 1] = cols[tmp2];
                }
                init = 0;
        }

        // check if time to shift colours
        if (cnt == 0) {
                // delay before changing colours
                _delay_ms(250);
                // reset counter
                cnt = 130;
                // change colour base-index
                col++;
                if (col > 7)
                        col = 0;
        }

        // cross-fade colours on LEDs
        for (i = 0; i < 9; i++) {
                tmp1 = 2 * i;
                tmp2 = (i + 1 + col) % 8;

                if(leds[tmp1].r < (cols[tmp2].r - 1))
                        leds[tmp1].r +=1;
                if(leds[tmp1].r > (cols[tmp2].r + 1))
                        leds[tmp1].r -= 1;

                if(leds[tmp1].g < (cols[tmp2].g - 1))
                        leds[tmp1].g += 1;
                if(leds[tmp1].g > (cols[tmp2].g + 1))
                        leds[tmp1].g -= 1;

                if(leds[tmp1].b < (cols[tmp2].b - 1))
                        leds[tmp1].b += 1;
                if(leds[tmp1].b > (cols[tmp2].b + 1))
                        leds[tmp1].b -= 1;

                leds[tmp1 + 1] = leds[tmp1];
        }

        // decrease colour fading counter
        cnt--;
        // insert fading delay
        _delay_ms(8);
}

int main(void)
{
        // configure pin for driving LEDs
        DDRD|=_BV(ws2812_pin);

        // preset colours
        cols[0].r = 100; cols[0].g = 0;   cols[0].b = 0;   // red
        cols[1].r = 128; cols[1].g = 128; cols[1].b = 0;   // yellow
        cols[2].r = 0;   cols[2].g = 128; cols[2].b = 0;   // green
        cols[3].r = 128; cols[3].g = 83;  cols[3].b = 0;   // orange
        cols[4].r = 15;  cols[4].g = 72;  cols[4].b = 128; // cyan
        cols[5].r = 64;  cols[5].g = 0;   cols[5].b = 64;  // violet
        cols[6].r = 68;  cols[6].g = 52;  cols[6].b = 10;  // brown
        cols[7].r = 0;   cols[7].g = 0;   cols[7].b = 128; // blue

        // turn all LEDs off
        uint8_t i;
        for (i = 0; i < 18; i++) {
                leds[i].r = 0;
                leds[i].g = 0;
                leds[i].b = 0;
        }

        // find a way to randomize timer
        // get some measurement k
        // use srand(k)

        // loop forever
        uint16_t step = 0;
        uint8_t mode = 0;
        init = 1;
        while(1) {
                // push data to LEDs
                ws2812_sendarray((uint8_t *)leds, 54);

                if (step == 48000) {
                        // reset step counter
                        step = 0;
                        // change mode of operation
                        mode = (mode + 1) & 1;
                        // perform mode-specific initialization
                        init = 1;
                }

                if (mode == 0)
                        rotate();
                else
                        windows();

                step++;
        }
}
