/*
 * RedisValInt.h
 *
 *  Created on: Feb 23, 2015
 *      Author: netmind
 */

#ifndef REDISVALINT_H_
#define REDISVALINT_H_

#include <stdint.h>

#include "RedisVal.h"

namespace aredis {

class RedisValInt: public RedisVal {
public:
	RedisValInt();
	virtual ~RedisValInt();
	void setVal(string &&str) override;
	int64_t getVal();
private:
	int64_t mVal;
};

typedef unique_ptr<RedisValInt> upRedisValInt;

} /* namespace aredis */

#endif /* REDISVALINT_H_ */
