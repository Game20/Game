/*****************************************************************
ファイル名	: server_system.c
機能		: サーバーの計算処理等を行う
*****************************************************************/



#include"server_common.h"
#include"server_func.h"

void CalcPos(int pos){

    RecvIntData(pos, &gClients[pos].plc.x);
    RecvIntData(pos, &gClients[pos].plc.y);

//HitJudge(
        /*当たり判定*/


}


