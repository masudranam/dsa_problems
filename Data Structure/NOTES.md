# Data Structure - Concepts & Intuition

Techniques met in this folder. Problem list: [README.md](README.md).
Format: [NOTES-entry.md](../.claude/dsa/templates/NOTES-entry.md).

## Index

- [BIT / Fenwick tree](#bit--fenwick-tree)
- [DSU (union-find)](#dsu-union-find)
- [LCA by binary lifting](#lca-by-binary-lifting)
- [Segment tree with a node struct](#segment-tree-with-a-node-struct)
- [Sparse table](#sparse-table)
- [Which structure: the decision table](#which-structure-the-decision-table)

---

## BIT / Fenwick tree

**Recognise it** -- prefix sums with point updates. Ten lines, so reach for it
before a segment tree whenever the query is a *prefix* of an invertible operation.

**The idea** -- `x & -x` isolates the lowest set bit, which is exactly the length of
the range each index covers:

```cpp
void add(int x, int y) { for (; x < N; x += x & -x) bit[x] += y; }
int  sum(int x) { int r = 0; for (; x > 0; x -= x & -x) r += bit[x]; return r; }
```

**Why it works** -- index `x` stores the sum of the `x & -x` elements ending at `x`.
Walking down by `x -= x & -x` visits a set of disjoint blocks that exactly tile
`[1, x]`; walking up by `x += x & -x` visits every block that contains `x`.

**Complexity** -- O(log n) both ways, with a much smaller constant than a segment
tree.

**Pitfalls**
- **1-indexed.** `x -= x & -x` at `x = 0` loops forever, so index 0 cannot be used.
- Range sum is `sum(r) - sum(l-1)`, which needs an invertible operation -- a BIT
  cannot do range *min* this way.

**Template** -- [Segment tree/BIT.cpp](Segment%20tree/BIT.cpp), and inline in
[../Combinatorics/cf.cpp](../Combinatorics/cf.cpp) for counting inversions.

**Drill next** -- CSES "Dynamic Range Sum Queries", then inversion counting
(CSES "Distinct Values Queries" style), then a 2D BIT.

---

## DSU (union-find)

**Recognise it** -- merging groups and asking "same group?", offline problems sorted
by weight, Kruskal, and any "add edges one at a time, report connectivity" problem.

**The idea** -- forest of parent pointers with two optimisations that only matter
together:

```cpp
int get(int v) { return p[v] = (v == p[v] ? v : get(p[v])); }   // path compression
void merge(int u, int v) {
    u = get(u); v = get(v);
    if (Size[u] < Size[v]) swap(u, v);   // union by size: small into large
    Size[u] += Size[v];
    p[v] = u;
}
```

**Why it works** -- union by size keeps depth logarithmic; path compression flattens
what you touch. Together they give near-constant amortised cost (inverse Ackermann).

**The counting trick** -- keeping `Size` lets you answer aggregate questions during
the merge, which is the real reason DSU shows up in hard problems.
`Path Query (CF-582G).cpp` uses `res += Size[u] * Size[v]` when merging: the number
of **new pairs** connected by this edge is exactly the product of the two component
sizes, since every left-node/right-node pair becomes reachable at once. The
commented-out `c(x) = x*(x-1)/2` lines are the same count done as
"pairs after minus pairs before".

**Pitfalls**
- Always `get()` both endpoints before comparing or merging.
- Merging by size means the returned root is not predictable -- never assume `u`
  stays the root.
- DSU does not support deletion. "Remove edges" problems get solved by
  **processing in reverse** so removals become additions.

**Template** -- [DSU/Path Query (CF-582G).cpp](DSU/Path%20Query%20%28CF-582G%29.cpp)

**Drill next** -- CSES "Road Construction" (maintain largest component while
adding roads), then Kruskal MST, then a "process queries offline in reverse"
problem to internalise the deletion workaround.

---

## LCA by binary lifting

**Recognise it** -- repeated "lowest common ancestor" or "distance between two
nodes in a tree" queries; also the k-th ancestor.

**The idea** -- `anc[u][i]` = the `2^i`-th ancestor of `u`, filled during one DFS:

```cpp
anc[u][0] = p;
for (int i = 1; i < 19; i++)
    anc[u][i] = ~anc[u][i-1] ? anc[anc[u][i-1]][i-1] : -1;
```

Query in two phases: lift the deeper node to equal depth, then lift both together
by the largest jumps that keep them **apart**. They end as the two children of the
LCA, so the answer is `anc[u][0]`.

**Why it works** -- every integer is a sum of distinct powers of two, so any depth
difference is reachable by jumps of decreasing size. The second phase stops *below*
the LCA deliberately: jumping to where the ancestors are equal could overshoot past
the lowest common one, so the invariant is "never make them equal".

**Complexity** -- O(n log n) build, O(log n) per query.

**Pitfalls**
- `~x` is `x != -1` for the -1 sentinel -- idiomatic here but easy to misread.
- Table width must exceed `log2(n)`: the local file declares `anc[N][26]` but loops
  to 19/18. Harmless, but the loop bound is the one that has to be right, and it
  must match in `dfs` and `lca`.
- `d[]` must be filled before any query.
- `dist(u,v) = d[u] + d[v] - 2*d[lca]`.

**Template** -- [LCA/lca.cpp](LCA/lca.cpp)

**Seen in** -- [LCA/Terror_in_Terminus.cpp](LCA/Terror_in_Terminus.cpp)

**Drill next** -- CSES "Company Queries I" (k-th ancestor) then "II" (LCA), then a
problem needing max-edge-on-path, which is the same table carrying a second value.

---

## Segment tree with a node struct

**Recognise it** -- `q` queries mixing **updates** with **range aggregates**. The
tell is updates and queries interleaved; if all updates come first, prefix sums or
a sparse table are cheaper.

**The idea** -- the local template keeps a `struct node { int mn, s, mx, lz; }` and a
`Merge(a, b)` function, with the identity element supplied by the default
constructor (`mx = -M, mn = M, s = 0`).

**Why the struct matters** -- it separates *what* is being combined from *how* the
tree recurses. To switch from range-sum to range-min you edit `Merge` and the
constructor only. That is the whole design: the tree is generic over any
**associative** operation with an identity.

**Lazy propagation** -- `prop(i, l, r)` pushes a pending assignment down before
descending. The order is what makes it correct: push down, recurse, then re-merge
on the way up. `lz = -1` is the "nothing pending" sentinel, which means a real
assignment of -1 cannot be represented -- a live limitation of this copy.

**Complexity** -- O(log n) per operation, O(4n) memory (hence `t.assign(4*n + 10, ...)`).

**Pitfalls**
- The identity must be genuinely neutral. Returning a default `node()` for an
  out-of-range query only works because `mx = -M` and `mn = M` lose to everything.
- Half-covered ranges must recurse both ways; forgetting a `prop` before reading a
  child is the classic lazy bug and shows up only after a specific update order.
- `Merge` in this file currently combines `s` only -- extend it before relying on
  `mn`/`mx`.

**Template** -- [Segment tree/segmentTree.cpp](Segment%20tree/segmentTree.cpp);
pointer-based variant in [Segment tree/using_pointer.cpp](Segment%20tree/using_pointer.cpp)

**Seen in** -- [Segment tree/E_Linear_Kingdom_Races.cpp](Segment%20tree/E_Linear_Kingdom_Races.cpp),
[Segment tree/E_Lucky_Queries.cpp](Segment%20tree/E_Lucky_Queries.cpp),
[Segment tree/Playing_with_OR.cpp](Segment%20tree/Playing_with_OR.cpp),
[Segment tree/Vacation_Query.cpp](Segment%20tree/Vacation_Query.cpp),
[Segment tree/Maximum Subarray(cf-1796D).cpp](Segment%20tree/Maximum%20Subarray%28cf-1796D%29.cpp)

**Drill next** -- CSES "Dynamic Range Minimum Queries", then "Range Update Queries"
(forces lazy), then the max-subarray-in-range node (store total/prefix/suffix/best)
which is the best exercise for designing `Merge` yourself.

---

## Sparse table

**Recognise it** -- many range min/max queries, **no updates**.

**The idea** -- `tb[i][j]` = the aggregate over `[i, i + 2^j - 1]`, built in
O(n log n). Any range is covered by two overlapping power-of-two blocks, so an
idempotent operation answers in O(1): `min(tb[l][k], tb[r - 2^k + 1][k])`.

**Why it works** -- overlap is harmless for min/max because `min(x, x) = x`. This is
exactly why sparse tables do **not** work for sums: the overlap would double-count.
The local copy answers by stepping non-overlapping blocks in a loop instead
(O(log n) per query), which is the version that works for any associative
operation -- worth knowing which of the two you have copied.

**Pitfalls**
- Idempotent operations only, for the O(1) form.
- Static data only. One update means rebuilding everything.
- Memory is `n * log n` -- `tb[N][20][2]` at large `N` is easy to overflow.
- The template is a snippet with **no `main()`** and relies on globals `n`, `k`,
  `a[]` existing -- it compiles but will not link on its own.

**Template** -- [SparseTable/sparse_table.cpp](SparseTable/sparse_table.cpp)

**Seen in** -- [SparseTable/RMQSQ _spoj](SparseTable/RMQSQ%20_spoj),
[SparseTable/Square_Queries](SparseTable/Square_Queries)

**Drill next** -- SPOJ RMQSQ (already here -- redo it from scratch), then CSES
"Static Range Minimum Queries", then LCA-via-Euler-tour-plus-RMQ to connect this
to the LCA note.

---

## Which structure: the decision table

**Recognise it** -- the choice itself, given "n up to 2e5, q up to 2e5".

| Need | Structure |
|------|-----------|
| range sum, point update | BIT |
| range min/max, no update | sparse table |
| range anything, point or range update | segment tree |
| range update + range query | segment tree with lazy |
| merge groups, connectivity | DSU |
| tree ancestor / LCA queries | binary lifting |
| k-th smallest, order statistics | BIT over values, or a merge-sort tree |

**Why it matters** -- all of these pass the time limit; picking the heaviest one
that fits costs debugging time and introduces bugs a BIT would not have. Prefer the
simplest structure that answers the actual query.

---

## Still to write up

`Segment tree/using_pointer.cpp` (dynamic/implicit segment tree -- worth its own
entry on when node allocation beats a fixed array).
