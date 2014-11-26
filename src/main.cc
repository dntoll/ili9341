#include <iostream>
#include "ili9341.h"

using namespace std;

int main()
{
	ili9341 inst;
	inst.setBrightness(255);
	inst.clearScreen();
	inst.setColor(10,10,255,0,0);
	cout << "Hello World \n";

	inst.setBrightness(255);
	return 0;
}
