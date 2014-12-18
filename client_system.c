#include "client_func.h"

void EXIT();

int j, k, l;
SDL_Rect P_START;
int stageP = 1;
int mapread = 1;
int stepUD = 0;

int mynum;
int gClientNum;

static void PlayerHitJudge(void);

int oldx, oldy; // PlayerHitJudgeで使用.プレイヤーの古い座標を格納

int max_map_object = 0;
int bottun2 = 0;
wiimote_t wiimote;


//ステータス初期化
void InitStatus(void){ // キャラのステータスの初期化
    int i;
    P.x = P_START.x;
    P.y = P_START.y; //12*bit;
    newposx = P.x;
    newposy = P.y;
    gameRect.x = 0;
    PA.y = 0;
    jumpflag = 0;
    jump_LR = 0;
    jump_a = 0;
    hitx = 0;
    hity = 0;
    timekey = 0;
    hithold = 0;
    shiftdef = 0;
    gimmickflag = 0;
    stepflag = 0;
    LR = 0;
    UD = 0;

    for(i = 0; i < gClientNum; i++){
        player[i].anim.x = 0;//PA.x;
        player[i].anim.y = 0;//PA.y;
        player[i].anim.w = 60;//PA.w;
        player[i].anim.h = 75;//PA.h;
    }

//ステージ２移行時
    if(stageP != mapread)
        Mapshift();

    if(stageP == 1)
	objectinit1();
    if(stageP == 2)
	objectinit2();

    for(i = 0; i < gClientNum; i++){
        player[i].pos.w = 60;
        player[i].pos.h = 75;
    }

    exit_p = 0;
    titlep = 1;
    titlep2 = 0;

}


