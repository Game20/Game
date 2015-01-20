//#include<stdio.h>
//#include<stdlib.h>
//#include<string.h>
#include<SDL/SDL.h>
//#include <SDL/SDL_image.h>       // SDL_imageを用いるために必要なヘッダファイルをインクルード
//#include <SDL/SDL_gfxPrimitives.h>	// gfxによる図形処理をするために必要なヘッダファイルをインクルード
#include <SDL/SDL_ttf.h>	// TrueTypeフォントを表示するために必要なヘッダファイルをインクルード
//#include <SDL/SDL_opengl.h> // SDLでOpenGLを扱うために必要なヘッダファイルをインクルード
#include <SDL/SDL_mixer.h> //SDLで効果音等を扱うために必要なヘッダファイルをインクルード
#include"common.h"

#include <libcwiimote/wiimote.h>
#include <libcwiimote/wiimote_api.h>

/*****************************構造体**********************************/

typedef struct{
    SDL_Rect pos; // 座標
    SDL_Rect anim;
    //SDL_Rect anime; // アニメーション
    //int jumpf; // ジャンプフラグ
    //int jump_a; // jump acceleration
    //int LR; // 右左
    //int UD; // 上下
    //int status; // ステータス
    //int deadf; // 生死フラグ0=生存 1=死
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
#define SUM_switchblock 31
#define SUM_steps 7

extern Player player[MAX_CLIENTS];

Object object[SUM_object]; //構造体の配列化
Object switchblock[SUM_switchblock];//スイッチ効果のブロック配列
Object steps[SUM_steps];//ハシゴの配列

SDL_Surface *window, *mapwindow, *objectwindow; // ウィンドウデータへのポインタ

SDL_Rect gameRect; // ゲームウィンドウの座標

extern SDL_Event event; // SDLによるイベントを検知するための構造

//char gMapDataFile[] = "map.data"; // マップデータファイルパス
//extern char gMapDataFile2[] = "map2.data";
//extern char gMapDataFile3[] = "map3.data";
//extern char gMapDataFile4[] = "map4.data";
//extern char gMapImgFile[] = "map.png"; // マップ画像

//extern void SetMap(void);

extern SDL_Surface *gMessages[ 100 ];

extern TTF_Font* gTTF;	// TrueTypeフォントデータへのポインタ

extern SDL_Surface *usa, *usa2, *neko, *inu, *panda;  // 画像データへのポインタ
extern SDL_Surface *objectimage;
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
extern int titlep2;

extern int stageP;
extern int mapread;

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
//extern int InitWindows(int clientID,int num,char name[][MAX_NAME_SIZE]);
//extern void DestroyWindow(void);
//extern void WindowEvent(int num);
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
extern void SendEscapeCommand(void);

/** client_se.c */
extern void loadSounds();///サウンド,BGMを読み込む
extern void playBGM(num);///BGM再生
extern void playSE(num);///効果音再生


/* 未定 */
void story(void);
void result(void);



/* Wiiリモコンを用いるための構造体を宣言（初期化） */
//extern wiimote_t wiimote = WIIMOTE_INIT; // Wiiリモコンの状態格納用
//extern wiimote_report_t report = WIIMOTE_REPORT_INIT; // レポートタイプ用















