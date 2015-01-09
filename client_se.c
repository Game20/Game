#include"common.h"
#include"client_func.h"

Mix_Music *bgm[5]; ///BGM用
Mix_Chunk *se[10]; ///SE用


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
}


/**
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



void playSE(int num)
{
Mix_PlayChannel(num, se[num], 0);
}