void objectinit1(void){


    //スイッチブロック
    switchblock[0].gimmick = 1;//縦幅
    switchblock[0].status = 10;//横幅
    switchblock[0].dst.x = 127;
    switchblock[0].dst.y = 12;

    switchblock[1].gimmick = 1;//縦幅
    switchblock[1].status = 3;//横幅
    switchblock[1].dst.x = 192;
    switchblock[1].dst.y = 10;

    switchblock[2].gimmick = 1;//縦幅
    switchblock[2].status = 3;//横幅
    switchblock[2].dst.x = 193;
    switchblock[2].dst.y = 8;

    switchblock[3].gimmick = 1;//縦幅
    switchblock[3].status = 3;//横幅
    switchblock[3].dst.x = 192;
    switchblock[3].dst.y = 6;

    switchblock[4].gimmick = 1;//縦幅
    switchblock[4].status = 3;//横幅
    switchblock[4].dst.x = 193;
    switchblock[4].dst.y = 4;

    switchblock[5].gimmick = 1;//縦幅
    switchblock[5].status = 2;//横幅
    switchblock[5].dst.x = 204;
    switchblock[5].dst.y = 2;

    switchblock[6].gimmick = 1;//縦幅
    switchblock[6].status = 3;//横幅
    switchblock[6].dst.x = 216;
    switchblock[6].dst.y = 6;

    switchblock[7].gimmick = 1;//縦幅
    switchblock[7].status = 3;//横幅
    switchblock[7].dst.x = 221;
    switchblock[7].dst.y = 12;

    switchblock[8].gimmick = 1;//縦幅
    switchblock[8].status = 3;//横幅
    switchblock[8].dst.x = 226;
    switchblock[8].dst.y = 2;

    switchblock[9].gimmick = 1;//縦幅
    switchblock[9].status = 3;//横幅
    switchblock[9].dst.x = 235;
    switchblock[9].dst.y = 12;

    switchblock[10].gimmick = 1;//縦幅
    switchblock[10].status = 1;//横幅
    switchblock[10].dst.x = 263;
    switchblock[10].dst.y = 12;



    for(j=0; j<SUM_switchblock; j++){
	switchblock[j].flaghold = 0;
	switchblock[j].src.x = 0;
	switchblock[j].src.y = 0;
	switchblock[j].src.w = 60;
	switchblock[j].src.h = 60;
	switchblock[j].dst.x *= bit;
	switchblock[j].dst.y *= bit;
        for(k=0; k<switchblock[j].gimmick; k++){
            for(l=0; l<switchblock[j].status; l++){
                SDL_BlitSurface(blockimage, &switchblock[j].src, mapwindow, &switchblock[j].dst); // switchblock貼り付け
                switchblock[j].dst.x += bit;
            }
            switchblock[j].dst.y += bit;
        }
	switchblock[j].dst.x -= l*60;
	switchblock[j].dst.y -= k*60;
    }

    //ハシゴ
    steps[0].status = 9;//長さ
    steps[0].dst.x = 120;
    steps[0].dst.y = 3;

    steps[1].status = 9;//長さ
    steps[1].dst.x = 143;
    steps[1].dst.y = 3;

    steps[2].status = 4;//長さ
    steps[2].dst.x = 220;
    steps[2].dst.y = 2;

    steps[3].status = 5;//長さ
    steps[3].dst.x = 252;
    steps[3].dst.y = 7;

    steps[4].status = 6;//長さ
    steps[4].dst.x = 246;
    steps[4].dst.y = 2;


    for(j=0; j<SUM_steps; j++){
	steps[j].src.x = 60;
	steps[j].src.y = 60;
	steps[j].src.w = 60;
	steps[j].src.h = 60;
	steps[j].dst.x *= bit;
	steps[j].dst.y *= bit;
        for(k=0; k<steps[j].status; k++){
            SDL_BlitSurface(objectimage, &steps[j].src, mapwindow, &steps[j].dst);
            steps[j].dst.y += bit;
        }
        steps[j].dst.y -= (k+1)*60;
    }

    SDL_BlitSurface(mapwindow, NULL, objectwindow, NULL); // マップ貼り付け

    object[0].gimmick = 1; //岩
    object[0].dst.x = 38;
    object[0].dst.y = 12;

    object[1].gimmick = 1;
    object[1].dst.x = 71;
    object[1].dst.y = 5;

    object[2].gimmick = 2; //スイッチ
    object[2].flaghold = 0;
    object[2].dst.x = 122;
    object[2].dst.y = 3;

    object[3].gimmick = 2;
    object[3].flaghold = 0;
    object[3].dst.x = 141;
    object[3].dst.y = 3;

    object[4].gimmick = 0; //中間ポイント
    object[4].dst.x = 85;
    object[4].dst.y = 10;

    object[5].gimmick = 3; //バネ
    object[5].status = 5;
    object[5].dst.x = 155;
    object[5].dst.y = 9;

    object[6].gimmick = 3; //バネ
    object[6].status = 5;
    object[6].dst.x = 165;
    object[6].dst.y = 12;

    object[7].gimmick = 3; //バネ
    object[7].status = 5;
    object[7].dst.x = 170;
    object[7].dst.y = 12;

    object[8].gimmick = 3; //バネ
    object[8].status = 5;
    object[8].dst.x = 175;
    object[8].dst.y = 12;

    object[9].gimmick = 3; //バネ
    object[9].status = 5;
    object[9].dst.x = 180;
    object[9].dst.y = 12;

    object[10].gimmick = 0;
    object[10].flaghold = -1;//不可視
    object[10].dst.x = 165;
    object[10].dst.y = 4;

    object[11].gimmick = 2; //スイッチ
    object[11].flaghold = 1;
    object[11].dst.x = 197;
    object[11].dst.y = 11;

    object[12].gimmick = 2; //スイッチ
    object[12].flaghold = 2;
    object[12].dst.x = 190;
    object[12].dst.y = 8;

    object[13].gimmick = 2; //スイッチ
    object[13].flaghold = 3;
    object[13].dst.x = 197;
    object[13].dst.y = 6;

    object[14].gimmick = 2; //スイッチ
    object[14].flaghold = 4;
    object[14].dst.x = 190;
    object[14].dst.y = 4;

    object[15].gimmick = 2; //スイッチ
    object[15].flaghold = 4;
    object[15].dst.x = 197;
    object[15].dst.y = 2;

    object[16].gimmick = 1;
    object[16].dst.x = 202;
    object[16].dst.y = 1;

    object[17].gimmick = 2; //スイッチ
    object[17].flaghold = 5;
    object[17].dst.x = 206;
    object[17].dst.y = 11;

    object[18].gimmick = 1;
    object[18].dst.x = 208;
    object[18].dst.y = 1;

    object[19].gimmick = 3; //バネ
    object[19].status = 5;
    object[19].dst.x = 212;
    object[19].dst.y = 1;

    object[20].gimmick = 2; //スイッチ
    object[20].flaghold = 7;
    object[20].dst.x = 222;
    object[20].dst.y = 5;

    object[21].gimmick = 3; //バネ
    object[21].status = 6;
    object[21].dst.x = 227;
    object[21].dst.y = 11;

    object[22].gimmick = 1;
    object[22].dst.x = 229;
    object[22].dst.y = 1;

    object[23].gimmick = 2; //スイッチ
    object[23].flaghold = 6;
    object[23].dst.x = 217;
    object[23].dst.y = 11;

    object[24].gimmick = 2; //スイッチ
    object[24].flaghold = 8;
    object[24].dst.x = 233;
    object[24].dst.y = 5;

    object[25].gimmick = 2; //スイッチ
    object[25].flaghold = 9;
    object[25].dst.x = 225;
    object[25].dst.y = 12;

    object[26].gimmick = 0;
    object[26].dst.x = 220;
    object[26].dst.y = 1;

    object[27].gimmick = 2; //スイッチ
    object[27].flaghold = 10;
    object[27].dst.x = 268;
    object[27].dst.y = 11;

    object[28].gimmick = 3; //バネ
    object[28].status = 4;
    object[28].dst.x = 266;
    object[28].dst.y = 3;

    object[29].gimmick = 4; //カギ
    object[29].status = 0;
    object[29].dst.x = 255;
    object[29].dst.y = 11;

    object[30].gimmick = 6; //扉
    object[30].dst.x = 248;
    object[30].dst.y = 6;

    for(j=0; j<SUM_object; j++){
        if(object[j].dst.x == NULL)
            break;
	object[j].movex = 0;
	object[j].movey = 0;
	if(object[j].gimmick == 1 || object[j].gimmick == 3)
            object[j].flaghold = 0;
	if(object[j].gimmick == 2)
            object[j].status = 0;
	object[j].src.x = object[j].gimmick*bit;
	object[j].src.y = 0;
	object[j].src.w = 60;
	object[j].src.h = 60;
	object[j].dst.x *= bit;
	object[j].dst.y *= bit;
	if(object[j].gimmick == 6){
            object[j].src.w = 120;
            object[j].src.h = 120;
            SDL_BlitSurface(objectimage, &object[j].src, mapwindow, &object[j].dst); // objectをmapに貼り付け
        }
	if(object[j].flaghold != -1 || object[j].status == 1)
            SDL_BlitSurface(objectimage, &object[j].src, objectwindow, &object[j].dst); // object貼り付け
    }

    max_map_object = j;

}

