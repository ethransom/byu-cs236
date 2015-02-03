#!/bin/bash

run_suite() {
	for file in "$1"/*.txt; do
		expected="$1/$(basename $file).out"
		if test -e $expected; then
			if (./main $file | diff - $expected); then
				echo "[SUCCESS] $file"
			else
				echo "[FAILURE] $file did not match $expected"
				exit 1
			fi
		else
			echo "[SKIP] no out file for $file"
		fi
	done
}

run_suite inputs/project_1

echo "All Tests Succeeded!"
