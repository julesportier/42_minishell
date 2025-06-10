#!/bin/bash

# Minishell comparison test script
# Usage: ./test_minishell.sh

YOUR_SHELL="./minishell"
COMMANDS_FILE="./test/tester/inputs/input_file"
BASH_OUTPUT_FILE="./test/tester/outputs/bash_output_file"
MS_OUTPUT_FILE="./test/tester/outputs/ms_output_file"
RESULTS_FILE="./test/tester/outputs/diff_log"

# Create output directories if they don't exist
mkdir -p ./test/tester/outputs

# Check if files exist
if [ ! -x "$YOUR_SHELL" ]; then
    echo "Error: Minishell '$YOUR_SHELL' not found or not executable"
    echo "Make sure you've compiled minishell with 'make' first"
    exit 1
fi

if [ ! -f "$COMMANDS_FILE" ]; then
    echo "Error: Commands file '$COMMANDS_FILE' not found"
    exit 1
fi

echo "=== Minishell Comparison Test ===" > "$RESULTS_FILE"
echo "Minishell: $YOUR_SHELL" >> "$RESULTS_FILE"
echo "Commands file: $COMMANDS_FILE" >> "$RESULTS_FILE"
echo "Test started at: $(date)" >> "$RESULTS_FILE"
echo "" >> "$RESULTS_FILE"

# Clear output files
> "$BASH_OUTPUT_FILE"
> "$MS_OUTPUT_FILE"

total_tests=0
failed_tests=0

echo "Starting tests..."
echo "Results will be saved to: $RESULTS_FILE"

while IFS= read -r command || [ -n "$command" ]; do
    # Skip empty lines and comments
    if [[ -z "$command" || "$command" =~ ^[[:space:]]*# ]]; then
        continue
    fi

    total_tests=$((total_tests + 1))
    echo "Testing command $total_tests: $command"

    # Create temporary files for outputs
    your_output=$(mktemp)
    bash_output=$(mktemp)

    # Run command in minishell
    echo "$command" | timeout 10s "$YOUR_SHELL" | grep -v '❯' > "$your_output" 2>&1
    your_exit_code=$?

    # Filter out colored prompts and prompt lines from minishell output
    sed -e '/\[1;32m❯\[0m/d' -e '/\[1;31m❯ \[0m/d' -e '/Documents\/Projects\/minishell/d' "$your_output" > "${your_output}.filtered"

    # Append to minishell output file
    echo "Command: $command" >> "$MS_OUTPUT_FILE"
    cat "${your_output}.filtered" >> "$MS_OUTPUT_FILE"
    echo "Exit code: $your_exit_code" >> "$MS_OUTPUT_FILE"
    echo "---" >> "$MS_OUTPUT_FILE"

    # Run command in bash
    echo "$command" | timeout 10s bash > "$bash_output" 2>&1
    bash_exit_code=$?

    # Filter out any potential prompts from bash output (though bash shouldn't show them)
    sed -e '/\[1;32m❯\[0m/d' -e '/\[1;31m❯ \[0m/d' -e '/Documents\/Projects\/minishell/d' "$bash_output" > "${bash_output}.filtered"

    # Append to bash output file
    echo "Command: $command" >> "$BASH_OUTPUT_FILE"
    cat "${bash_output}.filtered" >> "$BASH_OUTPUT_FILE"
    echo "Exit code: $bash_exit_code" >> "$BASH_OUTPUT_FILE"
    echo "---" >> "$BASH_OUTPUT_FILE"

    # Compare filtered outputs
    your_content=$(cat "${your_output}.filtered")
    bash_content=$(cat "${bash_output}.filtered")

    if [ "$your_content" != "$bash_content" ] || [ "$your_exit_code" != "$bash_exit_code" ]; then
        failed_tests=$((failed_tests + 1))

        echo "DIFFERENCE FOUND - Test $total_tests" >> "$RESULTS_FILE"
        echo "Command: $command" >> "$RESULTS_FILE"
        echo "" >> "$RESULTS_FILE"

        echo "Minishell output:" >> "$RESULTS_FILE"
        echo "---" >> "$RESULTS_FILE"
        echo "$your_content" >> "$RESULTS_FILE"
        echo "---" >> "$RESULTS_FILE"
        echo "Minishell exit code: $your_exit_code" >> "$RESULTS_FILE"
        echo "" >> "$RESULTS_FILE"

        echo "Bash output:" >> "$RESULTS_FILE"
        echo "---" >> "$RESULTS_FILE"
        echo "$bash_content" >> "$RESULTS_FILE"
        echo "---" >> "$RESULTS_FILE"
        echo "Bash exit code: $bash_exit_code" >> "$RESULTS_FILE"
        echo "" >> "$RESULTS_FILE"
        echo "================================================" >> "$RESULTS_FILE"
        echo "" >> "$RESULTS_FILE"

        echo "  → FAILED (difference found)"
    else
        echo "  → PASSED"
    fi

    # Clean up temporary files
    rm -f "$your_output" "$bash_output" "${your_output}.filtered" "${bash_output}.filtered"

done < "$COMMANDS_FILE"

# Summary
echo "" >> "$RESULTS_FILE"
echo "=== TEST SUMMARY ===" >> "$RESULTS_FILE"
echo "Total tests: $total_tests" >> "$RESULTS_FILE"
echo "Passed: $((total_tests - failed_tests))" >> "$RESULTS_FILE"
echo "Failed: $failed_tests" >> "$RESULTS_FILE"
echo "Test completed at: $(date)" >> "$RESULTS_FILE"

echo ""
echo "=== TEST SUMMARY ==="
echo "Total tests: $total_tests"
echo "Passed: $((total_tests - failed_tests))"
echo "Failed: $failed_tests"
echo ""
echo "Output files created:"
echo "  Minishell outputs: $MS_OUTPUT_FILE"
echo "  Bash outputs: $BASH_OUTPUT_FILE"
echo "  Detailed differences: $RESULTS_FILE"

if [ $failed_tests -eq 0 ]; then
    echo "🎉 All tests passed!"
    exit 0
else
    echo "⚠️  $failed_tests test(s) failed. Check $RESULTS_FILE for details."
    exit 1
fi
