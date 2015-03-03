/*
 * RedisValArr.cpp
 *
 *  Created on: Feb 23, 2015
 *      Author: netmind
 */

#include "RedisValArr.h"
#include "RedisValString.h"
#include "RedisType.h"
#include "RedisValBulk.h"
#include "RedisValInt.h"

namespace aredis {

RedisValArr::RedisValArr() {
	mType = RV_ARRAY;
}

RedisValArr::~RedisValArr() {
}


void RedisValArr::addVal(upRedisVal val) {
	mVal.push_back(move(val));
}

void RedisValArr::setVal(string&& str) {
	mSize = stoll(str);
}


size_t RedisValArr::getCount() const {
	return mVal.size();
}


bool RedisValArr::isMature() const {
	if(mSize<=0)
		return true;
	else
		return (mSize == (int64_t)mVal.size());
}


int64_t aredis::RedisValArr::getSize() const {
	return mSize;
}

vector<upRedisVal>& RedisValArr::getArray() {
	return mVal;
}



const string& RedisValArr::getstr(int idx) {
	static string _ts;
	switch(mVal[idx]->getType())
	{
		case RV_STRING:
			return RVPTR_STR(mVal[idx])->getVal();
		case RV_BULK:
			return RVPTR_BULK(mVal[idx])->getVal();
		case RV_INTEGER:
		{
			auto num =  RVPTR_INT(mVal[idx])->getVal();

			_ts = to_string(num);
			return _ts;
		}
		default:
			_ts = "";
			return _ts;
	}
}

} /* namespace aredis */
