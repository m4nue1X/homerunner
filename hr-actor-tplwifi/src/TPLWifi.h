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

class TPLWifi {
public:
	virtual ~TPLWifi();

protected:
	vnl::String dev_name;
	int status_sock = -1;
	vnl::String err_str;

	int init(const vnl::String& device_name);
	int requestStatus(rapidjson::Document& cur_status);
	int request(const rapidjson::Document& req_doc, rapidjson::Document& resp_doc);

	void encode(const rapidjson::Document& doc, bool len_pfx, vnl::Array<uint8_t>& buf);
	int decode(const vnl::Array<uint8_t>& buf, bool len_pfx, rapidjson::Document& doc);

	static const rapidjson::Value* jsonGetValue(const vnl::Array<vnl::String>& path, const rapidjson::Value& doc);
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
