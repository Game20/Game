#!/bin/sh


#AddとCommitをしたい分だけしてもらう

echo "Do you Add & Commit? y/n"
read comfirm

while [ "${comfirm}" = "y" ]
do
#Add
    echo "Please File Name You 'Add' To Index." 
    echo -n ">> "
    read name
    git add ${name}
    echo "Done Add ${name} To Your Index."
#Commit
    echo "Do You Commit Files In Index Now? y/n"
    echo -n ">> "
    read ans
    if [ ${ans} = y ];
    then
    echo "Call 'commit.sh'"
    ./shell/commit.sh
    echo "Exit commit.sh"
    fi
#まだファイルをAddするかどうか確認する
    echo "More Files To Add? y/n"
    echo -n ">> "
    read comfirm
done


