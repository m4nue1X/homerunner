
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <iostream>
#include <string>
#include <vnl/Layer.h>
#include <vnl/ThreadEngine.h>
#include <vnl/Terminal.h>
#include <homerunner/Node.h>

std::string basename(const std::string& str) {
	char buf[str.size()+1];
	strncpy(buf, str.c_str(), str.size()+1);
	std::string bn(basename(buf));
	return bn;
}

void usage(const std::string& exe) {
	std::cout << basename(exe) << " -c <config-dir> [-i]" << std::endl;
}

int main(int argc, char** argv) {

	std::string dir_name;
	bool interactive = false;

	char c;
	while((c = getopt(argc, argv, "c:ih")) != -1) {
		switch(c) {
		case 'c':
			dir_name = optarg;
			break;
		case 'i':
			interactive = true;
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

	// start layer, load config
	vnl::Layer layer("homerunner", dir_name.c_str());

	vnl::ThreadEngine engine;

	if(interactive) {
		vnl::spawn(new homerunner::Node(&engine));
		vnl::run(new vnl::Terminal());
	} else {
		vnl::run(new homerunner::Node(&engine));
	}

}
