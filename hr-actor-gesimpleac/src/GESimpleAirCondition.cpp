/*
 * GESimpleAirCondition.cpp
 *
 *  Created on: Jan 7, 2017
 *      Author: manuel
 */

#include "GESimpleAirCondition.h"
#include <homerunner/Node.h>
#include <homerunner/ModuleFactory.h>

namespace homerunner {
namespace act {

GESimpleAirCondition::GESimpleAirCondition(const vnl::String& domain, const vnl::String& topic) : SimpleAirConditionBase(domain, topic) {
	// TODO Auto-generated constructor stub

}

GESimpleAirCondition::~GESimpleAirCondition() {
	// TODO Auto-generated destructor stub
}

void GESimpleAirCondition::main() {
	vnl::String irconn_inst_name;
	vnl::read_config<vnl::String>(my_domain, my_topic, "irconnector", irconn_inst_name);
	if(irconn_inst_name.size() <= 0) {
		log(ERROR).out << "No IRConnector configured" << vnl::endl;
		return;
	}
	irconn_client = Node::requireService<homerunner::connect::IRConnectorClient>(irconn_inst_name);

	bool testmode = false;
	vnl::read_config<bool>(my_domain, my_topic, "testmode", testmode);

	if(testmode) {
		set_timeout(5e6, std::bind(&GESimpleAirCondition::test, this), VNL_TIMER_ONCE);
	}

	run();
}

void GESimpleAirCondition::test() {
	log(INFO).out << "Test mode stated" << vnl::endl;
	homerunner::control::SimpleAirConditionCommand cmd;
	cmd.on = true;
	cmd.mode = homerunner::control::AirConditionMode::ACM_HEAT;
	cmd.temperature = 74;
	handle(cmd);
	if(!poll(5e6)) {
		return;
	}
	cmd.on = true;
	cmd.mode = homerunner::control::AirConditionMode::ACM_COOL;
	cmd.temperature = 78;
	handle(cmd);
	if(!poll(5e6)) {
		return;
	}
	cmd.on = false;
	handle(cmd);
	if(!poll(5e6)) {
		return;
	}
	cmd.on = true;
	cmd.mode = homerunner::control::AirConditionMode::ACM_HEAT;
	cmd.temperature = 74;
	handle(cmd);
	log(INFO).out << "Test finished." << vnl::endl;
}

void GESimpleAirCondition::handle(const homerunner::control::SimpleAirConditionCommand& cmd) {
	if(cmd.on != cur_on) {
		transmit(HRACT_GESAC_IR_ONOFF);
		cur_on = !cur_on;
	}
	if(cur_on) {
		if(cmd.mode != cur_mode) {
			bool success = true;
			switch(cmd.mode) {
				case homerunner::control::AirConditionMode::ACM_COOL: transmit(HRACT_GESAC_IR_COOL); break;
				case homerunner::control::AirConditionMode::ACM_HEAT: transmit(HRACT_GESAC_IR_HEAT); break;
				case homerunner::control::AirConditionMode::ACM_FAN: transmit(HRACT_GESAC_IR_FAN); break;
				default: success = false;
			}
			if(success) {
				cur_mode = cmd.mode;
				cur_temp = -1; // after mode set temperature is undefined
			}
		}

		if(cmd.temperature != cur_temp && cur_mode != homerunner::control::AirConditionMode::ACM_FAN) {
			if(cur_temp < 0) { // go to min or max fist
				for(int i=HRACT_GESAC_TEMP_MIN; i<HRACT_GESAC_TEMP_MAX; i++) {
					transmit((cur_mode == homerunner::control::AirConditionMode::ACM_COOL) ? HRACT_GESAC_IR_TEMP_PLUS : HRACT_GESAC_IR_TEMP_MINUS);
				}
				cur_temp = ((cur_mode == homerunner::control::AirConditionMode::ACM_COOL) ? HRACT_GESAC_TEMP_MAX : HRACT_GESAC_TEMP_MIN);
			}
			int n = std::abs(cmd.temperature - cur_temp);
			int ir_cmd = (cmd.temperature > cur_temp) ? HRACT_GESAC_IR_TEMP_PLUS : HRACT_GESAC_IR_TEMP_MINUS;
			for(int i=0; i<n; i++) {
				transmit(ir_cmd);
			}
			cur_temp = cmd.temperature;
		}

		if(cmd.level != cur_level) {
			if(cmd.level == homerunner::control::AirConditionLevel::ACL_HIGH) {
				transmit(HRACT_GESAC_IR_FAN_HIGH);
			} else {
				transmit(HRACT_GESAC_IR_FAN_LOW);
			}
			cur_level = cmd.level;
		}
	}
}

void GESimpleAirCondition::transmit(int cmd) {
	if(!irconn_client) {
		log(ERROR).out << "Transmit failed. No IRConnector service." << vnl::endl;
		return;
	}
	int timings[]{
		8512, 4224,
		512, 1578,
		512, 1578,
		512, 1578,
		512, 512,
		512, 1578,
		512, 512,
		512, 1578,
		512, 1578,
		512, 4224,
		512, 512,
		512, 512,
		512, 512,
		512, 512,
		512, 512,
		512, 512,
		512, 512,
		512, 512,
		597
	};

	int v = 1;
	for(int i=21; i<37; i+=2) {
		timings[i] = ((v & cmd) != 0) ? 1578 : 512;
		v = v << 1;
	}
	irconn_client->transmit(vnl::Array<int>(timings, 37), 1);
	poll(500e3);
}

} /* namespace act */
} /* namespace homerunner */

#pragma GCC visibility push(default)
extern "C" homerunner::ModuleFactory* homerunner_get_factory() {
	vnl::String mod_id = "GESimpleAirCondition";
	vnl::String mod_ver = vnl::String() << GIT_COMMIT_HASH << "@" << GIT_BRANCH << " (" << GIT_AUTHOR_DATE << ")";
	vnl::Array<vnl::String> req_libs;
	req_libs.push_back("libiguanaIR.so");

	return new homerunner::ModuleFactoryImpl<homerunner::act::GESimpleAirCondition>(mod_id, mod_ver, req_libs);
}
#pragma GCC visibility pop

