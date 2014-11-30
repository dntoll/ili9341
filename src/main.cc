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
		inst.setColor(rand()%320, rand()%240, 255, rand()%255, rand()%255);
	}
	cout << "Hello World \n";

	inst.setBrightness(255);
	return 0;
}
