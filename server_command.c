/*****************************************************************
ファイル名	: server_command.c
機能		: サーバーのコマンド処理
*****************************************************************/

#include"server_common.h"
#include"server_func.h"


//int x = 200;
//int y = 200;
static int cnt;//スタートフラグのカウント
int gClientNum;


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
        ++cnt;
        printf("aaaaa%d\n\n", gClientNum);
        if(cnt == gClientNum)
        {
            dataSize = 0;
            SetCharData2DataBlock(data, command, &dataSize);
            SendData(ALL_CLIENTS,data,dataSize);
            cnt = 0;
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

        CalcPos(pos);
        int k;
	//for(k=0;k<2;k++)
//		printf("(x,y) = (%d,%d)",gClients[0].plc.x,gClients[0].plc.y);
//		printf("(x,y) = (%d,%d)",gClients[1].plc.x,gClients[1].plc.y);

    dataSize = 0;
    SetCharData2DataBlock(data, command, &dataSize);

    int i;
    for(i=0; i< MAX_CLIENTS ; i++){
    SetIntData2DataBlock(data, gClients[i].plc.x, &dataSize);
    SetIntData2DataBlock(data, gClients[i].plc.y, &dataSize);
    SendData(pos, data, dataSize);
    }

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
