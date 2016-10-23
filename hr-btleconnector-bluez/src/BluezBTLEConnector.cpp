/*
 * BluezBTLEConnector.cpp
 *
 *  Created on: Oct 22, 2016
 *      Author: manuel
 */

#include "BluezBTLEConnector.h"

namespace homerunner {
namespace connect {

BluezBTLEConnector::BluezBTLEConnector() {
	// TODO Auto-generated constructor stub

}

BluezBTLEConnector::~BluezBTLEConnector() {
	// TODO Auto-generated destructor stub
}

} /* namespace connect */
} /* namespace homerunner */

#pragma GCC visibility push(default)

extern "C" vnl::Object* create_module() {
	return new homerunner::connect::BluezBTLEConnector();
}

extern "C" int destroy_module(vnl::Object* obj) {
	if(obj && dynamic_cast<homerunner::connect::BluezBTLEConnector>(obj)) {
		delete dynamic_cast<homerunner::connect::BluezBTLEConnector>(obj);
		return 0;
	}
	return -1;
}

#pragma GCC visibility pop

int destroy_module(vnl::Object* mod);
