#!/bin/bash

# =============================================================================
# BSQ Test Suite — Based on subject.txt specification
# =============================================================================
# Tests cover:
#   1. Subject example (the exact example from the PDF)
#   2. Smallest valid map (1x1)
#   3. Top-left priority (when multiple equal squares exist)
#   4. Full obstacle map (biggest square = 0, just reprint)
#   5. No obstacles (entire map is the square)
#   6. Multiple files as arguments
#   7. Invalid maps → "map error" on stderr
#   8. Stdin reading (no arguments)
#   9. Different printable characters for empty/obstacle/full
#  10. Edge cases (single row, single column, rectangular maps)
# =============================================================================

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
BOLD='\033[1m'
RESET='\033[0m'

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
BSQ_DIR="$(dirname "$SCRIPT_DIR")"
MAPS_DIR="$SCRIPT_DIR/maps"
EXPECTED_DIR="$SCRIPT_DIR/expected"
BSQ_BIN="$SCRIPT_DIR/bsq"

PASS=0
FAIL=0
TOTAL=0

# ---------- compile ----------
echo -e "${CYAN}${BOLD}══════════════════════════════════════════${RESET}"
echo -e "${CYAN}${BOLD}        BSQ Test Suite (subject-based)     ${RESET}"
echo -e "${CYAN}${BOLD}══════════════════════════════════════════${RESET}"
echo ""

echo -e "${YELLOW}Compiling...${RESET}"
cc -Wall -Wextra -Werror -o "$BSQ_BIN" "$BSQ_DIR"/BSQ.c 2>&1
if [ $? -ne 0 ]; then
    echo -e "${RED}Compilation failed!${RESET}"
    exit 1
fi
echo -e "${GREEN}Compilation OK${RESET}"
echo ""

# ---------- helpers ----------
run_test() {
    local name="$1"
    local args="$2"          # file arguments (space separated), empty = stdin
    local stdin_file="$3"    # file to pipe into stdin (empty = none)
    local expected_out="$4"  # expected stdout file
    local expected_err="$5"  # expected stderr file (empty = expect nothing)

    TOTAL=$((TOTAL + 1))

    local actual_out=$(mktemp)
    local actual_err=$(mktemp)

    if [ -n "$stdin_file" ]; then
        "$BSQ_BIN" $args < "$stdin_file" > "$actual_out" 2> "$actual_err"
    else
        "$BSQ_BIN" $args > "$actual_out" 2> "$actual_err"
    fi

    local out_ok=true
    local err_ok=true

    # Compare stdout
    if [ -n "$expected_out" ] && [ -f "$expected_out" ]; then
        diff -q "$expected_out" "$actual_out" > /dev/null 2>&1
        if [ $? -ne 0 ]; then
            out_ok=false
        fi
    fi

    # Compare stderr
    if [ -n "$expected_err" ] && [ -f "$expected_err" ]; then
        diff -q "$expected_err" "$actual_err" > /dev/null 2>&1
        if [ $? -ne 0 ]; then
            err_ok=false
        fi
    elif [ -z "$expected_err" ]; then
        # Expect empty stderr
        if [ -s "$actual_err" ]; then
            err_ok=false
        fi
    fi

    if $out_ok && $err_ok; then
        echo -e "  ${GREEN}✓ PASS${RESET}  $name"
        PASS=$((PASS + 1))
    else
        echo -e "  ${RED}✗ FAIL${RESET}  $name"
        FAIL=$((FAIL + 1))
        if ! $out_ok; then
            echo -e "    ${RED}── stdout diff ──${RESET}"
            diff --color=always "$expected_out" "$actual_out" | head -20 | sed 's/^/    /'
        fi
        if ! $err_ok; then
            echo -e "    ${RED}── stderr diff ──${RESET}"
            if [ -n "$expected_err" ] && [ -f "$expected_err" ]; then
                diff --color=always "$expected_err" "$actual_err" | head -20 | sed 's/^/    /'
            else
                echo -e "    Expected empty stderr, got:"
                head -5 "$actual_err" | sed 's/^/      /'
            fi
        fi
    fi

    rm -f "$actual_out" "$actual_err"
}

