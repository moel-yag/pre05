#!/bin/bash

GREEN='\033[0;32m'
RED='\033[0;31m'
CYAN='\033[0;36m'
NC='\033[0m'

gcc -Wall -Wextra -Werror main.c -o main 2>&1
if [ $? -ne 0 ]; then
    echo -e "${RED}Compilation failed!${NC}"
    exit 1
fi
echo -e "${GREEN}Compilation OK${NC}\n"

PASS=0
FAIL=0

# $'...' syntax allows explicit \n without multiline mess
run_test() {
    local name="$1"
    local input="$2"
    local h="$3"
    local w="$4"
    local it="$5"
    local expected="$6"
    
    output=$(echo -n "$input" | ./main "$h" "$w" "$it")
    
    if [ "$output" = "$expected" ]; then
        echo -e "${GREEN}✓ PASS${NC}: $name"
        ((PASS++))
    else
        echo -e "${RED}✗ FAIL${NC}: $name"
        echo "  Command: echo -n '$input' | ./main $h $w $it"
        echo -e "  ${CYAN}Expected (${#expected} chars):${NC}"
        printf '%s' "$expected" | cat -A | sed 's/^/    /'
        echo ""
        echo -e "  ${CYAN}Got (${#output} chars):${NC}"
        printf '%s' "$output" | cat -A | sed 's/^/    /'
        echo ""
        ((FAIL++))
    fi
}

echo "=== Functional Tests ==="

# 1. Block at top-left (still life)
run_test "Block still life" "xdsax" 4 4 1 $'00  \n00  \n    \n    '

# 2. Single cell dies
run_test "Single cell dies" "x" 3 3 1 $'   \n   \n   '

# 3. Blinker horizontal -> vertical (1 iter, middle of 5x5)
# ss: move to (0,2), x: draw, dd: draw (1,2),(2,2), x: pen off
run_test "Blinker H to V" "ssxddx" 5 5 1 $'     \n 0   \n 0   \n 0   \n     '

# 4. Blinker vertical -> horizontal (2 iters)
run_test "Blinker V to H (2 iters)" "ssxddx" 5 5 2 $'     \n     \n000  \n     \n     '

# 5. Zero iterations
run_test "Zero iterations" "x" 3 3 0 $'0  \n   \n   '

# 6. Empty board
run_test "Empty board" "" 2 2 5 $'  \n  '

# 7. L-shape -> block
run_test "L-shape to block" "xxdxaxsx" 3 3 1 $'00 \n00 \n   '

# 8. Overcrowding
run_test "Overcrowding" "xddssaaawd" 3 3 1 $'0 0\n   \n0 0'

echo ""
echo "=== Error Handling ==="

# Test wrong argc directly
echo -n "Wrong argc (3 args)... "
./main 3 3 > /dev/null 2>&1
if [ $? -eq 1 ]; then
    echo -e "${GREEN}PASS${NC}"
    ((PASS++))
else
    echo -e "${RED}FAIL${NC} (expected rc=1)"
    ((FAIL++))
fi

# Test negative iterations
echo -n "Negative iterations... "
./main 3 3 -1 > /dev/null 2>&1
if [ $? -eq 1 ]; then
    echo -e "${GREEN}PASS${NC}"
    ((PASS++))
else
    echo -e "${RED}FAIL${NC} (expected rc=1)"
    ((FAIL++))
fi

# Test zero height
echo -n "Zero height... "
./main 0 3 1 > /dev/null 2>&1
if [ $? -eq 1 ]; then
    echo -e "${GREEN}PASS${NC}"
    ((PASS++))
else
    echo -e "${RED}FAIL${NC} (expected rc=1)"
    ((FAIL++))
fi

# Test zero width
echo -n "Zero width... "
./main 3 0 1 > /dev/null 2>&1
if [ $? -eq 1 ]; then
    echo -e "${GREEN}PASS${NC}"
    ((PASS++))
else
    echo -e "${RED}FAIL${NC} (expected rc=1)"
    ((FAIL++))
fi

echo ""
echo "====================="
echo -e "Passed: ${GREEN}$PASS${NC}"
echo -e "Failed: ${RED}$FAIL${NC}"
echo "====================="

[ $FAIL -eq 0 ] && echo -e "${GREEN}All tests passed!${NC}" && exit 0
echo -e "${RED}Some tests failed.${NC}" && exit 1
