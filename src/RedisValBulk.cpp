/*
 * RedisValBulk.cpp
 *
 *  Created on: Feb 23, 2015
 *      Author: netmind
 */

#include "RedisValBulk.h"

namespace aredis {

RedisValBulk::RedisValBulk() {
	mType = RV_BULK;
}

RedisValBulk::~RedisValBulk() {
}

void RedisValBulk::setVal(string&& str) {
	mSize = stoll(str);
}

void RedisValBulk::appendData(const char* buf, size_t len) {
	mVal.append(buf, len);
}


string RedisValBulk::fetchVal() {
	return move(mVal);
}

const string& RedisValBulk::getVal() {
	return mVal;
}

int64_t aredis::RedisValBulk::getSize() {
	return mSize;
}



} /* namespace aredis */