void objectinit2(void){


    //スイッチブロック
    switchblock[0].gimmick = 4;//縦幅
    switchblock[0].status = 1;//横幅
    switchblock[0].dst.x = 69;
    switchblock[0].dst.y = 1;

    switchblock[1].gimmick = 2;//縦幅
    switchblock[1].status = 1;//横幅
    switchblock[1].dst.x = 68;
    switchblock[1].dst.y = 6;

    switchblock[2].gimmick = 1;//縦幅
    switchblock[2].status = 5;//横幅
    switchblock[2].dst.x = 64;
    switchblock[2].dst.y = 5;

    switchblock[3].gimmick = 2;//縦幅
    switchblock[3].status = 1;//横幅
    switchblock[3].dst.x = 63;
    switchblock[3].dst.y = 6;

    switchblock[4].gimmick = 1;//縦幅
    switchblock[4].status = 1;//横幅
    switchblock[4].dst.x = 67;
    switchblock[4].dst.y = 7;

    switchblock[5].gimmick = 5;//縦幅
    switchblock[5].status = 1;//横幅
    switchblock[5].dst.x = 166;
    switchblock[5].dst.y = 7;

    switchblock[6].gimmick = 10;//縦幅
    switchblock[6].status = 1;//横幅
    switchblock[6].dst.x = 180;
    switchblock[6].dst.y = 2;

    switchblock[7].gimmick = 1;//縦幅
    switchblock[7].status = 1;//横幅
    switchblock[7].dst.x = 188;
    switchblock[7].dst.y = 8;

    switchblock[8].gimmick = 2;//縦幅
    switchblock[8].status = 1;//横幅
    switchblock[8].dst.x = 199;
    switchblock[8].dst.y = 10;

    switchblock[9].gimmick = 3;//縦幅
    switchblock[9].status = 1;//横幅
    switchblock[9].dst.x = 192;
    switchblock[9].dst.y = 6;

    switchblock[10].gimmick = 1;//縦幅
    switchblock[10].status = 3;//横幅
    switchblock[10].dst.x = 194;
    switchblock[10].dst.y = 9;

    switchblock[11].gimmick = 1;//縦幅
    switchblock[11].status = 1;//横幅
    switchblock[11].dst.x = 191;
    switchblock[11].dst.y = 5;

    switchblock[12].gimmick = 1;//縦幅
    switchblock[12].status = 5;//横幅
    switchblock[12].dst.x = 253;
    switchblock[12].dst.y = 2;


///

    switchblock[13].gimmick = 2;//縦幅
    switchblock[13].status = 1;//横幅
    switchblock[13].dst.x = 244;
    switchblock[13].dst.y = 10;

//1
    switchblock[14].gimmick = 1;//縦幅
    switchblock[14].status = 3;//横幅
    switchblock[14].dst.x = 225;
    switchblock[14].dst.y = 1;

    switchblock[15].gimmick = 1;//縦幅
    switchblock[15].status = 3;//横幅
    switchblock[15].dst.x = 220;
    switchblock[15].dst.y = 1;

    switchblock[16].gimmick = 1;//縦幅
    switchblock[16].status = 3;//横幅
    switchblock[16].dst.x = 230;
    switchblock[16].dst.y = 1;

    switchblock[17].gimmick = 1;//縦幅
    switchblock[17].status = 3;//横幅
    switchblock[17].dst.x = 235;
    switchblock[17].dst.y = 1;

    switchblock[18].gimmick = 1;//縦幅
    switchblock[18].status = 3;//横幅
    switchblock[18].dst.x = 240;
    switchblock[18].dst.y = 1;

//2
    switchblock[19].gimmick = 1;//縦幅
    switchblock[19].status = 3;//横幅
    switchblock[19].dst.x = 220;
    switchblock[19].dst.y = 4;

    switchblock[20].gimmick = 1;//縦幅
    switchblock[20].status = 3;//横幅
    switchblock[20].dst.x = 225;
    switchblock[20].dst.y = 7;

    switchblock[21].gimmick = 1;//縦幅
    switchblock[21].status = 3;//横幅
    switchblock[21].dst.x = 230;
    switchblock[21].dst.y = 3;

    switchblock[22].gimmick = 1;//縦幅
    switchblock[22].status = 3;//横幅
    switchblock[22].dst.x = 235;
    switchblock[22].dst.y = 6;

    switchblock[23].gimmick = 1;//縦幅
    switchblock[23].status = 3;//横幅
    switchblock[23].dst.x = 240;
    switchblock[23].dst.y = 5;

//3
    switchblock[24].gimmick = 1;//縦幅
    switchblock[24].status = 3;//横幅
    switchblock[24].dst.x = 220;
    switchblock[24].dst.y = 7;

    switchblock[25].gimmick = 1;//縦幅
    switchblock[25].status = 3;//横幅
    switchblock[25].dst.x = 225;
    switchblock[25].dst.y = 5;

    switchblock[26].gimmick = 1;//縦幅
    switchblock[26].status = 3;//横幅
    switchblock[26].dst.x = 230;
    switchblock[26].dst.y = 5;

    switchblock[27].gimmick = 1;//縦幅
    switchblock[27].status = 3;//横幅
    switchblock[27].dst.x = 235;
    switchblock[27].dst.y = 4;

//4
    switchblock[28].gimmick = 1;//縦幅
    switchblock[28].status = 3;//横幅
    switchblock[28].dst.x = 225;
    switchblock[28].dst.y = 3;

    switchblock[29].gimmick = 1;//縦幅
    switchblock[29].status = 3;//横幅
    switchblock[29].dst.x = 230;
    switchblock[29].dst.y = 7;

//	switchblock[30].gimmick = 1;//縦幅
//	switchblock[30].status = 3;//横幅
//	switchblock[30].dst.x = 240;
//	switchblock[30].dst.y = 5;


    for(j=0; j<SUM_switchblock; j++){
        if(switchblock[j].gimmick == 0)
            break;
	switchblock[j].flaghold = 0;
	switchblock[j].src.x = 0;
	switchblock[j].src.y = 0;
	switchblock[j].src.w = 60;
	switchblock[j].src.h = 60;
	switchblock[j].dst.x *= bit;
	switchblock[j].dst.y *= bit;

        if((j >= 5 && j <= 9) || (j >= 13 && j <= 18) || j == 24 || (j >= 26 && j <= 28))
            switchblock[j].src.x = 60;


	if(j != 12){
            for(l=0; l<switchblock[j].status; l++){
                SDL_BlitSurface(blockimage, &switchblock[j].src, mapwindow, &switchblock[j].dst); // switchblock貼り付け
                switchblock[j].dst.x += bit;
            }
            switchblock[j].dst.x -= l*60;
            for(k=0; k<switchblock[j].gimmick; k++){
                SDL_BlitSurface(blockimage, &switchblock[j].src, mapwindow, &switchblock[j].dst); // switchblock貼り付け
                switchblock[j].dst.y += bit;
            }
            switchblock[j].dst.y -= k*60;
	}
    }

    //ハシゴ
    steps[0].status = 6;//長さ
    steps[0].dst.x = 17;
    steps[0].dst.y = 5;

    steps[1].status = 8;//長さ
    steps[1].dst.x = 186;
    steps[1].dst.y = 4;

    steps[2].status = 4;//長さ
    steps[2].dst.x = 193;
    steps[2].dst.y = 8;

    steps[3].status = 5;//長さ
    steps[3].dst.x = 198;
    steps[3].dst.y = 4;

    steps[4].status = 5;//長さ
    steps[4].dst.x = 246;
    steps[4].dst.y = 8;

    steps[5].status = 4;//長さ
    steps[5].dst.x = 252;
    steps[5].dst.y = 9;

    steps[6].status = 10;//長さ
    steps[6].dst.x = 269;
    steps[6].dst.y = 3;


    for(j=0; j<SUM_steps; j++){
	steps[j].src.x = 60;
	steps[j].src.y = 60;
	steps[j].src.w = 60;
	steps[j].src.h = 60;
	steps[j].dst.x *= bit;
	steps[j].dst.y *= bit;
        for(k=0; k<steps[j].status; k++){
            SDL_BlitSurface(objectimage, &steps[j].src, mapwindow, &steps[j].dst);
            steps[j].dst.y += bit;
        }
        steps[j].dst.y -= (k+1)*60;
    }

    SDL_BlitSurface(mapwindow, NULL, objectwindow, NULL); // マップ貼り付け

    object[0].gimmick = 1; //岩
    object[0].dst.x = 18;
    object[0].dst.y = 5;

    object[1].gimmick = 1;
    object[1].dst.x = 32;
    object[1].dst.y = 7;

    object[2].gimmick = 1;
    object[2].dst.x = 35;
    object[2].dst.y = 7;

    object[3].gimmick = 1;
    object[3].dst.x = 38;
    object[3].dst.y = 7;

    object[4].gimmick = 1;
    object[4].dst.x = 41;
    object[4].dst.y = 7;

    object[5].gimmick = 1;
    object[5].dst.x = 44;
    object[5].dst.y = 7;

    object[6].gimmick = 2;
    object[6].flaghold = 0;
    object[6].dst.x = 33;
    object[6].dst.y = 12;

    object[7].gimmick = 2;
    object[7].flaghold = 1;
    object[7].dst.x = 36;
    object[7].dst.y = 12;

    object[8].gimmick = 2;
    object[8].flaghold = 2;
    object[8].dst.x = 39;
    object[8].dst.y = 12;

    object[9].gimmick = 2;
    object[9].flaghold = 3;
    object[9].dst.x = 42;
    object[9].dst.y = 12;

    object[10].gimmick = 2;
    object[10].flaghold = 4;
    object[10].dst.x = 45;
    object[10].dst.y = 12;

    object[11].gimmick = 3;
    object[11].status = 6;
    object[11].dst.x = 74;
    object[11].dst.y = 12;

    object[12].gimmick = 0;
//	object[12].flaghold = -1;
    object[12].dst.x = 74;
    object[12].dst.y = 2;


    object[13].gimmick = 3;
    object[13].status = 5;
    object[13].dst.x = 77;
    object[13].dst.y = 12;

    object[14].gimmick = 3;
    object[14].status = 3;
    object[14].dst.x = 79;
    object[14].dst.y = 12;

    object[15].gimmick = 3;
    object[15].status = 9;
    object[15].dst.x = 81;
    object[15].dst.y = 12;

    object[16].gimmick = 3;
    object[16].status = 4;
    object[16].dst.x = 83;
    object[16].dst.y = 12;

    object[17].gimmick = 3;
    object[17].status = 7;
    object[17].dst.x = 85;
    object[17].dst.y = 12;

    object[18].gimmick = 3;
    object[18].status = 6;
    object[18].dst.x = 87;
    object[18].dst.y = 12;

    object[19].gimmick = 3;
    object[19].status = 20;
    object[19].dst.x = 89;
    object[19].dst.y = 12;

    object[20].gimmick = 3;
    object[20].status = 6;
    object[20].dst.x = 93;
    object[20].dst.y = 12;

    object[21].gimmick = -1; //ループギミック
    object[21].dst.x = 130;
    object[21].dst.y = 7;

    object[22].gimmick = -1; //ループギミック
    object[22].dst.x = 130;
    object[22].dst.y = 3;

    object[23].gimmick = 3;
    object[23].status = 6;
    object[23].dst.x = 118;
    object[23].dst.y = 12;

    object[24].gimmick = 3;
    object[24].status = 6;
    object[24].dst.x = 143;
    object[24].dst.y = 12;

    object[25].gimmick = 0;
    object[25].dst.x = 158;
    object[25].dst.y = 8;

    object[26].gimmick = 2; //スイッチ
    object[26].flaghold = 5;
    object[26].dst.x = 162;
    object[26].dst.y = 11;

    object[27].gimmick = 2; //スイッチ
    object[27].flaghold = 5;
    object[27].dst.x = 170;
    object[27].dst.y = 11;

    object[28].gimmick = 2; //スイッチ
    object[28].flaghold = 6;
    object[28].dst.x = 177;
    object[28].dst.y = 11;

    object[29].gimmick = 2; //スイッチ
    object[29].flaghold = 6;
    object[29].dst.x = 183;
    object[29].dst.y = 11;

    object[30].gimmick = 2; //スイッチ
    object[30].flaghold = 7;
    object[30].dst.x = 177;
    object[30].dst.y = 5;

    object[31].gimmick = 2; //スイッチ
    object[31].flaghold = 7;
    object[31].dst.x = 188;
    object[31].dst.y = 11;


    object[32].gimmick = 2; //スイッチ
    object[32].flaghold = 9;
    object[32].dst.x = 197;
    object[32].dst.y = 11;

    object[33].gimmick = 2; //スイッチ
    object[33].flaghold = 8;
    object[33].dst.x = 191;
    object[33].dst.y = 8;

    object[34].gimmick = 2; //スイッチ
    object[34].flaghold = 0; //例外判定
    object[34].dst.x = 190;
    object[34].dst.y = 4;

    object[35].gimmick = 1;
    object[35].dst.x = 192;
    object[35].dst.y = 4;

    object[36].gimmick = 0;
    object[36].flaghold = -1;
    object[36].dst.x = 217;
    object[36].dst.y = 6;

//
    object[37].gimmick = 2; //スイッチ
    object[37].flaghold = 15;
    object[37].dst.x = 228;
    object[37].dst.y = 12;

    object[38].gimmick = 2; //スイッチ
    object[38].flaghold = 17;
    object[38].dst.x = 231;
    object[38].dst.y = 12;

    object[39].gimmick = 2; //スイッチ
    object[39].flaghold = 14;
    object[39].dst.x = 234;
    object[39].dst.y = 12;

    object[40].gimmick = 2; //スイッチ
    object[40].flaghold = 16;
    object[40].dst.x = 221;
    object[40].dst.y = 8;

    object[41].gimmick = 2; //スイッチ
    object[41].flaghold = 18;
    object[41].dst.x = 226;
    object[41].dst.y = 8;

    object[42].gimmick = 2; //スイッチ
    object[42].flaghold = 13;
    object[42].dst.x = 231;
    object[42].dst.y = 8;

    object[43].gimmick = 2; //スイッチ
    object[43].flaghold = 28;
    object[43].dst.x = 236;
    object[43].dst.y = 8;

    object[44].gimmick = 2; //スイッチ
    object[44].flaghold = 29;
    object[44].dst.x = 241;
    object[44].dst.y = 8;
//

    object[45].gimmick = 3;
    object[45].status = 7;
    object[45].dst.x = 247;
    object[45].dst.y = 12;

    object[46].gimmick = 2;
    object[46].flaghold = 12;
    object[46].dst.x = 253;
    object[46].dst.y = 4;


    object[47].gimmick = 5; //バネシーソー
    object[47].status = -1;
    object[47].flaghold = 48;
    object[47].dst.x = 258;
    object[47].dst.y = 12;

    object[48].gimmick = 5; //バネシーソー
    object[48].status = 0;
    object[48].flaghold = 47;
    object[48].dst.x = 264;
    object[48].dst.y = 12;

    object[49].gimmick = 4; //カギ
    object[49].status = 0;
    object[49].dst.x = 261;
    object[49].dst.y = 7;

    object[50].gimmick = 1;
    object[50].dst.x = 252;
    object[50].dst.y = 1;

    object[51].gimmick = 1;
    object[51].dst.x = 221;
    object[51].dst.y = 0;

    object[52].gimmick = 1;
    object[52].dst.x = 226;
    object[52].dst.y = 0;

    object[53].gimmick = 1;
    object[53].dst.x = 231;
    object[53].dst.y = 0;

    object[54].gimmick = 1;
    object[54].dst.x = 236;
    object[54].dst.y = 0;

    object[55].gimmick = 1;
    object[55].dst.x = 241;
    object[55].dst.y = 0;

    object[56].gimmick = 6; //扉
    object[56].dst.x = 266;
    object[56].dst.y = 2;


    for(j=0; j<SUM_object; j++){
        if(object[j].dst.x == NULL)
            break;
	object[j].movex = 0;
	object[j].movey = 0;
	if(object[j].gimmick == 1 || object[j].gimmick == 3)
            object[j].flaghold = 0;
	if(object[j].gimmick == 2)
            object[j].status = 0;
	object[j].src.x = object[j].gimmick*bit;
	object[j].src.y = 0;
	object[j].src.w = 60;
	object[j].src.h = 60;
	object[j].dst.x *= bit;
	object[j].dst.y *= bit;
	if(object[j].gimmick == 6){
            object[j].src.w = 120;
            object[j].src.h = 120;
            SDL_BlitSurface(objectimage, &object[j].src, mapwindow, &object[j].dst); // objectをmapに貼り付け
        }
	if(object[j].flaghold != -1 || object[j].status == 1)
            SDL_BlitSurface(objectimage, &object[j].src, objectwindow, &object[j].dst); // object貼り付け
    }
    max_map_object = j;
}


