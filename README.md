ili9341
=======

Simple ili9341 lib


Inspiration and direct code has been taken from
https://github.com/notro/fbtft/blob/master/fb_ili9341.c

http://henningkarlsen.com/electronics/library.php?id=51

And

http://www.elecfreaks.com/store/22-tft-lcd-tft0122sp-p-672.html

Pins
----
	static const unsigned char DC   = 4;// # gpio pin 16 = wiringpi no. 4 (BCM 23)
	static const unsigned char RST  = 5;// # gpio pin 18 = wiringpi no. 5 (BCM 24)
	static const unsigned char LED  = 1;// # gpio pin 12 = wiringpi no. 1 (BCM 18)

	//# SPI connection, these are only for documentation...
	static const unsigned char SCE  = 10;// # gpio pin 24 = wiringpi no. 10 (CE0 BCM 8) 
	static const unsigned char SCLK = 14;// # gpio pin 23 = wiringpi no. 14 (SCLK BCM 11)
	static const unsigned char DIN  = 12;// # gpio pin 19 = wiringpi no. 12 (MOSI BCM 10)

Dependencies
-----------
WiringPi
http://wiringpi.com/reference/spi-library/

	sudo apt-get update
	sudo apt-get upgrade
	sudo apt-get install git-core
	git clone git://git.drogon.net/wiringPi
	cd wiringPi
	./build
