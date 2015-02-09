/*****************************************************************
ファイル名	: client_func.h
機能		: クライアントで使用する定数や関数の宣言を行う
作成者氏名:船坂国之,坪井正夢,高松翔馬,森祥悟
最終更新日:2015.2.5
*****************************************************************/

#include<SDL/SDL.h>
#include <SDL/SDL_ttf.h>	// TrueTypeフォントを表示するために必要なヘッダファイルをインクルード
#include <SDL/SDL_mixer.h> //SDLで効果音等を扱うために必要なヘッダファイルをインクルード
#include"common.h"

#include <libcwiimote/wiimote.h>
#include <libcwiimote/wiimote_api.h>

/*****************************構造体**********************************/

typedef struct{
    SDL_Rect pos; // 座標
    SDL_Rect anim; // アニメーション
    int jewelP;
} Player; // プレイヤーの構造体


typedef struct{
    int gimmick;	// 0==中間ポイント 1==岩 2==スイッチ 3==バネ
    int status; // 状態
	int movex; //x座標移動
	int movey; //y座標移動
	int flaghold;
    SDL_Rect src;	//読み込み座標
    SDL_Rect dst;	//貼り付け座標
    SDL_Rect oldsrc;
    SDL_Rect olddst;
} Object; // オブジェクトの構造体
/********************************************************************/



/* マップ，ウインドウサイズ */
enum {
    MAP_Width    = 270, // マップ全体
    MAP_Height   = 14,
    WIND_Width   = 25, // ウィンドウの大きさ
    WIND_Height  = 14,
    bit = 60 // マップチップ60*60
};
//ゲームウィンドウの最大は1535*846

/* マップの種類 */
typedef enum {
    MT_sky = 0,
    MT_Wall = 1,
    MT_needle = 2,
} MapType;


#define SUM_object 60
#define SUM_switchblock 33
#define SUM_steps 7
#define SUM_jewel 30

extern Player player[MAX_CLIENTS];

Object object[SUM_object]; //構造体の配列化
Object switchblock[SUM_switchblock];//スイッチ効果のブロック配列
Object steps[SUM_steps];//ハシゴの配列
Object jewel[SUM_jewel];//ジュエルの配列

SDL_Surface *window, *mapwindow, *objectwindow; // ウィンドウデータへのポインタ

SDL_Rect gameRect; // ゲームウィンドウの座標

extern SDL_Event event; // SDLによるイベントを検知するための構造

extern SDL_Surface *gMessages[ 100 ];

extern TTF_Font* gTTF;	// TrueTypeフォントデータへのポインタ

extern SDL_Surface *usa, *usa2, *neko, *inu, *panda;  // 画像データへのポインタ
extern SDL_Surface *objectimage, *jewels;
extern SDL_Surface *blockimage;
extern SDL_Surface *gameclear;

extern TTF_Font* sTTF;

extern int jumpflag;
extern int jump_LR;

extern int newposx;
extern int newposy;
extern int hitx;
extern int hity;
extern int timekey;
extern int hithold;
extern int shiftdef;
extern int gimmickflag;
extern int G_flaghold;
extern int stepflag;
extern int keycommand;
extern SDL_Rect rect;
extern SDL_Rect dst;
extern int DEBAG1;
extern int DEBAG2;
extern int clearpoint;
extern int help;
extern int debugmode;
extern int setstage;

extern int max_map_object;

extern SDL_Rect PA;
extern SDL_Rect P;
extern SDL_Rect PMR;
extern SDL_Rect PM;


extern int time;	//時間
extern int LR;		//左右入力
extern int UD;		//上下入力

extern int exit_p;
extern int jump_a;

extern int titlep;
extern int clearset;

extern int stageP;
extern int mapread;
extern int jewelinit;

extern int fm;

extern int i;

/** 効果音・サウンド用 */
extern Mix_Music *bgm[5]; ///BGM用
extern Mix_Chunk *se[10]; ///SE用

/* client_system.c */
void EXIT(void);
void objectinit1(void);
void objectinit2(void);

/* client_net.c */
extern int SetUpClient(char* hostName,int *clientID,int *num,char clientName[][MAX_NAME_SIZE]);
extern void CloseSoc(void);
extern int RecvIntData(int *intData);
extern void SendData(void *data,int dataSize);
extern int SendRecvManager(void);

/* client_window.c */
extern void title(void);
extern void Mapshift(void);
extern void DrawChara(void); //他プレイヤー描画
extern void GameOver(int ClientNum);
extern void StageClear(void);
extern void GameClear(void);

/* client_command.c */
extern int ExecuteCommand(char command);
extern void SendEndCommand(void);
extern void SendMoveCommand(int x, int y, int animx, int animy); //追加
extern void SendObjectCommand(int num, int status, int x, int y, int movex, int movey);
extern void SendStartCommand(void);
extern void SendGameoverCommand(void);
extern void SendNewposCommand(void);
extern void SendGameclearCommand(void);
extern void SendWindowCommand(void);
extern void SendEscapeCommand(int titlep);
extern void SendTimeCommand(void);
extern void SendTitleCommand(int y, int titlep); // タイトルに関する情報を送信
extern void SendJewelCommand(int jewel); // jewelポイント送信

/** client_se.c */
extern void loadSounds();///サウンド,BGMを読み込む
extern void playBGM(num);///BGM再生
extern void playSE(num);///効果音再生


/* 未定 */
void story(void);
void result(void);













