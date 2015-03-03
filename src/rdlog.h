/*
 * rdlog.h
 *
 *  Created on: Feb 23, 2015
 *      Author: netmind
 */

#ifndef RDLOG_H_
#define RDLOG_H_


#define logd(FMT, ...) printf(FMT "\n", ## __VA_ARGS__)

//#define logd(FMT, ...) zlog_debug(zlogtag, FMT, ## __VA_ARGS__)
//#define logd(FMT, ...) zlog_debug(LOGCAT, FMT, ## __VA_ARGS__)


#endif /* RDLOG_H_ */
