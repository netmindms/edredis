/*
 * RedisValBulk.h
 *
 *  Created on: Feb 23, 2015
 *      Author: netmind
 */

#ifndef REDISVALBULK_H_
#define REDISVALBULK_H_

#include "RedisVal.h"

namespace aredis {

class RedisValBulk: public RedisVal {
public:
	RedisValBulk();
	virtual ~RedisValBulk();
	void setVal(string &&str) override;
	void appendData(const char* ptr, size_t len);
	string fetchVal();
	const string &getVal();
	int64_t getSize();

private:
	int64_t mSize;
	string mVal;

};

typedef unique_ptr<RedisValBulk> upRedisValBulk;

} /* namespace aredis */

#endif /* REDISVALBULK_H_ */
