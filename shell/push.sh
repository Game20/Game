#!/bin/sh


echo "Push To Remort Repostory? y/n"
echo -n ">> "
read ans

if [ ${ans} = y ]
then
git push https://github.com/Game20/Game.git master
fi
