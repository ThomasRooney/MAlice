#!/bin/bash


VALIDFILES=../malice_examples/valid/*.alice
INVALIDFILES=../malice_examples/invalid/*.alice

OUTPUT=autotest_output.txt

RUNNABLE = true

echo "Sanity Check"
echo "------------"

echo -ne "Does compile exist..."
if [ -f compile ];
then
echo  "yes"
else
echo "no"
RUNNABLE = false
fi

echo "Checking the reference compiler is accessible..."
command -v MAlice >/dev/null 2>&1 || { echo "The Reference Compiler needs to be available on the command line." >&2;RUNNABLE=false; }

echo -ne "Checking the malice_examples is located at ../malice_examples ..."
if [ -d "../malice_examples" ];
then 
echo "yes"
else
echo "no"
RUNNABLE = false
fi

echo -ne "Checking the valid directory is located at ../malice_examples/valid ..."
if [ -d "../malice_examples/valid" ];
then
echo "yes"
else
echo "no"
RUNNABLE=false
fi

echo -ne "Checking the invalid directory is located at ../malice_examples/invalid ..."
if [ -d "../malice_examples/invalid" ];
then
echo "yes"
else
echo "no"
RUNNABLE=false
fi

if $RUNNABLE ; then
echo "Everything setup correctly"
else
echo "Please correct setup"
exit $1 ;
fi

rm $OUTPUT

for vF in $VALIDFILES
do
echo "Processing Valid File: $vF"  >> $OUTPUT
echo "Processing Valid File: $vF"
echo "---------------------" >> $OUTPUT
echo "Reference Compiler Output:" >> $OUTPUT
echo "---------------------" >> $OUTPUT
MAlice $vF >> $OUTPUT
echo "---------------------" >> $OUTPUT
echo "Student Compiler Output:" >> $OUTPUT
echo "---------------------" >> $OUTPUT
./compile $vF >> $OUTPUT
echo "---------------------" >> $OUTPUT
done

for vF in $INVALIDFILES
do
echo "Processing Invalid File: $vF"  >> $OUTPUT
echo "Processing Invalid File: $vF"
echo "---------------------" >> $OUTPUT
echo "Reference Compiler Output:" >> $OUTPUT
echo "---------------------" >> $OUTPUT
MAlice $vF >> $OUTPUT
echo "---------------------" >> $OUTPUT
echo "Student Compiler Output:" >> $OUTPUT
echo "---------------------" >> $OUTPUT
./compile $vF &>> $OUTPUT
echo "---------------------" >> $OUTPUT
done
