CPP_FILES := $(wildcard src/*.cc)
OBJ_FILES := $(addprefix obj/,$(notdir $(CPP_FILES:.cc=.o)))


all: ili9341


ili9341: $(OBJ_FILES)
	g++ -g -lwiringPi -o $@ $^ 
	
obj/%.o: src/%.cc
	g++ -g $(CC_FLAGS) -c -o $@ $<

#ili9341: src/main.cc src/ili9341.h src/ili9341.cc src/Rect.h src/Rect.cc src/Color.h src/Color.cc
#	g++ src/main.cc src/ili9341.cc -o ili9341  -lwiringPi

clean: 
	rm obj/*.o ili9341

reload:
	git pull
	make
	sudo ./ili9341