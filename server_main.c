/*****************************************************************
ファイル名	: server_main.c
機能		: サーバーのメインルーチン
作成者氏名:森祥悟,船坂国之
最終更新日:2015.1.30
*****************************************************************/

#include<SDL/SDL.h>
#include"server_common.h"


static void SetCharData2DataBlock(void *data,char charData,int *dataSize);
//int argc,char *argv[]
int main(void)
{
    int player_amount;
    int num;
    int	endFlag = 1;


while(1){
    printf("プレイ人数(2〜4)を入力してください:");
    scanf("%d", &player_amount);

    if(player_amount >= 1 && player_amount <= MAX_CLIENTS){
        printf("プレイヤー人数は%d人です\n", player_amount);
        break;
    }
    else{
        printf("整数の2,3,4を入力してください\n");
        exit(-1);
    }
}



    /* 引き数チェック */
  //  if(argc != 2){
  //      fprintf(stderr,"Usage: number of clients\n");
   //     exit(-1);
   // }
    num = player_amount;

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
関数名	: SetCharData2DataBlock
機能	: 送信用データをセットする
*****************************************************************/
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
