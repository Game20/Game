#include <stdio.h>
#include <SDL/SDL.h>       // SDLを用いるために必要なヘッダファイルをインクルード
#include <SDL/SDL_image.h>       // SDL_imageを用いるために必要なヘッダファイルをインクルード
#include <SDL/SDL_gfxPrimitives.h>	// gfxによる図形処理をするために必要なヘッダファイルをインクルード
#include <SDL/SDL_ttf.h>	// TrueTypeフォントを表示するために必要なヘッダファイルをインクルード
#include <SDL/SDL_opengl.h> // SDLでOpenGLを扱うために必要なヘッダファイルをインクルード
#include "client_func.h"

void DisplayStatus(void);
void checkhit(void);
void SS(void);//Start Stetas 初期化
void title(void);
void EXIT(void);
void story(void);
void result(void);
void setstart(void);

void eventdisp(void);
void keycont(void);
void exepaste(void);
void EXITsetting(void);

/*サーバ移行予定の関数*/
void newpositionjadge(void);


SDL_Surface *window; // ウィンドウ（画像）データへのポインタ（グローバル）

//変数などの宣言(main外)

int screensizex = 1520; // max1535 / 60*60 25マス / 80*80 19マス 
int screensizey = 800;  // max846  / 60*60 14マス / 80*80 10マス

int time = 0;	//時間
int LR = 0;		//左右入力
int UD = 0;		//上下入力
int jumpflag = 0;	//
int jump_a = 0;	//
int jump_LR = 0;//
int LR_sift = 0;//
int newposx = 0, newposy = 0;	//新規位置　まだ実装できてない

int fm = 0;		//
int titlep = 1, titlep2 = 0;//
int exit_p = 0;	//


SDL_Rect PA = { 0, 0, 80, 100 }; //プレイヤーキャラ座標読み取り　あとで構造体化
SDL_Rect P = { 200, 640 };
SDL_Rect PMR = { 0, 0, 80, 100 }; 
SDL_Rect PM = { 0, 0 };//Player_Message(_Read) 1Pとかの表示 あとで構造体化

SDL_Event event; // SDLによるイベントを検知するための構造体
SDL_Event quit_event = {SDL_QUIT};	// 特定のイベント名を格納

SDL_Surface *usa;  // 画像データへのポインタ

SDL_TimerID timer_id;	//タイマ割り込みを行うためのタイマのID


/* フォント関連 */

#define MSG_NUM 6           /* メッセージの数 */

SDL_Color black = {0x00, 0x00, 0x00};
SDL_Color Yellow = {0xff, 0xff, 0x00};
TTF_Font* gTTF;	// TrueTypeフォントデータへのポインタ
TTF_Font* sTTF;	
static char gFontFile[] = "images/APJapanesefontT.ttf";

static char gMsgStrings[ 100 ][ 100 ] = { "はじめる", "あそびかた", "おわる", "つづける", "おわる", "1P"};
static SDL_Surface *gMessages[ 100 ];




// メイン関数 /////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[]) {

/*初期設定*/
setstart();

title();

	// 無限ループ
	while(1){

	SDL_FillRect(window, NULL, 0xffffffff);	// ウィンドウ背景初期化
	SDL_Delay(20);
	time++;

	// イベントを取得したら
	if(SDL_PollEvent(&event))
		eventdisp();	// イベント処理

	keycont();	/*キーボード操作*/

	newpositionjadge(); /*当たり判定　後でサーバに移行*/

	exepaste(); /*貼り付け設定 & 実行*/

	SDL_Flip(window);// 画面に図形を表示（反映）
	DisplayStatus();

	if(exit_p == 1)//終了フラグが立てばwhilebreak
	break;

    } //whileループ

/*終了設定*/
EXITsetting();
return 0;
} //main



