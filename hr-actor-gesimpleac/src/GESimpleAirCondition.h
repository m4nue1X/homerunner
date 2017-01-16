/*
 * GESimpleAirCondition.h
 *
 *  Created on: Jan 7, 2017
 *      Author: manuel
 */

#ifndef HR_ACT_GESIMPLEAIRCONDITION_H_
#define HR_ACT_GESIMPLEAIRCONDITION_H_

#include <homerunner/act/SimpleAirConditionSupport.hxx>
#include <homerunner/connect/IRConnectorClient.hxx>

#define HRACT_GESAC_IR_ONOFF 0xB
#define HRACT_GESAC_IR_COOL 0x0
#define HRACT_GESAC_IR_HEAT 0x3
#define HRACT_GESAC_IR_FAN 0x1
#define HRACT_GESAC_IR_TEMP_PLUS 0x4
#define HRACT_GESAC_IR_TEMP_MINUS 0x5
#define HRACT_GESAC_IR_FAN_HIGH 0x2
#define HRACT_GESAC_IR_FAN_LOW 0x9

#define HRACT_GESAC_TEMP_MIN 60
#define HRACT_GESAC_TEMP_MAX 85

namespace homerunner {
namespace act {

class GESimpleAirCondition : public SimpleAirConditionBase {
public:
	GESimpleAirCondition(const vnl::String& domain, const vnl::String& topic);
	virtual ~GESimpleAirCondition();

	void main();

	void handle(const homerunner::control::SimpleAirConditionCommand& cmd);

	void test();

protected:
	void transmit(int cmd);

private:
	homerunner::connect::IRConnectorClient* irconn_client = nullptr;

	bool cur_on = false;
	double cur_temp = -1;
	homerunner::control::AirConditionMode cur_mode = homerunner::control::AirConditionMode::ACM_UNKNOWN;
	homerunner::control::AirConditionLevel cur_level = homerunner::control::AirConditionLevel::ACL_UNKNOWN;

};

} /* namespace act */
} /* namespace homerunner */

#endif /* HR_ACT_GESIMPLEAIRCONDITION_H_ */
