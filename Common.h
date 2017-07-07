// **************************************************************
// File        : Common.h
// Description : 北斗数据平台系统公用宏定义文件
// Author      : LiuRuihan
// Date        : 2016-7-24
// Version     : V1.0.0.0
// Revisions   : 
// **************************************************************

#include <string.h>

/*********************************************************************
 *
 *                         公用宏定义
 *
 ********************************************************************/
#define DLL_PUBLIC  __attribute__((visibility("default")))
#define BATSDK_API  extern "C" DLL_PUBLIC

#ifndef NULL
#define NULL 0
#endif

#ifndef SAFE_DELETE
//Delete object by New create
#define SAFE_DELETE(p) { if(p != NULL) { delete (p); (p) = NULL; } }
#endif

#ifndef SAFE_DELETEA
//Delete Arrary
#define SAFE_DELETEA(p) { if(p != NULL) { delete[] (p); (p) = NULL; } }
#endif

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p) { if(p != NULL) { (p)->Release(); (p) = NULL; } }
#endif

#ifndef SAFE_FREE
#define SAFE_FREE(p) { if(p != NULL) { free(p); (p) = NULL; } }
#endif


#ifndef ARRSTRLEN
// for uint8_t[len]
#define ARRSTRLEN(array) \
    std::min(sizeof(array), strlen(reinterpret_cast<const char*>(array)))
#endif

#ifndef ARRAY2STR
// uint8_t[len] ==> std::string
#define ARRAY2STR(array) \
    std::string(reinterpret_cast<const char*>(array), ARRSTRLEN(array))
#endif

#ifndef STR2ARRAY
// std::string ==> uint8_t[len]
#define STR2ARRAY(str, array) \
    memcpy(array, str.data(), std::min(str.length(), sizeof(array)))
#endif
