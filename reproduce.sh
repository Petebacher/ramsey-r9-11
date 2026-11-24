#!/bin/bash
# Rebuild the graphs from Google's file, verify the hypotheses of Theorem 3, and check the graphs with exact programs.
# Needs: python3 (standard library), gcc (with OpenMP for check_enumerate).
# Default run: build seconds, clique-side and G-block checks a few minutes. The long independent-set searches are optional:
#   bash reproduce.sh long     (about 40 minutes for each of the long searches on one core of a 2020s desktop)
set -e
cd "$(dirname "$0")"
sha256sum -c SHA256SUMS
echo "== Rebuild"
python3 build.py input_google_R9_11_617.txt common_part_m18.txt rebuilt_618.txt
cmp rebuilt_618.txt R9_11_618.txt
python3 build.py t3 theorem3_R9_11_620.txt input_google_R9_11_617.txt rebuilt_620.txt
cmp rebuilt_620.txt R9_11_620.txt
echo "rebuilt graphs are byte-identical to R9_11_618.txt and R9_11_620.txt"
echo "== Hypotheses of Theorem 3"
python3 verify_theorem3.py theorem3_R9_11_620.txt input_google_R9_11_617.txt R9_11_620.txt
gcc -O3 -o check_russian_doll check_russian_doll.c
gcc -O3 -fopenmp -o check_enumerate check_enumerate.c
gcc -O3 -o check_split_branch_bound check_split_branch_bound.c
gcc -O3 -o check_branch_bound check_branch_bound.c
echo "== Clique side (Russian-doll method; in clique mode the line 'clique number = N' is the exact clique number)"
./check_russian_doll R9_11_620.txt 9 c | tee out_620_c.txt; grep -q "exists: no" out_620_c.txt
echo "== Clique side (plain enumeration, counts every size)"
./check_enumerate R9_11_620.txt 9 c | tee out_620_ce.txt; grep -q "exists: no" out_620_ce.txt
echo "== G block (rows and columns 0..563): no K9 and no independent 9-set"
python3 - <<'PY'
import re
d = [int(c) for c in re.sub(r'[^01]', '', open("R9_11_620.txt").read())]
N = int(round(len(d) ** 0.5))
open("G564.txt", "w").write("".join(" ".join(map(str, d[i * N:i * N + 564])) + "\n" for i in range(564)))
PY
./check_enumerate G564.txt 9 c | tee out_G564_c.txt; grep -q "exists: no" out_G564_c.txt
./check_enumerate G564.txt 9 i | tee out_G564_i.txt; grep -q "exists: no" out_G564_i.txt
echo "== v1.0 graph (618), clique side"
./check_russian_doll R9_11_618.txt 9 c | tee out_c.txt; grep -q "exists: no" out_c.txt
if [ "$1" = "long" ]; then
  echo "== Independent-set side, Russian-doll method (long)"
  ./check_russian_doll R9_11_620.txt 11 i | tee out_620_i.txt; grep -q "exists: no" out_620_i.txt
  echo "== Second program (long)"
  ./check_split_branch_bound R9_11_620.txt 564 9 11 | tee out_620_sbb.txt; grep -q "^PASS" out_620_sbb.txt
  echo "== v1.0 graph (618), independent-set side and second program (long)"
  ./check_russian_doll R9_11_618.txt 11 i | tee out_i.txt; grep -q "exists: no" out_i.txt
  ./check_branch_bound R9_11_618.txt 9 11 | tee out_bb.txt; grep -q "^PASS" out_bb.txt
fi
echo "ALL CHECKS PASSED"
