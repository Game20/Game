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
SDL_Surface *objectimage;
SDL_Surface *blockimage;
//SDL_Rect gameRect = { 0,0, WIND_Width*bit, WIND_Height*bit }; // ゲームウィンドウの座標


int gimmickflag = 0;
SDL_Rect white = {0, 0, 60, 60};
SDL_Rect revivalflag = {0, 900};

int switchON = 0;//サーバにも送なきゃかも
int switchhold = 0;
int stepflag = 0;
int j, k, l;

int DEBAG = 0;

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
/* 画像の読み込み */
    objectimage = IMG_Load( "images/object.png" );
    if( gMapImage == NULL ){
        printf("failed to open object.png .\n");
        exit(0);
    }

/* 画像の読み込み */
    blockimage = IMG_Load( "images/switchblock.png" );
    if( gMapImage == NULL ){
        printf("failed to open switchblock.png .\n");
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
            srcRect.x = gMaps[i][j] * bit;
			if(gMaps[i][j-1] == 1)
			srcRect.x = gMaps[i][j]*2*bit;
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
///////////////////////////////////////////////////////////////////////////


    for(i=0; i<SUM_object; i++){
	if( (newposx+gameRect.x >= object[i].dst.x - 45 && newposx+gameRect.x <= object[i].dst.x + 45) &&
        (newposy >= object[i].dst.y - 75 && newposy <= object[i].dst.y + 45)){
/*
            break;
    }
	//ギミック動作発生
    if(i != SUM_object){
*/

		//中間ポイントのとき
		if(object[i].gimmick == 0){
			if(P.x+gameRect.x >= object[i].dst.x-20 && P.x+gameRect.x <= object[i].dst.x + 40 &&
			   P.y >= object[i].dst.y+20 && newposy <= object[i].dst.y+40 && object[i].status != 1){
				object[i].status = 1;
				if(object[i].flaghold == -1){
				SDL_BlitSurface(objectimage, &object[i].src, mapwindow, &object[i].dst); // object貼り付け
				object[i].flaghold = 0;
				}
				revivalflag.x = object[i].dst.x;
				revivalflag.y = object[i].dst.y - 60;
				object[i].src.y = 60;
				SDL_BlitSurface(objectimage, &object[i].src, mapwindow, &revivalflag); // object貼り付け
			}
		}

		//岩/バネのとき
		if(object[i].gimmick == 1 || object[i].gimmick == 3){
            //岩とマップのx座標当たり判定があったとき
            if((gMaps[(object[i].dst.x-3)/bit][object[i].dst.y/bit] == 1 ||
                gMaps[(object[i].dst.x)/bit+1][object[i].dst.y/bit] == 1	||
				jumpflag == 1) && P.y > object[i].dst.y - 75 && object[i].flaghold == 0)
                hitx = 1;
            else if(P.y == object[i].dst.y - 15 && jumpflag == 0){
				hitx = 2;
				if(PA.x >= bit * 2){
				newposx = P.x + ((newposx - P.x) / 4);
				object[i].movex = newposx - P.x;
				}
				else{
				newposx = P.x;
				}
            }

			//Player on 岩
			if( object[i].flaghold == 1 &&
				(P.x+gameRect.x >= object[i].dst.x - 45 && P.x+gameRect.x <= object[i].dst.x + 45) &&
				(P.y >= object[i].dst.y + 30 && P.y <= object[i].dst.y + 43) && jump_a != -8)
			GameOver();
		}


		//スイッチのとき
		if(object[i].gimmick == 2){
			if( (newposx+gameRect.x >= object[i].dst.x - 40 && newposx+gameRect.x <= object[i].dst.x + 40) &&
				(P.y >= object[i].dst.y - 74 && P.y <= object[i].dst.y + 25) && object[i].status != 1)
				hitx = 1;
			if(P.x+gameRect.x >= object[i].dst.x - 35 && P.x+gameRect.x <= object[i].dst.x + 35 &&
			  P.y+50/**/ >= object[i].dst.y && P.y <= object[i].dst.y-35){
				if(newposy+75 >= object[i].dst.y+35){
	//				switchON = 1;
					object[i].status = 1; //ステータス：押されてる
					switchblock[object[i].flaghold].flaghold = 1;
					hity = -2;
					newposy = object[i].dst.y - 35;
					object[i].src.y = 60;
					SDL_BlitSurface(mapwindow, &object[i].src, mapwindow, &object[i].dst); // object貼り付け準備
					SDL_BlitSurface(objectimage, &object[i].src, mapwindow, &object[i].dst); // object貼り付け

				}
			}
			else{

				object[i].status = 0;
				switchblock[object[i].flaghold].flaghold = 0;
				object[i].src.y = 0;
				SDL_BlitSurface(mapwindow, &object[i].src, mapwindow, &object[i].dst); // object貼り付け準備
				SDL_BlitSurface(objectimage, &object[i].src, mapwindow, &object[i].dst); // object貼り付け
			}

	for(j=0; j<SUM_object; j++){
	if(object[j].gimmick == 2 && object[i].flaghold == object[j].flaghold && object[j].status == 1)
	switchblock[object[i].flaghold].flaghold = 1;
	}

	if(i >= 11 && i <= 14){
	if(object[i].status == 1)
	switchblock[object[i-1].flaghold].flaghold = 1;

	if(object[i].status == 0 && object[i-1].status == 0)
	switchblock[object[i-1].flaghold].flaghold = 0;
	if(object[i].status == 0 && object[i+1].status == 1)
	switchblock[object[i].flaghold].flaghold = 1;
	}
		}

		//バネのとき
		if(object[i].gimmick == 3){
			if(P.x+gameRect.x >= object[i].dst.x - 45 && P.x+gameRect.x <= object[i].dst.x + 45 &&
			  P.y+90 >= object[i].dst.y && P.y <= object[i].dst.y - 35){
				if(P.y+60 >= object[i].dst.y && jump_a <= 0)
				object[i].src.y = 60;
				else
				object[i].src.y = 0;
				SDL_BlitSurface(mapwindow, &object[i].src, mapwindow, &object[i].dst); // object貼り付け準備
				if(newposy+75 >= object[i].dst.y+35){
					newposy = object[i].dst.y-40;
					jump_a = 6 * object[i].status;	
				}
			SDL_BlitSurface(objectimage, &object[i].src, mapwindow, &object[i].dst); // object貼り付け	
			}
		}


	//オブジェクト全体の当たり判定
	if(object[i].gimmick != 2){
	if( (newposx+gameRect.x >= object[i].dst.x - 45 && newposx+gameRect.x <= object[i].dst.x + 45) &&
		(P.y >= object[i].dst.y - 74 && P.y <= object[i].dst.y + 35) )
		hitx = 1;
	if( (P.x+gameRect.x >= object[i].dst.x - 45 && P.x+gameRect.x <= object[i].dst.x + 45) &&
		(newposy >= object[i].dst.y - 75 && newposy <= object[i].dst.y + 43)){
			if(newposy >= object[i].dst.y && object[i].flaghold == 0)
			hity = 1;//上にヒット
		if(newposy <= object[i].dst.y && object[i].flaghold == 0 && object[i].gimmick != 3)
			hity = -1;//下にヒット
				if(object[i].flaghold == 1 && newposy <= object[i].dst.y + 20){
				jumpflag = 0;
				jump_LR = 0;
				newposy = object[i].dst.y - 75;
				}
		}
	}
    }
	}


	////再判定
	for(j=0; j<SUM_object; j++){

	if(object[j].gimmick == 1 || object[j].gimmick == 3){
	//岩の下にマップがなにもなかった場合
	if(gMaps[(object[j].dst.x)/bit][object[j].dst.y/bit+1] == 0 &&
	   gMaps[(object[j].dst.x+59)/bit][object[j].dst.y/bit+1] == 0 &&
		object[j].dst.y <= 20 * 60){
		object[j].flaghold = 1;
		if(object[j].movex != 0)
		object[j].movex = 0;
	}

	//オブジェクト同士の当たり判定
	if(object[j].movex != 0  || object[j].movey != 0 || object[j].flaghold == 1){
		for(k=0; k<SUM_object; k++){
			if(j != k && object[j].dst.x+60 >= object[k].dst.x && object[j].dst.x <= object[k].dst.x+60){
				if(object[j].dst.y+30 >= object[k].dst.y && object[j].dst.y-30 <= object[k].dst.y){
					object[j].movex = 0;
				}
				if(object[j].dst.y+60 >= object[k].dst.y && object[j].dst.y+30 <= object[k].dst.y){
					object[j].movey = 0;
					object[j].flaghold = 0;
				}
			}
		}
	}
	}

	//フラグホールドの処理
	if(object[j].flaghold == 1){
		if(object[j].gimmick == 1 || object[j].gimmick == 3){
		    if( gMaps[(object[j].dst.x)/bit][object[j].dst.y/bit+1] == 0 &&
			    gMaps[(object[j].dst.x+59)/bit][object[j].dst.y/bit+1] == 0 &&
				object[j].dst.y <= 20 * 60)
			object[j].movey = 4;
			else
			object[j].flaghold = 0;	
		}
	}
	if(object[j].movex != 0 || object[j].movey != 0){
	SDL_BlitSurface(mapwindow, &object[j].src, mapwindow, &object[j].dst); // object貼り準備
	object[j].dst.x += object[j].movex;
	object[j].dst.y += object[j].movey;
	SDL_BlitSurface(objectimage, &object[j].src, mapwindow, &object[j].dst); // object貼り付け
	object[j].movex = 0;
	object[j].movey = 0;
	}
	}


	//スイッチブロックの当たり判定と描写
	for(j=0; j<SUM_switchblock; j++){
		if(switchblock[j].src.y != switchblock[j].flaghold*60){
		switchblock[j].src.y = switchblock[j].flaghold*60;
			for(k=0; k<switchblock[j].gimmick; k++){
				for(l=0; l<switchblock[j].status; l++){
				SDL_BlitSurface(mapwindow, &switchblock[j].src, mapwindow, &switchblock[j].dst); // switchblock再貼り付け
				SDL_BlitSurface(blockimage, &switchblock[j].src, mapwindow, &switchblock[j].dst); // switchblock再貼り付け
				switchblock[j].dst.x += bit;
				}
			switchblock[j].dst.y += bit;
			}
		switchblock[j].dst.x -= l*60;
		switchblock[j].dst.y -= k*60;
		}

		if(switchblock[j].flaghold == 1){
		if( (newposx+gameRect.x >= switchblock[j].dst.x-60+20 && newposx+gameRect.x <= switchblock[j].dst.x+switchblock[j].status*bit-60 + 40) &&
			(P.y >= switchblock[j].dst.y-70 && P.y <= switchblock[j].dst.y+switchblock[j].gimmick*bit-60 + 25) )
			hitx = 1;
		if(P.x+gameRect.x >= switchblock[j].dst.x-60+20 && P.x+gameRect.x <= switchblock[j].dst.x+switchblock[j].status*bit-60 + 40 &&
		  newposy >= switchblock[j].dst.y-75 && newposy <= switchblock[j].dst.y+switchblock[j].gimmick*bit-60 + 43){
			if(newposy >= switchblock[j].dst.y)
			hity = 1;//上にヒット
			if(newposy <= switchblock[j].dst.y)
			hity = -1;//下にヒット
			}
		}
	}

	//ハシゴの判定
	if(stepflag >= 1){
	for(j=0; j<SUM_steps; j++){
		if( (P.x+gameRect.x+10 >= steps[j].dst.x-10 && P.x+gameRect.x <= steps[j].dst.x + 30) &&
			(P.y >= steps[j].dst.y + 45 && P.y+30 <= steps[j].dst.y + 25 + steps[j].status*bit)){
			stepflag = 2;
			if(newposy <= steps[j].dst.y + 45)
			newposy = P.y;
		break;
		}
	}
	if(j == SUM_steps || (UD == -1 && hity == -1)){
		UD = 0;
		stepflag = 0;
	}
	}

}

