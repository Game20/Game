#include<stdio.h>
//#include<SDL/SDL.h>
#include "client_func.h"

#define MSG_NUM 5           /* メッセージの数 */

// static
static char gFontFile[] = "images/APJapanesefontT.ttf";
static char gMsgStrings[ 100 ][ 100 ] = { "はじめる", "あそびかた", "おわる", "つづける", "おわる"};
static SDL_Color black = {0x00, 0x00, 0x00};
int setstartp;

TTF_Font* sTTF;
//SDL_Surface *usa;  // 画像データへのポインタ

char gMapDataFile[] = "map.data";
MapType gMaps[ MAP_Width ][ MAP_Height ]; // マップの性質
SDL_Surface *gMapImage; // マップ
//SDL_Rect gameRect = { 0,0, WIND_Width*bit, WIND_Height*bit }; // ゲームウィンドウの座標
SDL_Rect Player2; // 2Pの座標
int o = 1;


SDL_Rect Player2; // 2Pの座標

/*初期設定*/
void InitWindow(){

    // SDL初期化
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) {
        printf("failed to initialize SDL.\n");
        exit(-1);
    }

    SDL_WM_SetCaption("こころぴょんぴょん", NULL);	// ウィンドウ名などを指定

    // ウィンドウ生成
    if((window = SDL_SetVideoMode(WIND_Width * bit, WIND_Height * bit, 32, SDL_SWSURFACE/* | SDL_FULLSCREEN*/)) == NULL) {
        printf("failed to initialize videomode.\n");
        exit(-1);
    }

    /* マップ画面(フィールドバッファ)の作成 */
    if((mapwindow = SDL_CreateRGBSurface(SDL_HWSURFACE, MAP_Width*bit, MAP_Height*bit, 32, 0, 0, 0, 0)) == NULL) {
        printf("Error.");
        exit(-1);
    }

/* 画像の読み込み */
    gMapImage = IMG_Load( "images/map.png" );
    if( gMapImage == NULL ){
        printf("failed to open map.png .\n");
        exit(0);
    }

    MapLayout();

//SDL_JoystickEventState(SDL_ENABLE);
//Joystick = SDL_JoystickOpen(0);

    usa = IMG_Load("images/usa60.png"); //pngの読み込み


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
    FILE* fp = fopen( gMapDataFile, "r" );
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

    SDL_Rect srcRect = { 0,0, bit,bit };
    SDL_Rect dstRect = { 0 };

    for(i=0; i<MAP_Width; i++){
        dstRect.y = 0;
        for(j=0; j<MAP_Height; j++){
            srcRect.x = gMaps[i][j] * bit ;
            ret += SDL_BlitSurface(gMapImage, &srcRect, mapwindow, &dstRect );
            dstRect.y += bit;
        }
        dstRect.x += bit;
    }
    return 0;
}



