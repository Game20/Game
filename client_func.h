//#include<stdio.h>
//#include<stdlib.h>
//#include<string.h>
#include<SDL/SDL.h>
//#include <SDL/SDL_image.h>       // SDL_imageを用いるために必要なヘッダファイルをインクルード
//#include <SDL/SDL_gfxPrimitives.h>	// gfxによる図形処理をするために必要なヘッダファイルをインクルード
#include <SDL/SDL_ttf.h>	// TrueTypeフォントを表示するために必要なヘッダファイルをインクルード
//#include <SDL/SDL_opengl.h> // SDLでOpenGLを扱うために必要なヘッダファイルをインクルード
#include"common.h"


/*****************************構造体**********************************/
/*
<<<<<<< HEAD
  typedef struct{
  int x; // 座標
  int y;
  int x_anim; // アニメーション
  int y_anim;
  int jumpf; // ジャンプフラグ
  int jump_a; // jump acceleration
  int LR; // 右左
  int UD; // 上下
  int status; // ステータス
  int deadf; // 生死フラグ0=生存 1=死
  } player[]; // プレイヤーの構造体

  typedef struct{
  int x; // 座標
  int y;
  int status; // 状態
  } object[]; // オブジェクトの構造体
=======
typedef struct{
    int x; // 座標
    int y;
    int x_anim; // アニメーション
    int y_anim;
    int jumpf; // ジャンプフラグ
    int jump_a; // jump acceleration
    int LR; // 右左
    int UD; // 上下
    int status; // ステータス
    int deadf; // 生死フラグ0=生存 1=死
} player; // プレイヤーの構造体
>>>>>>> 9f111d1ff32bd89630a634bbf667cac83ebac0de

/********************************************************************/


/* マップ，ウインドウサイズ */
enum { 
    MAP_Width    = 38, // マップ全体
    MAP_Height   = 14,
    WIND_Width   = 25, // ウィンドウの大きさ
    WIND_Height  = 14,
    MAP_ChipSize = 60 // マップチップ60*60
};

/* マップの種類 */
typedef enum {
    MT_sky = 0,
    MT_Wall = 1,
    MT_goal = 2,
    MT_Needle1 = 3,
    MT_Needle2 = 4,
    MT_Needle3 = 5,
    MT_Needle4 = 6,
    MT_Fruit = 7
} MapType;

typedef struct{
	int gimmick;	// 1==岩 2==スイッチ 3==
    int status; // 状態
	SDL_Rect rect;	//読み込み座標
	SDL_Rect dst;	//貼り付け座標
} Object; // オブジェクトの構造体


SDL_Surface *window, *mapwindow; // ウィンドウデータへのポインタ

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

extern SDL_Surface *usa;  // 画像データへのポインタ

extern TTF_Font* sTTF;

extern int jumpflag;
extern int jump_LR;

extern int bit;
extern int newposx;
extern int newposy;
extern int hitx;
extern int hity;
extern int timekey;
extern int hithold;
extern int shiftdef;
extern int objecttouchflag;
extern int SUM_object;


extern SDL_Rect PA;
extern SDL_Rect P;
extern SDL_Rect PMR;
extern SDL_Rect PM;

extern int time;	//時間
extern int LR;		//左右入力
extern int UD;		//上下入力

extern int exit_p;
extern int jump_a;

extern int titlep, titlep2;
extern int fm;

extern int screensizex;


/* client_net.c */
extern int SetUpClient(char* hostName,int *clientID,int *num,char clientName[][MAX_NAME_SIZE]);
extern void CloseSoc(void);
extern int RecvIntData(int *intData);
extern void SendData(void *data,int dataSize);
extern int SendRecvManager(void);

/* client_win.c */
extern int InitWindows(int clientID,int num,char name[][MAX_NAME_SIZE]);
extern void DestroyWindow(void);
extern void WindowEvent(int num);
extern void DrawChara(int x, int y); //他プレイヤー描画

/* client_command.c */
extern int ExecuteCommand(char command);
extern void SendEndCommand(void);
extern void SendMoveCommand(int x, int y); //追加



