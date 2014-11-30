#include <iostream>
#include <stdlib.h>
#include "ili9341.h"

using namespace std;

int main()
{
	ili9341 inst;
	inst.setBrightness(255);
	
	inst.clearScreen();
	cout << "Write Some pixels \n";	

	for (int i = 0; i < 100; i++) {
		inst.setColor(rand()%240, rand()%320, rand()%255, rand()%255, rand()%255);
		inst.fillBox(rand()%240, rand()%320, 50, 50, rand()%255, rand()%255, rand()%255);	
	}
	
	inst.fillBox(10, 10, 100,100, 255,255,255);
	cout << "Hello World \n";

	inst.setBrightness(255);
	return 0;
}
