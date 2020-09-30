#!/bin/bash
echo "Testing init archive..."
cstore init -p pwd test >> test_log.txt

echo "Testing adding files..."
echo hello world > test_content.txt
cstore add -p pwd test test_content.txt non.exist >> test_log.txt

echo "Testing list..."
cstore list test > test_log.txt

echo "Testing extract..."
mv test_content.txt test_content_0.txt
cstore extract -p pwd test test_content.txt non.exist >> test_log.txt

echo "Testing delete..."
cstore delete -p pwd test test_content.txt non.exist >> test_log.txt

echo "Testing nonexist arhive..."
cstore add -p pwd n test_content.txt non.exist >> test_log.txt
cstore extract -p pwd n test_content.txt non.exist >> test_log.txt
cstore delete -p pwd n test_content.txt non.exist >> test_log.txt

echo "Testing wrong params..."
cstore >> test_log.txt
cstore delete  >> test_log.txt
cstore extract >> test_log.txt
cstore add >> test_log.txt
cstore init >> test_log.txt
cstore init -p pwd a b >> test_log.txt

echo "Testing integrity check"
cstore add -p pwd a test_content.txt non.exist >> test_log.txt
rm -f a/test_content.txt
cstore extract -p pwd a test_content.txt non.exist >> test_log.txt