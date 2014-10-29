#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<SDL/SDL.h>


/*****************************構造体**********************************/

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

/********************************************************************/


/* マップ，ウインドウサイズ */
enum { 
    MAP_Width    = 38, // マップ全体
    MAP_Height   = 10,
    WIND_Width   = 19, // ウィンドウの大きさ
    WIND_Height  = 10,
    MAP_ChipSize = 80 // マップチップ80*80
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

extern SDL_Surface *window, *mapwindow; // ウィンドウデータへのポインタ

extern SDL_Event event; // SDLによるイベントを検知するための構造

char gMapDataFile[] = "map.data"; // マップデータファイルパス
//extern char gMapDataFile2[] = "map2.data";
//extern char gMapDataFile3[] = "map3.data";
//extern char gMapDataFile4[] = "map4.data";
//extern char gMapImgFile[] = "map.png"; // マップ画像

extern void SetMap(void);
