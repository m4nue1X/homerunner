/*
 * IguanaIRConnector.cpp
 *
 *  Created on: Jan 7, 2017
 *      Author: manuel
 */

#include "IguanaIRConnector.h"
#include <homerunner/ModuleFactory.h>

namespace homerunner {
namespace connect {

IguanaIRConnector::IguanaIRConnector(const vnl::String& domain, const vnl::String& topic) : IRConnectorBase(domain, topic) {
	// TODO Auto-generated constructor stub
}

IguanaIRConnector::~IguanaIRConnector() {
	// TODO Auto-generated destructor stub
}

void IguanaIRConnector::main() {
	vnl::read_config<vnl::String>(my_domain, my_topic, "device-id", devid);

	if(connect() == 0) {
		run();
	}
}

int32_t IguanaIRConnector::transmit(const vnl::Array<int32_t >& pulse_timing, int32_t repeat) const {
	if(conn == INVALID_PIPE) {
		return -1;
	}

	int n = pulse_timing.size();
	unsigned int buf[n];
	for(int i=0; i<n; i++) {
		buf[i] = (const unsigned int)pulse_timing[i];
		if(i%2 == 0) {
			buf[i] |= IG_PULSE_BIT;
		}
	}

	iguanaPacket req = iguanaCreateRequest(IG_DEV_SEND, sizeof(unsigned int)*n, buf);
	iguanaPacket resp;
	for(int i=0; i<repeat; i++) {
		if(send(req, resp) < 0) {
			iguanaRemoveData(req, NULL);
			iguanaFreePacket(req);
			//log(INFO).out << "Sending failed" << vnl::endl;
			return -1;
		}
		iguanaFreePacket(resp);
	}
	iguanaRemoveData(req, NULL);
	iguanaFreePacket(req);
	return 0;
}

int IguanaIRConnector::connect() {
	char devid_c[256];
	devid.to_string(devid_c, 256);
	conn = iguanaConnect(devid_c);
	if(conn == INVALID_PIPE) {
		log(ERROR).out << "Unable to connect to device " << devid << vnl::endl;
		return -1;
	}

	iguanaPacket req = iguanaCreateRequest(IG_DEV_GETVERSION, 0, NULL);
	iguanaPacket resp = nullptr;
	if(send(req, resp) < 0) {
		log(ERROR).out << "Unable to get firmware version." << vnl::endl;
		iguanaFreePacket(req);
		return -1;
	}

	/* the get version request returns 2 version bytes */
	unsigned int len;
	unsigned char* buffer = iguanaRemoveData(resp, &len);
	log(INFO).out << "Firmware version: " << *((short*)buffer) << vnl::endl;
	free(buffer);
	iguanaFreePacket(resp);
	iguanaFreePacket(req);
	return 0;
}

int IguanaIRConnector::send(const iguanaPacket& req, iguanaPacket& resp) const {
	resp = nullptr;

	if(!iguanaWriteRequest(req, conn)) {
		//log(ERROR).out << "iguanaWriteRequest failed" << vnl::endl;
		return -1;
	}

	/* wait up to 1000 milliseconds for a response */
	resp = iguanaReadResponse(conn, 1000);
	if(iguanaResponseIsError(resp)) {
		//log(ERROR).out << "iguanaReadResponse failed or no response";
		iguanaFreePacket(resp);
		resp = nullptr;
		return -1;
	}

	return 0;
}

} /* namespace connect */
} /* namespace homerunner */

#pragma GCC visibility push(default)
extern "C" homerunner::ModuleFactory* homerunner_get_factory() {
	return new homerunner::ModuleFactoryImpl<homerunner::connect::IguanaIRConnector>("IguanaIRConnector", vnl::String() << GIT_COMMIT_HASH << "@" << GIT_BRANCH << " (" << GIT_AUTHOR_DATE << ")");
}
#pragma GCC visibility pop
