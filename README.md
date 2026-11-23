# R(9,11) ≥ 618

This repository gives a graph on 617 vertices that contains no clique of 9 vertices and no independent set of 11 vertices. It shows that the Ramsey number R(9,11) is at least 618.

The previous best lower bound we could find is R(9,11) ≥ 617. It comes from a 616-vertex graph published on 22 October 2026 by Google Research in the `google-research/google-research` repository, folder `ramsey_number_bounds/improved_bounds`, file `R(9, 11) >= 617.txt` (commit 0d94f8c). The dynamic survey *Small Ramsey Numbers* (S. P. Radziszowski, DS1 revision 18, April 2026) prints no lower bound for this cell. The survey's own formula in item 2.3.h, with R(3,9) = 36 and R(9,9) >= 565, gives R(9,11) >= 608; Google's graph raised this to 617. As of 23 November 2026 we found no public bound above 617: we searched the web, arXiv, Zenodo, GitHub, the Leaps in Bounds tracker (which lists Google's 617) and the records of other recent contributors.

## The construction

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

## Why the construction works

Write v_i (in G1) and u_i (in G2) for the two copies of the i-th vertex of M, and w_i for the new vertex.

- **No K9.** The only edges between G1 and G2 are the matching edges v_i u_i. So a clique that meets both parts holds exactly one v_i and one u_i with the same i. Every w_j in it is adjacent to v_i, so v_i together with the matching v_j form a clique of M, which has at most 7 vertices; the total is at most 1 + 1 + 6 = 8. If the clique misses G2, replace each w_j by v_j: w_j is never adjacent to its own v_j, so this gives a clique of G1 of the same size, at most 8. The same holds if it misses G1.
- **No independent 11-set.** Let I be independent, J the set of j with w_j in I, a the number of j in J with v_j in I, and b the number with u_j in I. Since v_j and u_j are adjacent, a + b ≤ |J|. The set (I in G1) together with {v_j : j in J} is independent in G1, so |I in G1| + |J| − a ≤ 8. Likewise |I in G2| + |J| − b ≤ 2. Adding gives |I| ≤ 10 + (a + b) − |J| ≤ 10.

## Verification

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

`reproduce.sh` runs everything: the checksum check, the rebuild, a byte-for-byte comparison, and both checkers. It needs only python3 and gcc.

## Files

- `R9_11_618.txt`: the 617-vertex graph as 617 lines of 0/1. Line i, column j is 1 when vertices i and j are adjacent.
- `input_google_R9_11_617.txt`: Google's 616-vertex graph (from google-research/google-research, Copyright Google LLC; that repository licenses source files under Apache 2.0 and datasets under CC BY 4.0, and this file is redistributed unchanged, renamed from `R(9, 11) >= 617.txt`, with attribution, under those terms).
- `common_part_m18.txt`: the 18 pairs that define M.
- `build.py`: rebuilds the graph.
- `check_russian_doll.c` (usage `file T i|c`) and `check_branch_bound.c` (usage `file K T`): the exact checkers.
- `reproduce.sh`, `SHA256SUMS`.

## Credit and method

The starting graph is Google Research's. Google's repository does not say how the graph was built; we found by inspecting the file that it has exactly the form of a 2.3.h construction with a common part of 17 vertices. The new part is the larger common subgraph (18 vertices instead of 17; it shares no pair with Google's 17).

This work was directed by Peter J. Chrisbacher. The search, construction and checks were carried out with AI coding agents (Anthropic's Claude Code), working under fixed rules: every claim needs two independently written exact checkers, and a check of published results before any announcement.

## License

- Code (`build.py`, the checkers, `reproduce.sh`): MIT.
- The new graph and this text: CC BY 4.0.
- `input_google_R9_11_617.txt` is Google's file, redistributed unchanged with attribution under the terms of google-research/google-research (Apache 2.0 for source files, CC BY 4.0 for datasets; the Apache text is in `LICENSES/`).

## How to cite

See `CITATION.cff`, or cite the Zenodo record for this repository once it is published.
