#include<stdio.h>
//#include<SDL/SDL.h>
#include "client_func.h"

#define MSG_NUM 11           /* メッセージの数 */

// static
static char gFontFile[] = "images/APJapanesefontT.ttf";
static char gMsgStrings[ 100 ][ 100 ] = { "さいしょから", "ステージ2から", "おわる", "つづける", "おわる", "1P", "2P", "3P", "4P", "Rabbit × Labyrinth", "ラビット×ラビリンス"};
static SDL_Color black = {0x00, 0x00, 0x00};
int setstartp;


TTF_Font* sTTF;

SDL_Surface *gMessages[ 100 ];

SDL_Surface *usa2, *neko, *inu, *panda;  // 画像データへのポインタ
SDL_Surface *gameclear, *titleCG;

char gMapDataFile[] = "map.data";
char gMapDataFile2[] = "map2.data";
MapType gMaps[ MAP_Width ][ MAP_Height ]; // マップの性質
//MapType gMaps2[ MAP_Width ][ MAP_Height ]; // マップの性質
SDL_Surface *gMapImage; // マップ
SDL_Surface *objectimage;
SDL_Surface *blockimage;
//SDL_Rect gameRect = { 0,0, WIND_Width*bit, WIND_Height*bit }; // ゲームウィンドウの座標


int gimmickflag = 0;
SDL_Rect white = {0, 0, 60, 60};
SDL_Rect revivalflag = {0, 0};
SDL_Rect oldsrc = {0, 0, 60, 60};
SDL_Rect olddst = {0, 0};
int revivalflagp = -1;

int stepflag = 0;
int keycommand = 0;
int j, k, l;
int keyhold = 0;
int switchcount = 0;
int clearpoint = 0;
int loopend = 0;

int DEBAG1 = 0;
int DEBAG2 = 0;

int mynum;
int gClientNum;
int start_flag;

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

    if((objectwindow = SDL_CreateRGBSurface(SDL_HWSURFACE, MAP_Width*bit, MAP_Height*bit, 32, 0, 0, 0, 0)) == NULL) {
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

    usa2 = IMG_Load("images/usa.png"); //pngの読み込み
    if( usa2 == NULL ){
        printf("failed to open usa.png .\n");
        exit(0);
    }

    neko = IMG_Load("images/neko.png"); //pngの読み込み
    if( neko == NULL ){
        printf("failed to open neko.png .\n");
        exit(0);
    }

    inu = IMG_Load("images/inu.png"); //pngの読み込み
    if( inu == NULL ){
        printf("failed to open inu.png .\n");
        exit(0);
    }

    panda = IMG_Load("images/panda.png"); //pngの読み込み
    if( panda == NULL ){
        printf("failed to open panda.png .\n");
        exit(0);
    }

    gameclear = IMG_Load("images/gameclear.jpg"); //pngの読み込み
    if( gameclear == NULL ){
        printf("failed to open gameclear.jpg .\n");
        exit(0);
    }

	titleCG = IMG_Load("images/title2.png");


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
    gMessages[10] = TTF_RenderUTF8_Blended(sTTF, gMsgStrings[10], black);

}

/* マップ配置 */
int MapLayout(){
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
    /*
      for(j=0; j<MAP_Height; j++){
      for(i=0; i<MAP_Width; i++){
      if(fscanf(fp2, "%d", &gMaps2[i][j]) != 1){
      fclose( fp2 );
      printf("failed to load map2 data.\n");
      return 0;
      }
      }
      }
      fclose( fp2 );
    */

    SDL_Rect srcRect = { 0,0, bit,bit };
    SDL_Rect dstRect = { 0 };
    srcRect.x = 0;
    for(i=0; i<MAP_Width; i++){
        dstRect.y = 0;
        for(j=0; j<MAP_Height; j++){
            srcRect.x = gMaps[i][j] * bit;

            if(gMaps[i][j-1] == 1)
                srcRect.x = gMaps[i][j]*2*bit;

            if(gMaps[i][j] >= 2){
                srcRect.x = 0;
                ret += SDL_BlitSurface(gMapImage, &srcRect, mapwindow, &dstRect );
                srcRect.x = (gMaps[i][j]+3)*bit;
            }
            ret += SDL_BlitSurface(gMapImage, &srcRect, mapwindow, &dstRect );
            dstRect.y += bit;
        }
        dstRect.x += bit;
    }

    dstRect.x = 0;
    dstRect.y = 0;

    return 0;
}


void Mapshift(){

    FILE* fp2 = fopen( gMapDataFile2, "r" );
    if( fp2 == NULL ){
        printf("failed to open map2 data.\n");
        return 0;
    }

    int i, j, ret = 0;
    for(j=0; j<MAP_Height; j++){
        for(i=0; i<MAP_Width; i++){
            if(fscanf(fp2, "%d", &gMaps[i][j]) != 1){
                fclose( fp2 );
                printf("failed to load map2 data.\n");
                return 0;
            }
        }
    }
    fclose( fp2 );

    SDL_Rect srcRect = { 0,0, bit,bit };
    SDL_Rect dstRect = { 0 };

    for(i=0; i<MAP_Width; i++){
        dstRect.y = 0;
        for(j=0; j<MAP_Height; j++){
            srcRect.x = (gMaps[i][j]+3) * bit;

            if(gMaps[i][j] == 2 || gMaps[i][j] == 3){
                srcRect.x = 3 * bit;
                ret += SDL_BlitSurface(gMapImage, &srcRect, mapwindow, &dstRect );
                srcRect.x = (gMaps[i][j]+3)*bit;
            }
            ret += SDL_BlitSurface(gMapImage, &srcRect, mapwindow, &dstRect );
            dstRect.y += bit;
        }
        dstRect.x += bit;
    }

    mapread = stageP;

}



