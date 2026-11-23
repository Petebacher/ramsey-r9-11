"""Build the 617-vertex graph for R(9,11) >= 618 from Google's 616-vertex R(9,11) graph.
Construction: survey item 2.3.h (Radziszowski, Small Ramsey Numbers, DS1 rev. 18). Google's file is a 2.3.h gluing of
G1 (rows 0..563, a (9,9)-graph) and G2 (rows 564..598, a (9,3)-graph) along a common induced subgraph of 17 vertices.
We use a different common induced subgraph M with 18 vertices, given in common_part_m18.txt as pairs u:y
(u = vertex of G2, 0..34; y = vertex of G1, 0..563). Output: 617 rows of 0/1, no separators.
Usage: python3 build.py input_google_R9_11_617.txt common_part_m18.txt out.txt   (standard library only)"""
import re, sys
src, mapf, out = sys.argv[1], sys.argv[2], sys.argv[3]
d = [int(c) for c in re.sub(r'[^01]', '', open(src).read())]
n = int(round(len(d) ** 0.5)); assert n * n == len(d) and n == 616
A = [d[i * n:(i + 1) * n] for i in range(n)]
n1, n2 = 564, 35
pairs = [tuple(map(int, p.split(':'))) for p in open(mapf).read().split()]
Y = [y for u, y in pairs]; U = [n1 + u for u, y in pairs]; m = len(pairs)
for i in range(m):
    for j in range(m):
        assert A[Y[i]][Y[j]] == A[U[i]][U[j]], 'not a common induced subgraph'
N = n1 + n2 + m
B = [[0] * N for _ in range(N)]
for i in range(n1 + n2):
    for j in range(n1 + n2):
        if (i < n1) == (j < n1):          # keep G1 and G2, no edges between them yet
            B[i][j] = A[i][j]
for (u, y) in pairs:                       # matching edge between the two copies of each M vertex
    B[y][n1 + u] = B[n1 + u][y] = 1
for k, (u, y) in enumerate(pairs):         # new vertex z_k: neighbours of y in G1 and of u in G2
    z = n1 + n2 + k
    for j in range(n1):
        B[z][j] = B[j][z] = A[y][j]
    for j in range(n2):
        B[z][n1 + j] = B[n1 + j][z] = A[n1 + u][n1 + j]
for a in range(m):                         # the new vertices induce M
    for b in range(m):
        B[n1 + n2 + a][n1 + n2 + b] = A[Y[a]][Y[b]]
assert all(B[i][j] == B[j][i] for i in range(N) for j in range(N)) and all(B[i][i] == 0 for i in range(N))
open(out, 'w').write('\n'.join(''.join(map(str, r)) for r in B) + '\n')
print('built', N, 'vertices, common part m =', m)
