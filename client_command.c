/*****************************************************************
ファイル名:client_command.c
　　　機能:クライアントのコマンド処理
作成者氏名:森祥悟,坪井正夢,高松翔馬
最終更新日:2015.2.6
*****************************************************************/

#include"common.h"
#include"client_func.h"

static void SetCharData2DataBlock(void *data,char charData,int *dataSize);
static void SetIntData2DataBlock(void *data,int intData,int *dataSize);
//static void RecvStartData(void);
static void RecvMoveData(void);
static void RecvObjectData(void);
static void RecvGameoverData(void);
static void RecvNewposData(void);
static void RecvWindowCommand(void);
static void RecvEscapeData(void);
static void RecvTimeCommand(void);
static void RecvTitleCommand(void);
static void RecvJewelCommand(void);

extern int mynum;
extern int gClientNum;
extern int start_flag = 0;

int gClientNum;

int EscapeCnt = 0;

/*****************************************************************
関数の名前:int ExecuteCommand(char command)
関数の機能:サーバーから送られてきたコマンドを元に,処理をする
*****************************************************************/
int ExecuteCommand(char command)
{
    int	endFlag = 1;

    switch(command){
    case END_COMMAND:
        exit_p = 1;
        endFlag = 0;
        clearset = 1;
        stageP = 3;
        break;
    case START_COMMAND:
        start_flag = 1;
        break;
    case NEWPOS_COMMAND:
        RecvNewposData();
        break;
    case MOVE_COMMAND: //移動コマンド
        RecvMoveData();
        break;
    case OBJECT_COMMAND:
        RecvObjectData();
        break;
    case GAMEOVER_COMMAND:
        RecvGameoverData();
        break;
    case CLEAR_COMMAND:
        GameClear();
        break;
    case WINDOW_COMMAND:
        RecvWindowCommand();
        break;
    case ESCAPE_COMMAND:
        RecvEscapeData();
        break;
    case TIME_COMMAND:
        RecvTimeCommand();
        start_flag = 1;
        break;
    case TITLEPOSITION_COMMAND:
        RecvTitleCommand();
        break;
    case JEWEL_COMMAND:
        RecvJewelCommand();
        break;

    }
    return endFlag;
}

/*****************************************************************
関数の名前:void SendStartCommand(void)
関数の機能: ゲーム開始をサーバに送る
*****************************************************************/
void SendStartCommand(void)
{
    unsigned char	data[MAX_DATA];
    int			dataSize;

    dataSize = 0;
    /* コマンドのセット */
    SetCharData2DataBlock(data,START_COMMAND,&dataSize);

    /* データの送信 */
    SendData(data,dataSize);
}

/*****************************************************************
関数の名前:void SendMoveCommand(int x, int y, int animx, int animy)
関数の機能:移動したことをサーバに送る
　　　引数:操作キャラのx,y座標とアニメーションのx,y座標
*****************************************************************/
void SendMoveCommand(int x, int y, int animx, int animy)
{
    unsigned char	data[MAX_DATA];
    int			dataSize;

    dataSize = 0;
    /* コマンドのセット */
    SetCharData2DataBlock(data,MOVE_COMMAND,&dataSize);

    /* データセット */
    SetIntData2DataBlock(data, x, &dataSize);
    SetIntData2DataBlock(data, y, &dataSize);
    SetIntData2DataBlock(data, animx, &dataSize);
    SetIntData2DataBlock(data, animy, &dataSize);

    /* データの送信 */
    SendData(data,dataSize);
}

/*****************************************************************
関数の名前:void SendObjectCommand(int num, int status, int x, int y, int movex, int movey)
関数の機能:オブジェクトのデータをサーバに送る
　　　引数:オブジェクトの番号,状態,座標,移動距離
*****************************************************************/
void SendObjectCommand(int num, int status, int x, int y, int movex, int movey)
{
    unsigned char	data[MAX_DATA];
    int			dataSize;

    dataSize = 0;
    /* コマンドのセット */
    SetCharData2DataBlock(data,OBJECT_COMMAND,&dataSize);

    /* データセット */
    SetIntData2DataBlock(data, num, &dataSize);    // オブジェクトナンバー
    SetIntData2DataBlock(data, status, &dataSize); // オブジェクトのステータス
    SetIntData2DataBlock(data, x, &dataSize);      // オブジェクトの座標
    SetIntData2DataBlock(data, y, &dataSize);
    SetIntData2DataBlock(data, movex, &dataSize);      // オブジェクトの座標
    SetIntData2DataBlock(data, movey, &dataSize);

    /* データの送信 */
    SendData(data,dataSize);
}

