#!/bin/bash
# Rebuild the graph from Google's file and check it with two independently written exact programs.
# Needs: python3 (standard library), gcc. Time: build seconds; clique checks about 1 minute each;
# independence checks about 1 hour each on one core of a 2020s desktop.
set -e
cd "$(dirname "$0")"
sha256sum -c SHA256SUMS
python3 build.py input_google_R9_11_617.txt common_part_m18.txt rebuilt.txt
cmp rebuilt.txt R9_11_618.txt
echo "rebuilt graph is identical to R9_11_618.txt"
gcc -O3 -o check_russian_doll check_russian_doll.c
gcc -O3 -o check_branch_bound check_branch_bound.c
echo "== Checker 1 (Ostergard Russian-doll, exact): clique side, then independence side"
echo "   (in clique mode it reports the clique number on the line labelled independence number, because it works on the complement)"
./check_russian_doll R9_11_618.txt 9 c | tee out_c.txt; grep -q "exists: no" out_c.txt
./check_russian_doll R9_11_618.txt 11 i | tee out_i.txt; grep -q "exists: no" out_i.txt
echo "== Checker 2 (Tomita-style branch and bound with colouring bound, exact)"
./check_branch_bound R9_11_618.txt 9 11 | tee out_bb.txt; grep -q "^PASS" out_bb.txt
echo "ALL CHECKS PASSED"
