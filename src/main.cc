#include <iostream>
#include <stdlib.h>
#include "BackBuffer.h"
#include <wiringPi.h>

using namespace std;

const int CE1 = 11;

int main()
{
	cout << "Started up" << endl;
	pinMode(CE1, OUTPUT);
	digitalWrite(CE1, 0);


	ili9341 lcd;
	BackBuffer inst(lcd);

	lcd.setBrightness(255);

	inst.clearScreen();
	cout << "Before flush" << endl;
	inst.flush();
	cout << "After flush" << endl;
	sleep(1);


	//inst.test();
	inst.fillBox(Rect(lcd.WIDTH/2-5, lcd.HEIGHT/2-5, 30, 12),Color::WHITE);

	inst.fillBox(Rect(0, 0, 25, 50), Color(255, 	0, 		0));
	inst.fillBox(Rect(lcd.WIDTH-25, 0, 25, 50), 	Color(0, 		255, 	0));
	inst.fillBox(Rect(0, lcd.HEIGHT-50, 25, 50), 	Color(0, 		0, 		255));
	inst.fillBox(Rect(lcd.WIDTH-25, lcd.HEIGHT-50, 25, 50),Color(255, 	0, 		255));



	inst.fillBox(Rect(-25, lcd.HEIGHT/2-25, 50, 50), Color::WHITE);
	inst.fillBox(Rect(lcd.WIDTH-25, lcd.HEIGHT/2-25, 50, 50), Color::WHITE);
	inst.fillBox(Rect(lcd.WIDTH/2-25, -25, 50, 50), Color::WHITE);
	inst.fillBox(Rect(lcd.WIDTH/2-25, lcd.HEIGHT-25, 50, 50), Color::WHITE);

	inst.flush();
	sleep(1);
	/*cout << "Before \n";
	inst.clearScreen();
	for (int i= 0;i< 60; i++) {
		//cout << "Write Some pixels \n";
		inst.fillBox(Rect(rand()%320, rand()%240, rand()%320, rand()%240),
						Color(rand()%255, rand()%255, rand()%255));
		inst.fillBox(Rect(rand()%320, rand()%240, rand()%320, rand()%240),
						Color(rand()%255, rand()%255, rand()%255));
		inst.fillBox(Rect(rand()%320, rand()%240, rand()%320, rand()%240),
				Color(rand()%255, rand()%255, rand()%255));
		inst.flush();

		//sleep(1);
		//cout << "Flushed Some pixels \n";
	}
*/
	sleep(3);

	lcd.setBrightness(0);
	return 0;
}