/*****************************************************************
関数の名前:void SendEndCommand(void)
関数の機能:プログラムの終了をサーバに送信
*****************************************************************/
void SendEndCommand(void)
{
    unsigned char	data[MAX_DATA];
    int			dataSize;

    dataSize = 0;
    /* コマンドのセット */
    SetCharData2DataBlock(data,END_COMMAND,&dataSize);

    /* データの送信 */
    SendData(data,dataSize);
}

/*****************************************************************
関数の名前:void SendGameoverCommand(void)
関数の機能:ゲームオーバーしたことをサーバに送信
*****************************************************************/
void SendGameoverCommand(void)
{
    unsigned char	data[MAX_DATA];
    int			dataSize;

    dataSize = 0;
    SetCharData2DataBlock(data, GAMEOVER_COMMAND, &dataSize);
    SetIntData2DataBlock(data, mynum, &dataSize);
    SendData(data, dataSize);
}

/*****************************************************************
関数の名前:void SendNewposCommand(void)
関数の機能:初期座標を受信するためのコマンドをサーバに送信
*****************************************************************/
void SendNewposCommand(void)
{
    unsigned char	data[MAX_DATA];
    int			dataSize;

    dataSize = 0;
    SetCharData2DataBlock(data, NEWPOS_COMMAND, &dataSize);
    SendData(data, dataSize);
}

/*****************************************************************
関数の名前:void SendGameclearCommand(void)
関数の機能:ゲームクリアしたことをサーバに送信
*****************************************************************/
void SendGameclearCommand(void)
{
    unsigned char	data[MAX_DATA];
    int			dataSize;

    dataSize = 0;
    SetCharData2DataBlock(data, CLEAR_COMMAND, &dataSize);
    SendData(data, dataSize);
}

/*****************************************************************
関数の名前:void SendWindowCommand(void)
関数の機能:ゲーム画面の座標をサーバに送信
*****************************************************************/
void SendWindowCommand(void)
{
    unsigned char	data[MAX_DATA];
    int			dataSize;

    dataSize = 0;
    SetCharData2DataBlock(data, WINDOW_COMMAND, &dataSize);
    SetIntData2DataBlock(data, gameRect.x, &dataSize);
    SendData(data, dataSize);
}

/*****************************************************************
関数の名前:void SendEscapeCommand(int titlep)
関数の機能:ポーズ画面の情報をサーバに送信
*****************************************************************/
void SendEscapeCommand(int titlep)
{
    unsigned char	data[MAX_DATA];
    int			dataSize;

    dataSize = 0;
    SetCharData2DataBlock(data, ESCAPE_COMMAND, &dataSize);
    SetIntData2DataBlock(data, titlep, &dataSize);
    SendData(data, dataSize);
}

/*****************************************************************
関数の名前:void SendTimeCommand(void)
関数の機能:時間の値をサーバに送信
*****************************************************************/
void SendTimeCommand(void)
{
    unsigned char	data[MAX_DATA];
    int			dataSize;

    dataSize = 0;
    SetCharData2DataBlock(data, TIME_COMMAND, &dataSize);
    SetIntData2DataBlock(data, time, &dataSize);
    SendData(data, dataSize);
}

/*****************************************************************
関数の名前:void SendTitleCommand(int y, int titlep)
関数の機能:タイトル画面でのカーソルの位置をサーバに送信
*****************************************************************/
void SendTitleCommand(int y, int titlep)
{
    unsigned char	data[MAX_DATA];
    int			dataSize;

    dataSize = 0;
    SetCharData2DataBlock(data, TITLEPOSITION_COMMAND, &dataSize);
    SetIntData2DataBlock(data, y, &dataSize);
    SetIntData2DataBlock(data, titlep, &dataSize);
    SendData(data, dataSize);
}