void eventdisp(){
    // イベントの種類ごとに処理
    switch (event.type) {
    case SDL_QUIT:
        exit_p = 1; //終了フラグ
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
                stepflag = 1;
				keycommand = 1;
                break;
            case SDLK_DOWN:
                stepflag = 1;
				keycommand = -1;
                break;
            case SDLK_RETURN:
                DEBAG1 = 5;
                break;

            case SDLK_SPACE: //スペースキーを押した時
                if(jumpflag == 0)//{
                    jumpflag = 1;
                jump_a = 12; //初速
                //} //←{}を消したら無限ジャンプ
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
                break;
            case SDLK_UP:
            case SDLK_DOWN:  //上下キーどちらかで
                UD = 0;
                break;
            case SDLK_RETURN:
                DEBAG1 = 0;
                break;
            }
        }


			//joys
			case SDL_JOYBUTTONDOWN:
				switch(event.jbutton.button){
					case 0:
//						stepflag = -1;
						break;
					/**/
					case 1:
					//	DEBAG2 = 5;
						break;
					case 2:
						keycommand = -1;
						break;
					case 3:
				        if(jumpflag == 0){
				            jumpflag = 1;
				            jump_a = 12; //初速
				        } //←{}を消したら無限ジャンプ
						break;

//					default:
//						DEBAG2 = 0;
//						break;
						}
						break;

			case SDL_JOYBUTTONUP:
				switch(event.jbutton.button){
					case 0:
//						stepflag = 0;
						break;
					case 2:
						keycommand = 0;
						break;
					/**/
					case 1:
						DEBAG2 = 0;
						break;
						}


			//スティック(左の十字キーも)
			case SDL_JOYAXISMOTION:
				//左右の動き
				if(event.jaxis.axis == 0)
				{
					if(event.jaxis.value < -0x7000)
					//	newposx -= 4;
						LR = -1;
					else if(event.jaxis.value >  0x7000)
					//	newposx += 4;
						LR = 1;
					else
						LR = 0;
				}

				//上下の動き
				if(event.jaxis.axis == 1){
					if(event.jaxis.value < -0x5000){	//ハシゴフラグ&カギ使用
						stepflag = 1;
						keycommand = 1;
					//	newposy -= 2;
						UD = 1;
						stepUD = 1;
					//	jumpflag = 0;
					}
					else if(event.jaxis.value >  0x7000){	//下移動
					//	newposy += 4;
						stepflag = 1;
						UD = -1;
						stepUD = -1;
					}
					else
					stepUD = 0;
				}
				break;
    }
}



