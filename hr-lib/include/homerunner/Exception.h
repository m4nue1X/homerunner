/*
 * Exception.h
 *
 *  Created on: Jan 7, 2017
 *      Author: manuel
 */

#ifndef EXCEPTION_H_
#define EXCEPTION_H_

#include <vnl/String.h>

namespace homerunner {

class Exception {
public:
	Exception(const vnl::String& message);
	virtual ~Exception();

	const vnl::String& what();

private:
	vnl::String msg;
};

} /* namespace homerunner */

#endif /* EXCEPTION_H_ */
