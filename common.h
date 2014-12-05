/*****************************************************************
ファイル名	: common.h
機能		: サーバーとクライアントで使用する定数の宣言を行う
*****************************************************************/

#ifndef _COMMON_H_
#define _COMMON_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<assert.h>
#include<math.h>

#define PORT			(u_short)8888	/* ポート番号 */

#define MAX_CLIENTS	    4				/* クライアント数の最大値 */
#define MAX_NAME_SIZE	10 				/* ユーザー名の最大値*/

#define MAX_DATA		200				/* 送受信するデータの最大値 */

#define END_COMMAND 'E' /* プログラム終了コマンド */

#define MOVE_COMMAND 'M' /*移動コマンド*/

#define OBJECT_COMMAND 'O' //オブジェクトの受け渡しに使うコマンド

#define ANIMATION_COMMAND 'A'/** アニメーションを送受信するコマンド */

#endif
