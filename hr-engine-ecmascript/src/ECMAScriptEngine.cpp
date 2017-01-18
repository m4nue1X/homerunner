/*
 * ECMAScriptEngine.cpp
 *
 *  Created on: Jan 1, 2017
 *      Author: manuel
 */

#include "ECMAScriptEngine.h"
#include <homerunner/ModuleFactory.h>

namespace homerunner {

ECMAScriptEngine::ECMAScriptEngine(const vnl::String& domain, const vnl::String& topic) : EngineBase(domain, topic) {
	// TODO Auto-generated constructor stub
}

ECMAScriptEngine::~ECMAScriptEngine() {
	// TODO Auto-generated destructor stub
}

} /* namespace homerunner */

#pragma GCC visibility push(default)

extern "C" homerunner::ModuleFactory* homerunner_get_factory() {
	new homerunner::ModuleFactoryImpl<homerunner::ECMAScriptEngine>("ECMAScriptEngine", vnl::String() << GIT_COMMIT_HASH << "@" << GIT_BRANCH << " (" << GIT_AUTHOR_DATE << ")");
}

#pragma GCC visibility pop