/* リモコン操作 */
void Operation(){

    /* Wiiリモコンの状態を取得・更新する */
//        if (wiimote_update(&wiimote) < 0)// {
//            wiimote_disconnect(&wiimote);
//            break;
//        }

    /* Wiiのキー（ボタン）ごとに処理
       if(wiimote.keys.up) { // 左
       LR = -1;
//        newposx -= 4;
if(jumpflag == 1)
jump_LR = -1;
}
else if(wiimote.keys.down) { // 右
LR = 1;
//   		newposx += 4;
if(jumpflag == 1)
jump_LR = 1;
}
else
LR = 0;

if(wiimote.keys.right || wiimote.keys.left){
stepflag = 1;
}
else{
stepflag = 0;
UD = 0;
}

if(stepflag == 2){
if(wiimote.keys.right){ // 上
newposy -= 2;
UD = 1;
jumpflag = 0;
}

<<<<<<< HEAD
    if(wiimote.keys.one) { // 1ボタン
	DEBAG2 = 5;
    }
	else
	DEBAG2 = 0;
=======
if(wiimote.keys.left){ // 下
newposy += 4;
UD = -1;
}
}
>>>>>>> aace77a996e722d7a84e8ef657d6aef44bd5c1ac

if(wiimote.keys.one) { // 1ボタン
DEBAG = 5;
}
else
DEBAG = 0;

if (wiimote.keys.two) { // 2ボタン
if(jumpflag == 0 && bottun2 == 0){
jumpflag = 1;
jump_a = 12; //初速
bottun2 = 1;
} //←{}を消したら無限ジャンプ

}
else
bottun2 = 0;
    */
}




