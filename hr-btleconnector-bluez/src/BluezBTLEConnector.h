/*
 * BluezBTLEConnector.h
 *
 *  Created on: Oct 22, 2016
 *      Author: manuel
 */

#ifndef SRC_BLUEZBTLECONNECTOR_H_
#define SRC_BLUEZBTLECONNECTOR_H_

#include <homerunner/connect/BTLEConnectorSupport.hxx>

namespace homerunner {
namespace connect {

class BluezBTLEConnector: public BTLEConnectorBase {
public:
	BluezBTLEConnector();
	virtual ~BluezBTLEConnector();
};

} /* namespace connect */
} /* namespace homerunner */

#endif /* SRC_BLUEZBTLECONNECTOR_H_ */
