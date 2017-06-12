/*
 * TPLWifiSwitchablePlug.cpp
 *
 *  Created on: Jan 15, 2017
 *      Author: manuel
 */

#include "TPLWifiSwitchablePlug.h"
#include <vnl/Random.h>

namespace homerunner {
namespace act {

TPLWifiSwitchablePlug::TPLWifiSwitchablePlug(const vnl::String& domain, const vnl::String& topic) : SwitchablePlugBase(domain, topic) {
	// nothing to do
}

TPLWifiSwitchablePlug::~TPLWifiSwitchablePlug() {
	// nothing to do yet
}

void TPLWifiSwitchablePlug::main() {
	vnl::read_config<vnl::String>(my_domain, my_topic, "device-name", dev_name);
	init(dev_name);

	int64_t interval = 10e6 + (vnl::rand() % 4000000) - 2000000l;
	set_timeout(interval, std::bind(&TPLWifiSwitchablePlug::updateStatus, this), VNL_TIMER_REPEAT);

	run();
}

void TPLWifiSwitchablePlug::handle(const homerunner::control::SwitchCommand& cmd) {

}

void TPLWifiSwitchablePlug::updateStatus() {
	log(INFO).out << "Updating status" << vnl::endl;
	rapidjson::Document status_doc;
	if(requestStatus(status_doc) < 0) {
		log(ERROR).out << err_str << vnl::endl;
	}

	TPLWifiSwitchablePlugStatus status;
	populateSystemStatus(status_doc, status);
	log(INFO).out << "Current status of " << dev_name << ": " << status.relay_state << vnl::endl;

}

} /* namespace act */
} /* namespace homerunner */