// 当たり判定
void hitjudge(void){

    hitx = 0;
    hity = 0;

    //左右
    if( gMaps[(newposx+gameRect.x+10)/bit][(P.y+15)/bit] == 1 ||
        gMaps[(newposx+gameRect.x+10)/bit][(P.y+10)/bit+1] == 1 ||
        gMaps[(newposx+gameRect.x+45)/bit][(P.y+15)/bit] == 1 ||
        gMaps[(newposx+gameRect.x+45)/bit][(P.y+10)/bit+1] == 1	||
		newposx < 0 || newposx > 24*60)

        hitx = 1;

    //上
    if( gMaps[(P.x+gameRect.x+10)/bit][(newposy+15)/bit] == 1 ||
        gMaps[(P.x+gameRect.x+45)/bit][(newposy+15)/bit] == 1)

        hity = 1;

    //下
    if( gMaps[(P.x+gameRect.x+10)/bit][(newposy+15)/bit+1] == 1 ||
        gMaps[(P.x+gameRect.x+45)/bit][(newposy+15)/bit+1] == 1	)
        hity = -1;


    //針or崖
    if((gMaps[(P.x+gameRect.x+10)/bit][(P.y+15)/bit+1] == 2 ||
        gMaps[(P.x+gameRect.x+45)/bit][(P.y+15)/bit+1] == 2	||
        gMaps[(P.x+gameRect.x+10)/bit][(P.y+15)/bit] == 3 ||
        gMaps[(P.x+gameRect.x+45)/bit][(P.y+15)/bit] == 3 ||
		P.y >= 14*bit) && clearpoint == 0){
        Mix_PauseMusic();
        playSE(1);

        SendGameoverCommand();
        GameOver(mynum);
        Mix_ResumeMusic();
    }
    //ゴール
    //	if( object[29].status == 1 && jumpflag == 0 &&
    //	    gMaps[(P.x+gameRect.x+45)/bit][(newposy+15)/bit] == 5 &&
    //		gMaps[(P.x+gameRect.x)/bit+1][(newposy+15)/bit] == 5 )

    /**/    for(i = 0; i <= max_map_object; i++) {
        object[i].oldsrc.x = object[i].src.x;
        object[i].oldsrc.y = object[i].src.y;
        object[i].olddst.x = object[i].dst.x;
        object[i].olddst.y = object[i].dst.y;
    }

    //オブジェクトの当たり判定
    ///////////////////////////////////////////////////////////////////////////
    for(i=0; i<=max_map_object; i++){
	if( (newposx+gameRect.x >= object[i].dst.x - 45 && newposx+gameRect.x <= object[i].dst.x + 45) &&
            (newposy >= object[i].dst.y - 75 && newposy <= object[i].dst.y + 45)){
/*
  break;
  }
  //ギミック動作発生
  if(i != max_map_object){
*/

            //中間ポイントのとき
            if(object[i].gimmick == 0){
                if(P.x+gameRect.x >= object[i].dst.x-20 && P.x+gameRect.x <= object[i].dst.x + 40 &&
                   P.y >= object[i].dst.y+20 && P.y <= object[i].dst.y+45 && object[i].status == 0 && P.y > newposy){
                    object[i].status = 2;
                    SendObjectCommand(i, object[i].status, object[i].dst.x, object[i].dst.y, object[i].movex, object[i].movey); // オブジェクトのデータの送信
                    //	newposy = object[i].dst.y+60;
                }
            }

            //岩/バネのとき
            if(object[i].gimmick == 1 || object[i].gimmick == 3){
                //岩とマップのx座標当たり判定があったとき
                if((gMaps[(object[i].dst.x-3)/bit][object[i].dst.y/bit] == 1 ||
                    gMaps[(object[i].dst.x)/bit+1][object[i].dst.y/bit] == 1	||
                    jumpflag == 1) && P.y > object[i].dst.y - 75 && object[i].flaghold == 0)
                    hitx = 1;
                else if(P.y == object[i].dst.y - 15 && jumpflag == 0 && keyhold == 0){
                    hitx = 2;
                    if(PA.x >= bit * 2){
                        //他プレイヤーとの当たり判定
                        for(j=0; j<MAX_CLIENTS; j++){
                            if(j != mynum){
                                if((newposx+LR*60+35 > player[j].pos.x && newposx+LR*60-35 < player[j].pos.x) &&
                                   (P.y > player[j].pos.y - 60 && P.y < player[j].pos.y + 60))
                                    hitx = 1;
                            }
                        }
                        if(hitx != 1){
                            object[i].movex = ((newposx - P.x)*2/5);
                            newposx = P.x + object[i].movex;
                        }
                    }
                    else{
                        newposx = P.x;
                    }
                }

                //岩 on Player
                if( object[i].flaghold == 1 &&
                    (P.x+gameRect.x >= object[i].dst.x - 45 && P.x+gameRect.x <= object[i].dst.x + 45) &&
                    (P.y >= object[i].dst.y + 30 && P.y <= object[i].dst.y + 43) && jump_a != -8) {
                    SendGameoverCommand();
                    GameOver(mynum);
                }
            }

            //スイッチのとき
            if(object[i].gimmick == 2 && object[i].status != 2){
                if( (newposx+gameRect.x >= object[i].dst.x - 38 && newposx+gameRect.x <= object[i].dst.x + 40) &&
                    (P.y+74 >= object[i].dst.y+40 && P.y <= object[i].dst.y + 25))
                    hitx = 1;

                if(P.x+gameRect.x >= object[i].dst.x - 35 && P.x+gameRect.x <= object[i].dst.x + 35 &&
                   P.y+50/**/ >= object[i].dst.y && P.y <= object[i].dst.y-35){
                    if(newposy+75 >= object[i].dst.y+35){
                        hity = -2;
                        newposy = object[i].dst.y - 35;
                        if(object[i].status == 0 && switchcount == 0){
                            playSE(2);
                            object[i].status = 1; //ステータス：押されてる
                            switchblock[object[i].flaghold].flaghold = 1;
                            switchcount = 1;
                        }
                    }
                }
                else if(switchcount == 1){
					object[i].status = 0;
					switchblock[object[i].flaghold].flaghold = 0;
                    switchcount = 0;
                        //他プレイヤーが乗っていれば再点火 まだ未検証
                        for(j=0; j<MAX_CLIENTS; j++){
                            if(j != mynum){
						    if(player[j].pos.x+gameRect.x >= object[i].dst.x - 35 && player[j].pos.x+gameRect.x <= object[i].dst.x + 35 &&
						       player[j].pos.y+75 >= object[i].dst.y+35 && player[j].pos.y <= object[i].dst.y-35){
						        object[i].status = 1;
						        switchblock[object[i].flaghold].flaghold = 1;
                            }
							}
                        }
                }

                for(j=0; j<=max_map_object; j++){
                    if(object[j].gimmick == 2 && object[i].flaghold == object[j].flaghold && object[j].status == 1)
                        switchblock[object[i].flaghold].flaghold = 1;
                }

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
                    if(i == 30 && object[i].status == 1)
                        switchblock[30].flaghold = 1;
                    if(i == 33){
                        switchblock[10].flaghold = object[i].status;
                        switchblock[11].flaghold = object[i].status;
                    }
                    if(i == 34 && object[i].status == 1 && object[i-1].status == 1 && object[35].flaghold != -1)
                        switchblock[11].flaghold = 0;
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
                }
            }

            //バネのとき
            if(object[i].gimmick == 3){
playSE(3);
                if(P.x+gameRect.x >= object[i].dst.x - 45 && P.x+gameRect.x <= object[i].dst.x + 45 &&
                   P.y+90 >= object[i].dst.y && P.y <= object[i].dst.y - 35){
                    if(P.y+60 >= object[i].dst.y && jump_a <= 0)
                        object[i].src.y = 60;
                    else
                        object[i].src.y = 0;
                    white.x = object[i].dst.x;
                    white.y = object[i].dst.y;
                    SDL_BlitSurface(mapwindow, &white, objectwindow, &object[i].dst); // object貼り付け準備
                    if(newposy+75 >= object[i].dst.y+35){
                        newposy = object[i].dst.y-40;
                        jump_a = 6 * object[i].status;
                        if(jump_a >= 7){
                            object[i].src.y = 0;
                            SDL_BlitSurface(objectimage, &object[i].src, objectwindow, &object[i].dst); // object貼り付け
                        }
                    }
                    SDL_BlitSurface(objectimage, &object[i].src, objectwindow, &object[i].dst); // object貼り付け
                }
            }

            //バネシーソーのとき
            if(object[i].gimmick == 5){
                if( (newposx+gameRect.x >= object[i].dst.x - 38 && newposx+gameRect.x <= object[i].dst.x + 40) &&
                    (P.y+74 >= object[i].dst.y+40 && P.y <= object[i].dst.y + 25))
                    hitx = 1;
                if(P.x+gameRect.x >= object[i].dst.x - 35 && P.x+gameRect.x <= object[i].dst.x + 38 &&
                   P.y+75 >= object[i].dst.y+30 && P.y <= object[i].dst.y-35 && jump_a <= 0){
                    newposy = object[i].dst.y - 40;
                    jump_LR = 0;
                    if(object[i].status == 0 && jump_a < -1){
                        object[i].status = 1;//PUSH
                        object[object[i].flaghold].status = -1;//PULL
            SendObjectCommand(object[i].flaghold, object[object[i].flaghold].status, object[object[i].flaghold].dst.x, object[object[i].flaghold].dst.y,
                              object[object[i].flaghold].movex, object[object[i].flaghold].movey); // オブジェクトのデータの送信
                    }
                }
            }

    //カギのとき
    if(object[i].gimmick == 4 && jumpflag == 0){
    //    playSE(4);
		if(keycommand == -1){
        white.x = object[i].dst.x;
        white.y = object[i].dst.y;
        SDL_BlitSurface(mapwindow, &white, objectwindow, &object[i].dst); // object貼り付け準備
        if(object[i].status == 0){
            playSE(5);
            object[i].status = mynum+1;
            object[i].dst.x = P.x;
            object[i].dst.y = P.y-44;
            keyhold = 1;
        }
        else if(object[i].status == mynum+1){
            playSE(6);
            object[i].status = 0;
            object[i].dst.x = P.x+gameRect.x+(LR*60);
            object[i].dst.y = P.y;
            keyhold = 0;
        }
            keycommand = 0;
    	SDL_BlitSurface(objectimage, &object[i].src, objectwindow, &object[i].dst); // object貼り付け
		}
	}

    //ループギミックのとき
    if(object[i].gimmick == -1){
	object[i].status = 1;
    }


            //オブジェクト全体の当たり判定
            if(object[i].gimmick != -1 && object[i].gimmick != 2 && object[i].gimmick != 4 && object[i].gimmick != 5 && object[i].gimmick != 6){
                if( (newposx+gameRect.x >= object[i].dst.x - 45 && newposx+gameRect.x <= object[i].dst.x + 45) &&
                    (P.y >= object[i].dst.y - 74 && P.y <= object[i].dst.y + 35) )
                    hitx = 1;
                if( (P.x+gameRect.x >= object[i].dst.x - 45 && P.x+gameRect.x <= object[i].dst.x + 45) &&
                    (newposy >= object[i].dst.y - 75 && newposy <= object[i].dst.y + 43)){
                    if(newposy >= object[i].dst.y && object[i].flaghold == 0)
                        hity = 1;//上にヒット
                    if(newposy <= object[i].dst.y && object[i].flaghold == 0 && object[i].gimmick != 3)
                        hity = -1;//下にヒット
                    if(object[i].flaghold != 0 && newposy <= object[i].dst.y + 20){
                        jumpflag = 0;
                        jump_LR = 0;
                        newposy = object[i].dst.y - 75;
                    }
                }
            }
        }
    }

