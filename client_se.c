/*****************************************************************
ファイル名:client_se.c
　　　機能:クライアントのBGM，SE再生
作成者氏名:船坂
最終更新日:2015.1.29
*****************************************************************/


#include"common.h"
#include"client_func.h"

Mix_Music *bgm[5]; ///BGM用
Mix_Chunk *se[10]; ///SE用

/*****************************************************************
関数の名前:void loadSounds()
関数の機能:SDL_Mixerを開き，BGM，SE用の音楽ファイルをロードする．
*****************************************************************/
void loadSounds()
{
if(Mix_OpenAudio(22050,AUDIO_S16,2,4096)==1)    // SDL_Mixerオープン
    SDL_Quit();
 Mix_AllocateChannels(16);//同時発生数を設定
///BGMを読み込む
bgm[0] = Mix_LoadMUS("BGM/op.mp3");
bgm[1] = Mix_LoadMUS("BGM/stage1.mp3");
bgm[2] = Mix_LoadMUS("BGM/stage2.mp3");
//bgm[3] = Mix_LoadMUS("BGM/boss.mp3");
bgm[4] = Mix_LoadMUS("BGM/clear.mp3");
///SEを読み込む
se[0] = Mix_LoadWAV("SE/jamp2.wav");
se[1] = Mix_LoadWAV("SE/death.wav");
se[2] = Mix_LoadWAV("SE/switch5.wav");
se[3] = Mix_LoadWAV("SE/bane.wav");
se[4] = Mix_LoadWAV("SE/save_p.wav");
se[5] = Mix_LoadWAV("SE/key_up.wav");
se[6] = Mix_LoadWAV("SE/key_put.wav");
se[7] = Mix_LoadWAV("SE/fall.wav");
se[8] = Mix_LoadWAV("SE/key_op.wav");
se[9] = Mix_LoadWAV("SE/clear.wav");
}


/**
関数の名前:void playBGM(int num)
関数の機能:もし音楽が流れている状態ならそれをフェードアウトし，引数に応じたBGMを再生する．
関数の引数:num
以下のnumに対応した添字をつけて呼び出すことでBGMを再生する
num = 0　：オープニング
num = 1　：ステージ１
num = 2　：ステージ２
num = 3　；NULL(BOSS戦が消えた為)
num = 4　：クリアー
num = 5　：NULL
*/
void playBGM(int num)
{
    if(Mix_PausedMusic() == 1){
        Mix_FadeOutMusic(4000);
        Mix_HaltMusic();
    }
        Mix_PlayMusic(bgm[num], -1);

}


/**
関数の名前:void playSE(int num)
関数の機能:引数に応じたSEを再生する．
関数の引数:num
以下のnumに対応した添字をつけて呼び出すことでSEを再生する
num = 0 :キャラのジャンプ音
num = 1 :死亡判定音
num = 2 :スイッチを押した時の音
num = 3 :バネでジャンプした時の音
num = 4 :中間ポイントの音
num = 5 :鍵を拾った時の音
num = 6 :鍵をおいた時の音
num = 7 :ものが落ちる時の音
num = 8 :鍵を使って扉を開ける時の音
num = 9 :ゲームクリアーしたときの音
num = 10:NULL
*/
void playSE(int num)
{
Mix_PlayChannel(num, se[num], 0);
}

