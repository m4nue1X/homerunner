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
	virtual vnl::Object* createInstance(const vnl::String& domain, const vnl::String& topic) = 0;
	virtual vnl::String moduleID() = 0;
	virtual vnl::String moduleVersion() = 0;
	virtual vnl::Array<vnl::String> requiredLibraries() { return vnl::Array<vnl::String>(); }
};

template<class T>
class ModuleFactoryImpl : public ModuleFactory {
public:

	ModuleFactoryImpl(const vnl::String& mod_id, const vnl::String& mod_ver, const vnl::Array<vnl::String>& req_libs) : module_id(mod_id),
			module_version(mod_ver), required_libs(req_libs) {
		// nothing to do
	}

	ModuleFactoryImpl(const vnl::String& mod_id, const vnl::String& mod_ver) : module_id(mod_id),
			module_version(mod_ver) {
		// nothing to do
	}

	virtual ~ModuleFactoryImpl() {
		// nothing to do
	}

	vnl::Object* createInstance(const vnl::String& domain, const vnl::String& topic) {
		return new T(domain, topic);
	}

	vnl::String moduleID() {
		return module_id;
	}

	vnl::String moduleVersion() {
		return module_version;
	}

	vnl::Array<vnl::String> requiredLibraries() {
		return required_libs;
	}

private:
	vnl::String module_id;
	vnl::String module_version;
	vnl::Array<vnl::String> required_libs;
};

} /* namespace homerunner */

typedef homerunner::ModuleFactory* (*homerunner_get_factory_func)();

#endif /* HR_MODULEFACTORY_H_ */
