/*
 * RedisFrame.h
 *
 *  Created on: Feb 16, 2015
 *      Author: netmind
 */

#ifndef REDISFRAME_H_
#define REDISFRAME_H_

#include <stdint.h>
#include <list>
#include <string>
#include <vector>
#include <stack>

#include "rdlog.h"
#include "RedisValString.h"
#include "RedisValInt.h"
#include "RedisValArr.h"
#include "RedisValErr.h"
#include "RedisValBulk.h"

#define BULK_CHUNK_SIZE 2048

using namespace std;
namespace aredis {

typedef pair<char, string> redistag;
typedef pair<char, upRedisVal> redisvalinfo;
//struct redistag {
//	char type;
//	string val;
//	redistag(char type, string&& val) {
//		this->type = type;
//		this->val = move(val);
//	}
//};



class RedisFrame {
private:
	enum {
		PS_FIRST,
		PS_STRING_START,
		PS_STRING_LINEBRK,
		PS_BULK_LINEBRK1,
		PS_BULK_LINEBRK2,
		PS_BULK_DATA_START,
		PS_BULK_DATA_END,
	};
public:
	enum {
		RFE_NONE,
		RFE_SEND,
		RFE_RSTR,
		RFE_RINT,
		RFE_BULK_START,
		RFE_BULK_CONT,
		RFE_BULK_END,
	};

	RedisFrame();
	virtual ~RedisFrame();
	int feedPacket(const char* ptr, size_t len);
	uint32_t sendPing();
	string getSendData();
	upRedisVal fetchResp();
	int64_t fetchRespInt();
	string fetchRespStr();
	string fetchBulk();
	void setBulkChunkSize(size_t len);
	bool sendDataEmpty() const;
	int32_t getRespId() const;
	int32_t queRawCmd(string &&cmd);
//	int32_t queCmd(const string &cmd);
private:
	list<string>  mSendList;
	int32_t mSendSeqSeed;
	int32_t mRespSeqSeed;
	int mPs;
	string mRespStr;
	list<upRedisVal> mValList;
	int64_t mBulkLen, mBulkRemain;
	unsigned char mCurTag;
	size_t mBulkChunkSize;
	upRedisValArr mCurArray;
	stack<upRedisValArr> mArrayStack;
	upRedisVal mCurVal;
private:
	void addValList();
	void checkArrayMature();
};

} /* namespace redis */

#endif /* REDISFRAME_H_ */
