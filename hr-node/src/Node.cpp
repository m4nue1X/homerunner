/*
 * Node.cpp
 *
 *  Created on: Jan 1, 2017
 *      Author: manuel
 */

#include "Node.h"
#include <vnl/Array.h>
#include <vnl/String.h>

namespace homerunner {

Node::Node() : NodeBase("homerunner", "Node") {
	// nothing to initalize
}

Node::~Node() {
	close(); // make sure modules are unloaded
}

void Node::main() {
	// process module config
	vnl::Array<vnl::String> mod_fnames;
	mod_fnames = modules.tokenize('\n');

	vnl::Array<vnl::String> mod_inst_lines = instances.tokenize('\n', false);
	for(vnl::Array<vnl::String>::const_iterator itlns = mod_inst_lines.cbegin(); itlns != mod_inst_lines.cend(); ++itlns) {
		vnl::Array<vnl::String> mod_inst_strs = itlns->tokenize(':', true, 2);
		if(mod_inst_strs.size() >= 2) {
			vnl::Array<vnl::String> inst_strs = mod_inst_strs[1].tokenize(',');
			for(vnl::Array<vnl::String>::const_iterator itinst = inst_strs.cbegin(); itinst != inst_strs.cend(); itinst++) {
				vnl::String inst_name;
				inst_name << mod_inst_strs[0] << "-" << *itinst;
				inst_mod_map.insert(inst_name, mod_inst_strs[0]);
			}
		}
	}

	// load modules & create instances
	for(vnl::Array<vnl::String>::const_iterator it = mod_fnames.cbegin(); it != mod_fnames.cend(); ++it) {
		log(INFO).out << "Loading module " << *it << vnl::endl;
		if(loadModule(*it) < 0) {
			log(WARN).out << "Unable to load module " << *it << vnl::endl;
		}
	}

	for(vnl::Map<vnl::String, vnl::String>::const_iterator it = inst_mod_map.cbegin(); it != inst_mod_map.cend(); it++) {
		log(INFO).out << "Creating instance " << it->first << vnl::endl;
		vnl::Object* obj = createInstance(it->second, it->first);
		if(obj) {
			vnl::spawn(obj);
		} else {
			log(WARN).out << "Unable to create instance: " << it->first << vnl::endl;
		}
	}

	run();

	close();
}

} /* namespace homerunner */

