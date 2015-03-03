/*
 * RedisValString.cpp
 *
 *  Created on: Feb 23, 2015
 *      Author: netmind
 */

#include "RedisValString.h"

namespace aredis {

RedisValString::RedisValString() {
	mType = RV_STRING;
}

RedisValString::~RedisValString() {
}


void RedisValString::setVal(string&& val) {
	mVal = move(val);
}


string RedisValString::fetchVal() {
	return move(mVal);
}


const string& RedisValString::getVal() {
	return mVal;
}

} /* namespace aredis */
