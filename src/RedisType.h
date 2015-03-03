/*
 * RedisCast.h
 *
 *  Created on: Feb 24, 2015
 *      Author: netmind
 */

#ifndef REDISTYPE_H_
#define REDISTYPE_H_
namespace aredis {
#define CAST_REDIS_VAL(TYPE, SRC) unique_ptr<TYPE>( (TYPE*)(SRC.release()) )
#define REDIS_STR(SRC) CAST_REDIS_VAL(RedisValString, SRC)
#define REDIS_INT(SRC) CAST_REDIS_VAL(RedisValInt, SRC)
#define REDIS_ERR(SRC) CAST_REDIS_VAL(RedisValErr, SRC)
#define REDIS_ARR(SRC) CAST_REDIS_VAL(RedisValArr, SRC)
#define REDIS_BULK(SRC) CAST_REDIS_VAL(RedisValBulk, SRC)

#define RVPTR_ARR(SRC) ((RedisValArr*)SRC.get())
#define RVPTR_STR(SRC) ((RedisValString*)SRC.get())
#define RVPTR_BULK(SRC) ((RedisValBulk*)SRC.get())
#define RVPTR_INT(SRC) ((RedisValInt*)SRC.get())

}
#endif /* REDISTYPE_H_ */
