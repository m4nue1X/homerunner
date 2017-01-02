/*
 * BluezBTLEConnector.cpp
 *
 *  Created on: Oct 22, 2016
 *      Author: manuel
 */

#include "BluezBTLEConnector.h"
#include <homerunner/util/Verison.h>
#include <homerunner/control/SwitchCommand.hxx>
//#include <bluetooth/bluetooth.h>
//#include <bluetooth/hci.h>
//#include <bluetooth/hci_lib.h>
//#include <bluetooth/l2cap.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define EIR_FLAGS 0x01
#define EIR_NAME_SHORT 0x08
#define EIR_NAME_COMPLETE 0x09
#define EIR_MANUFACTURE_SPECIFIC 0xff

#define ATT_CID 4

namespace homerunner {
namespace connect {

// BluezBTLEConnector implementation
// ------------------------------------------------------------------------------------

BluezBTLEConnector::BluezBTLEConnector(const vnl::String& domain, const vnl::String& topic) : BTLEConnectorBase(domain, topic) {
	// TODO Auto-generated constructor stub
}

BluezBTLEConnector::~BluezBTLEConnector() {
	// TODO Auto-generated destructor stub
}



void BluezBTLEConnector::main() {

	/*if(device_address.size() > 0) {
		char c_dev_addr[device_address.size()+1];
		device_address.to_string(c_dev_addr, device_address.size()+1); // initializes buf to 0
		hci_dev_id = hci_devid(c_dev_addr); // get specified bt device
		log(INFO).out << "Using device " << device_address << vnl::endl;
	} else {
		hci_dev_id = hci_get_route(NULL); // get default bt device
		log(INFO).out << "Using device (default)" << vnl::endl;
	}
	if(hci_dev_id < 0) {
		log(ERROR).out << "Unable to find bluetooth device " << ((device_address.size() > 0) ? device_address : vnl::String("")) << vnl::endl;
		return;
	}

	hci_sock = hci_open_dev(hci_dev_id);
	if(hci_sock < 0) {
		log(ERROR).out << "Unable to open bluetooth device" << vnl::endl;
		return;
	}

	int on = 1;
	if(::ioctl(hci_sock, FIONBIO, (char*)&on) < 0) {
		log(ERROR).out << "Unable to set bluetooth device socket to non-blocking" << vnl::endl;
		hci_close_dev(hci_sock);
		return;
	}

	// start scanning
	bool scanning = true;
	struct hci_filter hci_filt;
	hci_filter_clear(&hci_filt);
	hci_filter_set_ptype(HCI_EVENT_PKT, &hci_filt);
	hci_filter_set_event(EVT_LE_META_EVENT, &hci_filt);

	if(hci_le_set_scan_parameters(hci_sock, 0x01, htobs(0x0010), htobs(0x0010), 0x00, 0x00, 1000) < 0 ||
			hci_le_set_scan_enable(hci_sock, 0x01, 1, 1000) < 0 ||
			setsockopt(hci_sock, SOL_HCI, HCI_FILTER, &hci_filt, sizeof(hci_filt)) < 0) {
		log(ERROR).out << "Unable to start scanning (" << strerror(errno) << ")" << vnl::endl;
		scanning = false;
	}

	if(scanning) {
		log(INFO).out << "BluezBTLEConnector started" << vnl::endl;

		set_timeout(10e6, std::bind(&BluezBTLEConnector::emit, this), VNL_TIMER_ONCE);
		//set_timeout(1e6, std::bind(&BluezBTLEConnector::list_devices, this), VNL_TIMER_REPEAT);

		run();
	}

	// stop scanning
	log(INFO).out << "Closing device" << vnl::endl;
	hci_le_set_scan_enable(hci_sock, 0x00, 1, 1000);
	hci_close_dev(hci_sock);*/

	set_timeout(1e6, std::bind(&BluezBTLEConnector::emit, this), VNL_TIMER_ONCE);

	run();
}

vnl::String hexdump(const unsigned char* buf, int len) {
	vnl::String s;
	for(int i=0; i<len; i++) {
		s << vnl::hex(buf[i]) << " ";
	}
	return s;
}

void BluezBTLEConnector::list_devices() {
	int len = 0;
	unsigned char buf[HCI_MAX_EVENT_SIZE];
	do {
		len = read(hci_sock, buf, sizeof(buf));
		if(len <= 0) {
			break;
		}

		log(DEBUG).out << hexdump(buf, len) << vnl::endl;

		evt_le_meta_event* meta = (evt_le_meta_event*)(buf+HCI_EVENT_HDR_SIZE+1);
		len -= HCI_EVENT_HDR_SIZE+1;
		if(meta->subevent != EVT_LE_ADVERTISING_REPORT) {
			break;
		}

		le_advertising_info* info = (le_advertising_info*)(meta->data+1);
		if(info->length == 0) {
			break;
		}

		int off = 0;
		while(off < info->length) {
			size_t dlen = info->data[off];
			if(off+dlen >= info->length) { // data corrupt
				log(WARN).out << "Event data corrupt" << vnl::endl;
				break;
			}

			uint8_t* data = info->data+off+1;
			if(data[0] == EIR_NAME_SHORT || data[0] == EIR_NAME_COMPLETE) {
				size_t name_len = dlen-1;
				vnl::String name((char*)(data+1), name_len);
				char addr_buf[18];
				ba2str(&info->bdaddr, addr_buf);
				vnl::String addr(addr_buf);
				log(DEBUG).out << "Found device " << name << " (" << addr << ")" << vnl::endl;
			} else if(data[0] == EIR_FLAGS) {
				log(DEBUG).out << "Found flags" << vnl::endl;
			} else if(data[0] == EIR_MANUFACTURE_SPECIFIC) {
				log(DEBUG).out << "Found manufacturer specific" << vnl::endl;
			} else {
				log(DEBUG).out << "Found unknown data (" << vnl::hex(data[0]) << ")" << vnl::endl;
			}
			off += dlen + 1;
		}
	} while(len > 0);
}

void BluezBTLEConnector::emit() {
	homerunner::control::SwitchCommand* cmd = homerunner::control::SwitchCommand::create();
	cmd->on = true;
	publish(cmd, "homerunner", "homerunner::control::SwitchCommand");

	uint8_t btcmd[] = { 0x56, 0x00, 0x00, 0x00, 255, 0x0f, 0xaa, 0x09 };

	connect();

}

void BluezBTLEConnector::connect() {
	/*bdaddr_t src, dest;
	uint8_t dst_type = BDADDR_LE_PUBLIC;
	int sec = BT_SECURITY_LOW;

	src = {0, 0, 0, 0, 0, 0};
	//str2ba("F4:B8:5E:B7:0E:D6", &dest);
	str2ba("E4:F9:8B:EA:B6:22", &dest);

	// connect
	struct sockaddr_l2 srcaddr, dstaddr;
	struct bt_security btsec;

	int sock = socket(PF_BLUETOOTH, SOCK_SEQPACKET, BTPROTO_L2CAP);
	if(sock < 0) {
		log(ERROR).out << "Unable to create BT L2CAP socket" << vnl::endl;
		return;
	}

	memset(&srcaddr, 0, sizeof(srcaddr));
	srcaddr.l2_family = AF_BLUETOOTH;
	srcaddr.l2_cid = htobs(ATT_CID);
	srcaddr.l2_bdaddr_type = 0;
	bacpy(&srcaddr.l2_bdaddr, &src);

	if(bind(sock, (struct sockaddr*)&srcaddr, sizeof(srcaddr)) < 0) {
		log(ERROR).out << "Unable to bind BT L2CAP socket" << vnl::endl;
		::close(sock);
		return;
	}

	memset(&btsec, 0, sizeof(btsec));
	btsec.level = sec;
	if(setsockopt(sock, SOL_BLUETOOTH, BT_SECURITY, &btsec, sizeof(btsec)) < 0) {
		log(ERROR).out << "Unable to set BT L2CAP security level" << vnl::endl;
		::close(sock);
		return;
	}

	memset(&dstaddr, 0, sizeof(dstaddr));
	dstaddr.l2_family = AF_BLUETOOTH;
	dstaddr.l2_cid = htobs(ATT_CID);
	dstaddr.l2_bdaddr_type = dst_type;
	bacpy(&dstaddr.l2_bdaddr, &dest);

	log(DEBUG).out << "Connecting ..." << vnl::endl;
	if(::connect(sock, (struct sockaddr*)&dstaddr, sizeof(dstaddr)) < 0) {
		log(ERROR).out << "Unable to connect: " << strerror(errno) << vnl::endl;
		::close(sock);
		return;
	}
	log(INFO).out << "Connected." << vnl::endl;





	log(DEBUG).out << "Disconnecting" << vnl::endl;
	::close(sock);*/





}

// BluezBTLEConnectorFactory implementation
// ------------------------------------------------------------------------------------

BluezBTLEConnectorFactory* BluezBTLEConnectorFactory::instance = nullptr;

vnl::String BluezBTLEConnectorFactory::moduleID() {
	return "BluezBTLEConnector";
}

vnl::String BluezBTLEConnectorFactory::moduleVersion() {
	vnl::String verstr;
	verstr << GIT_COMMIT_HASH << "@" << GIT_BRANCH << " (" << GIT_AUTHOR_DATE << ")";
	return verstr;
}

BluezBTLEConnectorFactory::~BluezBTLEConnectorFactory() {
	if(BluezBTLEConnectorFactory::instance) {
		BluezBTLEConnectorFactory* tmp = BluezBTLEConnectorFactory::instance;
		BluezBTLEConnectorFactory::instance = nullptr;
		delete tmp;
	}
}

vnl::Object* BluezBTLEConnectorFactory::createInstance(const vnl::String& domain, const vnl::String& topic) {
	return new BluezBTLEConnector(domain, topic);
}

int BluezBTLEConnectorFactory::destoryInstance(vnl::Object* obj) {
	BluezBTLEConnector* tmp = dynamic_cast<BluezBTLEConnector*>(obj);
	if(tmp) {
		delete tmp;
		return 0;
	}
	return -1;
}

} /* namespace connect */
} /* namespace homerunner */

#pragma GCC visibility push(default)

extern "C" homerunner::ModuleFactory* homerunner_get_factory() {
	if(homerunner::connect::BluezBTLEConnectorFactory::instance == nullptr) {
		homerunner::connect::BluezBTLEConnectorFactory::instance = new homerunner::connect::BluezBTLEConnectorFactory();
	}
	return homerunner::connect::BluezBTLEConnectorFactory::instance;
}

#pragma GCC visibility pop

