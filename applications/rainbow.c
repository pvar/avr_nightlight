/*
 * 
 */ 

#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "ws2812.h"

#define PIXELS 18
#define COLORLENGTH (PIXELS/2)
#define FADE 32

struct cRGB colors[8];
struct cRGB led[PIXELS];

int main(void)
{
        DDRD|=_BV(ws2812_pin);

        uint8_t i;
        for (i = 0; i < PIXELS; i++) {
                led[i].r=64;
                led[i].g=32;
                led[i].b=128;
        }
		
        //Rainbowcolors
        colors[0].r=128; colors[0].g=128; colors[0].b=128;      // white
        colors[1].r=128; colors[1].g=000; colors[1].b=000;      // red
        colors[2].r=128; colors[2].g=64; colors[2].b=000;       // orange
        colors[3].r=64; colors[3].g=128; colors[3].b=000;       // yellow
        colors[4].r=000; colors[4].g=128; colors[4].b=000;      // green
        colors[5].r=000; colors[5].g=64; colors[5].b=128;       // light blue
        colors[6].r=000; colors[6].g=000; colors[6].b=128;      // blue
        colors[7].r=64; colors[7].g=000; colors[7].b=128;       // violet
    
        uint8_t j = 1;
        uint8_t k = 1;
        while(1) {
        //shift all vallues by one led
        uint8_t i=0;           
        for(i=PIXELS; i>1; i--) 
                led[i-1]=led[i-2];

        //change colour when colourlength is reached   
        if(k>COLORLENGTH) {
                j++;
                if(j>7)
                        j=0;
                k=0;
        }
        k++;

        //loop colours
        if(led[0].r<(colors[j].r-FADE))
                led[0].r+=FADE;

        if(led[0].r>(colors[j].r+FADE))
                led[0].r-=FADE;

        if(led[0].g<(colors[j].g-FADE))
                led[0].g+=FADE;

        if(led[0].g>(colors[j].g+FADE))
                led[0].g-=FADE;

        if(led[0].b<(colors[j].b-FADE))
                led[0].b+=FADE;

        if(led[0].b>(colors[j].b+FADE))
                led[0].b-=FADE;

        _delay_ms(64);
        ws2812_sendarray((uint8_t *)led, PIXELS*3);
    }
}
