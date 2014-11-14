#!/bin/sh

# ~ のついたファイル， 拡張子.oのファイルを削除する
echo "Call ./shell/reflesh.sh"
./shell/reflesh.sh
echo "Exit reflesh.sh"


#任意のファイルをAddし，必要ならばCommit.shを呼び出しコミットする
echo "Call ./shell/add.sh"
./shell/add.sh
echo "Exit 'add.sh'"

#ローカルリポジトリ内のファイルをPushする
echo "Call ./push.sh"
./shell/push.sh
echo "Exit 'push.sh'"



#Push
