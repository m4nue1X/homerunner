/*
 * jsoncoder.cpp
 *
 *  Created on: Jan 2, 2017
 *      Author: manuel
 */

#include <iostream>
#include <unistd.h>
#include <cstdint>
#include <cassert>

std::string encode(const std::string& json) {
	int n = json.length();
	std::string hex;
	std::string map = "0123456789abcdef";
	uint8_t key = 0xAB;
	for(int i=0; i<n+4; i++) {
		uint8_t b = 0;
		if(i < 4) { // length field
			b = (n >> (24 - (i*8))) & 0xff;
		} else {
			b = (uint8_t)(json[i-4] ^ key);
			key = b;
		}
		hex.push_back(map[b >> 4]);
		hex.push_back(map[b & 0xf]);
	}
	return hex;
}

std::string decode(const std::string& hex, bool lenpfx, long* len=nullptr) {
	int n = hex.length();
	if(n%2 != 0) {
		throw std::invalid_argument("Number of bytes is odd");
	}
	std::string val = "  ";
	std::string json;
	uint8_t key = 0xAB;
	for(int i=0; i<n; i+=2) {
		val[0] = hex[i];
		val[1] = hex[i+1];
		uint8_t b = std::stoi(val, 0, 16);
		if(lenpfx && i < 8) { // length field
			if(len) {
				(*len) |= (b << (24 - (i*8/2)));
			}
		} else {
			json.push_back((char)(b ^ key));
			key = b;
		}
	}
	return json;
}

int main(int argc, char** argv) {
	int mode = -1;
	bool lenpfx = false;

	char c;
	std::string val;
	while((c = getopt(argc, argv, "e:d:l")) != -1) {
		switch(c) {
		case 'e':
			mode = 1;
			val = optarg;
			break;
		case 'd':
			mode = 2;
			val = optarg;
			break;
		case 'l':
			lenpfx = true;
			break;
		}
	}

	if(mode < 0) {
		std::cout << "Usage: jsoncoder <-e <json>| -d <hex>> [-l]" << std::endl;
		return 1;
	}

	std::string res;
	long len = 0;
	if(mode == 1) {
		std::cout << val << std::endl;
		res = encode(val);
	} else {
		res = decode(val, lenpfx, &len);
	}
	std::cout << res << " (" << len << ")" << std::endl;

}
