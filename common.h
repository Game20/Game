/*****************************************************************
ファイル名	: common.h
機能		: サーバーとクライアントで使用する定数の宣言を行う
作成者氏名:森祥悟,坪井正夢,船坂国之,高松翔馬
最終更新日:2015.1.30
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
#define START_COMMAND 'S' /* ゲーム開始コマンド */
#define CLEAR_COMMAND 'C' /*ゲームクリア表示用コマンド*/

#define MOVE_COMMAND 'M' /*移動コマンド*/

#define OBJECT_COMMAND 'O' //オブジェクトの受け渡しに使うコマンド

#define NEWPOS_COMMAND 'N'//ゲーム開始時の座標のやり取り
#define WINDOW_COMMAND 'W'
#define ESCAPE_COMMAND 'Q' // ポーズ画面

#define TIME_COMMAND 'T' // 時間を共有

#define GAMEOVER_COMMAND 'G'//ゲームオーバーを知らせる

#define TITLEPOSITION_COMMAND 'P' // タイトルに関するデータ

#define JEWEL_COMMAND 'J' // jewelポイントの送受信

#endif
