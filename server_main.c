/*****************************************************************
ファイル名	: server_main.c
機能		: サーバーのメインルーチン
*****************************************************************/

#include<SDL/SDL.h>
#include"server_common.h"

//static int Judge2(void *data);	/* 追加 */
//static int Judge3(void *data);	/* 追加 */
static void SetCharData2DataBlock(void *data,char charData,int *dataSize);

int main(int argc,char *argv[])
{
    int num;
    int	endFlag = 1;

    SDL_Thread *thr1;

    /* 引き数チェック */
    if(argc != 2){
        fprintf(stderr,"Usage: number of clients\n");
        exit(-1);
    }
    if((num = atoi(argv[1])) < 0 ||  num > MAX_CLIENTS){
        fprintf(stderr,"clients limit = %d \n",MAX_CLIENTS);
        exit(-1);
    }
	
    if(num == 2){
//		thr1 = SDL_CreateThread(Judge2, NULL);
    }
    else if(num == 3){
//		thr1 = SDL_CreateThread(Judge3, NULL);
    }
	
    /* SDLの初期化 */
    if(SDL_Init(SDL_INIT_TIMER) < 0) {
        printf("failed to initialize SDL.\n");
        exit(-1);
    }

    /* クライアントとの接続 */
    if(SetUpServer(num) == -1){
        fprintf(stderr,"Cannot setup server\n");
        exit(-1);
    }
	
    /* メインイベントループ */
    while(endFlag){
        endFlag = SendRecvManager();
    };

    /* 終了処理 */
    Ending();

    return 0;
}

/*****************************************************************
関数名	: Judge2
機能	: 結果を判定する
引数	: なし
出力	: スレッド終了時に0を返す
*****************************************************************/
/*static int Judge2(void *date)
  {
  int i;
  unsigned char data[MAX_DATA];
  int datasize;
	
  for(i=0; i<2; i++){
  gClients[i].command = ' ';
  }
	
  while(1){
  if( gClients[0].command != ' ' && gClients[1].command != ' '){
  /* あいこ */
/*		if(gClients[0].command == gClients[1].command){
                datasize = 0;
                SetCharData2DataBlock(data, DRAW_COMMAND, &datasize);
                SendData(ALL_CLIENTS, data, datasize);
                for(i=0; i<2; i++){
                gClients[i].command = ' ';
                }
		}
		/*gClients[0]の勝ち */
/*		else if(gClients[0].command == GU_COMMAND && gClients[1].command == CO_COMMAND || gClients[0].command == CO_COMMAND && gClients[1].command == PA_COMMAND || gClients[0].command == PA_COMMAND && gClients[1].command == GU_COMMAND){
                datasize = 0;
                SetCharData2DataBlock(data, WIN_COMMAND, &datasize);
                SendData(0, data, datasize);
                datasize = 0;
                SetCharData2DataBlock(data, LOSE_COMMAND, &datasize);
                SendData(1, data, datasize);
                for(i=0; i<2; i++){
                gClients[i].command = ' ';
                }
		}
		/* gClients[1]の勝ち */
/*		else if(gClients[0].command == GU_COMMAND && gClients[1].command == PA_COMMAND || gClients[0].command == CO_COMMAND && gClients[1].command == GU_COMMAND || gClients[0].command == PA_COMMAND && gClients[1].command == CO_COMMAND){
                datasize = 0;
                SetCharData2DataBlock(data, WIN_COMMAND, &datasize);
                SendData(1, data, datasize);
                datasize = 0;
                SetCharData2DataBlock(data, LOSE_COMMAND, &datasize);
                SendData(0, data, datasize);
                for(i=0; i<2; i++){
                gClients[i].command = ' ';
                }
		}
		}
                }
                return 0;
                }

/*****************************************************************
関数名	: Judge3
機能	: 結果を判定する
引数	: なし
出力	: スレッド終了時に0を返す
*****************************************************************/
/*static int Judge3(void *date)
  {
  int i;
  unsigned char data[MAX_DATA];
  int datasize;
	
  for(i=0; i<3; i++){
  gClients[i].command = ' ';
  }
	
  while(1){
  if(gClients[0].command != ' ' && gClients[1].command != ' ' && gClients[2].command != ' '){
  /* あいこ */
/*		if(gClients[0].command == gClients[1].command && gClients[0].command == gClients[2].command || gClients[0].command != gClients[1].command && gClients[0].command != gClients[2].command && gClients[1].command != gClients[2].command){
                datasize = 0;
                SetCharData2DataBlock(data, DRAW_COMMAND, &datasize);
                SendData(ALL_CLIENTS, data, datasize);
                for(i=0; i<3; i++){
                gClients[i].command = ' ';
                }
		}
		/*gClients[0]の勝ち */
/*		else if(gClients[0].command == GU_COMMAND && gClients[1].command == CO_COMMAND && gClients[2].command == CO_COMMAND || gClients[0].command == CO_COMMAND && gClients[1].command == PA_COMMAND && gClients[2].command == PA_COMMAND || gClients[0].command == PA_COMMAND && gClients[1].command == GU_COMMAND && gClients[2].command == GU_COMMAND){
                datasize = 0;
                SetCharData2DataBlock(data, WIN_COMMAND, &datasize);
                SendData(0, data, datasize);
                datasize = 0;
                SetCharData2DataBlock(data, LOSE_COMMAND, &datasize);
                SendData(1, data, datasize);
                datasize = 0;
                SetCharData2DataBlock(data, LOSE_COMMAND, &datasize);
                SendData(2, data, datasize);
                for(i=0; i<3; i++){
                gClients[i].command = ' ';
                }
		}
		/* gClients[1]の勝ち */
