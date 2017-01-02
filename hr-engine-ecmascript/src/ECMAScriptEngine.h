/*
 * ECMAScriptEngine.h
 *
 *  Created on: Jan 1, 2017
 *      Author: manuel
 */

#ifndef ECMASCRIPTENGINE_H_
#define ECMASCRIPTENGINE_H_

#include <vnl/String.h>
#include <homerunner/EngineSupport.hxx>
#include <homerunner/ModuleFactory.h>

namespace homerunner {

class ECMAScriptEngine : public EngineBase {
public:
	ECMAScriptEngine(const vnl::String& domain, const vnl::String& topic);
	virtual ~ECMAScriptEngine();
};

class ECMAScriptEngineFactory : public ModuleFactory {
public:
	virtual ~ECMAScriptEngineFactory();
	virtual vnl::String moduleID();
	virtual vnl::String moduleVersion();
	virtual vnl::Object* createInstance(const vnl::String& domain, const vnl::String& topic);
	virtual int destoryInstance(vnl::Object* obj);

	static ECMAScriptEngineFactory* instance;
};

} /* namespace homerunner */

#endif /* ECMASCRIPTENGINE_H_ */
