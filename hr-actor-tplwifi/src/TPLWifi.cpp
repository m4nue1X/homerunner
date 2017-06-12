/*
 * TPLWifi.cpp
 *
 *  Created on: Jan 2, 2017
 *      Author: manuel
 */

#include "TPLWifi.h"
#include "TPLWifiSwitchablePlug.h"
#include <homerunner/util/Helper.h>
#include <sys/select.h>
#include <fcntl.h>
#include <cstdlib>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

namespace homerunner {
namespace act {

TPLWifi::~TPLWifi() {
	// TODO Auto-generated destructor stub
}

int TPLWifi::init(const vnl::String& device_name) {
	dev_name = device_name;

	if((status_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
		err_str.clear();
		err_str << "Unable to open socket: " << strerror(errno);
		return -1;
	}

	struct sockaddr_in bind_addr;
	bind_addr.sin_family = AF_INET;
	bind_addr.sin_port = htons(9999);
	bind_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	int one = 1;
	if(setsockopt(status_sock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(int)) < 0) { // since linux 3.9 SO_REUSEPORT could be used instead
		err_str.clear();
		err_str << "Unable set socket option SO_REUSEADDR: " << strerror(errno);
		return -1;
	}
	if(setsockopt(status_sock, SOL_SOCKET, SO_BROADCAST, &one, sizeof(int)) < 0) {
		err_str.clear();
		err_str << "Unable set socket option SO_BROADCAST: " << strerror(errno);
		return -1;
	}

	if(bind(status_sock, (sockaddr*)&bind_addr, sizeof(struct sockaddr_in)) < 0) {
		err_str.clear();
		err_str << "Unable to bind port 9999: " << strerror(errno);
		return -1;
	}

	return 0;
}

int TPLWifi::requestStatus(rapidjson::Document& cur_status) {
	struct timeval to;
	to.tv_sec = 0;
	to.tv_usec = 0;
	if(receiveStatus(to, cur_status) < 0) { // check for pending status messages first
		requestStatus();
		to.tv_sec = 1;
		return receiveStatus(to, cur_status);
	}
	return 0;
}

int TPLWifi::receiveStatus(struct timeval& to, rapidjson::Document& cur_status) {
	fd_set sock_set;
	uint8_t buf[4096];
	rapidjson::Document doc;
	int res = -1;
	err_str = "No response within one second";

	bool loop0 = true;
	while(loop0 || to.tv_sec > 0 || to.tv_usec > 0) {
		FD_ZERO(&sock_set);
		FD_SET(status_sock, &sock_set);
		while(select(status_sock+1, &sock_set, NULL, NULL, &to) > 0) { // process all pending packets
			sockaddr_in addr;
			socklen_t addr_len = sizeof(addr);
			ssize_t n = recvfrom(status_sock, (void*)buf, 4096, 0, (sockaddr*)&addr, &addr_len);
			if(n > 0) {
				vnl::Array<uint8_t> data(buf, n);
				if(decode(data, false, doc) == 0) {
					const rapidjson::Value* alias_val = jsonGetValue({ "system", "get_sysinfo", "alias" }, doc);
					if(alias_val != nullptr && alias_val->IsString()) {
						vnl::String alias(alias_val->GetString());
						if(alias == dev_name) { // we're done. however, we need to process the remaining packets (might contain a more current status) ...
							dev_addr = addr;
							cur_status.CopyFrom(doc, cur_status.GetAllocator());
							to.tv_sec = 0; // set timeout to 0, to avoid waiting for more packets
							to.tv_usec = 0;
							err_str.clear();
							res = 0;
						}
					}
				}
			}
		}
		loop0 = false;
	}
	return res;
}

int TPLWifi::requestStatus() {
	rapidjson::Document doc;
	rapidjson::Value null_val(rapidjson::kNullType);
	jsonPutValue({ "system", "get_sysinfo" }, null_val, doc);
	jsonPutValue({ "emeter", "get_realtime" }, null_val, doc);
	vnl::Array<uint8_t> data;
	encode(doc, false, data);
	uint8_t buf[data.size()];
	ssize_t n = data.to_array(buf, data.size());
	if(n > 0) {
		sockaddr_in bc_addr;
		bc_addr.sin_family = AF_INET;
		bc_addr.sin_port = htons(9999);
		bc_addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
		if(sendto(status_sock, (void*)buf, n, 0, (struct sockaddr*)&bc_addr, sizeof(sockaddr_in)) < 0) {
			err_str.clear();
			err_str << "Unable to send status request: " << strerror(errno);
			return -1;
		}
	}
	return 0;
}

int TPLWifi::request(const rapidjson::Document& req_doc, rapidjson::Document& resp_doc) {
	// TODO
}

void TPLWifi::populateSystemStatus(const rapidjson::Document& doc, TPLWifiSystemStatus& status, const vnl::Map<vnl::String, vnl::String>& alt_field_names) {
	jsonString(jsonGetValue({ "system", "get_systeminfo", "sw_ver" }, doc, alt_field_names.find("sw_ver")), status.sw_ver);
	jsonString(jsonGetValue({ "system", "get_systeminfo", "hw_ver" }, doc, alt_field_names.find("hw_ver")), status.hw_ver);
	jsonString(jsonGetValue({ "system", "get_systeminfo", "type" }, doc, alt_field_names.find("type")), status.dev_type);
	jsonString(jsonGetValue({ "system", "get_systeminfo", "model" }, doc, alt_field_names.find("model")), status.dev_model);
	vnl::String mac_str;
	jsonString(jsonGetValue({ "system", "get_systeminfo", "mac" }, doc, alt_field_names.find("mac")), mac_str);
	int j=0;
	for(size_t i=0; i<mac_str.size(); i+=2) {
		if(mac_str[i] == ':') {
			++i;
		}
		status.mac[j] = vnl::String::stoi(mac_str.substr(i, 2), 0, 16);
		j++;
	}
	jsonString(jsonGetValue({ "system", "get_systeminfo", "dev_name" }, doc, alt_field_names.find("dev_name")), status.dev_name);
	jsonString(jsonGetValue({ "system", "get_systeminfo", "alias" }, doc, alt_field_names.find("alias")), status.dev_alias);
	jsonString(jsonGetValue({ "system", "get_systeminfo", "oemId" }, doc, alt_field_names.find("oemId")), status.oem_id);
	jsonString(jsonGetValue({ "system", "get_systeminfo", "devId" }, doc, alt_field_names.find("devId")), status.dev_id);
	jsonString(jsonGetValue({ "system", "get_systeminfo", "hwId" }, doc, alt_field_names.find("hwId")), status.hw_id);
	jsonDouble(jsonGetValue({ "system", "get_systeminfo", "rssi" }, doc, alt_field_names.find("rssi")), status.rssi);
	jsonInt(jsonGetValue({ "system", "get_systeminfo", "err_code" }, doc, alt_field_names.find("err_code")), status.err_code);
}

int TPLWifi::send(const rapidjson::Document& req_doc, rapidjson::Document& resp_doc) {
	fd_set sock_set;
	struct timeval to;
	ssize_t buf_size = 1024*32; // 32k buffer should be sufficient
	uint8_t buf[buf_size];
	ssize_t n;

	int sock = socket(AF_INET, SOCK_STREAM, 0);

	// set socket to non-blocking
	int flags = fcntl(sock, F_GETFL);
	if(flags < 0) {
		return -1;
	}
	fcntl(sock, F_SETFL, flags | O_NONBLOCK);

	// try to connect
	connect(sock, (sockaddr*)&dev_addr, sizeof(dev_addr));

	// wait for connection
	homerunner::util::init_timeval(to, 5, 0);
	int r;
	do {
		FD_ZERO(&sock_set);
		FD_SET(sock, &sock_set);
		r = select(sock+1, NULL, &sock_set, NULL, &to);
	} while(r < 0 && errno == EAGAIN);

	// check if timeout or error
	if(r <= 0) { // connection not established
		err_str = (r == 0) ? "Connect timeout" : strerror(errno);
		close(sock);
		return -1;
	}

	// set back original flags (i. e. blocking)
	fcntl(sock, F_SETFL, flags);

	// send command
	vnl::Array<uint8_t> req_data;
	encode(req_doc, true, req_data);
	n = req_data.to_array(buf, buf_size);
	::send(sock, buf, n, 0);

	// receive response
	homerunner::util::init_timeval(to, 5, 0);
	do {
		FD_ZERO(&sock_set);
		FD_SET(sock, &sock_set);
		r = select(sock+1, &sock_set, NULL, NULL, &to);
	} while(r < 0 && errno == EAGAIN);
	if(r <= 0) {
		err_str = (r == 0) ? "Response timeout" : strerror(errno);
		close(sock);
		return -1;
	}

	n = recv(sock, buf, buf_size, 0);
	if(n <= 0) {
		err_str = (r == 0) ? "Response empty" : strerror(errno);
		close(sock);
		return -1;
	}

	vnl::Array<uint8_t> resp_data(buf, n);
	if(decode(resp_data, true, resp_doc) < 0) {
		err_str = "Unable to decode response";
		close(sock);
		return -1;
	}

	close(sock);
	return 0;
}

void TPLWifi::encode(const rapidjson::Document& doc, bool len_pfx, vnl::Array<uint8_t>& buf) {
	// serialize json
	rapidjson::StringBuffer strbuf;
	strbuf.Clear();
	rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
	doc.Accept(writer);
	const char* cmdstr = strbuf.GetString();

	// encode
	buf.clear();
	uint8_t key = 0xAB;
	int n = strlen(cmdstr);
	int o = (len_pfx) ? 4 : 0;
	for(int i=0; i<n+o; i++) {
		uint8_t b = 0;
		if(len_pfx && i < 4) { // length field
			b = (n >> (24 - (i*8))) & 0xff;
		} else {
			b = (uint8_t)(cmdstr[i-o] ^ key);
			key = b;
		}
		buf.push_back(b);
	}
}

int TPLWifi::decode(const vnl::Array<uint8_t>& buf, bool len_pfx, rapidjson::Document& doc) {
	int n = buf.size();
	// decode
	char cmdstr[n+1];
	memset(cmdstr, 0, n+1);
	int k = 0;

	uint8_t key = 0xAB;
	int i = (len_pfx) ? 4 : 0;
	for(; i<n; ++i) {
		uint8_t b = buf[i];
		cmdstr[k] = ((char)(b ^ key));
		key = b;
		++k;
	}

	// parse
	rapidjson::ParseResult ok = doc.Parse(cmdstr);
	if(!ok) {
		return -1;
	}
	return 0;
}

const rapidjson::Value* TPLWifi::jsonGetValue(const vnl::Array<vnl::String>& path, const rapidjson::Value& doc, const vnl::String* alt_field_name) {
	char cstr[256];
	const rapidjson::Value* cur_val = &doc;
	for(int i=0; i<path.size(); i++) {
		path[i].to_string(cstr, 256);
		for(;;) {
			if(cur_val->IsObject() && cur_val->HasMember(cstr)) {
				cur_val = &((*cur_val)[cstr]);
				break;
			} else if(alt_field_name != nullptr && i == path.size()-1) {
				alt_field_name->to_string(cstr, 256);
				alt_field_name = nullptr;
			} else {
				return nullptr;
			}
		}
	}
	return cur_val;
}

int TPLWifi::jsonString(const rapidjson::Value* val, vnl::String& field, const vnl::String& def) {
	if(val != nullptr && val->IsString()) {
		field = val->GetString();
		return 0;
	}
	field = def;
	return -1;
}

int TPLWifi::jsonInt(const rapidjson::Value* val, int& field, int def) {
	if(val != nullptr && val->IsInt()) {
		field = val->GetInt();
		return 0;
	}
	field = def;
	return -1;
}

int TPLWifi::jsonDouble(const rapidjson::Value* val, double& field, double def) {
	if(val != nullptr && val->IsDouble()) {
		field = val->GetDouble();
		return 0;
	}
	field = def;
	return -1;
}

int TPLWifi::jsonPutValue(const vnl::Array<vnl::String>& path, const rapidjson::Value& val, rapidjson::Document& doc) {
	if(path.size() < 1) {
		return -1;
	}

	char cstr[256];
	doc.SetObject();
	rapidjson::Value* cur_val = &doc;
	for(int i=0; i<path.size()-1; ++i) {
		path[i].to_string(cstr, 256);
		if(cur_val->IsObject() && !cur_val->HasMember(cstr)) {
			rapidjson::Value new_name(cstr, doc.GetAllocator());
			rapidjson::Value new_val(rapidjson::kObjectType);
			cur_val->AddMember(new_name, new_val, doc.GetAllocator());
			cur_val = &(*cur_val)[cstr];
		} else {
			if(cur_val->IsObject()) {
				cur_val = &(*cur_val)[cstr];
			} else {
				return -1;
			}
		}
	}

	path[path.size()-1].to_string(cstr, 256);

	rapidjson::Value new_name(cstr, doc.GetAllocator());
	rapidjson::Value new_val(val, doc.GetAllocator());
	cur_val->AddMember(new_name, new_val, doc.GetAllocator());
	return 0;
}

// TPLWifiFactory implementation
// -------------------------------------------------------------------------------------
TPLWifiFactory::TPLWifiFactory(const vnl::String& mod_id, const vnl::String& mod_ver) : ModuleFactoryImpl(mod_id, mod_ver) {
	// nothing to do
}

vnl::Object* TPLWifiFactory::createInstance(const vnl::String& domain, const vnl::String& topic, const vnl::String& type) {
	if(type == "plug") {
		return new TPLWifiSwitchablePlug(domain, topic);
	} else if(type == "bulb") {

	} else if(type == "rgbbulb") {

	}
	return nullptr;
}

} /* namespace act */
} /* namespace homerunner */

#pragma GCC visibility push(default)
extern "C" homerunner::ModuleFactory* homerunner_get_factory() {
	return new homerunner::act::TPLWifiFactory("TPLWifi", vnl::String() << GIT_COMMIT_HASH << "@" << GIT_BRANCH << " (" << GIT_AUTHOR_DATE << ")");
}
#pragma GCC visibility pop