void keycont(void){
    newposx = P.x;
    newposy = P.y;
    oldx = P.x;
    oldy = P.y;


    Uint8 *key = SDL_GetKeyState(NULL);
    if(key[SDLK_RIGHT] == SDL_PRESSED){	//右移動
        LR = 1;
        newposx += 4;
        if(jumpflag == 1)
            jump_LR = 1;
		DEBAG2 = 0;
    }
    if(key[SDLK_LEFT] == SDL_PRESSED){	//左移動
        LR = -1;
        newposx -= 4;
        if(jumpflag == 1)
            jump_LR = -1;
		DEBAG2 = 0;
    }
    if(stepflag == 2){
        if(key[SDLK_UP] == SDL_PRESSED){	//上移動
            newposy -= 2;
            UD = 1;
            jumpflag = 0;
        }
        if(key[SDLK_DOWN] == SDL_PRESSED){	//下移動
            newposy += 4;
            UD = -1;
        }
    }

if(LR != 0)
newposx = P.x + 4 * LR;
if(stepflag == 2){
if(stepUD == 1)
newposy = P.y-2;
if(stepUD == -1)
newposy = P.y+4;
}


/*その他詳細設定*/

    if(jump_LR == 1 || jump_LR == -1)
        newposx = P.x + 4 * jump_LR; //空中なら静止しない

    if(jumpflag == 1 && jump_a > -8)
        jump_a -= 1; //重力加速　

    if(jumpflag == 0){
        jump_a = 0;	//ジャンプフラグないとき重力ない
        jump_LR = 0;//LR判定もない
    }

    if(jumpflag == 1 && UD == 0)
	newposy -= jump_a;//加速度で移動(y軸なのでマイナス)

///*
//デバッグ用処理　速度10倍
newposx += (newposx - P.x) * (DEBAG1+DEBAG2)*3;
//*/
    hitjudge();

    PlayerHitJudge();

    if(hitx != 1)
        P.x = newposx;
    else if(jump_LR != 0)
        jump_LR *= 2;

    if(hity == 0)
        P.y = newposy;
    else if(hity == -1 || (hitx == 1 && hity == 1)){
        jumpflag = 0;
        if(P.y > 0)
            P.y = ((newposy - 10) / bit + 1) * bit - 15; //マスの上に調整
    }
    else if(hity == -2){
        jumpflag = 0;
        jump_LR = 0;
    }

    if(hity == 0 && UD == 0)
        jumpflag = 1;



//    SendMoveCommand(P.x + gameRect.x, P.y + gameRect.y, PA.x, PA.y);
}

