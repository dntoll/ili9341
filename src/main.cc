#include <iostream>
#include <stdlib.h>
#include "ili9341.h"
#include <wiringPi.h>

using namespace std;

const int CE1 = 11;

int main()
{
	cout << "Started up";
	pinMode(CE1, OUTPUT);
	digitalWrite(CE1, 0);
	ili9341 inst;
	inst.setBrightness(255);
	inst.clearScreen();
	inst.flush();
	
	cout << "Before \n";
	for (int i= 0;i< 5; i++) {
		cout << "Write Some pixels \n";
		inst.clearScreen();

		inst.fillBox(0, 0, 50, 50, 255, 0, 0);
		inst.fillBox(1, 0, 50, 50, 0, 255, 0);
		inst.fillBox(0, 120, 50, 50, 0, 0, 255);
	

		cout << "Wrote Some pixels \n";
		inst.flush();
		sleep(1);
		cout << "Flushed Some pixels \n";
	}
	sleep(5);
	inst.setBrightness(0);
	return 0;
}