/*****************************************************************
関数の名前:void void SendJewelCommand(jewel)
関数の機能:トレジャーポイント(ゲーム内の宝石獲得点)をサーバに送信
*****************************************************************/
void SendJewelCommand(jewel)
{
    unsigned char	data[MAX_DATA];
    int			dataSize;

    dataSize = 0;
    SetCharData2DataBlock(data, JEWEL_COMMAND, &dataSize);
    SetIntData2DataBlock(data, jewel, &dataSize);
    SendData(data, dataSize);
}

/*****
      static
*****/
/*****************************************************************
関数の名前:void SetIntData2DataBlock(void *data,int intData,int *dataSize)
関数の機能:int型のデータを送信データの最後にセットする
　　　引数:送信データ,セットするデータ,送信データサイズ
*****************************************************************/
static void SetIntData2DataBlock(void *data,int intData,int *dataSize)
{
    int tmp;

    /* 引き数チェック */
    assert(data!=NULL);
    assert(0<=(*dataSize));

    tmp = htonl(intData);

    /* int 型のデータを送信用データの最後にコピーする */
    memcpy(data + (*dataSize),&tmp,sizeof(int));
    /* データサイズを増やす */
    (*dataSize) += sizeof(int);
}


/*****************************************************************
関数の名前:void SetCharData2DataBlock(void *data,char charData,int *dataSize)
関数の機能:char型のデータを送信用データの最後にセットする
　　　引数:送信データ,セットするデータ,送信データの現在のサイズ
*****************************************************************/
static void SetCharData2DataBlock(void *data,char charData,int *dataSize)
{
    /* 引き数チェック */
    assert(data!=NULL);
    assert(0<=(*dataSize));

    /* char 型のデータを送信用データの最後にコピーする */
    *(char *)(data + (*dataSize)) = charData;
    /* データサイズを増やす */
    (*dataSize) += sizeof(char);
}

/*****************************************************************
関数の名前:void RecvMoveData(void)
関数の機能:各クライアントの位置座標をサーバから受信
*****************************************************************/
static void RecvMoveData(void)
{
    int i;
    for(i = 0; i < gClientNum; i++) {
        RecvIntData(&player[i].pos.x);
        RecvIntData(&player[i].pos.y);
        RecvIntData(&player[i].anim.x);
        RecvIntData(&player[i].anim.y);
        if(player[i].pos.x == NULL)
            break;
        player[i].pos.x -= gameRect.x;
        player[i].pos.y -= gameRect.y;
    }
}

