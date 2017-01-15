/*
 * Exception.cpp
 *
 *  Created on: Jan 7, 2017
 *      Author: manuel
 */

#include <homerunner/Exception.h>

namespace homerunner {

Exception::Exception(const vnl::String& message) : msg(message) {
	// nothing more to do
}

Exception::~Exception() {
	// nothing to do
}

const vnl::String& Exception::what() {
	return msg;
}

} /* namespace homerunner */
