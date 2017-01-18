/*
 * Node.cpp
 *
 *  Created on: Jan 1, 2017
 *      Author: manuel
 */

#include <homerunner/Node.h>
#include <vnl/Array.h>
#include <vnl/String.h>

namespace homerunner {

Node* Node::instance = nullptr;

Node::Node(vnl::Engine* engine) : NodeBase("homerunner", "Node"), eng(engine) {
	assert(Node::instance == nullptr);
	Node::instance = this;
}

Node::~Node() {
	close(); // make sure modules are unloaded
}

void Node::main() {
	// process module config
	vnl::Array<vnl::String> mod_fnames;
	vnl::Array<vnl::String> mod_fname_lines = modules.tokenize('\n');
	for(const vnl::String& mod_fname_line : mod_fname_lines) {
		if(mod_fname_line.find("//", 0, 2) < 0) {
			mod_fnames.push_back(mod_fname_line);
		}
	}

	// get lines
	vnl::Array<vnl::String> mod_inst_lines = instances.tokenize('\n');
	for(const vnl::String& mod_inst_line : mod_inst_lines) {
		if(mod_inst_line.find("//", 0, 2) < 0) {
			vnl::Array<vnl::String> mod_inst_strs = mod_inst_line.tokenize(':', true, 2);
			if(mod_inst_strs.size() >= 2) {
				vnl::Array<vnl::String> mod_strs = mod_inst_strs[0].tokenize('/', true, 2);
				vnl::String mod_name = mod_strs[0];
				vnl::String mod_subtype = (mod_strs.size() > 1) ? mod_strs[1] : "";
				vnl::Array<vnl::String> inst_strs = mod_inst_strs[1].tokenize(',');
				for(const vnl::String& inst_str : inst_strs) {
					vnl::String inst_name;
					inst_name << mod_name << "-" << inst_str;
					inst_mod_map.insert(inst_name, vnl::pair<vnl::String, vnl::String>(mod_name, mod_subtype));
				}
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

	for(vnl::Map<vnl::String, vnl::pair<vnl::String, vnl::String>>::const_iterator it = inst_mod_map.cbegin(); it != inst_mod_map.cend(); it++) {
		log(INFO).out << "Creating instance " << it->first << vnl::endl;
		vnl::Object* obj = createInstance(it->second.first, it->first, it->second.second);
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

