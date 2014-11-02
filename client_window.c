#include<stdio.h>
//#include<SDL/SDL.h>
#include "client_func.h"

#define MSG_NUM 5           /* メッセージの数 */

// static 
static char gFontFile[] = "images/APJapanesefontT.ttf";
static char gMsgStrings[ 100 ][ 100 ] = { "はじめる", "あそびかた", "おわる", "つづける", "おわる"};
static SDL_Color black = {0x00, 0x00, 0x00};
int setstartp;
int screensizex;
int screensizey;
TTF_Font* sTTF;
//SDL_Surface *usa;  // 画像データへのポインタ

char gMapDataFile1[] = "map1.data";
MapType gMaps[ MAP_Width ][ MAP_Height ]; // マップの性質
SDL_Surface *gMapImage; // マップ
char gMapImgFile[] = "images/map.png"; // マップ画像

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
	
	/* マップ画面(フィールドバッファ)の作成 */
    if((mapwindow = SDL_CreateRGBSurface(SDL_HWSURFACE, MAP_Width*MAP_ChipSize, MAP_Height*MAP_ChipSize, 32, 0, 0, 0, 0)) == NULL) {
        printf("Error.");
        exit(-1);
    }

/* 画像の読み込み */
    gMapImage = IMG_Load( gMapImgFile );
    if( gMapImage == NULL ){
        printf("failed to open map.png .\n");
        exit(0);
    }

MapLayout();

//SDL_JoystickEventState(SDL_ENABLE);
//Joystick = SDL_JoystickOpen(0);

	usa = IMG_Load("images/usa.png");//pngの読み込み


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

/* マップ配置 */
int MapLayout()
{
/* マップ読み込み */
    FILE* fp = fopen( gMapDataFile1, "r" );
    if( fp == NULL ){
        printf("failed to open map data.\n");
        return 0;
    }
    int i, j, ret = 0;
    for(j=0; j<MAP_Height; j++){
        for(i=0; i<MAP_Width; i++){
            if(fscanf(fp, "%d", &gMaps[i][j]) != 1){
                fclose( fp );
                printf("failed to load map data.\n");
                return 0;
            }
        }
    }
    fclose( fp );

    SDL_Rect srcRect = { 0,0, MAP_ChipSize,MAP_ChipSize };
    SDL_Rect dstRect = { 0 };

    for(i=0; i<MAP_Width; i++){
        dstRect.y = 0;
        for(j=0; j<MAP_Height; j++){
            srcRect.x = gMaps[i][j] * MAP_ChipSize ;
            ret += SDL_BlitSurface(gMapImage, &srcRect, mapwindow, &dstRect );
            dstRect.y += MAP_ChipSize;
        }
        dstRect.x += MAP_ChipSize;
    }
    return 0;
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


