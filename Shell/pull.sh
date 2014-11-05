#!/bin/sh

#リモートリポジトリ内の更新されたファイルをプルします
#起動後は指示に従って入力してください

echo "リモートリポジトリからプルし，ファイルを最新状態に更新します"
echo "よろしいですか？ y/n"
echo -n ">>"
read answer

if [ ${answer} = y ];
then
    echo "プルしたい対象のリモートリポジトリの名前を入力してください"
    echo -n ">> "
    read name
    git pull $name
    echo "プルが完了しました"
else
    "プルはキャンセルされました"
fi