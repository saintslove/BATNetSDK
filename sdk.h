/*
 * sdk.h
 *
 *  Created on: 2017年2月28日
 *      Author: wong
 */

#ifndef SDK_H_
#define SDK_H_

#include "CCMS.h"

int Init(int nDevType, char* strDevSN,  bool bAutoLogin);

void Release();

int CreateClientObj(CCMS_NETADDR* pServerAddr, int nProtocolType, bool bRaw);

int CreateServerObj(CCMS_NETADDR* pListenAddr, int nProtocolType, bool bRaw);

int DeleteObj(int nHandle);

#endif /* SDK_H_ */
