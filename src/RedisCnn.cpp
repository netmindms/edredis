/*
 * RedisCnn.cpp
 *
 *  Created on: Feb 23, 2015
 *      Author: netmind
 */

#include "RedisCnn.h"

#include "rdlog.h"

namespace aredis {

RedisCnn::RedisCnn() {
	// TODO Auto-generated constructor stub

}

RedisCnn::~RedisCnn() {
	// TODO Auto-generated destructor stub
}

int RedisCnn::connect(const string& ipaddr, int port) {
	mSock.socketOpen(SOCKET_NORMAL);
	mSock.setOnNetListener(this);
	return mSock.connect(ipaddr.c_str(), port);
}

void RedisCnn::close() {
	mSock.socketClose();
}

int32_t RedisCnn::sendPing() {
	auto sid = mFrameLay.sendPing();
	tx();
	return sid;
}

void RedisCnn::IOnNet(EdSmartSocket* psock, int event) {
	if (event == NETEV_CONNECTED) {
		logd("connected...");
	} else if (event == NETEV_DISCONNECTED) {
		logd("disconnected...");
	} else if (event == NETEV_READ) {
		char buf[1024];
		auto len = mSock.recvPacket(buf, 1024);
		if (len > 0) {
			logd("recv cnt=%d", len);
			mFrameLay.feedPacket(buf, len);
			for (;;) {
				auto resp = mFrameLay.fetchResp();
				if (resp != nullptr) {
					auto rid = mFrameLay.getRespId();
					logd("resp id=%u", mFrameLay.getRespId());
//					mOnLis(mFrameLay.getRespId(), move(resp));
					auto itr = mCbMap.find(rid);
					if(itr != mCbMap.end()) {
						itr->second(rid, move(resp));
						mCbMap.erase(itr);
					} else {
						logd("*** cb not found, id=%d", rid);
					}
					logd("cb map size=%u", (uint32_t)mCbMap.size());
				} else {
					break;
				}
			}
		}
	}
}

void RedisCnn::setOnResplistener(
		function<void(uint32_t, upRedisVal)> lis) {
	mOnLis = lis;
}

void RedisCnn::tx() {
	for (;;) {
		if (mFrameLay.sendDataEmpty() == false) {
			if (mSock.isWritable() == true) {
				string s = mFrameLay.getSendData();
				mSock.sendPacket(s.data(), s.size());
			} else {
				mSock.reserveWrite();
			}
		} else {
			break;
		}
	}
}

int32_t RedisCnn::queCmd(const string& cmd, redisCb cb) {
	auto sid = mFrameLay.queRawCmd(cmd+"\r\n");
	if(sid > 0) {
		mCbMap[sid] = cb;
	}
	return sid;
}

void RedisCnn::quitCmd(int32_t rid) {
	auto itr = mCbMap.find(rid);
	if(itr != mCbMap.end()) {
		mCbMap.erase(itr);
	}
}

}
