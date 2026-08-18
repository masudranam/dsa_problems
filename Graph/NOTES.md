# Graph - Concepts & Intuition

Techniques met in this folder. Problem list: [README.md](README.md).
Format: [NOTES-entry.md](../.claude/dsa/templates/NOTES-entry.md).

## Index

- [0-1 BFS (deque trick)](#0-1-bfs-deque-trick)
- [Bridges and articulation points (low-link)](#bridges-and-articulation-points-low-link)
- [Dijkstra with a priority queue](#dijkstra-with-a-priority-queue)
- [Shortest path: which algorithm](#shortest-path-which-algorithm)
- [Topological sort (Kahn)](#topological-sort-kahn)

---

## 0-1 BFS (deque trick)

**Recognise it** -- shortest path where every edge weight is **0 or 1**. Often
disguised: "minimum number of walls to break", "minimum direction changes", "some
moves are free". If you catch yourself reaching for Dijkstra and the only weights
are 0 and 1, this is simpler and faster.

**The idea** -- BFS with a `deque` instead of a `queue`. Relaxing a 0-edge
`push_front`, a 1-edge `push_back`.

**Why it works** -- ordinary BFS is correct because the queue stays sorted by
distance with at most two distinct values in it at once. A 0-edge reaches a node at
the *same* distance, so it belongs at the front; a 1-edge at distance+1, so at the
back. The deque preserves the sorted-by-distance invariant that made BFS correct --
no heap needed.

**Complexity** -- O(V + E), versus O(E log V) for Dijkstra.

**Pitfalls** -- still needs the distance check (`if (d[u] + w < d[v])`) before
pushing; a node can enter the deque more than once, unlike plain BFS where a
`visited` flag suffices.

**Template** -- [0_1_bfs](0_1_bfs) (extensionless -- compile with `-x c++`)

**Drill next** -- CSES "Monsters"-style grid BFS first, then Codeforces 1064D
(Labyrinth) which is 0-1 BFS in disguise, then AtCoder ABC 176D.

---

## Bridges and articulation points (low-link)

**Recognise it** -- "which edges, if removed, disconnect the graph", "critical
connections", counting components after each removal.

**The idea** -- one DFS assigning each node an entry time `in[u]` and a low-link
`low[u]` = the smallest entry time reachable from `u`'s subtree using at most one
back edge.

- edge `(u, v)` is a **bridge** iff `low[v] > in[u]`
- `u` is an **articulation point** iff some child has `low[v] >= in[u]`
  (root is special: it needs two or more DFS children)

**Why it works** -- `low[v] > in[u]` says the subtree at `v` has no back edge
climbing to `u` or above it, so the tree edge `(u,v)` is the only way out. Draw one
cycle and one bridge and the condition stops being a formula.

**Pitfalls**
- Skipping the parent must skip *the edge*, not *the vertex*. With parallel edges,
  `if (v == p) continue` wrongly ignores the second edge between the same pair --
  track the edge index instead.
- The `>=` for articulation points vs `>` for bridges is not interchangeable.
- The root of the DFS needs its own rule.

**Template** -- [Articulation_point/bridge_finding](Articulation_point/bridge_finding).
Note this copy is written to return `bool` "does a bridge exist" rather than
collecting all of them -- rework it before reusing for a "list every bridge" problem.

**Seen in** -- [Articulation_point/LOJ-1063(Ant_Hills)](Articulation_point/LOJ-1063%28Ant_Hills%29)

**Drill next** -- LeetCode 1192 (Critical Connections) for bridges; CSES
"Round Trip" to get comfortable with DFS trees first if the low-link argument
still feels opaque.

---

## Dijkstra with a priority queue

**Recognise it** -- shortest path, **non-negative** weights, one source.

**The idea** -- greedily settle the closest unsettled node. The local template uses
`priority_queue<ar<int,2>, vector<...>, greater<...>>` holding `{dist, node}`, which
sorts by distance because arrays compare lexicographically.

**Lazy deletion** -- the line that matters:

```cpp
if (u[0] > d[u[1]]) continue;   // a stale copy; its distance was improved later
```

Rather than decreasing a key in the heap, push a fresh entry and discard stale pops.

**Why it works** -- with non-negative weights, the first time a node is popped its
distance is final: any other route to it goes through some other unsettled node
that is already at least as far away, and edges cannot reduce distance.

**Complexity** -- O(E log V). The `{dist, node}` ordering is essential -- `{node, dist}`
would make the heap sort by node id and silently return wrong answers.

**Pitfalls**
- **Negative weights break it.** Not "make it slow" -- make it wrong. Use
  Bellman-Ford.
- `memset(d, 0x3fff, sizeof(d))` on an array of redefined `long long` `int` fills
  bytes, not values. Check the sentinel really exceeds any achievable distance and
  will not overflow when an edge weight is added to it.

**Template** -- [Dijkstra/dijkstra](Dijkstra/dijkstra)

**Seen in** -- [Dijkstra/cf(449B)](Dijkstra/cf%28449B%29),
[Dijkstra/Country_Roads(LOJ-1002)](Dijkstra/Country_Roads%28LOJ-1002%29),
[Dijkstra/E_Paired_Payment.cpp](Dijkstra/E_Paired_Payment.cpp)

**Drill next** -- CSES "Shortest Routes I", then a layered-graph Dijkstra
(state = node plus a small extra dimension) such as CF 1725M or the "at most one
free edge" family.

---

## Shortest path: which algorithm

**Recognise it** -- the decision itself, before writing code. Getting this wrong
costs either a wrong answer or a TLE, so it is worth a note of its own.

| Situation | Use | Cost |
|-----------|-----|------|
| unweighted (all weights equal) | BFS | O(V + E) |
| weights in {0, 1} | [0-1 BFS](#0-1-bfs-deque-trick) | O(V + E) |
| non-negative weights, one source | [Dijkstra](#dijkstra-with-a-priority-queue) | O(E log V) |
| negative weights, or need cycle detection | Bellman-Ford | O(V * E) |
| all pairs, small V (<= ~500) | Floyd-Warshall | O(V^3) |

**Why it matters** -- Bellman-Ford's real use is not "slower Dijkstra": relaxing
`V-1` times and then checking whether anything *still* improves detects a negative
cycle. That is what problems asking about arbitrage or infinite gain want.

**Seen in** -- [bellman_ford/LOJ-1074(Extended Traffic)](bellman_ford/LOJ-1074%28Extended%20Traffic%29),
[floyd_worshall/cf(33B)](floyd_worshall/cf%2833B%29),
[floyd_worshall/F_Road_Blocked.cpp](floyd_worshall/F_Road_Blocked.cpp)

**Drill next** -- CSES "High Score" (Bellman-Ford with a reachable negative cycle),
CSES "Shortest Routes II" (Floyd-Warshall).

---

## Topological sort (Kahn)

**Recognise it** -- ordering with prerequisites; also the standard way to detect a
cycle in a **directed** graph.

**The idea** -- repeatedly output any node of in-degree 0 and decrement its
neighbours' in-degrees. If fewer than `n` nodes come out, the graph has a cycle.

**Why it works** -- a node with no unmet prerequisite is always safe to place next,
and removing it cannot create a new dependency. A DAG always has such a node; if
none exists, what remains is a cycle.

**Complexity** -- O(V + E).

**Pitfalls**
- The "did I output all `n`?" check *is* the cycle detection -- omitting it turns a
  cyclic input into a silently truncated answer.
- Kahn's order is not unique; problems wanting the lexicographically smallest one
  need a priority queue instead of a plain queue.
- **The local copy is broken**: `for (int i = 0; i < n; i)` never increments `i`, so
  it loops forever. Fix it to `i++` before reusing (left as-is here rather than
  edited without asking).

**Template** -- [topologicalSort](topologicalSort)

**Drill next** -- CSES "Course Schedule" then LeetCode 210; then CF 1385E, which
needs a topological order to orient the undirected edges.

---

## Still to write up

`Diameter_dfs.cpp`, `dfs`, `isCycleDG`, `isCycleUG`, `MaxFlow/1214D(cf)`.

`dfs` is a snippet with no `main()` -- it will compile but not link, which is
correct: it is meant to be pasted into the template.