////再判定
    for(j=0; j<=max_map_object; j++){

        if(object[j].gimmick == 1 || object[j].gimmick == 3 || (object[j].gimmick == 4 && object[j].status == 0)){
            //岩の下にマップがなにもなかった場合
            if(gMaps[(object[j].dst.x)/bit][object[j].dst.y/bit+1] == 0 &&
               gMaps[(object[j].dst.x+59)/bit][object[j].dst.y/bit+1] == 0 &&
               object[j].dst.y <= 20 * 60 && object[j].flaghold != -1){
                //playSE(4);
                object[j].flaghold = 1;

                for(k=0; k<=SUM_switchblock; k++){
                    if(((switchblock[k].src.x == 0 && switchblock[k].src.y == 60) || (switchblock[k].src.x == 60 && switchblock[k].src.y == 0)) &&
                       object[j].dst.x >= switchblock[k].dst.x-59 && object[j].dst.x <= switchblock[k].dst.x+switchblock[k].status*bit &&
                       object[j].dst.y >= switchblock[k].dst.y-60 && object[j].dst.y <= switchblock[k].dst.y+switchblock[k].gimmick*bit-110){
                        object[j].flaghold = 0;
                    }
                }

                if(object[j].flaghold == 1 && object[j].movex != 0)
                    object[j].movex = 0;

            }

            //オブジェクト同士の当たり判定
            if(object[j].movex != 0  || object[j].movey != 0 || object[j].flaghold == 1){
                for(k=0; k<=max_map_object; k++){
                    if(j != k && object[j].dst.x+60 >= object[k].dst.x && object[j].dst.x <= object[k].dst.x+60){
                        if(object[j].dst.y+30 >= object[k].dst.y && object[j].dst.y-30 <= object[k].dst.y){
                            object[j].movex = 0;
                        }
                        if(object[j].dst.y+60 >= object[k].dst.y && object[j].dst.y+20 <= object[k].dst.y && object[j].flaghold != -1){
                            object[j].movey = 0;
                            object[j].flaghold = 0;
                        }
                        if(object[j].gimmick == 3 && object[k].gimmick == 2 && object[j].dst.y+60 >= object[k].dst.y && object[j].dst.y+50 <= object[k].dst.y && object[k].status != 2)
                            object[j].flaghold = 1;
                        if(object[j].gimmick == 1 && object[k].gimmick == 2 && object[j].dst.y+60 >= object[k].dst.y && object[j].dst.y+20 <= object[k].dst.y && object[k].status != 2)
                            object[j].flaghold = 1;
                        if(object[k].gimmick == 2 && object[j].dst.y+30 >= object[k].dst.y && object[j].dst.y+20 <= object[k].dst.y && object[k].status == 0){
                            if(object[j].gimmick == 1 && object[j].flaghold != -1){
                                object[k].status = 2;
                                switchblock[object[k].flaghold].flaghold = 1;
                                object[k].src.y = bit;

                                if(stageP == 2){
                                    if(k == 33){
                                        switchblock[10].flaghold = 1;
                                        switchblock[11].flaghold = 1;
                                    }
                                    if(k == 40){
                                        switchblock[25].flaghold = 1;
                                    }
                                    if(k == 41){
                                        switchblock[22].flaghold = 1;
                                        switchblock[26].flaghold = 1;
                                    }
                                }

                                white.x = object[k].dst.x;
                                white.y = object[k].dst.y;
                                SDL_BlitSurface(mapwindow, &white, objectwindow, &object[k].dst); // object貼り付け準備
                                SDL_BlitSurface(objectimage, &object[k].src, objectwindow, &object[k].dst); // object貼り付け
                                object[j].flaghold = -1;
                                object[j].movey = 2;
                                SDL_BlitSurface(objectimage, &object[j].src, objectwindow, &object[j].dst); // object貼り付け

                            }
                        }
                        if(object[j].gimmick == 1 && object[k].gimmick == 5 && object[j].dst.y+60 >= object[k].dst.y && object[j].dst.y+30 <= object[k].dst.y){
                            object[j].flaghold = 1;
                            if(object[j].dst.y+35 >= object[k].dst.y && object[j].status == 0){
                                object[object[k].flaghold].status = -2;
                                object[j].flaghold = -1;
                            }
                        }
                    }
                }
            }
        }

        //中間ポイントのとき
        if(object[j].gimmick == 0 && object[j].status == 2){
            playSE(4);
            if(object[j].flaghold == -1){

                SDL_BlitSurface(objectimage, &object[j].src, objectwindow, &object[j].dst); // object貼り付け
                object[j].flaghold = 0;
            }
            revivalflag.x = object[j].dst.x;
            revivalflag.y = object[j].dst.y-60;
            object[j].src.y = 60;
            SDL_BlitSurface(objectimage, &object[j].src, objectwindow, &revivalflag); // object貼り付け
            object[j].src.y = 0;
            object[j].status = 1;
        }

        //スイッチのとき
        if(object[j].gimmick == 2 && object[j].status != 2){
            if(object[j].src.y != object[j].status*bit){
                object[j].src.y = object[j].status*bit;
                white.x = object[j].dst.x;
                white.y = object[j].dst.y;
                SDL_BlitSurface(mapwindow, &white, objectwindow, &object[j].dst); // object貼り付け準備
                SDL_BlitSurface(objectimage, &object[j].src, objectwindow, &object[j].dst); // object貼り付け
            }
/*			for(k=0; k<=max_map_object; k++){
			if(object[k].gimmick == 2 && object[j].flaghold == object[k].flaghold && object[k].status == 1)
			switchblock[object[j].flaghold].status = 1;
			}*/
        }

        //フラグホールドの処理
        if(object[j].flaghold == 1){
            if(object[j].gimmick == 1 || object[j].gimmick == 3 || object[j].gimmick == 4){
                if( gMaps[(object[j].dst.x)/bit][object[j].dst.y/bit+1] == 0 &&
                    gMaps[(object[j].dst.x+59)/bit][object[j].dst.y/bit+1] == 0 &&
                    object[j].dst.y <= 20 * 60)
                    object[j].movey = 4;
                else
                    object[j].flaghold = 0;
            }
        }
        if(object[j].movex != 0 || object[j].movey != 0){
            white.x = object[j].dst.x;
            white.y = object[j].dst.y;
            SDL_BlitSurface(mapwindow, &white, objectwindow, &object[j].dst); // object貼り付け準備
            object[j].dst.x += object[j].movex;
            object[j].dst.y += object[j].movey;
            SDL_BlitSurface(objectimage, &object[j].src, objectwindow, &object[j].dst); // object貼り付け
            object[j].movex = 0;
            object[j].movey = 0;
//	SendObjectCommand(j, object[j].status, object[j].dst.x, object[j].dst.y,
//                          object[j].movex, object[j].movey); // オブジェクトのデータの送信
        }
    }


