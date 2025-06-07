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
		if [[ "$limiter" =~ ^\'.*\'$ ]]; then
			limiter="${limiter#\'}"
			limiter="${limiter%\'}"
		elif [[ "$limiter" =~ ^\".*\"$ ]]; then
			limiter="${limiter#\"}"
			limiter="${limiter%\"}"
		fi
		printf '%s\n' "$cmd"
		while read -r cmd; do
			printf '%s\n' "$cmd"
			printf '%s\n' "$cmd" >>$ms_output_file
			if [[ "$cmd" == "$limiter" ]]; then
				echo ↓ >>$ms_output_file
				break;
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
		if [[ "$limiter" =~ ^\'.*\'$ ]]; then
			limiter="${limiter#\'}"
			limiter="${limiter%\'}"
		elif [[ "$limiter" =~ ^\".*\"$ ]]; then
			limiter="${limiter#\"}"
			limiter="${limiter%\"}"
		fi
		printf '%s\n' "$cmd"
		while read -r cmd; do
			printf '%s\n' "$cmd"
			printf '%s\n' "$cmd" >>$bash_output_file
			if [[ "$cmd" == "$limiter" ]]; then
				echo ↓ >>$bash_output_file
				break;
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
# diff -U 5 "$bash_output_file" "$ms_output_file" | grep -vE '^(---|\+\+\+|@@ )' > "$diff_log"

if [ ${PIPESTATUS[0]} -eq 0 ]; then
	exit_value=0
else
	exit_value=1
fi

# Find differences

temp_dir=test/tester/outputs/temp_dir
mkdir -p "$temp_dir"

# Splits blocsk to single files
awk -v RS= -v dir="$temp_dir" '{print > (dir "/file1_block_" NR ".txt")}' "$ms_output_file"
awk -v RS= -v dir="$temp_dir" '{print > (dir "/file2_block_" NR ".txt")}' "$bash_output_file"

# Count number of blocks
blocks1=$(ls "$temp_dir"/file1_block_*.txt 2>/dev/null | wc -l)
blocks2=$(ls "$temp_dir"/file2_block_*.txt 2>/dev/null | wc -l)
max_blocks=$(( blocks1 > blocks2 ? blocks1 : blocks2 ))

# Clear the output file
> "$diff_log"

# Compare each block
diff_nbr=0
for ((i=1; i<=max_blocks; i++)); do
    f1="$temp_dir/file1_block_$i.txt"
    f2="$temp_dir/file2_block_$i.txt"

    [[ -f $f1 ]] || touch "$f1"
    [[ -f $f2 ]] || touch "$f2"

    if ! diff -q "$f1" "$f2" > /dev/null; then
        echo "Difference in block $i:" >> "$diff_log"
        echo "---- file1 ----" >> "$diff_log"
        cat "$f1" >> "$diff_log"
        echo "---- file2 ----" >> "$diff_log"
        cat "$f2" >> "$diff_log"
        echo "-----------------------------" >> "$diff_log"
        echo "" >> "$diff_log"
		diff_nbr=$(($diff_nbr + 1))
    fi
done

# Clean up the temp directory
rm -r "$temp_dir"

cat $diff_log
echo Differences found : $diff_nbr
exit $exit_value