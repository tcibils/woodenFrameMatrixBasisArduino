General code for arduino LED matrix, for future usage.

*Two different cases of soldering the LED Matrix*

If matrix (0,0) is top left, then in function outputDisplay, you need "columnIndex%2 == 1" first, for formula leds[(columnIndex + 1)*displayNumberOfRows - rowIndex - 1], and "columnIndex%2 == 0" second, for formula leds[columnIndex*displayNumberOfRows + rowIndex] (this is the case for pre-soldered LED Matrix)

If matrix (0,0) is bottom left, then in function outputDisplay, you need "columnIndex%2 == 0" first, for formula leds[(columnIndex + 1)*displayNumberOfRows - rowIndex - 1], and "columnIndex%2 == 1" second, for formula leds[columnIndex*displayNumberOfRows + rowIndex](this is the case for my home made LED Matrix)


*SNES Controller : links are as below :*

Black = ground

Green = +5V (wtf, I know)

Yellow = Clock

White = Latch

Red = Data


*Sources for SNES :*

Code is copied from here : https://skyduino.wordpress.com/2014/02/23/tuto-arduino-et-gamepad-de-snes/

Source of the article : https://gamefaqs.gamespot.com/snes/916396-snes/faqs/5395
