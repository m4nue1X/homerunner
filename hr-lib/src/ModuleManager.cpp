/*
 * ModuleManager.cpp
 *
 *  Created on: Nov 6, 2016
 *      Author: manuel
 */

#include <homerunner/ModuleManager.h>
#include <dlfcn.h>
#include <string>

namespace homerunner {

ModuleManager::ModuleManager() {
	// nohting to initlaize
}

ModuleManager::~ModuleManager() {
	close();
}

void ModuleManager::close() {
	for(vnl::Map<vnl::String, ModuleContainer>::const_iterator it = mod_conts.cbegin(); it != mod_conts.cend(); ++it) {
		dlclose(it->second.dl_handle);
	}
	mod_conts.clear();
}

int ModuleManager::loadModule(const vnl::String& mod_name) {
	ModuleContainer mod_cont;

	dlerror();
	std::string stl_mod_name(mod_name.to_string());
	mod_cont.dl_handle = dlopen(stl_mod_name.c_str(), RTLD_NOW | RTLD_GLOBAL);
	if(mod_cont.dl_handle == NULL) {
		std::cerr << "Unable to load module " << mod_name << ": " << dlerror() << std::endl;
		return -1;
	}

	homerunner::ModuleFactory* (*get_factory)() = (homerunner_get_factory_func)(dlsym(mod_cont.dl_handle, "homerunner_get_factory"));
	if(!get_factory) {
		std::cerr << "Unable to find module factory. " << mod_name << " seems not to be a HomeRunner module." << std::endl;
		dlclose(mod_cont.dl_handle);
		return -1;
	}

	mod_cont.factory = get_factory();
	if(!mod_cont.factory) {
		std::cerr << "Unable to get instance of module factory of " << mod_name << std::endl;
		dlclose(mod_cont.dl_handle);
		return -1;
	}

	/*for(const vnl::String& libname : mod_cont.factory->requiredLibraries()) {
		std::string stl_libname(libname.to_string());
		if(dlopen(stl_libname.c_str(), RTLD_NOW | RTLD_GLOBAL) == NULL) {
			std::cerr << "Unable to load " << stl_libname << " required by " << stl_mod_name << std::endl;
		}
	}*/

	mod_conts.insert(mod_cont.factory->moduleID(), mod_cont);
	return 0;
}

vnl::Object* ModuleManager::createInstance(const vnl::String& mod_name, const vnl::String& inst_name) {
	ModuleContainer* mc = mod_conts.find(mod_name);
	if(!mc) {
		std::cerr << "Unable to find module " << mod_name << std::endl;
		return nullptr;
	}

	//std::cout << "Creating instance " << inst_name << std::endl;
	return mc->factory->createInstance("homerunner", inst_name);
}

} /* namespace homerunner */
