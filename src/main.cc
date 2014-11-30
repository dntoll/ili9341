#include <iostream>
#include <stdlib.h>
#include "ili9341.h"

using namespace std;

int main()
{
	ili9341 inst;
	inst.setBrightness(255);
	
	for (int i= 0;i< 5; i++) {
		cout << "Write Some pixels \n";	
		inst.clearScreen();
		sleep(1);
	
		for (int i = 0; i < 5; i++) {
			inst.setColor(rand()%240, rand()%320, rand()%255, rand()%255, rand()%255);
			inst.fillBox(rand()%240, rand()%320, 50, 50, rand()%255, rand()%255, rand()%255);	
		}
	}
	
	inst.setBrightness(255);
	return 0;
}
