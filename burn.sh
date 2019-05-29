#! /bin/bash
sudo avrdude -p t2313 -c usbasp -U flash:w:light.hex:i
