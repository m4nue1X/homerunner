/*
 * BluezBTLEConnector.h
 *
 *  Created on: Oct 22, 2016
 *      Author: manuel
 */

#ifndef SRC_BLUEZBTLECONNECTOR_H_
#define SRC_BLUEZBTLECONNECTOR_H_

#include <vnl/String.h>
#include <homerunner/connect/BTLEConnectorSupport.hxx>
#include <homerunner/ModuleFactory.h>

namespace homerunner {
namespace connect {

class BluezBTLEConnector: public BTLEConnectorBase {
public:
	BluezBTLEConnector(const vnl::String& domain, const vnl::String& topic);
	virtual ~BluezBTLEConnector();

protected:
	void main();

	void list_devices();
	void emit();

private:
	int hci_dev_id = -1;
	int hci_sock = -1;

	void connect();

};

class BluezBTLEConnectorFactory : public ModuleFactory {
public:
	virtual ~BluezBTLEConnectorFactory();
	virtual vnl::String moduleID();
	virtual vnl::String moduleVersion();
	virtual vnl::Object* createInstance(const vnl::String& domain, const vnl::String& topic);
	virtual int destoryInstance(vnl::Object* obj);

	static BluezBTLEConnectorFactory* instance;
};

} /* namespace connect */
} /* namespace homerunner */

#endif /* SRC_BLUEZBTLECONNECTOR_H_ */
