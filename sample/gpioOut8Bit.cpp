#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "Gpio.hpp"
class GpioByteDisplay : public RaspiGpio {
public:
	void init_ports();
	void write_byte(unsigned char value);
private: 
	static int pins[];
};

int main (int argc, char* argv[])
{
	GpioByteDisplay gpio;
	gpio.init_ports();
	int n = 0;
	int fixed_bit_pattern = -1;	
	int wait_microsec = 100000;

	if(argc > 1) {
		int x = atoi(argv[1]);
		if(x < 256) {
			fixed_bit_pattern = (int)((unsigned)x)&0xff;
		} else {
			wait_microsec = x;
		}
	}
	if(fixed_bit_pattern < 0) {
		int n = 0;
		while (1) {
			gpio.write_byte((unsigned char)n);	
			n = ++n % 256;	
			usleep(wait_microsec);
		}
	} else {
		gpio.write_byte((unsigned char)fixed_bit_pattern);	
	}
    return 0;
}

int GpioByteDisplay::pins[] = {2,3,4,7,8,9,10,11,};
void GpioByteDisplay::init_ports() {
	for(int i = 0; i < sizeof(pins)/sizeof(pins[0]); i++) {
		(*this)[pins[i]].setMode(GPIO_OUTPUT);
	}
}

void GpioByteDisplay::write_byte(unsigned char value) {
	for(int i = 0; i < sizeof(pins)/sizeof(pins[0]); i++) {
		(*this)[pins[i]] = ((value & (1 << i)) != 0);
	}
}
