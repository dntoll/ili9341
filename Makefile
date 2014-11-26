all: ili9341

ili9341: src/main.cc src/ili9341.h src/ili9341.cc
	g++ src/main.cc ili9341.cc -o ili9341  -lwiringPi

clean: 
	rm *.o ili9341
