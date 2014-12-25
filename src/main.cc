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
		cout << "Write Some pixels";
		inst.clearScreen();
		
	
		for (int i = 0; i < 5; i++) {
			//inst.setColor(rand()%240, rand()%320, rand()%255, rand()%255, rand()%255);
			inst.fillBox(rand()%240, rand()%320, 50, 50, 255, 255, 0);

		}
		cout << "Wrote Some pixels \n";
		inst.flush();
		sleep(1);
		cout << "Flushed Some pixels \n";
	}
	
	inst.setBrightness(0);
	return 0;
}
