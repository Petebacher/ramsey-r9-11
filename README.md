# R(9,11) ≥ 620

This repository gives a graph on 619 vertices that contains no clique of 9 vertices and no independent set of 11 vertices. It shows that the Ramsey number R(9,11) is at least 620.

Version 1.1 adds this record. Version 1.0 (R(9,11) ≥ 618, a 617-vertex graph) is kept in this repository as history. A companion result, R(10,12) ≥ 857, is released separately at https://github.com/Petebacher/ramsey-r10-12.

## Results

| Bound | Graph file | Vertices | Previous best public bound we could find | Gain |
|---|---|---|---|---|
| R(9,11) ≥ 620 | `R9_11_620.txt` | 619 | 618 (our own v1.0, 23 November 2026); before that 617 (Google Research) | +2 over v1.0, +3 over Google |
| R(9,11) ≥ 618 (superseded by 620) | `R9_11_618.txt` | 617 | 617 (Google Research) | +1 |

Prior art. Google Research published R(9,11) ≥ 617 (a 616-vertex graph) on 22 October 2026 in `google-research/google-research`, folder `ramsey_number_bounds/improved_bounds`, file `R(9, 11) >= 617.txt` (commit 0d94f8c). The Leaps in Bounds tracker lists that value. The dynamic survey *Small Ramsey Numbers* (S. P. Radziszowski, DS1 revision 18, April 2026) prints no lower bound for this cell. These are the best public bounds we could find as of 24 November 2026; we searched the web, arXiv, Zenodo, GitHub, the Leaps in Bounds tracker and the repositories of other recent contributors. We did not search social media, MathOverflow or mailing lists, so this is not a claim about everything that is known.

## Where the graph comes from

The graph uses **Theorem 3** of Xu Xiaodong, Shao Zehui and S. P. Radziszowski, "More constructive lower bounds on classical Ramsey numbers", SIAM J. Discrete Math. 25 (2011) 394-400. We read the theorem in the authors' preprint (dated 14 September 2010); the theorem number is as in the preprint, and we could not check it against the journal version. The theorem says:

> Let k ≥ 4 and s, t ≥ 2. Let G be a (k,s)-graph (no K_k, no independent s-set) and H a (k,t)-graph. Let M be a graph isomorphic to an induced subgraph of both G and H. If the vertex set of M can be split into two nonempty sets W1 and W2 so that for i = 1, 2 and every vertex v of M outside Wi the graph M[Wi + v] has no K_(k−1), then R(k, s+t−1) ≥ n(G) + n(H) + n(M) + 1.

The inputs are not new. G is the 564-vertex block (rows 0 to 563) and H the 35-vertex block (rows 564 to 598) of Google's 616-vertex R(9,11) file. **The new part is the larger common subgraph M (20 vertices) together with a valid split.**

| | R(9,11) ≥ 620 |
|---|---|
| k, s, t | 9, 9, 3 |
| G | 564 vertices, a (9,9)-graph (281-regular) |
| H | 35 vertices, clique number 8, independence number 2, so a (9,3)-graph |
| M | 20 vertices, 140 edges, clique number 8 |
| split | 14 + 6 |
| largest clique in any M[Wi + v] | 7 (limit k−2 = 7) |
| count | 564 + 35 + 20 = 619 |
| s + t − 1 | 9 + 3 − 1 = 11 |

Note: M contains a K8, so item 2.3.h of the survey (the rule used for 618, which needs a common part without K_(k−1) = K8) does not apply to this graph. The split in Theorem 3 is what makes it work.

The pairing and the split are a plain file, `theorem3_R9_11_620.txt`. It lists the 20 pairs u:v (u a vertex of G, v the matching vertex of H) and the split. M was found by local search for large common induced subgraphs under the full Theorem 3 condition.

## The graph F built from the data

F has the vertices of G, then the vertices of H, then 20 new vertices z_1 ... z_20, one per pair (u_i, v_i):
- the edges of G and of H are kept;
- u_i is joined to v_i (a matching; these are the only edges between G and H);
- z_i is joined to the neighbours of u_i in G and to the neighbours of v_i in H (not to u_i or v_i themselves);
- z_i and z_j are joined when u_i u_j is an edge of G and i, j are on the same side of the split.
`build.py` does this using the Python standard library only.

