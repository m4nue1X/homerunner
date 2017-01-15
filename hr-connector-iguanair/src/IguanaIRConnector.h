/*
 * IguanaIRConnector.h
 *
 *  Created on: Jan 7, 2017
 *      Author: manuel
 */

#ifndef HR_CONN_IGUANAIRCONNECTOR_H_
#define HR_CONN_IGUANAIRCONNECTOR_H_

#include <homerunner/connect/IRConnectorSupport.hxx>
extern "C" { // see http://stackoverflow.com/questions/11117746/g-cannot-find-symbols-when-linking
#include <iguanaIR.h>
}

namespace homerunner {
namespace connect {

class IguanaIRConnector : public IRConnectorBase {
public:
	IguanaIRConnector(const vnl::String& domain, const vnl::String& topic);
	virtual ~IguanaIRConnector();

protected:
	void main();

	int32_t transmit(const vnl::Array<int32_t >& pulse_timing, int32_t repeat) const;

private:
	vnl::String devid = "0";
	PIPE_PTR conn = INVALID_PIPE;

	int connect();
	int send(const iguanaPacket& req, iguanaPacket& resp) const;
};

} /* namespace connect */
} /* namespace homerunner */

#endif /* HR_CONN_IGUANAIRCONNECTOR_H_ */
