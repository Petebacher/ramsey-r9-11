"""Build the graphs of this repository from Google's files, standard library only.

(a) R(9,11) >= 618 (v1.0): the 617-vertex graph, survey item 2.3.h (Radziszowski, Small Ramsey Numbers, DS1 rev. 18).
    Google's file is a 2.3.h gluing of G1 (rows 0..563, a (9,9)-graph) and G2 (rows 564..598, a (9,3)-graph) along a
    common induced subgraph of 17 vertices. We use a different common induced subgraph M with 18 vertices, given in
    common_part_m18.txt as pairs u:y (u = vertex of G2, 0..34; y = vertex of G1, 0..563). Output: 617 rows of 0/1, no separators.
    Usage: python3 build.py input_google_R9_11_617.txt common_part_m18.txt out.txt

(b) R(9,11) >= 620 (v1.1): Theorem 3 of Xu, Shao, Radziszowski, "More constructive lower bounds on
    classical Ramsey numbers", SIAM J. Discrete Math. 25 (2011) 394-400. The data file (theorem3_*.txt) names k, the sizes
    of G and H, where H sits in Google's file, the 20 pairs (u in G, v in H) and the split W1/W2 of M.
    The new vertices z_i are joined to N_G(u_i) and N_H(v_i); u_i is joined to v_i; z_i, z_j are joined when u_i u_j is an
    edge of G and i, j lie on the same side of the split. Output: n rows of n space-separated 0/1 entries, one row per line,
    no header line (the same layout as Google's matrices, without the brackets and line wrapping).
    Usage: python3 build.py t3 theorem3_R9_11_620.txt input_google_R9_11_617.txt out.txt
"""
import re, sys

def read_matrix(path):
    d = [int(c) for c in re.sub(r'[^01]', '', open(path).read())]
    n = int(round(len(d) ** 0.5)); assert n * n == len(d)
    return [d[i * n:(i + 1) * n] for i in range(n)]


def read_spec(path):
    spec = {}
    for line in open(path):
        line = line.split('#')[0].split()
        if line:
            spec[line[0]] = line[1:]
    return spec


def build_t3(specf, srcf, out):
    sp = read_spec(specf)
    nG, nH, h0 = int(sp['nG'][0]), int(sp['nH'][0]), int(sp['h_start'][0])
    pairs = [tuple(map(int, p.split(':'))) for p in sp['pairs']]
    W1 = set(map(int, sp['W1']))
    m = len(pairs)
    A = read_matrix(srcf)
    G = [r[:nG] for r in A[:nG]]
    H = [r[h0:h0 + nH] for r in A[h0:h0 + nH]]
    U = [u for u, v in pairs]; V = [v for u, v in pairs]
    for i in range(m):
        for j in range(m):
            assert G[U[i]][U[j]] == H[V[i]][V[j]], 'not a common induced subgraph'
    N = nG + nH + m
    B = [[0] * N for _ in range(N)]
    for i in range(nG):
        for j in range(nG):
            B[i][j] = G[i][j]
    for i in range(nH):
        for j in range(nH):
            B[nG + i][nG + j] = H[i][j]
    for i in range(m):                                  # matching u_i -- v_i
        B[U[i]][nG + V[i]] = B[nG + V[i]][U[i]] = 1
    for i in range(m):                                  # z_i copies the neighbours of u_i in G and of v_i in H
        z = nG + nH + i
        for j in range(nG):
            B[z][j] = B[j][z] = G[U[i]][j]
        for j in range(nH):
            B[z][nG + j] = B[nG + j][z] = H[V[i]][j]
        for j in range(m):                              # M[W1] and M[W2], no W1-W2 edges
            if (i in W1) == (j in W1):
                B[z][nG + nH + j] = G[U[i]][U[j]]
    assert all(B[i][j] == B[j][i] for i in range(N) for j in range(N)) and all(B[i][i] == 0 for i in range(N))
    open(out, 'w').write(''.join(' '.join(map(str, r)) + '\n' for r in B))
    print('built', N, 'vertices, common part m =', m)


if len(sys.argv) == 5 and sys.argv[1] == 't3':
    build_t3(sys.argv[2], sys.argv[3], sys.argv[4])
    sys.exit(0)


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
