/*****************************************************************
ファイル名	: server_command.c
機能		: サーバーのコマンド処理
*****************************************************************/

#include"server_common.h"
#include"server_func.h"

//int x = 200;
//int y = 200;

int gClientNum;
int cnt = 0;///START_COMMANDの数
/************OBJECT_COMMAND用の変数宣言*************/
static int object_num;//オブジェクト配列の添字
static int object_status;//オブジェクトの状態
Plc object_place;//オブジェクトの座標(貼り付け先の座標)
Plc object_move;//移動判定を格納する(client_window内のmovex,movey)
//Plc object_cpy;//オブジェクトの貼り付け元の座標



/*****************************************************************
関数名	: ExecuteCommand
機能	: クライアントから送られてきたコマンドを元に，
		  引き数を受信し，実行する
引数	: char	command		: コマンド
		  int	pos			: コマンドを送ったクライアント番号
出力	: プログラム終了コマンドが送られてきた時には0を返す．
		  それ以外は1を返す
*****************************************************************/
int ExecuteCommand(char command,int pos)
{
    unsigned char	data[MAX_DATA];
    int			dataSize,intData;
    int			endFlag = 1;

    /* 引き数チェック */
    assert(0<=pos && pos<MAX_CLIENTS);

#ifndef NDEBUG
    printf("#####\n");
    printf("ExecuteCommand()\n");
    printf("Get command %c\n",command);
#endif
    switch(command){
    case START_COMMAND:
        dataSize = 0;

        int x,y;

        cnt = cnt + 1;
        x=180;/*1Pの初期座標*/
        y=630;/*1Pの初期座標*/
        SetCharData2DataBlock(data, NEWPOS_COMMAND, &dataSize);
        SetIntData2DataBlock(data, pos, &dataSize);
        x = x + (pos * 180); /*プレイヤーごとの初期座標計算*/
 //       y = y - (pos * 60);
        SetIntData2DataBlock(data, x, &dataSize);
        SetIntData2DataBlock(data, y, &dataSize);

        SendData(pos, data, dataSize);
        if(cnt == gClientNum){
            dataSize = 0;
            SetCharData2DataBlock(data, command, &dataSize);
            SendData(ALL_CLIENTS, data, dataSize);
        }
        break;
    case END_COMMAND:
        dataSize = 0;
        /* コマンドのセット */
        SetCharData2DataBlock(data,command,&dataSize);

        /* 全ユーザーに送る */
        SendData(ALL_CLIENTS,data,dataSize);
        endFlag = 0;
        break;

    case MOVE_COMMAND: //クライアントの移動後座標を送信

        CalcPos(pos);/*座標受け取り*/

        dataSize = 0;
        SetCharData2DataBlock(data, command, &dataSize);

        int i;
        for(i=0; i< MAX_CLIENTS ; i++){
            SetIntData2DataBlock(data, gClients[i].plc.x, &dataSize);
            SetIntData2DataBlock(data, gClients[i].plc.y, &dataSize);
            SetIntData2DataBlock(data, gClients[i].anim.x, &dataSize);
            SetIntData2DataBlock(data, gClients[i].anim.y, &dataSize);

        }
        SendData(pos, data, dataSize);
        break;

    case OBJECT_COMMAND:/**オブジェクトの番号・状態・座標を受け取り，全体に送る*/
        RecvIntData(pos, &object_num);//オブジェクトの添字を受け取る
        RecvIntData(pos, &object_status);//オブジェクトの状態を受け取る
//        RecvIntData(pos, &object_cpy.x);//オブジェクトの貼り付け元のx座標を受け取る
//        RecvIntData(pos, &object_cpy.y);//オブジェクトの貼り付け元のy座標を受け取る
        RecvIntData(pos, &object_place.x);//オブジェクトのx座標を受け取る
        RecvIntData(pos, &object_place.y);//オブジェクトのy座標を受け取る
        RecvIntData(pos, &object_move.x);//オブジェクトの移動判定movexを受け取る
        RecvIntData(pos, &object_move.y);//オブジェクトの移動判定moveyを受け取る

        dataSize = 0;
        SetCharData2DataBlock(data, command, &dataSize);//コマンドをセット
        SetIntData2DataBlock(data, object_num, &dataSize);//オブジェクトの添字をセット
        SetIntData2DataBlock(data, object_status, &dataSize);//オブジェクトの状態をセット
//        SetIntData2DataBlock(data, object_cpy.x, &dataSize);//オブジェクトの貼り付け元x座標をセット
//        SetIntData2DataBlock(data, object_cpy.y, &dataSize);//オブジェクトの貼り付け元y座標をセット
        SetIntData2DataBlock(data, object_place.x, &dataSize);//オブジェクトのx座標をセット
        SetIntData2DataBlock(data, object_place.y, &dataSize);//オブジェクトのy座標をセット
        SetIntData2DataBlock(data, object_move.x, &dataSize);//オブジェクトの移動判定movex座標をセット
        SetIntData2DataBlock(data, object_move.y, &dataSize);//オブジェクトの移動判定movey座標をセット

        SendData(ALL_CLIENTS, data, dataSize);
        break;


    default:
        /* 未知のコマンドが送られてきた */
        fprintf(stderr,"0x%02x is not command!\n",command);
    }
    return endFlag;
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
void SetIntData2DataBlock(void *data,int intData,int *dataSize)
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
void SetCharData2DataBlock(void *data,char charData,int *dataSize)
{
    /* 引き数チェック */
    assert(data!=NULL);
    assert(0<=(*dataSize));

    /* int 型のデータを送信用データの最後にコピーする */
    *(char *)(data + (*dataSize)) = charData;
    /* データサイズを増やす */
    (*dataSize) += sizeof(char);
}