/*		else if(gClients[1].command == GU_COMMAND && gClients[0].command == CO_COMMAND && gClients[2].command == CO_COMMAND || gClients[1].command == CO_COMMAND && gClients[0].command == PA_COMMAND && gClients[2].command == PA_COMMAND || gClients[1].command == PA_COMMAND && gClients[0].command == GU_COMMAND && gClients[2].command == GU_COMMAND){
                datasize = 0;
                SetCharData2DataBlock(data, WIN_COMMAND, &datasize);
                SendData(1, data, datasize);
                datasize = 0;
                SetCharData2DataBlock(data, LOSE_COMMAND, &datasize);
                SendData(0, data, datasize);
                datasize = 0;
                SetCharData2DataBlock(data, LOSE_COMMAND, &datasize);
                SendData(2, data, datasize);
                for(i=0; i<3; i++){
                gClients[i].command = ' ';
                }
		}
		/* gClients[2]の勝ち */
/*		else if(gClients[2].command == GU_COMMAND && gClients[0].command == CO_COMMAND && gClients[	1].command == CO_COMMAND || gClients[2].command == CO_COMMAND && gClients[0].command == PA_COMMAND && gClients[1].command == PA_COMMAND || gClients[2].command == PA_COMMAND && gClients[0].command == GU_COMMAND && gClients[1].command == GU_COMMAND){
                datasize = 0;
                SetCharData2DataBlock(data, WIN_COMMAND, &datasize);
                SendData(2, data, datasize);
                datasize = 0;
                SetCharData2DataBlock(data, LOSE_COMMAND, &datasize);
                SendData(0, data, datasize);
                datasize = 0;
                SetCharData2DataBlock(data, LOSE_COMMAND, &datasize);
                SendData(1, data, datasize);
                for(i=0; i<3; i++){
                gClients[i].command = ' ';
                }
		}
		/* gClients[0]とgClients[1]の勝ち */
/*		else if((gClients[2].command == PA_COMMAND && gClients[0].command == CO_COMMAND && gClients[1].command == CO_COMMAND) || (gClients[2].command == GU_COMMAND && gClients[0].command == PA_COMMAND && gClients[1].command == PA_COMMAND) || (gClients[2].command == CO_COMMAND && gClients[0].command == GU_COMMAND && gClients[1].command == GU_COMMAND)){
                datasize = 0;
                SetCharData2DataBlock(data, WIN_COMMAND, &datasize);
                SendData(1, data, datasize);
                datasize = 0;
                SetCharData2DataBlock(data, WIN_COMMAND, &datasize);
                SendData(0, data, datasize);
                datasize = 0;
                SetCharData2DataBlock(data, LOSE_COMMAND, &datasize);
                SendData(2, data, datasize);
                for(i=0; i<3; i++){
                gClients[i].command = ' ';
                }
		}
		/* gClients[0]とgClients[2]の勝ち */
/*		else if((gClients[1].command == PA_COMMAND && gClients[0].command == CO_COMMAND && gClients[2].command == CO_COMMAND) || (gClients[1].command == GU_COMMAND && gClients[0].command == PA_COMMAND && gClients[2].command == PA_COMMAND) || (gClients[1].command == CO_COMMAND && gClients[0].command == GU_COMMAND && gClients[2].command == GU_COMMAND)){
                datasize = 0;
                SetCharData2DataBlock(data, WIN_COMMAND, &datasize);
                SendData(2, data, datasize);
                datasize = 0;
                SetCharData2DataBlock(data, WIN_COMMAND, &datasize);
                SendData(0, data, datasize);
                datasize = 0;
                SetCharData2DataBlock(data, LOSE_COMMAND, &datasize);
                SendData(1, data, datasize);
                for(i=0; i<3; i++){
                gClients[i].command = ' ';
                }
		}
		/* gClients[1]とgClients[2]の勝ち */
/*		else if((gClients[0].command == PA_COMMAND && gClients[1].command == CO_COMMAND && gClients[2].command == CO_COMMAND) || (gClients[0].command == GU_COMMAND && gClients[1].command == PA_COMMAND && gClients[2].command == PA_COMMAND) || (gClients[0].command == CO_COMMAND && gClients[1].command == GU_COMMAND && gClients[2].command == GU_COMMAND)){
                datasize = 0;
                SetCharData2DataBlock(data, WIN_COMMAND, &datasize);
                SendData(2, data, datasize);
                datasize = 0;
                SetCharData2DataBlock(data, WIN_COMMAND, &datasize);
                SendData(1, data, datasize);
                datasize = 0;
                SetCharData2DataBlock(data, LOSE_COMMAND, &datasize);
                SendData(0, data, datasize);
                for(i=0; i<3; i++){
                gClients[i].command = ' ';
                }
		}
		}
                }
                return 0;
                }
*/
static void SetCharData2DataBlock(void *data,char charData,int *dataSize)
{
    /* 引き数チェック */
    assert(data!=NULL);
    assert(0<=(*dataSize));

    /* int 型のデータを送信用データの最後にコピーする */
    *(char *)(data + (*dataSize)) = charData;
    /* データサイズを増やす */
    (*dataSize) += sizeof(char);
}
