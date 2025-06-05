#! /bin/bash

input_file=test/tester/inputs/input_file
ms_output_file=test/tester/outputs/ms_output_file
bash_output_file=test/tester/outputs/bash_output_file
diff_log=test/tester/outputs/diff_log
redir_folder=test/tester/redirs

>"$ms_output_file"
>"$bash_output_file"
>"$diff_log"

rm -f $redir_folder/*

while IFS='' read -r cmd || [ -n "$cmd" ]; do
	printf 'Command: %s\n' "$cmd" >>$ms_output_file
	if [[ "$cmd" == *"<<"* ]]; then
		limiter=$(printf '%s' "$cmd" | grep -oP '<<\s*\K([^\s]+)')
		printf '%s\n' "$cmd"
		while read -r cmd; do
			printf '%s\n' "$cmd"
			if [[ "$cmd" == "$limiter" ]]; then
			break
			fi
		done
	else
	printf '%s\n' "$cmd"
	fi | ./minishell 2>&1 | grep -v '>' >>$ms_output_file
	echo Exit value: ${PIPESTATUS[1]} >>$ms_output_file
	shopt -s nullglob
	redirs=($redir_folder/*)
	if [[ ${redirs[@]} > 0 ]]; then
		echo New files ↓
		for file in "${redirs[@]}"; do
		printf '%s : %s\n' "$file" "$(cat -A "$file")"
		done
	fi >>$ms_output_file
	shopt -u nullglob
	rm -f $redir_folder/*
	echo >>$ms_output_file
done <$input_file

rm -f $redir_folder/*

while IFS='' read -r cmd || [ -n "$cmd" ]; do
	printf 'Command: %s\n' "$cmd" >>$bash_output_file
	if [[ "$cmd" == *"<<"* ]]; then
		limiter=$(printf '%s' "$cmd" | grep -oP '<<\s*\K([^\s]+)')
		printf '%s\n' "$cmd"
		while read -r cmd; do
			printf '%s\n' "$cmd"
			if [[ "$cmd" == "$limiter" ]]; then
			break
			fi
		done
	else
	printf '%s\n' "$cmd"
	fi | bash 2>&1 | grep -v '>' >>$bash_output_file
	echo Exit value: ${PIPESTATUS[1]} >>$bash_output_file
	shopt -s nullglob
	redirs=($redir_folder/*)
	if [[ ${redirs[@]} > 0 ]]; then
		echo New files ↓
		for file in "${redirs[@]}"; do
		printf '%s : %s\n' "$file" "$(cat -A "$file")"
		done
	fi >>$bash_output_file
	shopt -u nullglob
	rm -f $redir_folder/*
	echo >>$bash_output_file
done <$input_file

#diff -U 2 --label '' --label '' $bash_output_file $ms_output_file > $diff_log
diff -U 2 "$bash_output_file" "$ms_output_file" | grep -vE '^(---|\+\+\+|@@ )' > "$diff_log"

if [ ${PIPESTATUS[0]} -eq 0 ]; then
	echo Success
	exit 0
else
	cat $diff_log
	exit 1
fi
