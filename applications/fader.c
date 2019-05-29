/*
 * 
 */ 

#include <util/delay.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "ws2812.h"

#define PIXELS 16
struct cRGB leds[PIXELS];
struct cRGB tmp[PIXELS];

int main(void)
{
        DDRD|=_BV(ws2812_pin);

        uint8_t i;
        for (i = 0; i < PIXELS; i++) {
                leds[i].r = rand() % 48;
                leds[i].g = rand() % 24;
                leds[i].b = rand() % 64;
        }
        tmp[0].r = leds[0].r;
        tmp[0].g = leds[0].g;
        tmp[0].b = leds[0].b;

        uint8_t k = 0;
        while(1) {
                ws2812_sendarray((uint8_t *)leds, PIXELS*3);

                if (leds[0].r < leds[15].r)
                        tmp[0].r = leds[0].r + 1;
                else
                        tmp[0].r = leds[0].r - 1;
                if (leds[0].g < leds[15].g)
                        tmp[0].g = leds[0].g + 1;
                else
                        tmp[0].g = leds[0].g - 1;
                if (leds[0].b < leds[15].b)
                        tmp[0].b = leds[0].b + 1;
                else
                        tmp[0].b = leds[0].b - 1;
                for(i = 1; i < PIXELS; i++) {
                        if (leds[i].r < leds[i - 1].r)
                                tmp[i].r = leds[i].r + 1;
                        else
                                tmp[i].r = leds[i].r - 1;
                        if (leds[i].g < leds[i - 1].g)
                                tmp[i].g = leds[i].g + 1;
                        else
                                tmp[i].g = leds[i].g - 1;
                        if (leds[i].b < leds[i - 1].b)
                                tmp[i].b = leds[i].b + 1;
                        else
                                tmp[i].b = leds[i].b - 1;
                }
                for (i = 0; i < PIXELS; i++) {
                        leds[i].r = tmp[i].r;
                        leds[i].g = tmp[i].g;
                        leds[i].b = tmp[i].b;
                }


                k++;

                _delay_ms(32);
        }
}
