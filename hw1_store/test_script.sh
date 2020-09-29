#!/bin/bash
echo "Testing init archive..."
./cstore init -p pwd test > test_log.txt

echo "Testing adding files..."
echo hello world > test_content.txt
./cstore add -p pwd test test_content.txt non.exist > test_log.txt

echo "Testing list..."
./cstore list test > test_log.txt

echo "Testing extract..."
mv test_content.txt test_content_0.txt
./cstore extract -p pwd test test_content.txt non.exist > test_log.txt

echo "Testing delete..."
./cstore delete -p pwd test test_content.txt non.exist