/*
  void mapobject(void){

//object[].gimmick = {1, 2, 1};

object[0].gimmick = 1; //岩
object[0].status = 0; //押されてない状態
object[0].src.x = 180;
object[0].src.y = 225;
object[0].dst.x = 18 * bit - gameRect.x;
object[0].dst.y = 13 * bit - gameRect.x;

}
*/


void EXIT(void){
    //int a = P.x;
    //int b = P.y;
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
			case SDL_JOYAXISMOTION:
				//左右の動き
				if(event.jaxis.axis == 0)
				{
					if(event.jaxis.value < -0x7000)
					P.x += 300;
					if(P.x >= 700)
					P.x = 100;
					if(event.jaxis.value >  0x7000)
					P.x -= 300;
					if(P.x <= 0)
					P.x = 400;
				}
				break;
			case SDL_JOYBUTTONDOWN:
					if(event.type!=SDL_KEYDOWN){
					if(P.x == 400){
					exit_p = 1;
					titlep = 0;
					}
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

        SDL_BlitSurface(usa2, &PA, SDL_GetVideoSurface(), &P);
        SDL_Flip(window);// 画面に図形を表示（反映）
    }

    //P.x = a;
    //P.y = b;

}


/*終了設定*/
void EXITsetting(){
    // フォントを閉じる
    TTF_CloseFont(gTTF);  gTTF=NULL;
    TTF_CloseFont(sTTF);  gTTF=NULL;

    SDL_FillRect(window, NULL, 0xffffffff);	// ウィンドウ背景初期化
    SDL_Flip(window);// 画面に図形を表示（反映）
    InitStatus();
}


