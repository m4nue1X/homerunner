/*
 * ECMAScriptEngine.h
 *
 *  Created on: Jan 1, 2017
 *      Author: manuel
 */

#ifndef HR_ECMASCRIPTENGINE_H_
#define HR_ECMASCRIPTENGINE_H_

#include <vnl/String.h>
#include <homerunner/EngineSupport.hxx>

namespace homerunner {

class ECMAScriptEngine : public EngineBase {
public:
	ECMAScriptEngine(const vnl::String& domain, const vnl::String& topic);
	virtual ~ECMAScriptEngine();
};

} /* namespace homerunner */

#endif /* HR_ECMASCRIPTENGINE_H_ */
