all: ili9341

ili9341: src/main.cc src/ili9341.h src/ili9341.cc src/Rect.h src/Rect.cc src/Color.h src/Color.cc
	g++ src/main.cc src/ili9341.cc -o ili9341  -lwiringPi

clean: 
	rm *.o ili9341
