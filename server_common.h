/*****************************************************************
ファイル名	: server_common.h
機能		: サーバーで使用する定数の宣言を行う
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
}CLIENT;

extern CLIENT	gClients[MAX_CLIENTS];	/* クライアント */

#endif
