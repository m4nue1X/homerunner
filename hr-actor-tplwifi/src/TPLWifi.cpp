/*
 * TPLWifi.cpp
 *
 *  Created on: Jan 2, 2017
 *      Author: manuel
 */

#include "TPLWifi.h"
#include "TPLWifiSwitchablePlug.h"
#include <sys/select.h>
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
	ssize_t n;
	uint8_t buf[4096];
	rapidjson::Document doc;
	int res = -1;
	err_str = "No response within a second";

	bool loop0 = true;
	while(loop0 || to.tv_sec > 0 || to.tv_usec > 0) {
		FD_ZERO(&sock_set);
		FD_SET(status_sock, &sock_set);
		while(select(status_sock+1, &sock_set, NULL, NULL, &to) > 0) { // process all pending packets
			n = recv(status_sock, (void*)buf, 4096, 0);
			if(n > 0) {
				vnl::Array<uint8_t> data(buf, n);
				if(decode(data, false, doc) == 0) {
					const rapidjson::Value* alias_val = jsonGetValue({ "system", "get_sysinfo", "alias" }, doc);
					if(alias_val != nullptr && alias_val->IsString()) {
						vnl::String alias(alias_val->GetString());
						if(alias == dev_name) { // we're done. however, we need to process the remaining packets (might contain a more current status) ...
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

const rapidjson::Value* TPLWifi::jsonGetValue(const vnl::Array<vnl::String>& path, const rapidjson::Value& doc) {
	char cstr[256];
	const rapidjson::Value* cur_val = &doc;
	for(const vnl::String& str : path) {
		str.to_string(cstr, 256);
		if(cur_val->IsObject() && cur_val->HasMember(cstr)) {
			cur_val = &((*cur_val)[cstr]);
		} else {
			return nullptr;
		}
	}
	return cur_val;
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

