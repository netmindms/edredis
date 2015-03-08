/*
 * RedisCnn.h
 *
 *  Created on: Feb 23, 2015
 *      Author: netmind
 */

#ifndef REDISCNN_H_
#define REDISCNN_H_

#include <ednio/EdNio.h>
#include "RedisFrame.h"
using namespace edft;

namespace aredis {

#define TYPE_GET(res) (res->getType()==RV_BULK)
#define TYPE_SET(res) (res->getType()==RV_STRING)

typedef function<void(int32_t, upRedisVal)> redisCb;

class RedisCnn  {
public:
	RedisCnn();
	virtual ~RedisCnn();
	int connect(const string& ipaddr, int port=6379);
	void close();
	int32_t sendPing();
	int32_t queCmd(const string &cmd, redisCb cb);
	void setOnResplistener(function<void (uint32_t sid, upRedisVal val)> lis);
	void tx();
	void quitCmd(int32_t rid);
private:
	RedisFrame mFrameLay;
	EdSmartSocket mSock;
	unordered_map<int32_t, redisCb> mCbMap;
	function<void (uint32_t, upRedisVal)> mOnLis;


};

}
#endif /* REDISCNN_H_ */
