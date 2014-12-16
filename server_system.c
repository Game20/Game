/*****************************************************************
ファイル名	: server_system.c
機能		: サーバーの計算処理等を行う
*****************************************************************/



#include"server_common.h"
#include"server_func.h"

void CalcPos(int pos){//各プレイヤーの座標を受け取る

    RecvIntData(pos, &gClients[pos].plc.x);
    RecvIntData(pos, &gClients[pos].plc.y);
    RecvIntData(pos, &gClients[pos].anim.x);
    RecvIntData(pos, &gClients[pos].anim.y);

}