## Why it works (Theorem 3, short form)

- **No K_k.** A clique that meets both G and H has exactly one vertex u_j in G and one v_j in H, since only matching edges join them. Its other k−2 vertices cannot be in both parts of the split, so they are new vertices z_i with all their indices i in one Wi, and those indices together with j form a K_(k−1) in M[Wi + j] (or in M[Wi], when j itself is in Wi). The split condition excludes this. A clique missing H or G is mapped to a clique of G (or H) of the same size by replacing each z_j by u_j (v_j), so it has fewer than k vertices.
- **No independent set of size s + t − 1.** Split an independent set I into its parts in G, in H, in W1 and in W2, and sort the G and H vertices by whether their partner z is in I. The parts that sit next to G embed in G as one independent set, giving at most s − 1 vertices; the others embed in H, giving at most t − 1. So |I| ≤ s + t − 2.

The full proof is in the paper. The direct checks below do not depend on it.

## Verification

Everything below was run on the files in this repository (the sha256 values are in `SHA256SUMS`).

**File format.** `R9_11_620.txt` is 619 lines, each a row of 619 entries 0/1 separated by single spaces. There is no header line. (Google's own files use the same entries inside `[[ ... ]]` with wrapped lines. `R9_11_618.txt` is the older layout, rows of 0/1 with no separators.) All checkers read every `0` and `1` character of the file and nothing else, so do not add a header line.

**1. Hypotheses of Theorem 3** (`verify_theorem3.py`, standard library only, seconds): F is symmetric with zero diagonal; its G and H blocks are the stated rows of Google's file; the G-H edges are exactly the matching; every z_i has exactly the neighbourhoods N_G(u_i), N_H(v_i); M is the same graph in G and in H; Z = M[W1] + M[W2] with no W1-W2 edge; the split condition holds; H is a (9,3)-graph; the counts add up. All pass (`logs/verify_theorem3_R9_11_620.log`).

**2. G is a (9,9)-graph.** Not done in pure Python (too slow). Done with `check_enumerate` (exhaustive enumeration, no pruning): G has no K9 and no independent 9-set (`logs/enumerate_G564_*.log`).

**3. Direct exact checks of the finished graph.** Every run says "no":

| Program | Method | R9_11_620.txt (619 vertices) |
|---|---|---|
| `check_russian_doll.c` | Östergård's Russian-doll method, exact | clique number exactly 8 (33 s); independence number exactly 10 (2364 s, 39 minutes) |
| `check_split_branch_bound.c` | branch and bound; independent sets via full enumeration of the small part | no K9 (84,110,586 nodes); no independent 11-set (11,452,390,026 nodes) |
| `check_enumerate.c` | plain enumeration with no pruning at all, counts every size | 0 cliques of size 9; 0 independent sets of size 11 |

Counts from `check_enumerate.c`, which anyone can match against a rerun: 276,922,186 cliques of size 8 and 0 of size 9; 29,994,093,590 independent sets of size 10 and 0 of size 11.

The searches also find sets one size below the target (cliques of size 8, independent sets of size 10), which works as a positive control. `logs/check_enumerate_R9_11_620_control_clique8_expect_YES.log` shows a deliberate control run (target 8, answer YES).

How independent the checks are: `check_split_branch_bound.c` uses the same colouring branch and bound for cliques as `check_branch_bound.c`, so on the clique side it is the same method; the Russian-doll method and the plain enumeration are different from it and from each other. `check_branch_bound.c` (the v1.0 program, shipped unchanged) was not rerun on the 619-vertex graph. A further private check program without a published source gave the same answers; we do not count it here.

**Logs.** `logs/` holds the finished runs. Notes:
- The logs named `rdoll_old_*`, `split_bb_*` and `enumerate_*` were made on files with identical content (same sha256) under earlier working names such as `t3_9_619_s11.txt`; the name inside the log is that working name (in the `enumerate_*` logs a directory path was removed from the first line). The long searches were not repeated.
- The `rdoll_old_*` logs were written by the version of `check_russian_doll.c` before a printing fix. In clique mode that version called the result "independence number" (it works on the complement, so the number it prints is the clique number), and it printed "K4 count: 0" even when the count was skipped for n > 300. The shipped `check_russian_doll.c` fixes only these printed labels; the search code is unchanged. The change is in `logs/check_russian_doll_label_fix.diff`. Read "independence number" in the clique-mode `rdoll_old_*` logs as "clique number", and ignore "K4 count: 0".
- The logs whose names start with `check_` were produced by the shipped programs on the shipped files on 24 November 2026 (clique sides only; the machine was busy, so the cpu times are longer than in the `rdoll_old_*` logs, with the same node counts: 995,556,528 for R9_11_620.txt). They show the corrected labels.

`reproduce.sh` runs the checksum check, both rebuilds with byte comparison, the hypothesis verifier, and the fast clique-side checks (a few minutes). The long independent-set searches are optional: `reproduce.sh long`.

## Files

- `R9_11_620.txt`: the new graph (format above). `R9_11_618.txt`: the v1.0 graph, kept as history.
- `input_google_R9_11_617.txt`: Google's 616-vertex R(9,11) graph, redistributed unchanged (renamed from `R(9, 11) >= 617.txt`) with attribution; see License.
- `theorem3_R9_11_620.txt`: pairing and split. `common_part_m18.txt`: the v1.0 pairs.
- `build.py`: rebuilds both graphs (usage at the top of the file). `verify_theorem3.py`: the hypothesis checker.
- `check_russian_doll.c` (usage `file T i|c`), `check_split_branch_bound.c` (usage `file n1 k t`), `check_enumerate.c` (usage `file T c|i`, needs OpenMP), `check_branch_bound.c` (v1.0, usage `file K T`).
- `reproduce.sh`, `SHA256SUMS`, `logs/`.

## Credit and method

The starting graph is Google Research's. Theorem 3 is due to Xu, Shao and Radziszowski. The new part is the 20-vertex common subgraph and its split, found by search, and the checks.

This work was directed by Peter J. Chrisbacher. The search, construction and checks were carried out with AI coding agents (Anthropic's Claude Code), working under fixed rules: every claim needs independently written exact checks and a check of published results before any announcement. A separate AI review re-derived the theorem's proof, re-checked every hypothesis and wrote the unpruned enumeration program that is shipped here as check_enumerate.c; the logs/enumerate_* files are that review's runs, so the enumeration is one of the three programs above, not a fourth.

## Changelog

- **v1.1 (24 November 2026).** Adds R(9,11) ≥ 620 (Theorem 3 with a 20-vertex common part and split), its pairing file, `verify_theorem3.py`, two more checker sources, and logs. `build.py` gains the `t3` mode. `check_russian_doll.c`: printed labels fixed (see above). R(9,11) ≥ 618 is superseded.
- **v1.0 (23 November 2026).** R(9,11) ≥ 618 by survey item 2.3.h with an 18-vertex common part.

## License

- Code (`build.py`, `verify_theorem3.py`, the checkers, `reproduce.sh`): MIT.
- The new graph, the pairing file and this text: CC BY 4.0.
- `input_google_R9_11_617.txt` is Google's file, redistributed unchanged with attribution under the terms of google-research/google-research (Apache 2.0 for source files, CC BY 4.0 for datasets; the Apache text is in `LICENSES/`). The graphs `R9_11_620.txt` and `R9_11_618.txt` contain blocks of Google's graph unchanged (plus new vertices and edges).

## How to cite

See `CITATION.cff`, or cite the Zenodo record for this repository (the concept DOI 10.5281/zenodo.24477511 covers all versions; version 1.0 is 10.5281/zenodo.24477512 and version 1.1 is 10.5281/zenodo.24510998).

---

# Earlier result in this repository (v1.0): R(9,11) ≥ 618

### The 618 construction (survey item 2.3.h)

The graph comes from item 2.3.h of the survey (DS1 revision 18, page 12), which the survey credits to Xu Xiaodong, Xie Zheng and S. P. Radziszowski (J. Graph Theory 47 (2004) 231-239):

> For 2 ≤ p ≤ q and 3 ≤ k, if (k, p)-graph G and (k, q)-graph H have a common induced subgraph on m vertices without K_{k−1}, then R(k, p + q − 1) > n(G) + n(H) + m.

Inspecting Google's 616-vertex file shows it has exactly the form of a construction of this kind:
- G1: rows 0 to 563, a (9,9)-graph on 564 vertices (it is 281-regular on 2 × 281 + 2 vertices and appears to be the Mathon and Shearer graph for R(9,9) ≥ 565);
- G2: rows 564 to 598, a (9,3)-graph on 35 vertices (the complement of the unique critical graph for R(3,9) = 36);
- a common induced subgraph with m = 17 vertices.

This gives 564 + 35 + 17 + 1 = 617.

We found a different common induced subgraph M of G1 and G2 with **m = 18** vertices and no K8. Its clique number is 7. The pairs are listed in `common_part_m18.txt` as `u:y`, where u is a vertex of G2 (0 to 34) and y is the matching vertex of G1 (0 to 563). With k = 9, p = 3 and q = 9, item 2.3.h gives R(9,11) > 564 + 35 + 18 = 617.

`build.py` builds the graph from Google's file and the pairs. It uses the Python standard library only.
- G1 and G2 are kept, with no edges between them.
- For each pair (u, y), the copy of the vertex in G1 is joined to its copy in G2.
- For each pair, a new vertex z is joined to the G1-neighbours of y and the G2-neighbours of u.
- The 18 new vertices induce M.

M was found by a local search for large common induced subgraphs.

### Why the 618 construction works

Write v_i (in G1) and u_i (in G2) for the two copies of the i-th vertex of M, and w_i for the new vertex.

- **No K9.** The only edges between G1 and G2 are the matching edges v_i u_i. So a clique that meets both parts holds exactly one v_i and one u_i with the same i. Every w_j in it is adjacent to v_i, so v_i together with the matching v_j form a clique of M, which has at most 7 vertices; the total is at most 1 + 1 + 6 = 8. If the clique misses G2, replace each w_j by v_j: w_j is never adjacent to its own v_j, so this gives a clique of G1 of the same size, at most 8. The same holds if it misses G1.
- **No independent 11-set.** Let I be independent, J the set of j with w_j in I, a the number of j in J with v_j in I, and b the number with u_j in I. Since v_j and u_j are adjacent, a + b ≤ |J|. The set (I in G1) together with {v_j : j in J} is independent in G1, so |I in G1| + |J| − a ≤ 8. Likewise |I in G2| + |J| − b ≤ 2. Adding gives |I| ≤ 10 + (a + b) − |J| ≤ 10.

### Verification of the 618 graph

We checked every hypothesis directly with exact searches:
- G1 (rows 0 to 563) has no K9 and no independent 9-set, so it is a (9,9)-graph.
- G2 (rows 564 to 598) has no K9 and no independent 3-set, so it is a (9,3)-graph.
- M is an induced subgraph of both, with the pairing in `common_part_m18.txt`.
- M has clique number 7, so it has no K8.

We also checked the finished graph directly with two exact programs, written independently with different algorithms:

| Program | Algorithm | Result |
|---|---|---|
| `check_russian_doll.c` | Östergård's Russian-doll method | Clique number 8. Independence number exactly 10 (about 1.3 × 10^11 search nodes, 55 minutes on one core). On Google's 616-vertex graph, as a control, it gives clique number 8 and independence number 10. |
| `check_branch_bound.c` | Tomita-style branch and bound with a colouring bound | No K9 (8.3 × 10^7 nodes). No independent set of size 11 (1.1 × 10^10 nodes). |

An independent review wrote a third exact search (24-thread branch and bound) and got the same answers: no K9, and no independent set of size 11, though one of size 10 exists.

The v1.0 files are `R9_11_618.txt`, `common_part_m18.txt`, `input_google_R9_11_617.txt`, `build.py` (old mode), `check_russian_doll.c` and `check_branch_bound.c`. The v1.0 text is kept as it was, apart from headings and dropping its file list.
