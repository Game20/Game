#!/bin/sh

echo "reflesh.shを起動します"

./shell/reflesh.sh


echo "全ファイルをコミット及びプッシュしますか？(非推奨) y/n"
echo -n ">> "
read answer
if [ ${answer} = y ];
then
    ./shell/push_all.sh
fi

if [ ${answer} != y ];
then
    echo "ファイルを個別にコミット及びプッシュしますか？(推奨) y/n"
    echo -n ">> "
    read answer2
    if [ ${answer2} = y ];
    then
	echo "ファイル名を入力してください"
	echo -n ">> "
	read file_name
	./shell/push_single.sh ${file_name}
    fi
fi

echo "pull.shを起動します"

./shell/pull.sh