# =============================================================================
# SECTION 1 — VALID MAPS (correct output on stdout, nothing on stderr)
# =============================================================================
echo -e "${BOLD}─── Valid Maps ───${RESET}"

run_test \
    "Subject example (9×27, 7×7 square)" \
    "$MAPS_DIR/01_subject_example.txt" \
    "" \
    "$EXPECTED_DIR/01_subject_example.txt" \
    ""

run_test \
    "1×1 empty cell → entire map is the square" \
    "$MAPS_DIR/02_one_cell_empty.txt" \
    "" \
    "$EXPECTED_DIR/02_one_cell_empty.txt" \
    ""

run_test \
    "1×1 obstacle cell → no square, just reprint" \
    "$MAPS_DIR/03_one_cell_obstacle.txt" \
    "" \
    "$EXPECTED_DIR/03_one_cell_obstacle.txt" \
    ""

run_test \
    "No obstacles → entire map is the square (5×5)" \
    "$MAPS_DIR/04_no_obstacles.txt" \
    "" \
    "$EXPECTED_DIR/04_no_obstacles.txt" \
    ""

run_test \
    "Full obstacles → no square drawn (3×3)" \
    "$MAPS_DIR/05_all_obstacles.txt" \
    "" \
    "$EXPECTED_DIR/05_all_obstacles.txt" \
    ""

run_test \
    "Top-left priority: two equal squares, pick top-left" \
    "$MAPS_DIR/06_top_left_priority.txt" \
    "" \
    "$EXPECTED_DIR/06_top_left_priority.txt" \
    ""

run_test \
    "Rectangular map — wider than tall (3×10)" \
    "$MAPS_DIR/07_wide_rectangle.txt" \
    "" \
    "$EXPECTED_DIR/07_wide_rectangle.txt" \
    ""

run_test \
    "Rectangular map — taller than wide (10×3)" \
    "$MAPS_DIR/08_tall_rectangle.txt" \
    "" \
    "$EXPECTED_DIR/08_tall_rectangle.txt" \
    ""

run_test \
    "Single row (1×20)" \
    "$MAPS_DIR/09_single_row.txt" \
    "" \
    "$EXPECTED_DIR/09_single_row.txt" \
    ""

run_test \
    "Single column (10×1)" \
    "$MAPS_DIR/10_single_column.txt" \
    "" \
    "$EXPECTED_DIR/10_single_column.txt" \
    ""

run_test \
    "Custom characters: digits and symbols (0=empty 1=obstacle 2=full)" \
    "$MAPS_DIR/11_custom_chars.txt" \
    "" \
    "$EXPECTED_DIR/11_custom_chars.txt" \
    ""

run_test \
    "Large open area with obstacle island (10×20)" \
    "$MAPS_DIR/12_obstacle_island.txt" \
    "" \
    "$EXPECTED_DIR/12_obstacle_island.txt" \
    ""

run_test \
    "Square at bottom-right corner" \
    "$MAPS_DIR/13_bottom_right.txt" \
    "" \
    "$EXPECTED_DIR/13_bottom_right.txt" \
    ""

run_test \
    "Obstacle diagonal — biggest square is 1×1 at top-left" \
    "$MAPS_DIR/14_diagonal_obstacles.txt" \
    "" \
    "$EXPECTED_DIR/14_diagonal_obstacles.txt" \
    ""

run_test \
    "3×3 square possible among obstacles" \
    "$MAPS_DIR/15_3x3_among_obstacles.txt" \
    "" \
    "$EXPECTED_DIR/15_3x3_among_obstacles.txt" \
    ""

echo ""

# =============================================================================
# SECTION 2 — INVALID MAPS (expect "map error\n" on stderr)
# =============================================================================
echo -e "${BOLD}─── Invalid Maps (expect \"map error\" on stderr) ───${RESET}"

run_test \
    "Missing header line (no first line info)" \
    "$MAPS_DIR/err_01_no_header.txt" \
    "" \
    "$EXPECTED_DIR/err_empty_stdout.txt" \
    "$EXPECTED_DIR/err_map_error.txt"

