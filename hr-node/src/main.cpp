
#include <vnl/Layer.h>
#include <vnl/ThreadEngine.h>
#include <vnl/Terminal.h>
#include <vnl/String.h>
#include <vnl/Array.h>
#include <homerunner/Module.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <vector>
#include <dlfcn.h>
#include <iostream>
#include <string>

/*std::string basename(const std::string& str) {
	char buf[str.size()+1];
	strncpy(buf, str.c_str(), str.size()+1);
	char* bn = basename(buf);
	return bn;
}*/

void usage(const std::string& exe) {
	std::cout << exe << " -c <config-dir>" << std::endl;
}

int main(int argc, char** argv) {

	std::string dir_name;

	char c;
	while((c = getopt(argc, argv, "c:h")) != -1) {
		switch(c) {
		case 'c':
			dir_name = optarg;
			break;
		case 'h':
			usage(argv[0]);
			return 0;
		}
	}

	struct stat st;
	if(stat(dir_name.c_str(), &st) != 0 || (st.st_mode & S_IFDIR) == 0) {
		usage(argv[0]);
		return 1;
	}

	vnl::Layer layer("homerunner", dir_name.c_str());

	vnl::Array<vnl::String> mod_names;
	const vnl::String* mod_names_str = vnl::Layer::get_config("homerunner","node","modules");
	if(mod_names_str != nullptr) {
		mod_names = mod_names_str->tokenize(',');
	}

	if(mod_names.size() <= 0) {
		std::cout << "No modules configured. Exiting." << std::endl;
		layer.shutdown();
		return 0;
	}

	vnl::Array<void*> dl_handles;
	for(vnl::Array<vnl::String>::const_iterator it = mod_names.cbegin(); it != mod_names.cend(); it++) {
		dlerror();
		std::string mod_name(it->to_string());
		std::cout << "Loading module " << mod_name << std::endl;
		void* dl_handle = dlopen(mod_name.c_str(), RTLD_NOW | RTLD_GLOBAL);
		if(dl_handle == NULL) {
			std::cerr << "Unable to load module " << mod_name << ": " << dlerror() << std::endl;
			continue;
		}
		vnl::Object* (*create_mod_func)() = (create_module)(dlsym(dl_handle, "create_module"));
		int (*destroy_mod_func)(vnl::Object*) = (destroy_module)(dlsym(dl_handle, "destroy_module"));
		if(!create_mod_func || !destroy_mod_func) {
			std::cerr << mod_name << " seems not to be a HomeRunner module." << std::endl;
			dlclose(dl_handle);
			continue;
		}
		vnl::Object* obj = create_mod_func();
		if(!obj) {
			std::cerr << "Unable to create instance of " << mod_name << std::endl;
			dlclose(dl_handle);
			continue;
		}
		dl_handles.push_back(dl_handle);
		vnl::spawn(obj);
	}

	vnl::run(new vnl::Terminal());

	for(void* dl_handle : dl_handles) {
		dlclose(dl_handle);
	}
}
