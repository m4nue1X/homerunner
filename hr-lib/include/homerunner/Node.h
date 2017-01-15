/*
 * Node.h
 *
 *  Created on: Jan 1, 2017
 *      Author: manuel
 */

#ifndef HOMERUNNER_NODE_H_
#define HOMERUNNER_NODE_H_

#include "ModuleManager.h"
#include <homerunner/NodeSupport.hxx>
#include <vnl/Engine.h>
#include <cassert>
#include <iostream>

namespace homerunner {

class Node: public NodeBase, public ModuleManager {
public:
	Node(vnl::Engine* engine);
	virtual ~Node();

	template<class T>
	static T* requireService(const vnl::String& inst_name);

protected:
	static Node* instance;

	void main();

private:
	vnl::Engine* eng = nullptr;

	vnl::Map<vnl::String, vnl::String> inst_mod_map;
};

template<class T>
T* Node::requireService(const vnl::String& inst_name) {
	assert(Node::instance);
	assert(Node::instance->eng);
	if(Node::instance->inst_mod_map.find(inst_name) == nullptr) {
		std::cerr << "No instance named " << inst_name << " found" << std::endl;
	}
	T* cli = new T(vnl::Address("homerunner", inst_name));
	cli->connect(Node::instance->eng);
	return cli;
}

} /* namespace homerunner */

#endif /* HOMERUNNER_NODE_H_ */