////再判定2
    for(j=0; j<=max_map_object; j++){
        //バネシーソーのとき
        if(object[j].gimmick == 5 && (object[j].status == -1 || object[j].status == -2)){
            if(P.x+gameRect.x >= object[j].dst.x - 45 && P.x+gameRect.x <= object[j].dst.x + 45 &&
               P.y+75 >= object[j].dst.y+20 && P.y+75 <= object[j].dst.y+50){
                if(object[j].status == -1 && jump_a == 0)
                    jump_a = 5*6;
                if(object[j].status == -2)
                    jump_a = 6*6;
            }
            if(object[j].status == -1)
                object[j].status = 0;
            if(object[j].status == -2)
                object[j].gimmick = -2;//
            object[object[j].flaghold].status = 1;

            SDL_BlitSurface(mapwindow, &white, objectwindow, &object[j].dst); // object貼り付け準備
            SDL_BlitSurface(mapwindow, &white, objectwindow, &object[object[j].flaghold].dst); // object貼り付け準備
            object[j].src.y = 0;
            object[object[j].flaghold].src.y = object[object[j].flaghold].status*bit;
            SDL_BlitSurface(objectimage, &object[j].src, objectwindow, &object[j].dst); // object貼り付け
            SDL_BlitSurface(objectimage, &object[object[j].flaghold].src, objectwindow, &object[object[j].flaghold].dst); // object貼り付け
            SDL_BlitSurface(objectimage, &object[50].src, objectwindow, &object[50].dst);

        }
    }

