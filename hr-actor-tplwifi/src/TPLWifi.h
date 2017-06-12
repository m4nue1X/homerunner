/*
 * TPLWifi.h
 *
 *  Created on: Jan 2, 2017
 *      Author: manuel
 */

#ifndef HR_ACT_TPLWIFI_H_
#define HR_ACT_TPLWIFI_H_

#include <homerunner/ModuleFactory.h>
#include <homerunner/Exception.h>
#include <vnl/String.h>
#include <vnl/Array.h>
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>

namespace homerunner {
namespace act {

class TPLWifiSystemStatus {
public:
	vnl::String sw_ver;
	vnl::String hw_ver;
	vnl::String dev_type;
	vnl::String dev_model;
	uint8_t mac[6];
	vnl::String dev_name;
	vnl::String dev_alias;
	vnl::String oem_id;
	vnl::String dev_id;
	vnl::String hw_id;
	int err_code = 0;
	double rssi = 0;
};

class TPLWifi {
public:
	virtual ~TPLWifi();

protected:
	vnl::String dev_name;
	sockaddr_in dev_addr; // is populated when first status received
	int status_sock = -1;
	int ctrl_sock = -1;
	vnl::String err_str;

	int init(const vnl::String& device_name);
	int requestStatus(rapidjson::Document& cur_status);
	int request(const rapidjson::Document& req_doc, rapidjson::Document& resp_doc);
	void populateSystemStatus(const rapidjson::Document& doc, TPLWifiSystemStatus& status, const vnl::Map<vnl::String, vnl::String>& alt_field_names = vnl::Map<vnl::String, vnl::String>());

	int send(const rapidjson::Document& req_doc, rapidjson::Document& resp_doc);

	void encode(const rapidjson::Document& doc, bool len_pfx, vnl::Array<uint8_t>& buf);
	int decode(const vnl::Array<uint8_t>& buf, bool len_pfx, rapidjson::Document& doc);

	static const rapidjson::Value* jsonGetValue(const vnl::Array<vnl::String>& path, const rapidjson::Value& doc, const vnl::String* alt_field_name=nullptr);
	static int jsonString(const rapidjson::Value* val, vnl::String& field, const vnl::String& def="");
	static int jsonInt(const rapidjson::Value* val, int& field, int def=-1);
	static int jsonDouble(const rapidjson::Value* val, double& field, double def=0);
	static int jsonPutValue(const vnl::Array<vnl::String>& path, const rapidjson::Value& val, rapidjson::Document& doc);

private:
	int receiveStatus(struct timeval& to, rapidjson::Document& cur_status);
	int requestStatus();

};

// TPLWifiFactory
// -------------------------------------------------------------------------------------
class TPLWifiSwitchablePlug;

class TPLWifiFactory : public ModuleFactoryImpl<TPLWifiSwitchablePlug> {
public:
	TPLWifiFactory(const vnl::String& mod_id, const vnl::String& mod_ver);
	virtual vnl::Object* createInstance(const vnl::String& domain, const vnl::String& topic, const vnl::String& type);
};

} /* namespace act */
} /* namespace homerunner */

#endif /* HR_ACT_TPLWIFI_H_ */
