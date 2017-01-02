/*
 * ModuleFactory.h
 *
 *  Created on: Nov 6, 2016
 *      Author: manuel
 */

#ifndef HR_MODULEFACTORY_H_
#define HR_MODULEFACTORY_H_

#include <vnl/String.h>
#include <vnl/Object.h>

namespace homerunner {

class ModuleFactory {
public:
	virtual ~ModuleFactory() { };
	virtual vnl::String moduleID() = 0;
	virtual vnl::String moduleVersion() = 0;
	virtual vnl::Object* createInstance(const vnl::String& domain, const vnl::String& topic) = 0;
	virtual int destoryInstance(vnl::Object* obj) = 0;
};

} /* namespace hr */

typedef homerunner::ModuleFactory* (*homerunner_get_factory_func)();

#endif /* HR_MODULEFACTORY_H_ */
