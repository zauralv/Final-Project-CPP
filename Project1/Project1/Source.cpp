#include <iostream>
#include <iomanip>
#include <fstream>
#include "Restaurant.h"

using namespace std;

int main() {
	try {
		ControlRestaurant::Start();
	}
	catch (Exceptions::InvalidArgumentException ex) {
		ex.showError();
	}
	catch (Exceptions::FileWriteException ex) {
		ex.showError();
	}
	return 0;
}