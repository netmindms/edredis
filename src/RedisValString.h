/*
 * RedisValString.h
 *
 *  Created on: Feb 23, 2015
 *      Author: netmind
 */

#ifndef REDISVALSTRING_H_
#define REDISVALSTRING_H_

#include <string>

#include "RedisVal.h"

using namespace std;
namespace aredis {

class RedisValString;
typedef unique_ptr<RedisValString> upRedisValString;

class RedisValString : public RedisVal {
public:
	RedisValString();
	virtual ~RedisValString();
	void setVal(string &&val) override;
	string fetchVal();
	const string& getVal();

private:
	string mVal;
};



} /* namespace aredis */

#endif /* REDISVALSTRING_H_ */
