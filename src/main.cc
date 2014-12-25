#include <iostream>
#include <stdlib.h>
#include "ili9341.h"
#include <wiringPi.h>

using namespace std;

const int CE1 = 11;

int main()
{
	pinMode(CE1, OUTPUT);
	digitalWrite(CE1, 0);
	ili9341 inst;
	inst.setBrightness(255);
	
	for (int i= 0;i< 5; i++) {
		cout << "Write Some pixels \n";	
		inst.clearScreen();
		
	
		for (int i = 0; i < 5; i++) {
			//inst.setColor(rand()%240, rand()%320, rand()%255, rand()%255, rand()%255);
			inst.fillBox(rand()%240, rand()%320, 50, 50, 255, 0, 255);

		}
		sleep(1);
	}
	
	inst.setBrightness(255);
	return 0;
}
