
#include <vnl/Layer.h>
#include <vnl/ThreadEngine.h>
#include <vnl/Terminal.h>
#include <homerunner/Module.h>
#include <vector>
#include <dlfcn.h>
#include <iostream>

int main(int argc, char** argv) {
	vnl::Layer layer("homerunner");

	std::vector<void*> dlhndls;
	for(int i=1; i<argc; i++) {
		dlerror();
		void* hndl = dlopen(argv[i], RTLD_NOW | RTLD_GLOBAL);
		if(hndl == NULL) {
			std::cerr << "Unable to load module " << argv[i] << ": " << dlerror() << std::endl;
			continue;
		}
		vnl::Object* (*create_mod)() = (create_module)(dlsym(hndl, "create_module"));
		int (*destroy_mod)(vnl::Object*) = (destroy_module)(dlsym(hndl, "destroy_module"));
		if(!create_mod || !destroy_mod) {
			std::cerr << argv[i] << " seems not to be a HomeRunner module." << std::endl;
			dlclose(hndl);
			continue;
		}
		vnl::Object* obj = create_mod();
		if(!obj) {
			std::cerr << "Unable to instanciate " << argv[i] << std::endl;
			dlclose(hndl);
			continue;
		}
		dlhndls.push_back(hndl);
		vnl::spawn(obj);
	}

	vnl::run(new vnl::Terminal());

	for(void* hndl : dlhndls) {
		dlclose(hndl);
	}
}
