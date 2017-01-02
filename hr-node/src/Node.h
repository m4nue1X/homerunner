/*
 * Node.h
 *
 *  Created on: Jan 1, 2017
 *      Author: manuel
 */

#ifndef HOMERUNNER_NODE_H_
#define HOMERUNNER_NODE_H_

#include <homerunner/NodeSupport.hxx>
#include "ModuleManager.h"

namespace homerunner {

class Node: public NodeBase, public ModuleManager {
public:
	Node();
	virtual ~Node();

protected:
	void main();

private:
	vnl::Map<vnl::String, vnl::String> inst_mod_map;

};

} /* namespace homerunner */

#endif /* HOMERUNNER_NODE_H_ */