/*初期設定*/
void setstart(){

	// SDL初期化
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) {
		printf("failed to initialize SDL.\n");
		exit(-1);
	}

	SDL_WM_SetCaption("こころぴょんぴょん", NULL);	// ウィンドウ名などを指定

	// ウィンドウ生成
	if((window = SDL_SetVideoMode(screensizex, screensizey, 32, SDL_SWSURFACE/* | SDL_FULLSCREEN*/)) == NULL) {
		printf("failed to initialize videomode.\n");
		exit(-1);
	}

//SDL_JoystickEventState(SDL_ENABLE);
//Joystick = SDL_JoystickOpen(0);


	usa= IMG_Load("images/usa.png");//pngの読み込み


    // フォントからメッセージ作成 
    // 初期化 
    if(TTF_Init() < 0){
        printf("error");
    }
    // フォントを開く 
    gTTF = TTF_OpenFont( gFontFile, 48 );
    if( gTTF == NULL ){
        printf("error");
    }

    // メッセージ作成 
    int h;
    for(h=0; h<MSG_NUM; h++){
        gMessages[h] = TTF_RenderUTF8_Blended(gTTF, gMsgStrings[h], black);
    }

	sTTF = TTF_OpenFont( gFontFile, 30 );
	gMessages[5] = TTF_RenderUTF8_Blended(sTTF, gMsgStrings[5], black);

}


void eventdisp(){
	// イベントの種類ごとに処理
	switch (event.type) {
		case SDL_QUIT:
			exit_p = 1; //終了フラグが立ったぞー
			break;

		if (event.type == SDL_KEYDOWN ){ 
			case SDL_KEYDOWN:// キーボードのキーが押された時
			switch(event.key.keysym.sym){

			case SDLK_RIGHT:
			LR = 1;
			break;
			case SDLK_LEFT:
			LR = -1;
			break;

			case SDLK_UP:
			UD = 1;
			break;
			case SDLK_DOWN:  
			UD = -1;
			break;

			case SDLK_SPACE: //スペースキーを押した時
			if(jumpflag == 0){
			jumpflag = 1;}
			jump_a = 14; //初速んんんんんんんんん
				
			break;

			case SDLK_ESCAPE:	// エスケープキー
				EXIT();
			break;
	
			default:
			break;        //それ以外のイベントは無視する
			}
		}

		else if (event.type == SDL_KEYUP){
		case SDL_KEYUP:// キーボードのキーが離された時
			switch(event.key.keysym.sym){
			case SDLK_RIGHT:
			case SDLK_LEFT:  //左右キーどちらかで
			LR = 0;
			case SDLK_UP:
			case SDLK_DOWN:  //上下キーどちらかで
			UD = 0;
			break;
			}
		}
	}
}

void keycont(void){

	Uint8 *key = SDL_GetKeyState(NULL);
	if(key[SDLK_RIGHT] == SDL_PRESSED){	//右移動
		P.x += 4; 
		if(jumpflag == 1)
			jump_LR = 1;
	}
	if(key[SDLK_LEFT] == SDL_PRESSED){	//左移動
		P.x -= 4; 
		if(jumpflag == 1)
			jump_LR = -1;
	}
	if(key[SDLK_UP] == SDL_PRESSED){	//上移動
		P.y -= 2;
		UD = 1; //←あとで消す
		jumpflag = 0; //←あとで消す
	}
	if(key[SDLK_DOWN] == SDL_PRESSED){	//下移動
		P.y += 4;
	}


/*その他詳細設定*/

	if(key[SDLK_RIGHT] != SDL_PRESSED && key[SDLK_LEFT] != SDL_PRESSED && jump_LR != 0)
		P.x += 4 * jump_LR; //空中なら静止しない

	if(jumpflag == 1 && jump_a > -8)	
		jump_a -= 1; //重力加速　

	if(jumpflag == 0){		
		jump_a = 0;	//ジャンプフラグないとき重力ない
		jump_LR = 0;//LR判定もない
	}

	if(jumpflag == 1)
	P.y -= jump_a;//加速度で移動(y軸なのでマイナス)

}

