/*
 * RedisFrame.cpp
 *
 *  Created on: Feb 16, 2015
 *      Author: netmind
 */

#include <climits>
#include <assert.h>
#include <stdexcept>
#include <string.h>
#include "RedisFrame.h"

namespace aredis {

static auto _nullRtag = redistag(RV_NONE, string(""));

RedisFrame::RedisFrame() {
	mSendSeqSeed = 0;
	mRespSeqSeed = 0;
	mPs = PS_FIRST;
	mCurTag = RV_NONE;
	mBulkRemain = mBulkLen = 0;
	mBulkChunkSize = 0;
//	mArrayCnt = 0;
}

RedisFrame::~RedisFrame() {
}

int RedisFrame::feedPacket(const char* ptr, size_t len) {
	for (auto remain = len; remain > 0;) {
		if (mPs == PS_FIRST) {
			mPs = PS_STRING_START;
			if (*ptr == '+') {
				mCurTag = RV_STRING;
			} else if (*ptr == ':') {
				mCurTag = RV_INTEGER;
			} else if (*ptr == '$') {
				mCurTag = RV_BULK;
			} else if (*ptr == '*') {
				mCurTag = RV_ARRAY;
			} else if (*ptr == '-') {
				mCurTag = RV_ERROR;
			} else {
				return -1;
			}
			ptr++;
			remain--;
		} else if (mPs == PS_STRING_START) {
			const char *p = (const char*) memchr(ptr, '\r', remain);
			size_t cnt;
			if (p) {
				cnt = p - ptr;
				mPs = PS_STRING_LINEBRK;
				mRespStr.append(ptr, cnt);
				cnt++;
			} else {
				cnt = remain;
				mRespStr.append(ptr, cnt);
			}
			ptr += cnt;
			remain -= cnt;
		} else if (mPs == PS_STRING_LINEBRK) {
			if (*ptr++ == '\n') {
				remain--;
				if (mCurTag == RV_BULK) {
					mCurVal.reset(new RedisValBulk);
					try {
						mBulkRemain = mBulkLen = stoll(mRespStr);
					} catch (invalid_argument &err) {
						return -1;
					}
					if (mBulkRemain >= 0) {
						mPs = PS_BULK_DATA_START;
					} else {
						mPs = PS_FIRST;
					}
				} else {
					if (mCurTag == RV_STRING) {
						mCurVal.reset(new RedisValString);
					} else if (mCurTag == RV_INTEGER) {
						mCurVal.reset(new RedisValInt);
					} else if (mCurTag == RV_ARRAY) {
						mCurVal.reset(new RedisValArr);
					} else if (mCurTag == RV_ERROR) {
						mCurVal.reset(new RedisValErr);
					} else {
						assert(0);
					}
					mPs = PS_FIRST;
				}
				mCurVal->setVal(move(mRespStr));
				mRespStr.clear();
				if (mPs == PS_FIRST)
					addValList();
			} else {
				return -1;
			}
		} else if (mPs == PS_BULK_LINEBRK1) {
			if (*ptr++ == '\n') {
				remain--;
				mPs = PS_BULK_DATA_START;
				try {
					mBulkRemain = mBulkLen = stoll(mRespStr);
					mRespStr.clear();
				} catch (invalid_argument &err) {
					return -1;
				}
			} else {
				return -1;
			}
		} else if (mPs == PS_BULK_DATA_START) {
			size_t cnt = min((int64_t) remain, mBulkRemain);
			((RedisValBulk*) mCurVal.get())->appendData(ptr, cnt);
			ptr += cnt;
			remain -= cnt;
			mBulkRemain -= cnt;
			if (mBulkRemain == 0) {
				mPs = PS_BULK_DATA_END;
			}

		} else if (mPs == PS_BULK_DATA_END) {
			if (*ptr++ == '\r') {
				remain--;
				mPs = PS_BULK_LINEBRK2;
			} else {
				return -1;
			}

		} else if (mPs == PS_BULK_LINEBRK2) {
			if (*ptr++ == '\n') {
				remain--;
				mPs = PS_FIRST;
				addValList();
			} else {
				return -1;
			}
		}

	}
	return 0;
}

uint32_t RedisFrame::sendPing() {
	string ds = "PING\r\n";
	++mSendSeqSeed;
	mSendList.push_back(move(ds));
	return mSendSeqSeed;
}

string RedisFrame::getSendData() {
	if (!mSendList.empty()) {
		string data = mSendList.front();
		mSendList.pop_front();
		return move(data);
	}
	return "";
}

upRedisVal RedisFrame::fetchResp() {
//	logd("fetch resp, val list cnt=%d", mValList.size());
	if (mValList.empty() == false) {
		auto val = move(mValList.front());
		mValList.pop_front();
		mRespSeqSeed++;
		return move(val);
	} else {
		return nullptr;
	}

}

int64_t RedisFrame::fetchRespInt() {
	try {
		auto val = dynamic_cast<RedisValInt*>(fetchResp().release());
		int64_t ret = val->getVal();
		delete val;
		return ret;
	} catch (out_of_range &err) {
		throw -1;
	}

}

string RedisFrame::fetchRespStr() {
	try {
		auto val = dynamic_cast<RedisValString*>(fetchResp().release());
		auto ret = val->fetchVal();
		delete val;
		return ret;
	} catch (out_of_range &err) {
		throw -1;
	}
}

void RedisFrame::setBulkChunkSize(size_t len) {
	mBulkChunkSize = len;
}

bool RedisFrame::sendDataEmpty() const {
	return mSendList.empty();
}

int32_t RedisFrame::getRespId() const {
	return mRespSeqSeed;
}

int32_t RedisFrame::queRawCmd(string&& cmd) {
	cmd.append("\r\n");
	if(mSendSeqSeed==INT_MAX)
		mSendSeqSeed = 1;
	else
		++mSendSeqSeed;
	mSendList.push_back(move(cmd));
	return mSendSeqSeed;
}
//
//int32_t RedisFrame::queCmd(const string& cmd) {
//	string s = cmd+"";
//	queCmd(move(s));
//}

void RedisFrame::addValList() {
	if (mCurTag == RV_ARRAY) {
		if (mCurArray != nullptr) {
			mArrayStack.push(move(mCurArray));
		}
		mCurArray.reset((RedisValArr*) mCurVal.release());
		checkArrayMature();
	} else {
		if (mCurArray != nullptr) {
			mCurArray->addVal(move(mCurVal));
			checkArrayMature();
		} else {
			mValList.push_back(move(mCurVal));
		}
	}
}


void RedisFrame::checkArrayMature() {
	if (mCurArray->isMature()) {
		if (mArrayStack.empty() == true) {
			mValList.push_back(move(mCurArray));
		} else {
			upRedisValArr arr = move(mArrayStack.top());
			arr->addVal(move(mCurArray));
			mCurArray = move(arr);
			mArrayStack.pop();
			checkArrayMature();
		}
	}
}

} /* namespace aredis */
