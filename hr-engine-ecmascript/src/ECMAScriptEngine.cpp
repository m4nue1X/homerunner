/*
 * ECMAScriptEngine.cpp
 *
 *  Created on: Jan 1, 2017
 *      Author: manuel
 */

#include "ECMAScriptEngine.h"

namespace homerunner {

ECMAScriptEngine::ECMAScriptEngine(const vnl::String& domain, const vnl::String& topic) : EngineBase(domain, topic) {
	// TODO Auto-generated constructor stub
}

ECMAScriptEngine::~ECMAScriptEngine() {
	// TODO Auto-generated destructor stub
}

// ECMAScriptEngineFactory implementation
// ------------------------------------------------------------------------------------

ECMAScriptEngineFactory* ECMAScriptEngineFactory::instance = nullptr;

vnl::String ECMAScriptEngineFactory::moduleID() {
	return "ECMAScriptEngine";
}

vnl::String ECMAScriptEngineFactory::moduleVersion() {
	vnl::String verstr;
	verstr << GIT_COMMIT_HASH << "@" << GIT_BRANCH << " (" << GIT_AUTHOR_DATE << ")";
	return verstr;
}

ECMAScriptEngineFactory::~ECMAScriptEngineFactory() {
	if(ECMAScriptEngineFactory::instance) {
		ECMAScriptEngineFactory* tmp = ECMAScriptEngineFactory::instance;
		ECMAScriptEngineFactory::instance = nullptr;
		delete tmp;
	}
}

vnl::Object* ECMAScriptEngineFactory::createInstance(const vnl::String& domain, const vnl::String& topic) {
	return new ECMAScriptEngine(domain, topic);
}

int ECMAScriptEngineFactory::destoryInstance(vnl::Object* obj) {
	ECMAScriptEngine* tmp = dynamic_cast<ECMAScriptEngine*>(obj);
	if(tmp) {
		delete tmp;
		return 0;
	}
	return -1;
}

} /* namespace homerunner */

#pragma GCC visibility push(default)

extern "C" homerunner::ModuleFactory* homerunner_get_factory() {
	if(homerunner::ECMAScriptEngineFactory::instance == nullptr) {
		homerunner::ECMAScriptEngineFactory::instance = new homerunner::ECMAScriptEngineFactory();
	}
	return homerunner::ECMAScriptEngineFactory::instance;
}

#pragma GCC visibility pop