void exepaste(void){

	// 向きの管理
	if(LR == -1 || jump_LR == -1)
	PA.y = 200; //左
	else if(LR == 1 || jump_LR == 1)
	PA.y = 100; //右
	else if(jumpflag == 0)
	PA.y = 0; //正面
	if(UD != 0)
	PA.y = 300; //後ろ

	// アニメーション
	if(time % 10 == 0)
	PA.x = (PA.x + 80) % 160;
	if(jumpflag == 1 || UD == -1)
	PA.x = 80; //アニメーション固定

	SDL_BlitSurface(usa, &PA, SDL_GetVideoSurface(), &P);


/*
	SDL_BlitSurface(Player[paste0], &PA[paste0], SDL_GetVideoSurface(), &P[paste0]);
	SDL_BlitSurface(Player[paste1], &PA[paste1], SDL_GetVideoSurface(), &P[paste1]);
	SDL_BlitSurface(Player[paste2], &PA[paste2], SDL_GetVideoSurface(), &P[paste2]);
	SDL_BlitSurface(Player[paste3], &PA[paste3], SDL_GetVideoSurface(), &P[paste3]);
*/

	/*『1P』とかの表示　あとで4人分まで拡張*/
	PM.x = P.x + 25;
	PM.y = P.y - 30;
	SDL_BlitSurface(gMessages[5], &PMR, SDL_GetVideoSurface(), &PM);

}



// 残り時間などの表示
void DisplayStatus(void){
	char title[ 160 ];
	sprintf(title,"本日のこころぴょんぴょん指数 : %d ぴょんぴょん", time);

	/* ウインドウのタイトルをセット */
	SDL_WM_SetCaption(title,NULL);
}


//タイトル
void title(void){
P.x = 150;
P.y = 400;
PA.x = 0;
PA.y = 0;

titlep = 1;//ループ条件
	// 無限ループ
	while(titlep){
		SDL_FillRect(window, NULL, 0xffffffff);	// ウィンドウ背景初期化
		// イベントを取得したなら
		if(SDL_PollEvent(&event)){
			// イベントの種類ごとに処理
			switch (event.type) {
			case SDL_QUIT:
				EXIT();
				break;
            case SDL_KEYDOWN:// キーボードのキーが押された時
                switch(event.key.keysym.sym){
				case SDLK_UP:
					P.y -= 100;
					if(P.y == 300)
					P.y = 600;
					break;	
				case SDLK_DOWN:
					P.y += 100;
					if(P.y == 700)
					P.y = 400;
					break;

				case SDLK_RETURN: //エンターを押した時
					if(P.y == 400){
					titlep = 0;
					titlep2 = 0;
					}
					//if(P.y == 500)
					//howto();
					if(P.y == 600)
					EXIT();
					break;
				case SDLK_ESCAPE:	// エスケープキー
					EXIT();
					break;
					SDL_Delay(1000);
				}
			//スティック(左の十字キーも)
			case SDL_JOYAXISMOTION:
				//上下の動き
				if(event.jaxis.axis == 1) 
				{
					if(event.jaxis.value < -0x7000)
						P.y -= 100;
						if(P.y == 300)
						P.y = 600;

					if(event.jaxis.value >  0x7000)
						P.y += 100;
						if(P.y == 700)
						P.y = 400;
				}
				break;
			}
		}

    /* メッセージ表示 */
    SDL_Rect srcRect = {0,0,0,0};
    SDL_Rect dstRect = {270,420};

for(fm=0; fm <= 2; fm++){

        srcRect.w = gMessages[fm]->w;
        srcRect.h = gMessages[fm]->h;
 
        SDL_BlitSurface(gMessages[fm], &srcRect, SDL_GetVideoSurface(), &dstRect);
	dstRect.y += 100;
}

SDL_BlitSurface(usa, &PA, SDL_GetVideoSurface(), &P);
SDL_Flip(window);// 画面に図形を表示（反映）
	}
P.y = 640;
}


