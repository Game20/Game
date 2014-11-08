/*****************************************************************
ファイル名	: server_system.c
機能		: サーバーの計算処理等を行う
*****************************************************************/



#include"server_common.h"
#include"server_func.h"

void CalcPos(int pos, int *intData){

    gClients[pos].plc.x = RecvIntData(pos, &intData);
    gClients[pos].plc.y = RecvIntData(pos, &intData);


//HitJudge(
        /*当たり判定*/


}

