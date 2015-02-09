/*****************************************************************
ファイル名	: server_system.c
機能		: サーバーの計算処理等を行う
作成者氏名:船坂国之,高松翔馬
最終更新日:2014.12.14
*****************************************************************/


#include"server_common.h"
#include"server_func.h"

/*****************************************************************
関数名	: CalcPos
機能	: キャラのマップ上の座標とアニメーションの座標を受け取る
引数　　: pos         コマンドを送ってきたクライアント
*****************************************************************/
void CalcPos(int pos){//各プレイヤーの座標を受け取る

    RecvIntData(pos, &gClients[pos].plc.x);
    RecvIntData(pos, &gClients[pos].plc.y);
    RecvIntData(pos, &gClients[pos].anim.x);
    RecvIntData(pos, &gClients[pos].anim.y);

}
