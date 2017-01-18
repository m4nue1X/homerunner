/*
 * ModuleManager.h
 *
 *  Created on: Nov 6, 2016
 *      Author: manuel
 */

#ifndef MODULEMANAGER_H_
#define MODULEMANAGER_H_

#include <vnl/String.h>
#include <vnl/Map.h>
#include <vnl/Object.h>
#include <homerunner/ModuleFactory.h>
#include <homerunner/Exception.h>

namespace homerunner {

struct ModuleContainer {
	void* dl_handle;
	ModuleFactory* factory;
};

class ModuleManager {
public:
	ModuleManager();
	virtual ~ModuleManager();

protected:
	int loadModule(const vnl::String& mod_name);
	void close();

	vnl::Object* createInstance(const vnl::String& mod_name, const vnl::String& inst_name, const vnl::String& inst_type);

private:
	vnl::Map<vnl::String, ModuleContainer> mod_conts;

};

} /* namespace homerunner */

#endif /* MODULEMANAGER_H_ */
