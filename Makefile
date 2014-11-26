all: ili9341

ili9341: src/main.cc
	g++ src/main.cc -o ili9341  -lwiringPi

clean: 
	rm *.o appendall
