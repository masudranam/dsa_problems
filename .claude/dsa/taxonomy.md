# Category taxonomy

The routing table for `/dsa-add`. Fourteen categories, each a top-level folder
with its own `README.md` (problem index) and `NOTES.md` (concepts).

## The routing rule

A problem goes in the category of **the technique you had to know to solve it** --
not the objects in the statement. A problem about a tree that you solve by
sorting edges and running DSU is `Data Structure` (DSU), not `Graph`. A problem
about strings that is really a knapsack is `DP`.

Ask: *"if I had not known X, would I have been stuck?"* X is the category.

When two techniques genuinely share the load, pick the one that was **harder to
see**, and cross-link from the other category's `NOTES.md`. Do not copy the file
into two folders.

## The categories

| Folder | Belongs here | Statement signals that route here |
|--------|--------------|-----------------------------------|
| `DP` | overlapping subproblems, optimal substructure | "count the number of ways", "minimum cost to", "longest/shortest subsequence", small `n` with a budget/capacity dimension, "at most k operations" |
| `Graph` | traversal, shortest path, connectivity, flow, trees as graphs | explicit edges, "cities and roads", reachability, cycles, components, bipartite, "minimum number of moves" on a state space |
| `Data Structure` | the difficulty is in *maintaining* something under updates | "q queries", online updates mixed with queries, range sum/min/max, "after each operation print" |
| `Greedy` | a local exchange argument gives the global optimum | "maximum number of", sort-then-scan, "is it always optimal to", exchange/interval scheduling |
| `Binary Search` | monotone predicate, or search on the answer | "minimise the maximum", "maximise the minimum", "smallest x such that", answer is a value with a checkable feasibility test |
| `Two Pointers` | sliding window, or two indices sweeping | "longest subarray such that", "count pairs with sum", contiguous windows, sorted-array pair hunting |
| `Bit Manipulation` | XOR/AND/OR structure, subset enumeration over bits | "XOR of", "bitwise AND of the whole array", `n <= 20` with subsets, per-bit independence |
| `Number Theory` | divisibility, primes, modular arithmetic | gcd/lcm, "divisible by", primes/factorisation, "modulo 1e9+7" where the *maths* is the difficulty, multiplicative inverse |
| `Combinatorics` | counting formulas rather than counting DP | binomials, permutations, inclusion-exclusion, "how many distinct arrangements", Catalan/Stirling |
| `String algorithm` | the string structure itself is the problem | substring search, palindromes, borders/prefix-function, hashing, suffix structures, "occurrences of" |
| `Geometry` | points, lines, areas, angles | coordinates, convex hull, intersection, "area of", closest pair |
| `Constructive` | build *any* valid object, no optimisation | "construct any array such that", "print -1 if impossible", "does there exist" with a witness to output |
| `Game Theory` | two players, optimal play | "Alice and Bob play", "who wins", nim-like piles, Grundy/sprague-grundy, "both play optimally" |
| `LinkList` | pointer-based structures, and stack/queue mechanics | linked list surgery, "reverse the list", implementing a stack/queue, monotonic stack |

## Subfolders

Inside a category, a technique with more than one problem gets a subfolder named
after the technique, matching what is already there:

```
Data Structure/Segment tree/    DSU/    LCA/    SparseTable/
Graph/Dijkstra/    bellman_ford/    floyd_worshall/    MaxFlow/    Articulation_point/
String algorithm/Hashing/    SuffixArray/    Z aolgorithm/
Number Theory/BigMod/    GCD/    Totient_phi/
```

A single problem for a brand-new technique can sit directly in the category
folder; promote it to a subfolder when a second one arrives. The existing
folder names (including the `Z aolgorithm` typo) stay as they are -- renaming
them only churns git history.

## Categories that do not exist yet

`Greedy`, `Binary Search`, `Two Pointers`, `Bit Manipulation`, `Geometry`,
`Constructive` and `Game Theory` have no folder yet. `/dsa-add` creates the
folder plus its `README.md` and `NOTES.md` on first use -- do not pre-create
empty ones, an empty folder reads as "nothing learned here" when it actually
means "not started".

`Geometry` has a head start: the extensionless `geometry` file at the repo root
belongs there whenever that category opens.

## Not categories

- `Codeforces/` -- ~840 files from before the harness, flat, uncategorised.
  **New problems never go here.** Left alone as an archive; see the backfill
  note in [CLAUDE.md](../../CLAUDE.md).
- `SRBD Code Contest 2024/` -- contest-specific, kept as-is.
- `python_programm/` -- simulation/statistics coursework, unrelated to DSA.
