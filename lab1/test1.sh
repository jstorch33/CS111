#set up files
#random input is the same for each run
base64 /dev/urandom | \
    head -c 10000000 > a.txt

#Ouput files or simpsh, bash, and execline respectively
touch S1.txt S2.txt S3.txt
touch B1.txt B2.txt B3.txt
touch E1.txt E2.txt E3.txt

#Error files for simpsh and execline
touch err1.txt err2.txt err3.txt
touch Eerr1.txt Eerr2.txt Eerr3.txt

#test1
#sort a and double space. delete every 5 line and then write to b.
./simpsh \
    --verbose\
    --profile \
    --rdonly a.txt \
    --append --rdwr S1.txt \
    --wronly err1.txt \
    --pipe \
    --pipe \
    --command 0 4 2 sort \
    --command 3 6 2 shuf \
    --command 5 1 2 sort \
    --wait

#bash version
time cat a.txt | \
    sort | \
    shuf | \
    sort >> B1.txt

#Execline version
time ./execline1


#test2
#capatalize a then delete white spaces and put into c
#sort b and reverse ordering and put into d
./simpsh \
    --verbose \
    --profile \
    --rdonly a.txt \
    --wronly S2.txt \
    --wronly err2.txt \
    --pipe \
    --pipe \
    --pipe \
    --pipe \
    --command 0 4 2 tr 'a-z' 'A-Z' \
    --command 3 6 2 tr 'A-G' 'a-g' \
    --command 5 8 2 sort \
    --command 7 10 2 shuf \
    --command 9 1 2 sort \
    --wait

#bash version
time cat a.txt | \
    tr 'a-z' 'A-Z' | \
    tr 'A-G' 'a-g' | \
    sort | \
    shuf | \
    sort > B2.txt

#Execline version
time ./execline2


#test3
#sort words and change all lowercase to uppercase then all A-G to lowercase
./simpsh \
    --verbose \
    --profile \
    --rdonly a.txt \
    --append --wronly S3.txt \
    --pipe \
    --pipe \
    --pipe \
    --pipe \
    --wronly err3.txt \
    --command 0 3 10 sort \
    --command 2 5 10 tr 'a-z' 'A-Z' \
    --command 4 7 10 shuf \
    --command 6 9 10 tr 'A-G' 'a-g' \
    --command 8 1 10 sort \
    --wait

#bash version
time cat a.txt | \
    sort | \
    tr 'a-z' 'A-Z' | \
    shuf | \
    tr 'A-G' 'a-g' | \
    sort >> B3.txt

#Execline version
time ./execline3