/*サーバ移行予定地*/

void newpositionjadge(){

    if(P.x >= WIND_Width * bit-60)
        //P.x = WIND_Width * bit-60;
        if(P.x <= 0)
            //P.x = 0;//画面外に出ない

            if(P.y >= WIND_Height * bit) {//画面下にいけば
                SendGameoverCommand();
                GameOver(mynum);
            }

}

/* プレイヤー同士の当たり判定 */
void PlayerHitJudge(void)
{
    int i;
    for(i=0; i<gClientNum; i++){
        if(i != mynum){
            if((newposx > player[i].pos.x - 45 && newposx < player[i].pos.x + 45) &&
               (P.y > player[i].pos.y - 60 && P.y < player[i].pos.y + 60)){
                hitx = 1;
				if(P.x > player[i].pos.x - 40 && P.x < player[i].pos.x + 40)
				if(P.x > player[i].pos.x)
				P.x = player[i].pos.x + 45;
				}
            if((P.x > player[i].pos.x - 40 && P.x < player[i].pos.x + 40) &&
               (newposy > player[i].pos.y - 60 && newposy < player[i].pos.y + 60)){
                hity = 1;
                jumpflag = 0;
                if(stepflag >= 1){
                    stepflag = 0;
                    UD = 0;
                }
				if(P.y > player[i].pos.y - 55 && P.y < player[i].pos.y + 45)
				if(P.y > player[i].pos.y){
				P.y = player[i].pos.y + 60;
				newposy = P.y;
				jump_a = 0;
				}
            }
        }
    }
}





