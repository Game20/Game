#include <stdio.h>
#include <SDL/SDL.h>       // SDLを用いるために必要なヘッダファイルをインクルード
#include <SDL/SDL_image.h>       // SDL_imageを用いるために必要なヘッダファイルをインクルード
#include <SDL/SDL_gfxPrimitives.h>	// gfxによる図形処理をするために必要なヘッダファイルをインクルード
#include <SDL/SDL_ttf.h>	// TrueTypeフォントを表示するために必要なヘッダファイルをインクルード
#include <SDL/SDL_opengl.h> // SDLでOpenGLを扱うために必要なヘッダファイルをインクルード
#include "client_func.h"

SDL_Joystick *Joystick;

/**************************変数などの宣言(main外)**************************************/
SDL_Rect gameRect = { 0,0, WIND_Width*bit, WIND_Height*bit }; // ゲームウィンドウの座標
SDL_Rect whitedisplay = { 200,0, 60, 75 };
int time = 0;	//時間
int LR = 0;		//左右入力
int UD = 0;		//上下入力
int jumpflag = 0;//ジャンプフラグ　空中にいるか否か
int jump_a = 0;	//ジャンプにおける加速度
int jump_LR = 0;//ジャンプ中の移動フラグ保持
int newposx = 0, newposy = 0;//新規位置
int hitx = 0, hity = 0;//当たり判定(xのと yのと)

int timekey = 0;
int hithold = 0;
int shiftdef = 0;

int fm = 0;		//
int titlep = 1;//
int exit_p = 0;	//

int i;
int start_flag;

SDL_Rect PA = { 0, 0, 60, 75 }; //プレイヤーキャラ座標読み取り　あとで構造体化
SDL_Rect P = { 200, 705 };
SDL_Rect PMR = { 0, 0, 60, 100 };
SDL_Rect PM = { 0, 0 };//Player_Message(_Read) 1Pとかの表示 あとで構造体化

extern SDL_Rect P_START;

SDL_Event event; // SDLによるイベントを検知するための構造体
SDL_Event quit_event = {SDL_QUIT};	// 特定のイベント名を格納

SDL_Surface *usa;  // 画像データへのポインタ

Player player[MAX_CLIENTS]; // プレイヤーの状態を格納
int mynum;
int gClientNum;

SDL_Surface *window, *mapwindow; // ウィンドウデータへのポインタ

/* フォント関連 */
#define MSG_NUM 6           /* メッセージの数 */
SDL_Color black = {0x00, 0x00, 0x00};
SDL_Color Yellow = {0xff, 0xff, 0x00};
TTF_Font* gTTF;	// TrueTypeフォントデータへのポインタ
extern TTF_Font* sTTF;
static char gFontFile[] = "images/APJapanesefontT.ttf";
static char gMsgStrings[ 100 ][ 100 ] = { "さいしょから", "ステージ2から", "おわる", "つづける", "おわる", "1P"};


/***************************************************************************
関数の名前:int main(int argc, char* argv[])
作成者氏名:森祥悟,坪井正夢,船坂国之,高松翔馬
最終更新日:2015.1.24
関数の説明:クライアントのmain文
***************************************************************************/
int main(int argc, char* argv[]) {
    // SDL初期化
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_AUDIO) < 0) {
        printf("failed to initialize SDL.\n");
        exit(-1);
    }

    loadSounds();//サウンドデータをロード

    SDL_JoystickEventState(SDL_ENABLE);
    Joystick = SDL_JoystickOpen(0);

    int i, j; //forループで使用
    int		endFlag=1;
    int		num;
    char	name[MAX_CLIENTS][MAX_NAME_SIZE];
    char	localHostName[]="localhost";
    char	*serverName;
    int		clientID;

    /* 引き数チェック */
    if(argc == 1){
    	serverName = localHostName;
    }
    else if(argc == 2){
    	serverName = argv[1];
    }
    else{
        fprintf(stderr, "Usage: %s, Cannot find a Server Name.\n", argv[0]);
        return -1;
    }

    /* サーバーとの接続 */
    if(SetUpClient(serverName,&clientID,&num,name)==-1){
        fprintf(stderr,"setup failed : SetUpClient\n");
        return -1;
    }

    /*初期設定*/
    InitWindow();

    for(i = 0; i < MAX_CLIENTS; i++){
        player[i].pos.x = 0;
    }

    SendNewposCommand(); // プレイヤーの初期座標を受信するためのコマンドを送信

    while(start_flag == 0){
        endFlag = SendRecvManager();
    }

    start_flag = 0;

    title();

    for(i = 0; i < gClientNum; i++){
        if(player[i].pos.x != 0) {
            P_START.x = player[i].pos.x;
            P_START.y = player[i].pos.y;
            break;
        }
    }

    if(exit_p == 1)
        endFlag = 0;

    InitStatus(); // キャラのステータスの初期化
    titlep = 1;

    // 無限ループ
    while(endFlag || !exit_p){

	SDL_Delay(20);
	time++;

	// イベントを取得したら
	if(SDL_PollEvent(&event)){
            eventdisp();	// イベント処理
        }

	keycont();	/*キーボード操作*/
	newpositionjadge(); /*当たり判定　後でサーバに移行*/
	scroll(); //画面スクロール
	exepaste(); /*貼り付け設定 & 実行*/

        for(i = 0; i <= 100; i++) { // データの受信
            endFlag = SendRecvManager();
        }

        DrawChara(); // キャラの描画
        SDL_Flip(window); // 画面に図形を表示（反映）

	DisplayStatus();

	//ゲームクリア後の初期化
	if(clearset == 1){
            title();
            MapLayout();
            InitStatus();
            clearset = 0;
	}

	if(exit_p == 1)//終了フラグが立てばwhilebreak
            break;

    } //whileループ

    SDL_JoystickClose(Joystick);
    EXITsetting();
    return 0;
} //main


