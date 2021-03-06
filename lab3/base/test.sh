#!/bin/bash

#make two files
yes file0| head -n 7000 > test/file.txt
yes file1| head -n 3000 > test/file1.txt

#file wont completely write out
#filecopy will be smaller then file
./crash 30
cp test/file.txt test/filecopy.txt

#make a new regular file
./crash -1
yes file2| head -n 1000 > test/file2.txt

#file made but empty
./crash 5
cp test/file2.txt test/file2copy.txt

#file2copy will not even be created
./crash 1
cp test/file2.txt test/file2copy2.txt

#link file1 to another file1
./crash 3
echo "info" >> test/file1.txt
ln test/file1.txt test/file1copy.txt

#shows that the second file was never made
#yet link count is 2 for file1.txt
#link goes through but file1copy is not in directory
./crash -1
ls -la test
rm test/file1.txt 
rm test/file1copy.txt
