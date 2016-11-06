/*
 * Module.h
 *
 *  Created on: Oct 22, 2016
 *      Author: manuel
 */

#include <vnl/Object.h>

#ifndef HR_MODULE_H_
#define HR_MODULE_H_

#define API __attribute__ ((visibility ("default")))

typedef vnl::Object* (*create_module)();
typedef int (*destroy_module)(vnl::Object* instance);

#endif /* HR_MODULE_H_ */