/*
clear
git branch
git checkout client
git add client_window.c map.data client_func.h client_system.c 
git commit -m "オブジェクト絵追加 ギミック判定強化 マップ拡張"
git checkout master
git branch
git merge client
./shell.sh client_window.c map.data client_func.h client_system.c
git branch -d client
git checkout -b client
*/

void scroll(void){

    shiftdef = P.x - (WIND_Width * bit/2-30);

    if(gameRect.x + shiftdef >= 0 && gameRect.x + shiftdef <= (MAP_Width - WIND_Width) * 60){
        gameRect.x += shiftdef;
        P.x -= shiftdef;
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

    SDL_BlitSurface(mapwindow, &gameRect, window, NULL); // マップ貼り付け

//    SDL_BlitSurface(usa, &object[0].src, mapwindow, &object[0].dst); // object貼り付け

    SDL_BlitSurface(usa, &PA, window, &P); //キャラ貼り付け


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
///*
                    if(P.y == 500){
                    DEBAG = 5;
                    titlep = 0;
                    titlep2 = 0;
		}
//*/
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

/************プレイヤー表示関数**********************/
void DrawChara(void)
{
//int i;
//    for(j = 0; j < MAX_CLIENTS; j++){
//        SDL_BlitSurface(usa, &PA, mapwindow, &player[j].pos); //キャラ貼り付け
//   }

//    SDL_Flip(mapwindow);
//    SDL_Flip(window);
}


void GameOver(void){

	PA.x = 3 * bit;
	PA.y = 3 * 75;

	SDL_BlitSurface(objectimage, &object[i].src, mapwindow, &object[i].dst); // object貼り付け
    SDL_BlitSurface(mapwindow, &gameRect, window, NULL); // マップ貼り付け
    SDL_BlitSurface(usa, &PA, window, &P);
    SDL_Flip(window);// 画面に図形を表示（反映）
    SDL_Delay(1400);

	for(j=0; j<SUM_object; j++){
	SDL_BlitSurface(mapwindow, &object[j].src, mapwindow, &object[j].dst); // object貼り準備
	}
	printf("\n\n\n %d \n\n\n", object[4].status);
    InitStatus();

}


