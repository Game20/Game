/*****************************************************************
ファイル名	: server_common.h
機能		: サーバーで使用する定数の宣言を行う
作成者氏名:船坂国之,高松翔馬,森祥悟
最終更新日:2014.12.14
*****************************************************************/

#ifndef _SERVER_COMMON_H_
#define _SERVER_COMMON_H_

#include"common.h"

#define ALL_CLIENTS	-1   /* 全クライアントにデータを送る時に使用する */



//x座標，y座標を表す構造体
typedef struct{
    int x;
    int y;
}Plc;


/* 追加 */
/* クライアントを表す構造体 */
typedef struct{
    int		fd;
    char	name[MAX_NAME_SIZE];
    char	command;
    Plc plc;//x座標y座標
    Plc anim;
}CLIENT;

CLIENT	gClients[MAX_CLIENTS];	/* クライアント */

extern int gClientsNum;

#endif
