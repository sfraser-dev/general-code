#!/bin/bash

# Tutorial taken from http://www.freeos.com/guides/lsst/index.html

clear
echo "Hello $USER"
echo -e "Today is \c";date
echo "Number of user login : `who | wc -l`"
echo "Calendar"
cal

expr 1 + 3
expr 10 / 2
expr 2 \* 3
expr 20 % 3
echo "3 k 20 3 / p" | dc

echo "Enter your name: "
echo " ... skipping ..."
#read fname
echo "Hello $fname!"

#if [ $# -lt 2 ]; then 
#    echo "*** Error too few args ($#) ***"
#    echo "Usage: $0 val1 val2"
#    exit 1  # exit with error
#fi
#val1=$1
#val2=$2
#echo "$val1 + $val2 = `expr $val1 + $val2`"

echo "In bc, false (5 < 2): $(echo "5 < 2" | bc)"
echo "In bc, true (5 > 2): $(echo "5 > 2" | bc)"

for i in 1 2 3 4 5; 
do
    echo "welcome $i times"
done

for (( i=1; i<=9; i++ ))
do
    for (( j=1; j<=9; j++ ))
    do
	tot=`expr $i + $j`
	tmp=`expr $tot % 2`
	if [ $tmp -eq 0 ]; then
	    echo -e -n "\033[47m "
	else
	    echo -e -n "\033[40m "
	fi
    done
    echo -e -n "\033[40m" # set background color to black
    echo "" # print newline
done

SayHello() {
    echo "Hello from function $1"
    return
}

SayHello "Stewie"

echo "printing command/shell name: $0"
echo "printing command args as a sequence of strings: $@"
echo "printing command args as a single string: $*"
echo "printing the number of command args: $#"
echo "printing the PID of the current shell script: $$"
while getopts t:z: opt
do
    case "$opt" in
	t) echo "t option is $OPTARG";;
	z) echo "z option is $OPTARG";;
	[?]) echo "not t or z cmd line arguments";;
    esac
done
shift 2

echo -e "\nCutting column two from file snameTab.txt"
cut -f2 snameTab.txt
echo -e "\nCutting column two from file snameComma.txt"
cut -f2 -d, snameComma.txt # change delimiter to comma (for csv files)

echo "Pasting the two files together"
paste snameTab.txt smarkTab.txt

echo -e "\nJoining two files together based on a common field ..."
echo "... (default common field (ID) is column 1"
join snameTab.txt smarkTab.txt

echo -e "\ntranslate h to 3 and 2 to x"
tr "h3" "2x" <snameTab.txt
echo -e "\nhello, world ... translate to uppercase"
echo "hello, world" | tr "[a-z]" "[A-Z]"

echo -e "\nusing awk to manipulate data select all records ..."
echo -e "... containing \"good\" and print the 3rd field ..."
echo "... note, in awk, row=record, column=field"
awk '/good/ {print $3}'  inventoryTab.txt
awk '/good/ {print $1 " " $3}'  inventoryTab.txt

# 1) What is the difference between awk and sed ?
# Both are tools that transform text. BUT awk can do more things besides just manipulating text. 
# Its a programming language by itself with most of the things you learn in programming, like arrays, 
# loops, if/else flow control etc You can "program" in sed as well, but you won't want to maintain the code written in it.
#
# 2) What kind of application are best use cases for sed and awk tools ?
# Conclusion: Use sed for very simple text parsing. Anything beyond that, awk is better. 
# In fact, you can ditch sed altogether and just use awk. Since their functions overlap and awk can do more, 
# just use awk. You will reduce your learning curve as well.

echo -e "\ncat teaormilk.txt"
cat teaormilk.txt
echo -e "\nuse sed to substitute milk by tea globally on file teoormilk.txt"
sed '/tea/s//milk/g' teaormilk.txt

echo -e "\nremoving duplicate lines using uniq"
echo "cat personame.txt"
cat personame.txt
echo -e "\nuniq personame.txt"
uniq personame.txt
echo -e "\nsort personame.txt| uniq"
sort personame.txt | uniq # sort the file personame.txt into alphabetical order

echo -e "\ngrep doesn't understand words, just expressions, and prints ..."
echo -e "... all line where it finds that expression \"too\" in this case"
grep "too" demo-file.txt

echo "" 
exit 0 # exit ok
