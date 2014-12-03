#!/bin/bash

for file in proj_2_tests/*.txt; do
	expected="proj_2_tests/$(basename $file).out"
	if test -e $expected; then
		if (./main $file - | diff - $expected); then
			echo "[SUCCESS] $file"
		else
			echo "[FAILURE] $file did not match $expected"
			exit 1
		fi
	else
		echo "[SKIP] no out file for $file"
	fi
done

echo "All Tests Succeeded!"
