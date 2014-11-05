#!/bin/sh
#バルスする

echo "reflesh.shを起動します"

./Shell/reflesh.sh


echo "全ファイルをコミット及びプッシュしますか？(非推奨) y/n"
echo -n ">> "
read answer
if [ ${answer} = y ];
then
    ./Shell/push_all.sh
fi

if [ ${answer} != y ];
    echo "ファイルを個別にコミット及びプッシュしますか？(推奨) y/n"
    echo -n ">> "
    read answer2
    if [ ${answer2} = y ];
    then
	echo "ファイル名を入力してください"
	echo -n ">> "
	read file_name
	./Shell/push_single.sh ${file_name}
    fi
fi

echo "pull.shを起動します"

./Shell/pull.sh