run_test \
    "Inconsistent line lengths" \
    "$MAPS_DIR/err_02_unequal_lines.txt" \
    "" \
    "$EXPECTED_DIR/err_empty_stdout.txt" \
    "$EXPECTED_DIR/err_map_error.txt"

run_test \
    "Duplicate characters (empty == obstacle)" \
    "$MAPS_DIR/err_03_duplicate_chars.txt" \
    "" \
    "$EXPECTED_DIR/err_empty_stdout.txt" \
    "$EXPECTED_DIR/err_map_error.txt"

run_test \
    "Invalid character in map body" \
    "$MAPS_DIR/err_04_invalid_char.txt" \
    "" \
    "$EXPECTED_DIR/err_empty_stdout.txt" \
    "$EXPECTED_DIR/err_map_error.txt"

run_test \
    "Height is 0" \
    "$MAPS_DIR/err_05_zero_height.txt" \
    "" \
    "$EXPECTED_DIR/err_empty_stdout.txt" \
    "$EXPECTED_DIR/err_map_error.txt"

run_test \
    "Height mismatch — fewer lines than header says" \
    "$MAPS_DIR/err_06_height_mismatch.txt" \
    "" \
    "$EXPECTED_DIR/err_empty_stdout.txt" \
    "$EXPECTED_DIR/err_map_error.txt"

run_test \
    "Nonexistent file" \
    "this_file_does_not_exist_at_all.txt" \
    "" \
    "$EXPECTED_DIR/err_empty_stdout.txt" \
    "$EXPECTED_DIR/err_map_error.txt"

run_test \
    "Empty file (0 bytes)" \
    "$MAPS_DIR/err_07_empty_file.txt" \
    "" \
    "$EXPECTED_DIR/err_empty_stdout.txt" \
    "$EXPECTED_DIR/err_map_error.txt"

run_test \
    "Negative height" \
    "$MAPS_DIR/err_08_negative_height.txt" \
    "" \
    "$EXPECTED_DIR/err_empty_stdout.txt" \
    "$EXPECTED_DIR/err_map_error.txt"

run_test \
    "Missing full character in header" \
    "$MAPS_DIR/err_09_missing_full_char.txt" \
    "" \
    "$EXPECTED_DIR/err_empty_stdout.txt" \
    "$EXPECTED_DIR/err_map_error.txt"

echo ""

# =============================================================================
# SECTION 3 — MULTIPLE FILES
# =============================================================================
echo -e "${BOLD}─── Multiple File Arguments ───${RESET}"

run_test \
    "Two valid files" \
    "$MAPS_DIR/01_subject_example.txt $MAPS_DIR/04_no_obstacles.txt" \
    "" \
    "$EXPECTED_DIR/multi_01_two_valid.txt" \
    ""

run_test \
    "Valid + invalid + valid (error sandwiched)" \
    "$MAPS_DIR/02_one_cell_empty.txt $MAPS_DIR/err_01_no_header.txt $MAPS_DIR/03_one_cell_obstacle.txt" \
    "" \
    "$EXPECTED_DIR/multi_02_valid_invalid_valid.txt" \
    "$EXPECTED_DIR/err_map_error.txt"

echo ""

# =============================================================================
# SECTION 4 — STDIN READING
# =============================================================================
echo -e "${BOLD}─── Stdin Reading (no file arguments) ───${RESET}"

run_test \
    "Read valid map from stdin" \
    "" \
    "$MAPS_DIR/02_one_cell_empty.txt" \
    "$EXPECTED_DIR/02_one_cell_empty.txt" \
    ""

echo ""

# =============================================================================
# SUMMARY
# =============================================================================
echo -e "${BOLD}══════════════════════════════════════════${RESET}"
echo -e "  Total: ${BOLD}$TOTAL${RESET}  |  ${GREEN}Passed: $PASS${RESET}  |  ${RED}Failed: $FAIL${RESET}"
echo -e "${BOLD}══════════════════════════════════════════${RESET}"

rm -f "$BSQ_BIN"

if [ $FAIL -eq 0 ]; then
    echo -e "\n${GREEN}${BOLD}All tests passed! ✓${RESET}\n"
    exit 0
else
    echo -e "\n${RED}${BOLD}Some tests failed ✗${RESET}\n"
    exit 1
fi