// 当たり判定
void hitjudge(void){

    hitx = 0;
    hity = 0;

//左右
    if( gMaps[(newposx+gameRect.x+10)/bit][(P.y+15)/bit] == 1 ||
	gMaps[(newposx+gameRect.x+10)/bit][(P.y+10)/bit+1] == 1 ||
	gMaps[(newposx+gameRect.x+45)/bit][(P.y+15)/bit] == 1 ||
	gMaps[(newposx+gameRect.x+45)/bit][(P.y+10)/bit+1] == 1	)

	hitx = 1;

//上
    if( gMaps[(P.x+gameRect.x+10)/bit][(newposy+15)/bit] == 1 ||
	gMaps[(P.x+gameRect.x+45)/bit][(newposy+15)/bit] == 1	)

	hity = 1;

//下
    if( gMaps[(P.x+gameRect.x+10)/bit][(newposy+15)/bit+1] == 1 ||
	gMaps[(P.x+gameRect.x+45)/bit][(newposy+15)/bit+1] == 1	)

	hity = -1;


//オブジェクトの当たり判定
////////////あとで消す///////////////////
    if(o == 1){
        object[0].gimmick = 1; //岩
        object[0].status = 0; //押されてない状態

        object[0].rect.x = 180;
        object[0].rect.y = 240;
        object[0].rect.w = 60;
        object[0].rect.h = 60;

        object[0].dst.x = 2 * bit - gameRect.x;
        object[0].dst.y = 1 * bit;
        o = 0;
    }
////////////////////////////////////////
    if(gimmickflag == 1 && G_flaghold == 0)
        gimmickflag = 0;

	if(gimmickflag == 0)
    for(i=0; i<SUM_object+1; i++){
	if( (newposx >= object[i].dst.x - 45 && newposx <= object[i].dst.x + 45) &&
        (newposy >= object[i].dst.y - 75 && newposy <= object[i].dst.y + 60)   ) {
            gimmickflag = 1;
            break; //
		}
    }

    if(gimmickflag == 1){
		if(object[i].gimmick == 1){	//岩のとき

            //岩とマップのx座標当たり判定があったとき
            if((gMaps[(object[i].dst.x+gameRect.x)/bit][object[i].dst.y/bit] == 1 ||
                gMaps[(object[i].dst.x+gameRect.x)/bit+1][object[i].dst.y/bit] == 1	||
				jumpflag == 1) && P.y > object[i].dst.y - 75 && G_flaghold == 0)
                hitx = 1;
            else if(P.y == object[i].dst.y - 15 && jumpflag == 0){
				hitx = 2;
				if(PA.x >= bit * 2){
				newposx = P.x + ((newposx - P.x) / 4);
				object[i].dst.x  += newposx - P.x;
				}
				else{
				newposx = P.x;
				}
            }
			//岩の下にマップがなにもなかった場合
		    if(gMaps[(object[i].dst.x+gameRect.x)/bit][object[i].dst.y/bit+1] == 0 &&
			   gMaps[(object[i].dst.x+gameRect.x+59)/bit][object[i].dst.y/bit+1] == 0){
				object[i].dst.y += 4;
				G_flaghold = 1;
			}
			else
			G_flaghold = 0;

			//Player on 岩
			if( G_flaghold == 1 &&
				(P.x >= object[i].dst.x - 40 && P.x <= object[i].dst.x + 40) &&
				(P.y >= object[i].dst.y + 50 && P.y <= object[i].dst.y + 60))
			GameOver();
		}

	if( (newposx >= object[i].dst.x - 45 && newposx <= object[i].dst.x + 45) &&
		(P.y >= object[i].dst.y - 74 && P.y <= object[i].dst.y + 60) )
		hitx = 1;

	if( (P.x >= object[i].dst.x - 45 && P.x <= object[i].dst.x + 45) &&
		(newposy >= object[i].dst.y - 75 && P.y <= object[i].dst.y + 60) ){
		if(G_flaghold == 0)
		hity = -1;
		if(G_flaghold == 1){
		jumpflag = 0;
		jump_LR = 0;
		newposy = object[i].dst.y - 75;
		}
		}
    }


}





void exepaste(void){

    // 向きの管理
    if(UD != 0)
	PA.y = (bit / 4 * 5) * 3; //後ろ
    else if(LR == -1 || jump_LR <= -1)
	PA.y = (bit / 4 * 5) * 2; //左
    else if(LR == 1 || jump_LR >= 1)
	PA.y = (bit / 4 * 5) * 1; //右
    else
	PA.y = (bit / 4 * 5) * 0; //正面


    // アニメーション
    if((PA.y != 0 && time % 20 == 0) || time % 20 == 10){
	PA.x = (PA.x + bit) % (bit * 2);
	//PUSH
	if(hitx != 0 && jumpflag == 0 && UD == 0)
            PA.x += bit * 2;
    }
    if(jumpflag == 1 || UD == -1)
	PA.x = bit; //アニメーション固定

    if(hitx != hithold){
	PA.x = 0;
	hithold = hitx; //ヒットホールドの更新
    }
    else if(hithold != 0 && UD != 0){
        if(timekey == 0){
            PA.x = 0;
            timekey++;
        }
    }
    else if(hithold != 0 && timekey != 0)
	timekey = 0;


    SDL_BlitSurface(mapwindow, &gameRect, window, NULL); // マップ貼り付け

    SDL_BlitSurface(usa, &object[0].rect, window, &object[0].dst); // マップ貼り付け

    SDL_BlitSurface(usa, &PA, SDL_GetVideoSurface(), &P); //キャラ貼り付け


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
    sprintf(title,"本日のこころぴょんぴょん指数 : %d いいなさい", time);

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

/************他プレイヤー表示関数**********************/
void DrawChara(int x, int y)
{
    Player2.x = x;
    Player2.y = y;

    SDL_BlitSurface(usa, &PA, mapwindow, &Player2); //キャラ貼り付け
    SDL_Flip(mapwindow);


}


void GameOver(void){

    SDL_Surface *go_img; // gameover_image
    go_img = IMG_Load("images/game_over.png");

    SDL_Delay(1000);
    SDL_BlitSurface(go_img, NULL, window, &gameRect);
    SDL_Flip(window);// 画面に図形を表示（反映）
    SDL_Delay(1000);
    InitStatus();


}


