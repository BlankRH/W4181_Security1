#!/bin/bash

echo "Testing basic functions..."
cstore init -p pwd test0 test1 test2 test3 test4 test5 test6 >> test_log.txt

echo hello world > test_content.txt
cstore add -p pwd test0 test_content.txt non.exist >> test_log.txt

cstore list test0 >> test_log.txt

mv test_content.txt test_content_0.txt
cstore extract -p pwd test0 test_content.txt non.exist >> test_log.txt

cstore delete -p pwd test0 test_content.txt non.exist >> test_log.txt

cstore add -p pwd test0 test_content.txt non.exist

echo
echo "********************"
echo

echo "Testing nonexist arhive..."
cstore add -p pwd n test_content.txt non.exist >> test_log.txt
cstore extract -p pwd n test_content.txt non.exist >> test_log.txt
cstore delete -p pwd n test_content.txt non.exist >> test_log.txt

echo
echo "********************"
echo

echo "Testing wrong params..."
cstore >> test_log.txt
cstore delete  >> test_log.txt
cstore extract >> test_log.txt
cstore add >> test_log.txt
cstore init >> test_log.txt
cstore list test1 test2 >> test_log.txt

echo
echo "********************"
echo

echo "Testing integrity check..."
cstore add -p pwd test1 test_content.txt non.exist >> test_log.txt
rm -f test1/test_content.txt
cstore extract -p pwd test1 test_content.txt non.exist >> test_log.txt

cstore add -p pwd test2 test_content.txt non.exist >> test_log.txt
mv ./test2/test_content.txt ./test2/newname.txt
cstore extract -p pwd test2 non.exist >> test_log.txt

echo
echo "********************"
echo

echo "Testing wrong pwd..."
cstore add -p pd test0 test_content.txt non.exist >> test_log.txt
cstore extract -p pd test0 non.exist >> test_log.txt
cstore delete -p pd test0 non.exist >> test_log.txt

echo
echo "********************"
echo

echo "Testing missing important files"
rm -f ./test3/metadata.txt
cstore extract -p pwd test3 hashcode.txt >> test_log.txt
rm -f ./test3/hashcode.txt
cstore extract -p pwd test4 metadata.txt >> test_log.txt

echo
echo "********************"
echo

echo "Testing concat files"
cstore add -p pwd test6 test_content.txt non.exist >> test_log.txt
cat ./test6/non.exist >> test6/test_content.txt
rm -f test6/non.exist
cp non.exist test6/non.exist
cstore extract -p pwd test6 test_content.txt non.exist >> test_log.txt