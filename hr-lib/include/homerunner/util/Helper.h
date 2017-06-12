/*
 * Helper.h
 *
 *  Created on: Jun 11, 2017
 *      Author: manuel
 */

#ifndef HELPER_H_
#define HELPER_H_

#include <sys/time.h>

namespace homerunner {
namespace util {

void init_timeval(struct timeval& tv, time_t sec, int usec);

} /* namespace util */
} /* namespace homerunner */

#endif /* HELPER_H_ */
