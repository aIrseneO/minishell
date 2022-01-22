#!/bin/bash

################################################################################
#     Copyright (c) 2022 Arsene Temfack                                        #
#                                                                              #
#     SPDX-License-Identifier: MIT                                             #
################################################################################

MSG="Run a serie of shell commands on two binaries and compare the output.\n
   Check this script for configurations.\n
   Commands are stored in all files '*.cf'\n
   in the same directory as the script.\n\n
\tUsage: $0 [ --help | -h | help ] [common-shell personnilize-shell workdir]\n
\t\t e.g.: $0 bash minishell /minishell/test"

if [[ "$#" = "1" ]] &&
		[[ "$1" = "--help" || "$1" = "-h" || "$1" = "help" ]]; then
	echo -e $MSG; exit 0
fi

SHELL=$1
MY_SHELL=$2
WORKDIR=$3

if [[ "$#" != "3" ]] || \
	   [[ ! -f $(which $SHELL) ]] || [[ ! -f $(which $MY_SHELL) ]] || \
	   [[ ! -d $WORKDIR ]]; then
	echo -e $MSG; exit 1
fi

############################## Modify as needed ################################
# Set valgrind on/off (default off).
VALGRIND=			#off
#VALGRIND=valgrind	#on

# Get numerated tests, not used just a useful command for troubleshouting.
##grep -n -v -e "^#" -e "^$" $FILE

################################################################################

# Result test file, it only includes failed tests,
# it will be deleted every time the script is successfully run.
RESULT_TEST=$WORKDIR/result-test.cf

# Get all the test files
FILES=$(ls $WORKDIR/*.cf | grep -v $RESULT_TEST)

GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[0;33m'

CHECK="\u2713"
CROSS="\u2717"

# Remove the result file
rm $RESULT_TEST

i=0
TESTS=0
# Loop through all lines in all test files
for FILE in $FILES; do

	TESTS=$(($TESTS + `grep -v -e "^#" -e "^$" $FILE | wc -l`))

	# Take only lines that don't start with a '#'
	grep -v -e "^#" -e "^$" $FILE | while read -r LINE; do
		
		i=$(($i + 1))
		
		# Execute the command with SHELL and take the exit number
		O_SHELL=$($SHELL -c "$LINE" 2>/dev/null)
		SHELL_EXIT=$?
		
		# Execute the command with minishell and take the exit number
		O_MY_SHELL=$($VALGRIND $MY_SHELL -c "$LINE" 2>/dev/null)
		MY_SHELL_EXIT=$?
		
		# check if the test is successful and act accordingly
		if [[ "$O_SHELL" == "$O_MY_SHELL" ]] && \
				[[ "$SYSTEM_EXIT" == "$MINISHELL_EXIT" ]]; then
			printf "$YELLOW test: $i  $GREEN ok $CHECK"
			#printf "$YELLOW test: $i - $LINE $GREEN ok $CHECK"
		else
			if [[ "$SHELL_EXIT" == "$MY_SHELL_EXIT" ]]; then
				printf "$YELLOW test: $i - Wrong exit code  $RED ko $CROSS"
				echo -e "# File: $FILE - Wrong exit code\n$LINE" >> $RESULT_TEST
			else
				printf "$YELLOW test: $i - Wrong output  $RED ko $CROSS"
				echo -e "# File: $FILE - Wrong output\n$LINE" >> $RESULT_TEST
			fi
		fi
		echo -e "\n"
	done
done

# Result
if [[ -f $RESULT_TEST ]]; then
	FAILED=$(wc -l $RESULT_TEST | awk '{printf("%d", $1 / 2)}')
	PASSED=$(($TESTS - $FAILED))
	printf "$YELLOW Tests: $TESTS\n$GREEN Passed: $PASSED $CHECK\n
		$RED Failed: $FAILED $CROSS\n"
else
	FAILED=0
	printf "$YELLOW Tests: $TESTS\n$GREEN Passed: $TESTS $CHECK\n"
fi

exit $FAILED
