#include <iostream>
#include "InputPoller.h"

int main(void) {
	InputPoller* poller = new InputPoller();

	delete poller;
	return 0;
}


