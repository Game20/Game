/*****************************************************************
ファイル名	: server_func.h
機能		: サーバーの外部関数の定義
*****************************************************************/

#ifndef _SERVER_FUNC_H_
#define _SERVER_FUNC_H_

#include"server_common.h"

/* server_net.c */
extern int SetUpServer(int num);
extern void Ending(void);
extern int RecvIntData(int pos,int *intData);
extern void SendData(int pos,void *data,int dataSize);
extern int SendRecvManager(void);

/* server_command.c */
extern void SetIntData2DataBlock(void *data,int intData,int *dataSize);
extern void SetCharData2DataBlock(void *data,char charData,int *dataSize);

extern int ExecuteCommand(char command,int pos);

/* server_system.c */
extern void CalcPos(int pos);/*座標計算*/

#endif
