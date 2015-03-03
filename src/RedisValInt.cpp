/*
 * RedisValInt.cpp
 *
 *  Created on: Feb 23, 2015
 *      Author: netmind
 */

#include "RedisValInt.h"

namespace aredis {

RedisValInt::RedisValInt() {
	mType = RV_INTEGER;
	mVal = 0;
}

RedisValInt::~RedisValInt() {
}


int64_t RedisValInt::getVal() {
	return mVal;
}


void RedisValInt::setVal(string&& str) {
	mVal = stoll(str);
}

} /* namespace aredis */
