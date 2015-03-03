/*
 * RedisValErr.h
 *
 *  Created on: Feb 23, 2015
 *      Author: netmind
 */

#ifndef REDISVALERR_H_
#define REDISVALERR_H_

#include "RedisVal.h"

namespace aredis {

class RedisValErr: public RedisVal {
public:
	RedisValErr();
	virtual ~RedisValErr();
	string getVal();
	void setVal(string &&str) override;
private:
	string mVal;
};

typedef unique_ptr<RedisValErr> upRedisValErr;

} /* namespace aredis */

#endif /* REDISVALERR_H_ */
