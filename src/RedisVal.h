/*
 * RedisValBase.h
 *
 *  Created on: Feb 23, 2015
 *      Author: netmind
 */

#ifndef REDISVAL_H_
#define REDISVAL_H_

#include <string>
#include <vector>
#include <memory>

using namespace std;

namespace aredis {

enum {
	RV_NONE,
	RV_STRING='+',
	RV_INTEGER=':',
	RV_BULK='$',
	RV_ARRAY='*',
	RV_ERROR='-',
};

class RedisVal {
public:
	RedisVal();
	virtual ~RedisVal();
	virtual void setVal(string &&val)=0;
	int getType() const { return mType; };
protected:
	int mType;
};

typedef unique_ptr<RedisVal> upRedisVal;

} /* namespace aredis */

#endif /* REDISVAL_H_ */