//スイッチブロックの当たり判定と描写
    for(j=0; j<=SUM_switchblock; j++){
        if(switchblock[j].src.y != switchblock[j].flaghold*60){
            switchblock[j].src.y = switchblock[j].flaghold*60;
            for(l=0; l<switchblock[j].status; l++){
                SDL_BlitSurface(mapwindow, &switchblock[j].src, objectwindow, &switchblock[j].dst); // switchblock再貼り付け
                SDL_BlitSurface(blockimage, &switchblock[j].src, objectwindow, &switchblock[j].dst); // switchblock再貼り付け
//				SDL_BlitSurface(blockimage, &switchblock[j].src, mapwindow, &switchblock[j].dst); // switchblock再貼り付け
                switchblock[j].dst.x += bit;
            }
            switchblock[j].dst.x -= l*60;
            for(k=0; k<switchblock[j].gimmick; k++){
                SDL_BlitSurface(mapwindow, &switchblock[j].src, objectwindow, &switchblock[j].dst); // switchblock再貼り付け
                SDL_BlitSurface(blockimage, &switchblock[j].src, objectwindow, &switchblock[j].dst); // switchblock再貼り付け
//			SDL_BlitSurface(blockimage, &switchblock[j].src, mapwindow, &switchblock[j].dst); // switchblock再貼り付け
                switchblock[j].dst.y += bit;
            }
            switchblock[j].dst.y -= k*60;
        }

        if((switchblock[j].src.x == 0 && switchblock[j].src.y == 60) || (switchblock[j].src.x == 60 && switchblock[j].src.y == 0)){
            if( (newposx+gameRect.x >= switchblock[j].dst.x-60+15 && newposx+gameRect.x <= switchblock[j].dst.x+switchblock[j].status*bit-60 + 40) &&
                (P.y >= switchblock[j].dst.y-70 && P.y <= switchblock[j].dst.y+switchblock[j].gimmick*bit-60 + 25) )
                hitx = 1;
            if(P.x+gameRect.x >= switchblock[j].dst.x-60+20 && P.x+gameRect.x <= switchblock[j].dst.x+switchblock[j].status*bit-60 + 40 &&
               newposy >= switchblock[j].dst.y-75 && newposy <= switchblock[j].dst.y+switchblock[j].gimmick*bit-60 + 43){
                if(switchblock[j].src.x == 0 && newposy >= switchblock[j].dst.y-60)
                    newposy = switchblock[j].dst.y+45;//上にヒット
//                if(switchblock[j].src.x == 60 && newposx >= switchblock[j].dst.x-45 && newposx <= switchblock[j].dst.x+45)
//                    newposx = switchblock[j].dst.x-50;//上にヒット

                if(newposy <= switchblock[j].dst.y)
                    hity = -1;//下にヒット
            }
        }
    }

