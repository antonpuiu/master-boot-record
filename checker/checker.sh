#!/bin/bash

# Neatu Darius <neatudarius@gmail.com>
# Visan Radu <visanr95@gmail.com>

tests_task1=4
points_task1=(5 10 5 10)

tests_task2=3
points_task2=(5 5 10)

tests_task3=3
points_task3=(10 10 10)

tests_bonus=6
points_bonus=(1 5 2 5 2 5)

executable=mbr

total=0
test=0

make 

if test ! -x $executable; then
    echo "Executable file missing."
    exit 1
fi

echo "--------------- TASK 1 ------------------"
for (( test=1; test<=$tests_task1; test++ )) do
	cp ./task1/mbr-$test.bin ./mbr.bin
	cp ./task1/mbr-$test.ref ./mbr.ref

	./$executable 1 > mbr.out

	if diff -q mbr.ref mbr.out >/dev/null;  then
		echo "Test $test .................... passed."
		((total = $total + ${points_task1[$test-1]}))
	else 
		echo "Test $test .................... failed."
	fi
done

echo "--------------- TASK 2 ------------------"
for (( test=1; test<=$tests_task2; test++ )) do
	cp ./task2/mbr-$test.bin ./mbr.bin
	cp ./task2/mbr-$test.op ./mbr.op
	cp ./task2/mbr-$test.ref ./mbr.ref

	./$executable 2 > mbr.out

	if diff -q mbr.ref mbr.out >/dev/null;  then
		echo "Test $test .................... passed."
		((total = $total + ${points_task2[$test-1]}))
	else 
		echo "Test $test .................... failed."
	fi
done

echo "--------------- TASK 3 ------------------"
for (( test=1; test<=$tests_task3; test++ )) do
	cp ./task3/mbr-$test.bin ./mbr.bin
	cp ./task3/mbr-$test.op ./mbr.op
	cp ./task3/mbr-$test.ref ./mbr.ref

	./$executable 3 > mbr.out

	if diff -q mbr.ref mbr.out >/dev/null;  then
		echo "Test $test .................... passed."
		((total = $total + ${points_task3[$test-1]}))
	else 
		echo "Test $test .................... failed."
	fi
done

echo "--------------- BONUS ------------------"
for (( test=1; test<=$tests_bonus; test++ )) do
	cp ./bonus/mbr-$test.bin ./mbr.bin
	cp ./bonus/mbr-$test.op ./mbr.op
	cp ./bonus/mbr-$test.ref ./mbr.ref

	./$executable 4 > mbr.out

	if diff -q mbr.ref mbr.out >/dev/null;  then
		echo "Test $test .................... passed."
		((total = $total + ${points_bonus[$test-1]}))
	else 
		echo "Test $test .................... failed."
	fi
done

echo "TOTAL $total / 100"

rm mbr mbr.bin  mbr.ref



