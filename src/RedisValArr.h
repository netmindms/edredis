/*
 * RedisValArr.h
 *
 *  Created on: Feb 23, 2015
 *      Author: netmind
 */

#ifndef REDISVALARR_H_
#define REDISVALARR_H_

#include <vector>

#include "RedisVal.h"

namespace aredis {

class RedisValArr: public RedisVal {
public:
	RedisValArr();
	virtual ~RedisValArr();
	void addVal(upRedisVal val);
	void setVal(string &&str) override;
	size_t getCount() const;
	int64_t getSize() const;
	bool isMature() const;
	vector<upRedisVal>& getArray();
	const string& getstr(int idx);
	template<class T> T* getElemPtr(int idx) {
		return ((T*)(mVal[idx].get()));
	};

private:
	vector<upRedisVal> mVal;
	int64_t mSize;
};

typedef unique_ptr<RedisValArr> upRedisValArr;

} /* namespace aredis */

#endif /* REDISVALARR_H_ */
