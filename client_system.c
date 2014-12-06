#include "client_func.h"

void EXIT();

int j, k, l;
SDL_Rect P_START;

//ステータス初期化
void InitStatus(void){ // キャラのステータスの初期化
int i;
    P.x = P_START.x;
    P.y = P_START.y; //12*bit;
	newposx = P.x;
	newposy = P.y;
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

	objectinit();

    for(i = 0; i < MAX_CLIENTS; i++){
        player[i].pos.w = 60;
        player[i].pos.h = 75;
    }

    exit_p = 0;
    titlep = 1;
    titlep2 = 0;

}


void objectinit(void){

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

	object[27].gimmick = 0;
	object[27].dst.x = 243;
	object[27].dst.y = 2;

	object[28].gimmick = 2; //スイッチ
	object[28].flaghold = 10;
	object[28].dst.x = 268;
	object[28].dst.y = 11;

	object[29].gimmick = 3; //バネ
	object[29].status = 4;
	object[29].dst.x = 266;
	object[29].dst.y = 3;

	object[30].gimmick = 5; //カギ
	object[30].dst.x = 256;
	object[30].dst.y = 11;


	for(j=0; j<SUM_object; j++){
	object[j].movex = 0;
	object[j].movey = 0;
	if(object[j].gimmick == 1 || object[j].gimmick == 3)
	object[j].flaghold = 0;
	object[j].src.x = object[j].gimmick*bit;
	object[j].src.y = 0;
if(j == 30){
object[j].src.x = 0;
object[j].src.y = 60;
}
	object[j].src.w = 60;
	object[j].src.h = 60;
	object[j].dst.x *= bit;
	object[j].dst.y *= bit;
	if(object[j].flaghold != -1 || object[j].status == 1)
	SDL_BlitSurface(objectimage, &object[j].src, mapwindow, &object[j].dst); // object貼り付け
	}

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
            case SDLK_DOWN:
                stepflag = 1;
                break;
            case SDLK_RETURN:
                DEBAG = 5;
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
            case SDLK_RETURN:  //上下キーどちらかで
                DEBAG = 0;
                break;
            }
        }
    }
}


void keycont(void){

    newposx = P.x;
    newposy = P.y;

    Uint8 *key = SDL_GetKeyState(NULL);
    if(key[SDLK_RIGHT] == SDL_PRESSED){	//右移動
        newposx += 4;
        if(jumpflag == 1)
            jump_LR = 1;
    }
    if(key[SDLK_LEFT] == SDL_PRESSED){	//左移動
        newposx -= 4;
        if(jumpflag == 1)
            jump_LR = -1;
    }
	if(stepflag == 2){
    if(key[SDLK_UP] == SDL_PRESSED){	//上移動
        newposy -= 2;
        UD = 1; //←あとで消す
        jumpflag = 0; //←あとで消す
    }
    if(key[SDLK_DOWN] == SDL_PRESSED){	//下移動
        newposy += 4;
        UD = -1;
    }
	}


/*その他詳細設定*/

    if(key[SDLK_RIGHT] != SDL_PRESSED && key[SDLK_LEFT] != SDL_PRESSED && (jump_LR == 1 || jump_LR == -1))
        newposx += 4 * jump_LR; //空中なら静止しない

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
newposx += (newposx - P.x) * DEBAG*3;
//*/

    hitjudge();

    if(hitx != 1)
        P.x = newposx;
    else if(jump_LR != 0)
        jump_LR *= 2;

    if(hity == 0)
        P.y = newposy;
    else if(hity == -1 || (hitx == 1 && hity == 1)){
		jumpflag = 0;
		P.y = ((newposy - 10) / bit + 1) * bit - 15; //マスの上に調整
	}
	else if(hity == -2){
	 	jumpflag = 0;
		jump_LR = 0;
	}

    if(hity == 0 && UD == 0)
        jumpflag = 1;

    SendMoveCommand(P.x + gameRect.x, P.y + gameRect.y);

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
    InitStatus();
}


/*サーバ移行予定地*/

void newpositionjadge(){

    if(P.x >= WIND_Width * bit-60)
        P.x = WIND_Width * bit-60;
    if(P.x <= 0)
        P.x = 0;//画面外に出ない

    if(P.y >= WIND_Height * bit)//画面下にいけば
    	GameOver();


}
