/*
 * TPLWifiSwitchablePlug.h
 *
 *  Created on: Jan 15, 2017
 *      Author: manuel
 */

#ifndef TPLWIFISWITCHABLEPLUG_H_
#define TPLWIFISWITCHABLEPLUG_H_

#include <homerunner/act/SwitchablePlugSupport.hxx>
#include "TPLWifi.h"

namespace homerunner {
namespace act {

class TPLWifiSwitchablePlugStatus : public TPLWifiSystemStatus {
public:
	bool relay_state = false;
	double on_time = 0;
};

class TPLWifiSwitchablePlug : public SwitchablePlugBase, public TPLWifi {
public:
	TPLWifiSwitchablePlug(const vnl::String& domain, const vnl::String& topic);
	virtual ~TPLWifiSwitchablePlug();

	void main();

	void handle(const homerunner::control::SwitchCommand& cmd);

private:
	void updateStatus();

};

} /* namespace act */
} /* namespace homerunner */

#endif /* TPLWIFISWITCHABLEPLUG_H_ */
