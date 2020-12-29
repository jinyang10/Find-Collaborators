#!/bin/bash

if [[ ! -f report.c ]]
then
	echo "Error cannot locate report.c"
	exit 1
fi

export tccounter=0
function testcase
{
	tccounter=$(expr $tccounter + 1);
	echo ""
	echo "[[ Test case $tccounter - $1 ]]"
}

testcase "compilation"
rm -f report
echo gcc -o report report.c
gcc -o report report.c
rc=$?
echo "gcc returned $rc for compilation"
if [[ ! -x report ]]
then
	echo "FATAL !! no executable was built, terminating testing"
	exit 1
fi

echo -e "\n\n[[ Test cases that should produce error messages ]]\n"

testcase "invalid usage"
echo ./report data.csv
./report data.csv
rc=$?
echo "./report returned $rc"

testcase "check stderr for error messages"
echo "    No message will be displayed if it is not being printed to stderr"
echo ./report data.csv "Jane Doe"  '2>&1 1>/dev/null'
./report data.csv "Jane Doe"  2>&1 1>/dev/null
rc=$?
echo "./report returned $rc"

testcase "check for input file"
echo ./report nosuchdata.csv "Jane Doe" rpt.txt
./report nosuchdata.csv "Jane Doe" rpt.txt
rc=$?
echo "./report returned $rc"

testcase "Non-existent user"
echo ./report data.csv "Jane Doe" rpt.txt
./report data.csv "Jane Doe" rpt.txt
rc=$?
echo "./report returned $rc"

testcase "Unable to open output file"
echo ./report data.csv "Markus Bender" /rpt.txt
./report data.csv "Markus Bender" /rpt.txt
rc=$?
echo "./report returned $rc"

rm -f rpt.txt
echo -e "\n\n[[ Test cases that should produce non-error outputs ]]\n"

testcase "Locate a user with no colaborators"
echo ./report data.csv "Markus Bender" rpt.txt
./report data.csv "Markus Bender" rpt.txt
rc=$?
echo "./report returned $rc"
cat rpt.txt && rm -f rpt.txt

testcase "Locate a user with colaborators"
echo ./report data.csv "Adaline Murphy" rpt.txt
./report data.csv "Adaline Murphy" rpt.txt
rc=$?
echo "./report returned $rc"
cat rpt.txt && rm -f rpt.txt

testcase "Ensure that the report file is overwritten"
echo "If you see this, this test case has failed" > rpt.txt
echo ./report data.csv "Adaline Murphy" rpt.txt
./report data.csv "Adaline Murphy" rpt.txt
rc=$?
echo "./report returned $rc"
cat rpt.txt && rm -f rpt.txt

testcase "Locate a user with colaborators"
echo ./report data.csv "Adaline Murphy" rpt.txt
./report data.csv "Adaline Murphy" rpt.txt
rc=$?
echo "./report returned $rc"
cat rpt.txt && rm -f rpt.txt

testcase "Locate a user with colaborators - make sure to search the file from the beginning"
echo "  Only a maximum of half points for question 12 if this test case does not produce correct results."
echo ./report data.csv "Frederic Kelley" rpt.txt
./report data.csv "Frederic Kelley" rpt.txt
rc=$?
echo "./report returned $rc"
cat rpt.txt && rm -f rpt.txt