//ハシゴの判定
    if(stepflag >= 1){
        for(j=0; j<=SUM_steps; j++){
            if( P.x+gameRect.x+10 >= steps[j].dst.x-10 && P.x+gameRect.x <= steps[j].dst.x + 30 &&
                P.y >= steps[j].dst.y + 40 && P.y+30 <= steps[j].dst.y + 25 + steps[j].status*bit && keyhold == 0){
                stepflag = 2;
            //    if(UD == 0)
            //        UD = 1;
                if(newposy < steps[j].dst.y + 40)
                    newposy = P.y;
                break;
            }
        }
        if(j == SUM_steps+1 || (UD == -1 && hity == -1)){
            UD = 0;
            stepflag = 0;
        }
    }

//カギの判定
if(stageP == 1 && object[29].status == mynum+1){
    white.x = object[29].dst.x;
    white.y = object[29].dst.y;
    SDL_BlitSurface(mapwindow, &white, objectwindow, &object[29].dst); // object貼り付け準備
    if(hitx == 0)
        object[29].dst.x = newposx+gameRect.x;
    if(hity == 0)
        object[29].dst.y = newposy-44;
    if(hity == -1)
        object[29].dst.y = P.y-44;
    SDL_BlitSurface(objectimage, &object[29].src, objectwindow, &object[29].dst); // object貼り付け
}
if(stageP == 2 && object[49].status == mynum+1){
    white.x = object[49].dst.x;
    white.y = object[49].dst.y;
    SDL_BlitSurface(mapwindow, &white, objectwindow, &object[49].dst); // object貼り付け準備
    if(hitx == 0)
        object[49].dst.x = newposx+gameRect.x;
    if(hity == 0)
        object[49].dst.y = newposy-44;
    if(hity == -1)
        object[49].dst.y = P.y-44;
    SDL_BlitSurface(objectimage, &object[49].src, objectwindow, &object[49].dst); // object貼り付け
}

if(keycommand == 1){
	if(stageP == 1 && (object[29].status == mynum+1 || object[30].status == 1) && P.x+gameRect.x >= 248*60+20 && P.x+gameRect.x <= 250*60-80 && P.y == 7*60-15){
		object[29].src.x = -60;
		object[30].src.x += 120;
		object[30].status = 1;
        SendObjectCommand(30, object[30].status, object[30].dst.x, object[30].dst.y, object[30].movex, object[30].movey); // オブジェクトのデータの送信
		SDL_BlitSurface(objectimage, &object[30].src, objectwindow, &object[30].dst); // object貼り付け
		clearpoint = 1;
	}
	if(stageP == 2 && (object[49].status == mynum+1 || object[56].status == 1) && P.x+gameRect.x >= 266*60+20 && P.x+gameRect.x <= 268*60-80 && P.y == 3*60-15){
		object[49].src.x = -60;
		object[56].src.x += 120;
		object[56].status = 1;
        SendObjectCommand(56, object[56].status, object[56].dst.x, object[56].dst.y, object[56].movex, object[56].movey); // オブジェクトのデータの送信
		SDL_BlitSurface(objectimage, &object[56].src, objectwindow, &object[56].dst); // object貼り付け
		clearpoint = 1;
	}
keycommand = 0;
}
if(object[30].status == 1 && object[30].src.x == 6*bit){
object[30].src.x = 8*bit;
SDL_BlitSurface(objectimage, &object[30].src, objectwindow, &object[30].dst); // object貼り付け
SDL_BlitSurface(objectimage, &object[30].src, mapwindow, &object[30].dst); // object貼り付け
}
//if(object[56].status == 1 && object[56].src.x == 6*bit){
//object[56].src.x = 8*bit;
//SDL_BlitSurface(objectimage, &object[56].src, objectwindow, &object[56].dst); // object貼り付け
//SDL_BlitSurface(objectimage, &object[56].src, mapwindow, &object[56].dst); // object貼り付け
//}


    for(i=0; i <= max_map_object; i++){
        if(object[i].oldsrc.y != object[i].src.y || object[i].olddst.x != object[i].dst.x || object[i].olddst.y != object[i].dst.y){
            SendObjectCommand(i, object[i].status, object[i].dst.x, object[i].dst.y,
                              object[i].movex, object[i].movey); // オブジェクトのデータの送信
        }
    }



