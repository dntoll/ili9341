all: ili9341

ili9341: src/main.cc src/ili9341.h src/ili9341.cc
	g++ src/main.cc src/ili9341.cc -o ili9341  -lwiringPi
	./ili9341

clean: 
	rm *.o ili9341
