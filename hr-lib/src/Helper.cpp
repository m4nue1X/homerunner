/*
 * Helper.cpp
 *
 *  Created on: Jun 11, 2017
 *      Author: manuel
 */

#include <homerunner/util/Helper.h>

namespace homerunner {
namespace util {

void init_timeval(struct timeval& tv, time_t sec, int usec) {
	tv.tv_sec = sec;
	tv.tv_usec = usec;
}

} /* namespace util */
} /* namespace homerunner */
