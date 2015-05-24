#include <stdio.h>
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include "Gpio.hpp"

using namespace std;
class GpioButton {
public:
	GpioButton();
	virtual ~GpioButton();
	void setPort(GpioPin& port);
	void setFixDuration(int count);
	void updateState();
	bool isStateFixed() const;
	bool getState() const;
private:
	GpioPin* port;
	int fix_duration_count;
	int duration_count;
	bool last_raw_state;
	bool state;
	bool unfixed_state;
	bool state_fixed;
};

void* watch_button_thread(void* args) {
	GpioButton* btn = (GpioButton*)args;
	while(true) {
		btn[0].updateState();
		btn[1].updateState();
        usleep(10000);
	}
}

int main ()
{
	RaspiGpio gpio;
	GpioButton btn[2];
	btn[0].setPort(gpio[7]);
	btn[1].setPort(gpio[8]);
	btn[0].setFixDuration(0);
	btn[1].setFixDuration(0);
	pthread_t thread;
	pthread_create(&thread, NULL, watch_button_thread, btn);
    while (1) {
		bool sw1 = btn[0].getState();
		bool sw2 = btn[1].getState();
		cerr << "sw1:" << sw1 << ", sw2:" << sw2 << endl;
        usleep(0);
    }
    return 0;
}

GpioButton::GpioButton()
: port(0), fix_duration_count(5),duration_count(0),
last_raw_state(false), state(false), state_fixed(true)
{
}
GpioButton::~GpioButton()
{
}
void GpioButton::setPort(GpioPin& port) {
	this->port = &port;
	this->port->setMode(GPIO_INPUT, GPIO_PULLDOWN);
}
void GpioButton::setFixDuration(int count) {
	this->fix_duration_count = count;
}
void GpioButton::updateState() {
	bool raw_state = *port;
	if(fix_duration_count <= 0) {
		state_fixed = true;
		state = raw_state;
	} else {
		if(raw_state == last_raw_state) {
			if(!state_fixed) {
				duration_count++;
				if(duration_count > fix_duration_count) {
					state_fixed = true;
					state = raw_state;
				}
			}
		} else {
			state_fixed = false;
			duration_count = 0;
		}
	}
	last_raw_state = raw_state;
}

bool GpioButton::isStateFixed() const {
	return state_fixed;
}
bool GpioButton::getState() const {
	return state;
}