if(clearpoint == 1){
hitx = 1;
P.y = 25*bit;
newposy = P.y;
object[29].dst.y = 25*bit;
object[49].dst.y = 25*bit;
if( (gClientNum == 1 && player[0].pos.y >= 20*bit) ||
	(gClientNum == 2 && player[0].pos.y >= 20*bit && player[1].pos.y >= 20*bit) ||
	(gClientNum == 3 && player[0].pos.y >= 20*bit && player[1].pos.y >= 20*bit && player[2].pos.y >= 20*bit) ||
	(gClientNum == 4 && player[0].pos.y >= 20*bit && player[1].pos.y >= 20*bit && player[2].pos.y >= 20*bit && player[3].pos.y >= 20*bit))
StageClear();
}

}

void scroll(void){

    int x;

    x = (player[0].pos.x + player[1].pos.x + player[2].pos.x + player[3].pos.x) / gClientNum;

    shiftdef = x - (WIND_Width * bit/2-30);

	for(j = 0; j < gClientNum; j++){
	if(player[j].pos.x - shiftdef <= 0)
	shiftdef = 0;
	if(player[j].pos.x - shiftdef >= 24*60)
	shiftdef = 0;
	}
	if(loopend == 1 && gameRect.x >= 100*60 && gameRect.x + shiftdef <= 150*60)
	shiftdef = 0;

//強制スクロール
if(stageP == 2 && gameRect.x >= 90*60 && gameRect.x <= 150*60){
shiftdef = 0;
gameRect.x += 3;
P.x -= 3;
if(P.x < 0){
if( gMaps[(newposx+gameRect.x+45)/bit][(P.y+15)/bit] == 0 ||
    gMaps[(newposx+gameRect.x+45)/bit][(P.y+10)/bit+1] == 0 )
P.x = 0;
if(P.x < -75){
SendGameoverCommand();
GameOver(mynum);
}
}
	//ループ発生
	if(gameRect.x >= 130*60 && (object[21].status != 0 || object[22].status != 0)){
	gameRect.x = 105*60;
	if(object[21].status == 1)
	SendObjectCommand(21, 2, object[21].dst.x, object[21].dst.y,
                          object[21].movex, object[21].movey); // オブジェクトのデータの送信
	if(object[22].status == 1)
	SendObjectCommand(22, 2, object[22].dst.x, object[22].dst.y,
                          object[22].movex, object[22].movey); // オブジェクトのデータの送信
	object[21].status = 0;
	object[22].status = 0;
	}

if(gameRect.x >= 130*60 && gameRect.x <= 131*60){
	object[21].status = 0;
	object[22].status = 0;
}

if(gameRect.x >= 149*60 && loopend != 1)
loopend = 1;

}

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
        if(hitx != 0 && jumpflag == 0 && UD == 0 && keyhold == 0)
            PA.x += bit * 2;
    }
    //   if(jumpflag == 1 || UD == -1)
    //       PA.x = bit; //アニメーション固定

    if(hitx != hithold){
        PA.x = 0;
        hithold = hitx; //ヒットホールドの更新
    }
    SendMoveCommand(P.x + gameRect.x, P.y + gameRect.y, PA.x, PA.y);

    SDL_BlitSurface(mapwindow, &gameRect, window, NULL); // マップ貼り付け

    SDL_BlitSurface(objectwindow, &gameRect, window, NULL); // object貼り付け

}



// 残り時間などの表示
void DisplayStatus(void){
    char title[ 160 ];
    sprintf(title,"Time : %d ", time);

    /* ウインドウのタイトルをセット */
    SDL_WM_SetCaption(title,NULL);
}


//タイトル
void title(void){

SDL_Rect info = {155, 0};
SDL_Rect infoD = {0, 0, 660, 60};

    P.x = 150;
    P.y = 400;
    PA.x = 0;
    PA.y = 0;

playBGM(0);///OPテーマを流す

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
                    if(P.y == 500)
                        stageP = 2;
                    titlep = 0;
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
			//ボタン
			case SDL_JOYBUTTONDOWN:
					if(event.type!=SDL_KEYDOWN){
					if(P.y == 500)
						stageP = 2;
					titlep = 0;
					if(P.y == 600)
					EXIT();
					break;
					}
				break;
            }
        }

