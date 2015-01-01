#include"common.h"
#include"client_func.h"

Mix_Music *bgm[5]; ///BGM用
Mix_Chunk *se[10]; ///SE用


void loadSounds()
{
if(Mix_OpenAudio(22050,AUDIO_S16,2,4096)==1)    // SDL_Mixerオープン
    SDL_Quit();
 Mix_AllocateChannels(16);//同時発生数を設定
///サウンドを読み込む
bgm[0] = Mix_LoadMUS("SE/start.wav");
bgm[1] = Mix_LoadMUS("SE/samba.wav");
bgm[2] = Mix_LoadMUS("SE/1.wav");
se[0] = Mix_LoadWAV("SE/???.wav");
}

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

