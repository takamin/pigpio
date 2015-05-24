#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "Gpio.hpp"
class GpioByteDisplay : public RaspiGpio {
public:
	void init_ports();
	void set_number(int value);
private: 
	static int pins[];
};

int main (int argc, char* argv[])
{
	GpioByteDisplay gpio;
	gpio.init_ports();
	int n = 0;
	int fixed_number = -1;	
	int wait_microsec = 100000;

	if(argc > 1) {
		int x = atoi(argv[1]);
		if(x < 10) {
			fixed_number = x % 10;
		} else {
			wait_microsec = x;
		}
	}
	if(fixed_number < 0) {
		int n = 0;
		while (1) {
			gpio.set_number(n);	
			n = ++n % 10;
			usleep(wait_microsec);
		}
	} else {
		gpio.set_number(fixed_number);	
	}
    return 0;
}

int GpioByteDisplay::pins[] = {14,3,4,7,8,9,10,11,};
void GpioByteDisplay::init_ports() {
	for(int i = 0; i < sizeof(pins)/sizeof(pins[0]); i++) {
		(*this)[pins[i]].setMode(GPIO_OUTPUT);
	}
}

void GpioByteDisplay::set_number(int value) {
	int pattern[][8] = {
		{ 1,1,1,1,1,1,0,0,},
		{ 0,1,1,0,0,0,0,0,},
		{ 1,1,0,1,1,0,1,0,},
		{ 1,1,1,1,0,0,1,0,},
		{ 0,1,1,0,0,1,1,0,},
		{ 1,0,1,1,0,1,1,0,},
		{ 1,0,1,1,1,1,1,0,},
		{ 1,1,1,0,0,1,0,0,},
		{ 1,1,1,1,1,1,1,0,},
		{ 1,1,1,1,0,1,1,0,},
	};
	int pin_index = value % 10;
	for(int i = 0; i < sizeof(pins)/sizeof(pins[0]); i++) {
		//(*this)[pins[i]] = (i == pin_index) ? 0 : 1;
		(*this)[pins[i]] = (pattern[pin_index][i]) ? 0 : 1;
	}
}