//SDL_BlitSurface(titleCG, NULL, window, NULL); // マップ貼り付け

	if(exit_p != 1){
        /* メッセージ表示 */
        SDL_Rect srcRect = {0,0,0,0};
        SDL_Rect dstRect = {270,420};

        for(fm=0; fm <= 2; fm++){

            srcRect.w = gMessages[fm]->w;
            srcRect.h = gMessages[fm]->h;

            SDL_BlitSurface(gMessages[fm], &srcRect, SDL_GetVideoSurface(), &dstRect);
            dstRect.y += 100;
        }

info.y = P.y - 40;
fm = mynum + 5;
SDL_BlitSurface(gMessages[fm], &infoD, SDL_GetVideoSurface(), &info);

        if(mynum == 0) {
            SDL_BlitSurface(usa2, &PA, window, &P); //キャラ貼り付け
        }

        if(mynum == 1) {//neko
            SDL_BlitSurface(neko, &PA, window, &P); //キャラ貼り付け
        }

        if(mynum == 2) {
            SDL_BlitSurface(inu, &PA, window, &P); //キャラ貼り付け
        }

        if(mynum == 3) {
            SDL_BlitSurface(panda, &PA, window, &P); //キャラ貼り付け
        }

//タイトルロゴ
SDL_Rect titleinfo = {270,170};
SDL_BlitSurface(gMessages[10], &infoD, SDL_GetVideoSurface(), &titleinfo);
titleinfo.x = 220;
titleinfo.y = 200;
SDL_BlitSurface(gMessages[9], &infoD, SDL_GetVideoSurface(), &titleinfo);

        SDL_Flip(window);// 画面に図形を表示（反映）
	}
    }
    P.y = 640;

}

/************プレイヤー表示関数**********************/
void DrawChara(void)
{


    for(j = 0; j < gClientNum; j++){
        player[j].anim.w = 60;
        if(j == 0) {
            SDL_BlitSurface(usa2, &player[j].anim, window, &player[j].pos); //キャラ貼り付け
        }

        if(j == 1) {//neko
            SDL_BlitSurface(neko, &player[j].anim, window, &player[j].pos); //キャラ貼り付け
        }

        if(j == 2) {
            SDL_BlitSurface(inu, &player[j].anim, window, &player[j].pos); //キャラ貼り付け
        }

        if(j == 3) {
            SDL_BlitSurface(panda, &player[j].anim, window, &player[j].pos); //キャラ貼り付け
        }
    }

}

void GameOver(int ClientNum){


    //PA.x = 3 * bit;
    //PA.y = 3 * 75;

    int endFlag = 1;

    SDL_BlitSurface(objectimage, &object[i].src, objectwindow, &object[i].dst); // object貼り付け
    SDL_BlitSurface(mapwindow, &gameRect, window, NULL); // マップ貼り付け
    SDL_BlitSurface(objectwindow, &gameRect, window, NULL); // マップ貼り付け

    DrawChara();

    white.x = player[ClientNum].pos.x;
    white.y = player[ClientNum].pos.y;
    white.h = 75;

    SDL_BlitSurface(mapwindow, &white, window, &player[ClientNum].pos); // 死亡キャラ貼り付け準備

    white.h = 60;

    PA.x = 3 * bit;
    PA.y = 3 * 75;

    SDL_BlitSurface(usa2, &PA, window, &player[ClientNum].pos);
//    SDL_Flip(window);// 画面に図形を表示（反映）
    SDL_Delay(700);

    for(j=0; j<=max_map_object; j++){
	white.x = object[j].dst.x;
	white.y = object[j].dst.y;
	SDL_BlitSurface(mapwindow, &white, objectwindow, &object[j].dst); // object貼り付け準備
    }

    InitStatus();

int revival = 0;

    for(j=0; j<=max_map_object; j++){
	if(object[j].gimmick == 0 && object[j].status == 1){
            gameRect.x = object[j].dst.x-420+(object[j].flaghold * bit);
            P.y = object[j].dst.y+60;
            newposy = P.y;
            object[j].src.y = 60;
            SDL_BlitSurface(objectimage, &object[j].src, objectwindow, &revivalflag); // flag貼り付け
            object[j].src.y = 0;
			revival = 1;
	}
    }

if(revival == 0){
gameRect.x = 0;
}

    if(keyhold == 1)
        keyhold = 0;

    //SendMoveCommand(player[mynum].pos.x, player[mynum].pos.y, player[mynum].anim.x, player[mynum].anim.y);
    SendMoveCommand(P.x, P.y, player[mynum].anim.x, player[mynum].anim.y);
    SendStartCommand();
    start_flag = 0;
    while(start_flag == 0){
        SendRecvManager();
    }
    scroll();
}

void StageClear(void){
/*
  while(1){
  SDL_Delay(1400);
  if()
  break;
  }
*/

    SDL_BlitSurface(objectimage, &object[i].src, objectwindow, &object[i].dst); // object貼り付け
    SDL_BlitSurface(mapwindow, &gameRect, window, NULL); // マップ貼り付け
    SDL_BlitSurface(objectwindow, &gameRect, window, NULL); // マップ貼り付け
    SDL_Flip(window);// 画面に図形を表示（反映）
    SDL_Delay(1400);

    for(j=0; j<=max_map_object; j++){
	white.x = object[j].dst.x;
	white.y = object[j].dst.y;
	SDL_BlitSurface(mapwindow, &white, objectwindow, &object[j].dst); // object貼り付け準備
	if(object[j].gimmick == 0)
            object[j].status = 0;
    }


    keyhold = 0;
	clearpoint = 0;
    stageP++;
if(stageP == 2)
playBGM(2);

    if(stageP == 3) {
	//SendGameclearCommand();
	GameClear();
    }

    InitStatus();

}

void GameClear(void){
    Mix_FadeOutMusic(2000);
    Mix_HaltMusic();
    playSE(9);
/*    SendStartCommand();
    start_flag = 0;
    while(start_flag == 0){
        SendRecvManager();
    }
*/

    SDL_BlitSurface(gameclear, NULL, window, NULL); // マップ貼り付け
    SDL_Flip(window);// 画面に図形を表示（反映）
    SDL_Delay(2000);

time = 0;
stageP = 1;
InitStatus();
titlep = 1;

//    exit_p = 1;
}












