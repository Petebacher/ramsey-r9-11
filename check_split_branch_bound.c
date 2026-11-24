/* Second exact checker with a structured independent-set search (written 2026-11-23, from scratch; its clique routine is the same
   Tomita-style colouring branch and bound as check_branch_bound.c, so on the clique side it is NOT an independent method).
   Usage: check_split_branch_bound <matrix file> <n1> <k> <t>
   Part 1: decides whether a K_k exists (generic clique B&B with colouring bound, whole graph).
   Part 2: decides whether an independent t-set exists, structured:
     every independent set I splits as T = I \ X1 (T inside the small part, vertices >= n1) and I cap X1.
     We enumerate ALL independent sets T of the small part, and for each ask whether
     X1 minus N(T) contains an independent set of size t - |T| (exact decision search).
   Prints PASS only if no K_k and no independent t-set. */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#define MAXN 896
#define W 14
typedef struct { uint64_t w[W]; } bs;
static int n, n1;
static bs adj[MAXN], nadj[MAXN];
static long long nodes;
static int found;
static inline void bset(bs *b, int i) { b->w[i >> 6] |= 1ULL << (i & 63); }
static inline void bclr(bs *b, int i) { b->w[i >> 6] &= ~(1ULL << (i & 63)); }
static inline int bget(const bs *b, int i) { return (b->w[i >> 6] >> (i & 63)) & 1; }
static inline int bempty(const bs *b) { for (int k = 0; k < W; k++) if (b->w[k]) return 0; return 1; }
static inline bs band(const bs *a, const bs *b) { bs r; for (int k = 0; k < W; k++) r.w[k] = a->w[k] & b->w[k]; return r; }
static inline int bfirst(const bs *b) { for (int k = 0; k < W; k++) if (b->w[k]) return k * 64 + __builtin_ctzll(b->w[k]); return -1; }

/* does graph g contain a clique of size >= need inside P (already have sz)? */
static void dec(bs *g, bs P, int sz, int need) {
  if (found) return;
  nodes++;
  if (sz >= need) { found = 1; return; }
  int order[MAXN], col[MAXN], m = 0, c = 0;
  bs U = P;
  while (!bempty(&U)) {
    c++;
    bs Q = U;
    while (!bempty(&Q)) {
      int v = bfirst(&Q);
      bclr(&Q, v); bclr(&U, v);
      for (int k = 0; k < W; k++) Q.w[k] &= ~g[v].w[k];
      order[m] = v; col[m] = c; m++;
    }
  }
  for (int i = m - 1; i >= 0 && !found; i--) {
    if (sz + col[i] < need) return;
    int v = order[i];
    dec(g, band(&P, &g[v]), sz + 1, need);
    bclr(&P, v);
  }
}

static int t;
static long long nT = 0, nTchecked = 0;
static int Tlist[64];
/* enumerate independent sets of small part; cand = small-part vertices still allowed (index > last, not adjacent) */
static void enumT(int last, int size, bs R /* allowed X1 vertices */) {
  if (found) return;
  nT++;
  int need = t - size;
  if (need <= 0) { found = 1; fprintf(stderr, "independent set entirely in small part of size %d\n", size); return; }
  nTchecked++;
  dec(nadj, R, 0, need);
  if (found) {
    fprintf(stderr, "FOUND independent %d-set: T =", t);
    for (int i = 0; i < size; i++) fprintf(stderr, " %d", Tlist[i]);
    fprintf(stderr, " plus %d vertices of X1\n", need);
    return;
  }
  for (int v = last + 1; v < n; v++) {
    int ok = 1;
    for (int i = 0; i < size; i++) if (bget(&adj[v], Tlist[i])) { ok = 0; break; }
    if (!ok) continue;
    Tlist[size] = v;
    bs R2 = band(&R, &nadj[v]);
    enumT(v, size + 1, R2);
  }
}

int main(int argc, char **argv) {
  if (argc < 5) { fprintf(stderr, "usage: check_split_branch_bound file n1 k t\n"); return 2; }
  n1 = atoi(argv[2]); int k = atoi(argv[3]); t = atoi(argv[4]);
  FILE *f = fopen(argv[1], "r"); if (!f) { perror("open"); return 2; }
  static unsigned char a[MAXN * MAXN]; int cnt = 0, ch;
  while ((ch = fgetc(f)) != EOF) if (ch == '0' || ch == '1') { if (cnt >= MAXN * MAXN) { fprintf(stderr, "too big\n"); return 2; } a[cnt++] = ch - '0'; }
  fclose(f);
  n = 0; while (n * n < cnt) n++;
  if (n * n != cnt) { fprintf(stderr, "not square\n"); return 2; }
  int bad = 0;
  for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) {
    if (a[i * n + j] != a[j * n + i] || (i == j && a[i * n + j])) bad++;
    if (i != j && a[i * n + j]) bset(&adj[i], j);
    if (i != j && !a[i * n + j]) bset(&nadj[i], j);
  }
  printf("n=%d n1=%d symmetry/diagonal errors=%d\n", n, n1, bad); fflush(stdout);
  bs all; memset(&all, 0, sizeof all); for (int i = 0; i < n; i++) bset(&all, i);
  found = 0; nodes = 0; dec(adj, all, 0, k);
  int hasK = found;
  printf("K_%d exists: %s (nodes %lld)\n", k, hasK ? "YES" : "no", nodes); fflush(stdout);
  bs X1; memset(&X1, 0, sizeof X1); for (int i = 0; i < n1; i++) bset(&X1, i);
  found = 0; nodes = 0;
  enumT(n1 - 1, 0, X1);
  int hasI = found;
  printf("independent %d-set exists: %s (small-part independent sets enumerated %lld, nodes %lld)\n", t, hasI ? "YES" : "no", nT, nodes);
  int pass = !bad && !hasK && !hasI;
  printf("%s: R(%d,%d) >= %d\n", pass ? "PASS" : "FAIL", k, t, pass ? n + 1 : -1);
  return pass ? 0 : 1;
}
