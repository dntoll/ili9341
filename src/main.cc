#include <iostream>
#include "ili9341.h"

using namespace std;

int main()
{
	ili9341 inst;
	inst.setBrightness(255);
	inst.clearScreen();
	cout << "Hello World \n";

	inst.setBrightness(0);
	return 0;
}