//ステータス初期化
void SS(void){
P.x = 200;
P.y = 400;
PA.y = 0;
time = 0;

exit_p = 0;

titlep = 1;
titlep2 = 0;

}

void EXIT(void){
int a = P.x;
int b = P.y;
P.x = 100;
P.y = 350;
PA.x = 0;
PA.y = 0;

titlep2 = 1;//ループ条件
	// 無限ループ
	while(titlep2){
		SDL_FillRect(window, NULL, 0x00808080);	// ウィンドウ背景初期化
		// イベントを取得したなら
		if(SDL_PollEvent(&event)){
			// イベントの種類ごとに処理
			switch (event.type) {
			case SDL_QUIT:
				exit_p = 1;
				titlep = 0;
				titlep2 = 0;
				break;
            case SDL_KEYDOWN:// キーボードのキーが押された時
                switch(event.key.keysym.sym){
				case SDLK_RIGHT:
					P.x += 300;
					if(P.x >= 700)
					P.x = 100;
					break;	
				case SDLK_LEFT:
					P.x -= 300;
					if(P.x <= 0)
					P.x = 400;
					break;

				case SDLK_RETURN: //エンターを押した時
					if(P.x == 400){
					exit_p = 1;
					titlep = 0;
					}
					titlep2 = 0;
					break;

				case SDLK_ESCAPE:	// エスケープキー
					exit_p = 1;
					titlep = 0;
					titlep2 = 0;
					break;
				}	
		}
	}


    /* メッセージ表示 */
    SDL_Rect srcRect = {0,0,0,0};
    SDL_Rect dstRect = {220,370};

for(fm=3; fm <= 4; fm++){
        srcRect.x = srcRect.y = 0;
        srcRect.w = gMessages[fm]->w;
        srcRect.h = gMessages[fm]->h;
 
        SDL_BlitSurface(gMessages[fm], &srcRect, SDL_GetVideoSurface(), &dstRect);
	dstRect.x += 300;
}

SDL_BlitSurface(usa, &PA, SDL_GetVideoSurface(), &P);
SDL_Flip(window);// 画面に図形を表示（反映）
	}

P.x = a;
P.y = b;

}

/*終了設定*/
void EXITsetting(){
    // フォントを閉じる 
	TTF_CloseFont(gTTF);  gTTF=NULL;
	TTF_CloseFont(sTTF);  gTTF=NULL;

	SDL_FillRect(window, NULL, 0xffffffff);	// ウィンドウ背景初期化
	SDL_Flip(window);// 画面に図形を表示（反映）
	SS();
}


/*サーバ移行予定地*/

void newpositionjadge(){

	if(P.x >= screensizex-100)
		P.x = screensizex-100;
	if(P.x <= 0)
		P.x = 0;//画面外に出ない

	if(P.y - jump_a > 640){
		jumpflag = 0;
		P.y = 640;//画面外に出ない あとで消す
	}
}


	/*貼り付け順*/
//	pasteorder();

/*貼り付け順決定（y軸上から）*/
/*
int paste0 = 0 paste1 = 1, paste2 = 2, paste3 = 3;
int pasten = 0;

if(P[0].y > P[1].y){
pasten = paste0;
paste0 = paste1;
paste1 = pasten;
}
if(P[0].y > P[2].y){
pasten = paste0;
paste0 = paste2;
paste2 = pasten;
}
if(P[0].y > P[3].y){
pasten = paste0;
paste0 = paste3;
paste3 = pasten;
}
if(P[1].y > P[2].y){
pasten = paste1;
paste1 = paste2;
paste2 = paste1;
}
if(P[1].y > P[3].y){
pasten = paste1;
paste1 = paste3;
paste3 = paste1;
}
if(P[2].y > P[3].y){
pasten = paste2;
paste2 = paste3;
paste3 = paste2;
}
*/






