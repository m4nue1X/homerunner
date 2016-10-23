/*
 * BluezBTLEConnector.cpp
 *
 *  Created on: Oct 22, 2016
 *      Author: manuel
 */

#include "BluezBTLEConnector.h"
#include <homerunner/control/SwitchCommand.hxx>

namespace homerunner {
namespace connect {

BluezBTLEConnector::BluezBTLEConnector() : BTLEConnectorBase("BTLEConnector", "BluezBTLEConnector") {
	// TODO Auto-generated constructor stub
}

BluezBTLEConnector::~BluezBTLEConnector() {
	// TODO Auto-generated destructor stub
}

void BluezBTLEConnector::main() {
	log(INFO).out << "BluezBTLEConnector started" << vnl::endl;

	set_timeout(10e6, std::bind(&BluezBTLEConnector::emit, this), VNL_TIMER_REPEAT);

	run();
}

void BluezBTLEConnector::emit() {
	homerunner::control::SwitchCommand* cmd = homerunner::control::SwitchCommand::create();
	cmd->on = true;
	publish(cmd, "homerunner", "homerunner::control::SwitchCommand");
}

} /* namespace connect */
} /* namespace homerunner */

#pragma GCC visibility push(default)

extern "C" vnl::Object* create_module() {
	return new homerunner::connect::BluezBTLEConnector();
}

extern "C" int destroy_module(vnl::Object* obj) {
	if(obj && dynamic_cast<homerunner::connect::BluezBTLEConnector*>(obj)) {
		delete dynamic_cast<homerunner::connect::BluezBTLEConnector*>(obj);
		return 0;
	}
	return -1;
}

#pragma GCC visibility pop

int destroy_module(vnl::Object* mod);
