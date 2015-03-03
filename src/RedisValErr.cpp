/*
 * RedisValErr.cpp
 *
 *  Created on: Feb 23, 2015
 *      Author: netmind
 */

#include "RedisValErr.h"

namespace aredis {

RedisValErr::RedisValErr() {
	mType = RV_ERROR;
}

RedisValErr::~RedisValErr() {
	// TODO Auto-generated destructor stub
}

string RedisValErr::getVal() {
	return move(mVal);
}

void RedisValErr::setVal(string&& str) {
	mVal = move(str);
}

} /* namespace aredis */
