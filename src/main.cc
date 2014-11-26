#include <iostream>
#include "ili9341.h"

using namespace std;

int main()
{
	ili9341 inst;
	inst.clearScreen();
	cout << "Hello World \n";
	return 0;
}
