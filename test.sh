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

run_test() {
    local name="$1"
    local input="$2"
    local w="$3"
    local h="$4"
    local it="$5"
    local expected="$6"
    
    output=$(echo -n "$input" | ./main "$w" "$h" "$it")
    
    if [ "$output" = "$expected" ]; then
        echo -e "${GREEN}✓ PASS${NC}: $name"
        ((PASS++))
    else
        echo -e "${RED}✗ FAIL${NC}: $name"
        echo "  Command: echo -n '$input' | ./main $w $h $it"
        echo -e "  ${CYAN}Expected (${#expected} chars):${NC}"
        printf '%s' "$expected" | cat -A | sed 's/^/    /'
        echo ""
        echo -e "  ${CYAN}Got (${#output} chars):${NC}"
        printf '%s' "$output" | cat -A | sed 's/^/    /'
        echo ""
        ((FAIL++))
    fi
}

echo "=== Official Subject Examples ==="

# sdxddssaaww: ring shape centered in 5x5
run_test "Subject: sdxddssaaww 5x5 it=0" "sdxddssaaww" 5 5 0 $'     \n OOO \n O O \n OOO \n     '

# dxss: vertical line at x=1 in 3x3
run_test "Subject: dxss 3x3 it=0" "dxss" 3 3 0 $' O \n O \n O '
run_test "Subject: dxss 3x3 it=1" "dxss" 3 3 1 $'   \nOOO\n   '
run_test "Subject: dxss 3x3 it=2" "dxss" 3 3 2 $' O \n O \n O '

echo ""
echo "=== Functional Tests ==="

run_test "Block still life" "xdsax" 4 4 1 $'OO  \nOO  \n    \n    '
run_test "Single cell dies" "x" 3 3 1 $'   \n   \n   '
run_test "Blinker H to V" "ssxddx" 5 5 1 $'     \n O   \n O   \n O   \n     '
run_test "Blinker V to H (2 iters)" "ssxddx" 5 5 2 $'     \n     \nOOO  \n     \n     '
run_test "Zero iterations" "x" 3 3 0 $'O  \n   \n   '
run_test "Empty board" "" 2 2 5 $'  \n  '
run_test "L-shape to block" "xxdxaxsx" 3 3 1 $'OO \nOO \n   '
run_test "Overcrowding" "xddssaaawd" 3 3 1 $'O O\n   \nO O'

echo ""
echo "=== Error Handling ==="

echo -n "Wrong argc (3 args)... "
./main 3 3 > /dev/null 2>&1
if [ $? -eq 1 ]; then
    echo -e "${GREEN}PASS${NC}"
    ((PASS++))
else
    echo -e "${RED}FAIL${NC} (expected rc=1)"
    ((FAIL++))
fi

echo -n "Negative iterations... "
./main 3 3 -1 > /dev/null 2>&1
if [ $? -eq 1 ]; then
    echo -e "${GREEN}PASS${NC}"
    ((PASS++))
else
    echo -e "${RED}FAIL${NC} (expected rc=1)"
    ((FAIL++))
fi

echo -n "Zero height... "
./main 5 0 1 > /dev/null 2>&1
if [ $? -eq 1 ]; then
    echo -e "${GREEN}PASS${NC}"
    ((PASS++))
else
    echo -e "${RED}FAIL${NC} (expected rc=1)"
    ((FAIL++))
fi

echo -n "Zero width... "
./main 0 5 1 > /dev/null 2>&1
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
