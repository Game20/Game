#!/bin/sh

echo "Please Your Comment About Change"
echo -n ">> "
read comment

git commit -m "${comment}"

echo "Done Commit."
