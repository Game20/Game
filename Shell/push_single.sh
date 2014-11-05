#!/bin/sh

echo "インデックスに指定されたファイルを登録します"
git add $1
echo "インデックスへの登録が完了しました"

echo "リモートリポジトリへプッシュしますか？ y/n"
echo -n ">> "
read answer


if [ ${answer} = y ];
then
    echo "ローカルリポジトリのファイルをリモートリポジトリにプッシュします"
    echo "リモートリポジトリの名前を入力してください"
    echo -n ">>"
    read name
    echo "${name}へプッシュします"
    echo "自分のアカウントのIDとパスワードを入力してください"
    git push -u ${name} master
    echo "リモートリポジトリへのプッシュが多分完了しました"
    echo "念のためにリモートリポジトリの内容を確認してください"
else
    echo "${name}へのプッシュがキャンセルされました"
fi