/*****************************************************************
関数の名前:void RecvObjectData(void)
関数の機能:オブジェクトの位置座標,状態,移動距離等をサーバから受信
　　　　　:ゲーム中の宝石の有無をサーバから受信
*****************************************************************/
static void RecvObjectData(void)
{
    int i;

    RecvIntData(&i);

    if(i < 100){ //オブジェクトなら
        SDL_BlitSurface(mapwindow, &object[i].dst, objectwindow, &object[i].dst); // object貼り付け準備
        RecvIntData(&object[i].status);
        RecvIntData(&object[i].dst.x);
        RecvIntData(&object[i].dst.y);
        RecvIntData(&object[i].movex);
        RecvIntData(&object[i].movey);
        SDL_BlitSurface(objectimage, &object[i].src, objectwindow, &object[i].dst); // object貼り付け

        if(object[i].gimmick == 2) {

            switchblock[object[i].flaghold].flaghold = object[i].status;
            if(object[i].status != 0)
                playSE(2);

            if(object[i].status)
                switchblock[object[i].flaghold].flaghold = 1;
            //特殊動作
            if(stageP == 1){
                if(i == 11 && object[i+1].status == 1)
                    switchblock[object[i].flaghold].flaghold = 1;
                if(i >= 12 && i <= 14){
                    if(object[i].status == 1)
                        switchblock[object[i-1].flaghold].flaghold = 1;
                    if(object[i].status == 0 && object[i-1].status == 0)
                        switchblock[object[i-1].flaghold].flaghold = 0;
                    if(object[i].status == 0 && object[i+1].status == 1)
                        switchblock[object[i].flaghold].flaghold = 1;
                }
            }

            if(stageP == 2){
                if(i == 30){
                    switchblock[30].flaghold = object[i].status;
                }
                if(i == 33 && object[i].status != 2){
                    switchblock[10].flaghold = object[i].status;
                    switchblock[11].flaghold = object[i].status;
                    switchblock[31].flaghold = object[i].status;
                }
                if(i == 34 && object[i].status == 1 && object[i-1].status == 1 && object[35].flaghold != -1){
                    switchblock[11].flaghold = 0;
                    switchblock[9].flaghold = 0;

                }
                if(i == 34)
                    switchblock[31].flaghold = 1;
//
                if(i == 37){
                    switchblock[20].flaghold = object[i].status;
                    switchblock[27].flaghold = object[i].status;
                }
                if(i == 38){
                    switchblock[23].flaghold = object[i].status;
                    switchblock[19].flaghold = object[i].status;
                }
                if(i == 39){
                    switchblock[24].flaghold = object[i].status;
                    switchblock[21].flaghold = object[i].status;
                }

                if(i == 40){
                    switchblock[25].flaghold = 1;
                }
                if(i == 41){
                    switchblock[22].flaghold = 1;
                    switchblock[26].flaghold = 1;
                }
            }



        }
    }

    if(i >= 100){ // 宝石なら
        i -= 100;
        RecvIntData(&jewel[i].status);
        RecvIntData(&jewel[i].dst.x);
        RecvIntData(&jewel[i].dst.y);
        RecvIntData(&jewel[i].movex);
        RecvIntData(&jewel[i].movey);
    }

}

/***************************************************************************
関数の名前:void RecvGameoverData(void)
関数の機能:ゲームオーバーをサーバから受信
****************************************************************************/
static void RecvGameoverData(void)
{
    int num;

    RecvIntData(&num);

    Mix_PauseMusic();///音楽一時停止
    playSE(1);///ゲームオーバー音

    GameOver(num);

    Mix_ResumeMusic();///音楽再開
}

/***************************************************************************
関数の名前:void RecvNewposData(void)
関数の機能:初期座標をサーバから受信
****************************************************************************/
static void RecvNewposData(void)
{
    RecvIntData(&mynum);
    RecvIntData(&player[mynum].pos.x);
    RecvIntData(&player[mynum].pos.y);
    RecvIntData(&gClientNum);

}

/***************************************************************************
関数の名前:void RecvWindowCommand(void)
関数の機能:ゲーム画面の座標をサーバから受信
****************************************************************************/
static void RecvWindowCommand(void)
{
    RecvIntData(&gameRect.x);
}

/***************************************************************************
関数の名前:void RecvEscapeData(void)
関数の機能:ポーズ画面の情報をサーバから受信
****************************************************************************/
static void RecvEscapeData(void)
{
    RecvIntData(&titlep);

    if(titlep == 0){
        EXIT();
    }
}

/***************************************************************************
関数の名前:void RecvEscapeData(void)
関数の機能:時間の値をサーバから受信
****************************************************************************/
static void RecvTimeCommand(void)
{
    RecvIntData(&time);
}

/***************************************************************************
関数の名前:void RecvTitleCommand(void)
関数の機能:タイトル画面での情報(カーソルの位置等)をサーバから受信
****************************************************************************/
static void RecvTitleCommand(void)
{
    RecvIntData(&P.y);
    RecvIntData(&titlep);

    if(titlep == 0){
        if(P.y == 500)
            stageP = 2;

    }

}

/***************************************************************************
関数の名前:void RecvJewelCommand(void)
関数の機能:各クライアントのトレジャーポイントをサーバから受信
****************************************************************************/
static void RecvJewelCommand(void)
{
    RecvIntData(&mynum);
    RecvIntData(&player[mynum].jewelP);
}


