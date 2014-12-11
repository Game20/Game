/*****************************************************************
ファイル名	: server_system.c
機能		: サーバーの計算処理等を行う
*****************************************************************/



#include"server_common.h"
#include"server_func.h"

void CalcPos(int pos){//各プレイヤーの座標を受け取る

    RecvIntData(pos, &gClients[pos].newplc.x);
    RecvIntData(pos, &gClients[pos].newplc.y);

//HitJudge(
    /*当たり判定*/


}

int HitJudge(int pos){
    int i;
    for(i=0; i<MAX_CLIENTS; i++){
        if(i != pos){
            if((gClients[pos].newplc.x > gClients[i].plc.x - 45 && gClients[pos].newplc.x < gClients[i].plc.x + 45) &&
               (gClients[pos].newplc.y > gClients[i].plc.y - 60 && gClients[pos].newplc.y < gClients[i].plc.y + 60)){
                return 1;

                /*if(gClients[pos].newplc.x < gClients[i].plc.x)
                    gClients[pos].newplc.x = gClients[pos].plc.x;
                else if(gClients[pos].newplc.x > gClients[i].plc.x)
                    gClients[pos].newplc.x = gClients[pos].plc.x;
                if(gClients[pos].newplc.y < gClients[i].plc.y)
                    gClients[pos].newplc.y = gClients[i].plc.y;
                else  if(gClients[pos].newplc.y > gClients[i].plc.y)
                    gClients[pos].newplc.y = gClients[pos].plc.y;*/
            }
        }
    }
    return 0;
}
