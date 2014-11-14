#!/bin/sh

# ~ のついたファイル， 拡張子.o, client server のファイルを削除する
echo "Call reflesh"
./shell/reflesh.sh
#echo "Exit reflesh.sh"
echo "_______________________________"

git pull game master

echo ""
echo "Git Pull Complete!"
echo "_______________________________"




if [ "$1" != "" ]; then
git add $1
echo "$1 added."
fi
if [ "$2" != "" ]; then
git add $2
echo "$2 added."
fi
if [ "$3" != "" ]; then
git add $3
echo "$3 added."
fi
if [ "$4" != "" ]; then
git add $4
echo "$4 added."
fi
if [ "$5" != "" ]; then
git add $5
echo "$5 added."
fi
if [ "$6" != "" ]; then
git add $6
echo "$6 added."
fi
if [ "$7" != "" ]; then
git add $7
echo "$7 added."
fi
if [ "$8" != "" ]; then
git add $8
echo "$8 added."
fi
if [ "$9" != "" ]; then
git add $9
echo "$9 added."
fi

echo ""
echo "Git Add Complete!"
echo "_______________________________"
echo "Please Commit Comment."
echo -n ">> "
read comment
git commit -m "$comment"

echo "Commit Complete!"
echo "_______________________________"

echo "Push Now. If you don't push, Ctr_C or Ctr_Z"
git push game master

echo "_______________________________"
echo "Push Complete!"
echo "_______________________________"
