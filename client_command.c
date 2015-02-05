/*****************************************************************
ファイル名	: client_command.c
機能		: クライアントのコマンド処理
*****************************************************************/

#include"common.h"
#include"client_func.h"

static void SetCharData2DataBlock(void *data,char charData,int *dataSize);
static void SetIntData2DataBlock(void *data,int intData,int *dataSize);
static void RecvStartData(void);
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
関数名	: ExecuteCommand
機能	: サーバーから送られてきたコマンドを元に，
		  引き数を受信し，実行する
引数	: char	command		: コマンド
出力	: プログラム終了コマンドがおくられてきた時には0を返す．
		  それ以外は1を返す
*****************************************************************/
int ExecuteCommand(char command)
{
    int	endFlag = 1;
/*#ifndef NDEBUG
    printf("#####\n");
    printf("ExecuteCommand()\n");
    printf("command = %c\n",command);
#endif*/
    switch(command){
    case END_COMMAND:
        exit_p = 1;
        endFlag = 0;
        clearset = 1;
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

/* 追加*/
/*****************************************************************
関数名	: SendStartCommand
機能	: ゲーム開始をサーバーに送る
引数	: なし
出力	: なし
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
関数名	: SendMoveCommand
機能	: 移動したことをサーバーに送る
引数	: なし
出力	: なし
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
関数名	: SendObjectCommand
機能	: 移動したことをサーバーに送る
引数	: なし
出力	: なし
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
関数名	: SendEndCommand
機能	: プログラムの終了を知らせるために，
		  サーバーにデータを送る
引数	: なし
出力	: なし
*****************************************************************/
void SendEndCommand(void)
{
    unsigned char	data[MAX_DATA];
    int			dataSize;

/*#ifndef NDEBUG
    printf("#####\n");
    printf("SendEndCommand()\n");
#endif*/
    dataSize = 0;
    /* コマンドのセット */
    SetCharData2DataBlock(data,END_COMMAND,&dataSize);

    /* データの送信 */
    SendData(data,dataSize);
}

void SendGameoverCommand(void)
{
    unsigned char	data[MAX_DATA];
    int			dataSize;

    dataSize = 0;
    SetCharData2DataBlock(data, GAMEOVER_COMMAND, &dataSize);
    SetIntData2DataBlock(data, mynum, &dataSize);
    SendData(data, dataSize);
}

void SendNewposCommand(void)
{
    unsigned char	data[MAX_DATA];
    int			dataSize;

    dataSize = 0;
    SetCharData2DataBlock(data, NEWPOS_COMMAND, &dataSize);
    SendData(data, dataSize);
}

void SendGameclearCommand(void)
{
    unsigned char	data[MAX_DATA];
    int			dataSize;

    dataSize = 0;
    SetCharData2DataBlock(data, CLEAR_COMMAND, &dataSize);
    SendData(data, dataSize);
}

void SendWindowCommand(void)
{
    unsigned char	data[MAX_DATA];
    int			dataSize;

    dataSize = 0;
    SetCharData2DataBlock(data, WINDOW_COMMAND, &dataSize);
    SetIntData2DataBlock(data, gameRect.x, &dataSize);
    SendData(data, dataSize);
}

void SendEscapeCommand(int titlep)
{
    unsigned char	data[MAX_DATA];
    int			dataSize;

    dataSize = 0;
    SetCharData2DataBlock(data, ESCAPE_COMMAND, &dataSize);
    SetIntData2DataBlock(data, titlep, &dataSize);
    SendData(data, dataSize);
}

void SendTimeCommand(void)
{
    unsigned char	data[MAX_DATA];
    int			dataSize;

    dataSize = 0;
    SetCharData2DataBlock(data, TIME_COMMAND, &dataSize);
    SetIntData2DataBlock(data, time, &dataSize);
    SendData(data, dataSize);
}

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
関数名	: SetIntData2DataBlock
機能	: int 型のデータを送信用データの最後にセットする
引数	: void		*data		: 送信用データ
		  int		intData		: セットするデータ
		  int		*dataSize	: 送信用データの現在のサイズ
出力	: なし
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
関数名	: SetCharData2DataBlock
機能	: char 型のデータを送信用データの最後にセットする
引数	: void		*data		: 送信用データ
		  int		intData		: セットするデータ
		  int		*dataSize	: 送信用データの現在のサイズ
出力	: なし
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


/**************************追加関数******************************/
static void RecvStartData(void)
{

}

static void RecvMoveData(void)
{
    int i;
    for(i = 0; i < gClientNum; i++) {
        //if(player[i+1].pos.x == NULL)
        //    break;
        RecvIntData(&player[i].pos.x);
        RecvIntData(&player[i].pos.y);
        RecvIntData(&player[i].anim.x);
        RecvIntData(&player[i].anim.y);
        if(player[i].pos.x == NULL)
            break;
        player[i].pos.x -= gameRect.x;
        player[i].pos.y -= gameRect.y;
        //if(player[i].pos.x == NULL)
        //    break;
    }
}

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

static void RecvGameoverData(void)
{
    int num;

    RecvIntData(&num);

    Mix_PauseMusic();///音楽一時停止
    playSE(1);///ゲームオーバー音

    GameOver(num);

    Mix_ResumeMusic();///音楽再開
}

static void RecvNewposData(void)
{
    RecvIntData(&mynum);
    RecvIntData(&player[mynum].pos.x);
    RecvIntData(&player[mynum].pos.y);
    RecvIntData(&gClientNum);

}

static void RecvWindowCommand(void)
{
    RecvIntData(&gameRect.x);
}

static void RecvEscapeData(void)
{
    RecvIntData(&titlep);

    if(titlep == 0){
        EXIT();
    }
}

static void RecvTimeCommand(void)
{
    RecvIntData(&time);
}

static void RecvTitleCommand(void)
{
    RecvIntData(&P.y);
    RecvIntData(&titlep);

    if(titlep == 0){
        if(P.y == 500)
            stageP = 2;
            //titlep = 1;
            /*
        if(P.y == 600)
            EXIT();*/
    }

}

static void RecvJewelCommand(void)
{
    RecvIntData(&mynum);
    RecvIntData(&player[mynum].jewelP);
}


