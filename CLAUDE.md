# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What this repository is

A personal competitive-programming archive — ~900 single-file C++ solutions plus a small set of
Python coursework scripts. There is no build system, no test suite, no package manifest, and no
shared library: **every `.cpp` file is a standalone `main()` program that reads stdin and writes
stdout** (a few pure-template files are snippets with no `main()` at all — see below).
Nothing links against anything else. Do not try to introduce a global build, refactor
files into shared headers, or "fix" duplication between files — the duplication is the point
(each file must stay pasteable into a judge's submission box on its own).

## Toolchain

The installed compiler is **MinGW g++ 6.3.0**, which is pre-C++17. Structured bindings
(`auto [a, b] = p`) and other C++17 features **do not compile here** even though online judges
accept them. Use `-std=c++14` unless a file already relies on something newer, and if a file
fails to compile locally, check for C++17 syntax before assuming a real bug.

```sh
# compile one solution (run from the repo root; note the quoting for paths with spaces)
g++ -std=c++14 -O2 "Codeforces/A_Subsonic_Subway.cpp" -o "$TEMP/a.exe"

# run it against a test file
"$TEMP/a.exe" < input.txt

# Python coursework (needs numpy/matplotlib for Q10 and similar)
python python_programm/Q1.py
```

There is no lint step and no test runner. "Verifying a change" means compiling the single file
and feeding it sample input by hand.

## The solution template

`Codeforces/cp.cpp` is the canonical starting template, copied into ~800 of the C++ files
verbatim. `Codeforces/A_A.cpp` is a scratch pad using the same skeleton (it calls `solve()`
without defining it, so it does not compile — that is expected, not a bug). New solutions should
match this shape:

```cpp
#include<bits/stdc++.h>
using namespace std;

#define print(a) for(auto x:a)cout<<x<<' ';cout<<'\n';
#define debug(x) cout<<#x<<" "<<x<<endl
#define all(a) (a).begin(),(a).end()
#define sz(a) (int)(a.size())
#define int   long long int
#define endl '\n'
#define ar array

const int M = 1e9 + 7;
const int N = 2e5 + 10;

void solve(){
}

signed main() {
   ios_base::sync_with_stdio (0);
   cin.tie (0);

   int t = 1;   cin >> t;
   for (int tc = 1; tc <= t; tc++) {
      solve();
   }
   return 0;
}
```

Consequences of that template worth knowing before editing any file:

- `#define int long long int` is repo-wide. That is why `main` is declared **`signed main()`** —
  writing `int main()` expands to `long long int main()` and fails to link. Same trap applies to
  any function or template argument you add that needs a genuine 32-bit `int`, and to
  `%d`-style format strings.
- Multi-test problems uncomment `cin >> t;`; single-test problems leave it commented
  (`int t = 1;   //cin >> t;`). Toggling that one line is the usual difference between files.
- `cout<<"Case "<<tc<<": "` inside the loop is commented out and enabled only for judges that
  want it (ICPC-style, some SRBD problems).
- A handful of files enable `freopen("input.txt","r",stdin)` / `freopen("output.txt","w",stdout)`.
  Those read relative to the **current working directory**, not the source file's directory, so
  run them from where the input file lives (e.g. `Codeforces/output/input.txt`).

## Layout and naming

Top-level directories are topic buckets, not modules:

- `Codeforces/` — the bulk of the archive (~840 files), flat. Filenames come from the Competitive
  Programming Helper (cph) VS Code extension, which derives them from the contest problem title:
  `A_Subsonic_Subway.cpp`, `D_XOR_Construction.cpp`, `G_2_Yunli_s_Subarray_Queries_hard_version.cpp`.
  The leading letter is the contest problem index, so it is also a rough difficulty signal.
- `DP/`, `Graph/`, `Data Structure/`, `Number Theory/`, `String algorithm/`, `Combinatorics/`,
  `LinkList/` — topic folders mixing two kinds of file: **reusable algorithm templates** named
  after the technique (`Data Structure/Segment tree/segmentTree.cpp`, `Graph/Dijkstra/dijkstra`,
  `String algorithm/Hashing/Hash`) and **solved problems** that apply it, named after the problem
  (`Data Structure/LCA/Terror_in_Terminus.cpp`, `Graph/Dijkstra/cf(449B)`). When adding a
  solution, put it in the folder for the technique it demonstrates. The technique files are the
  ones to read first when you need a working segment tree, DSU, sparse table, hashing, Z-function,
  or suffix array in this repo's idiom.
- Many older files in these topic folders have **no extension at all** (`Graph/dfs`,
  `Number Theory/gcd`, `String algorithm/Z aolgorithm/Z`). They are C++ source; compile them
  with an explicit `-x c++` (`g++ -x c++ -std=c++14 "Graph/dfs" -o out.exe`). Same for the two
  extensionless files at the repo root, `geometry` and `BoothMultiplication`. Some of these
  technique files are snippets with no `main()` (`Graph/dfs`,
  `Data Structure/SparseTable/sparse_table.cpp`) — they will compile but fail to link
  (`undefined reference to WinMain@16`), which means the file is meant to be pasted into a
  template, not run.
- `python_programm/` — numbered coursework scripts (`Q1.py`…`Q10.py`) on simulation and
  statistics (LCG, Monte Carlo, frequency distributions). Unrelated to the C++ side.
- `SRBD Code Contest 2024/` — contest-specific solutions, in `Round 1/` and `Round 2/`.

## The practice harness

The repo is also a study log. Each topic category carries two files next to the
solutions:

- **`README.md`** — the problem index. The table between the `BEGIN:INDEX` /
  `END:INDEX` markers is **generated**; never hand-edit inside them. The
  `## Coverage` checklist above the markers is hand-written and is preserved by
  regeneration.
- **`NOTES.md`** — the concept wiki: recognition signals, intuition, pitfalls,
  and problems to drill next, one `##` entry per technique.

[INDEX.md](INDEX.md) is the top-level dashboard. Everything is driven off a
metadata header at the top of each solution file:

```cpp
/* ------------------------------------------------------------------
 * Problem   : Subsonic Subway
 * Judge     : Codeforces 1968C
 * Link      : https://codeforces.com/contest/1968/problem/C
 * Category  : Binary Search
 * Technique : binary search on answer, greedy check
 * Status    : solved | hint | editorial | revisit | unsolved
 * ------------------------------------------------------------------ */
```

```sh
python .claude/dsa/index.py           # rebuild every table + INDEX.md
python .claude/dsa/index.py --check   # report what would change, write nothing
```

The harness lives entirely in `.claude/`: skills `dsa-add`, `dsa-note`,
`dsa-review` and `dsa-index` in `.claude/skills/`, the category routing rules in
[.claude/dsa/taxonomy.md](.claude/dsa/taxonomy.md), and the file templates in
`.claude/dsa/templates/` — `cp.cpp` for stdin/stdout judges, `leetcode.cpp`
(deliberately **without** `#define int long long`, so the method signature still
matches LeetCode's), and `solution.py`.

Two standing rules when working in here:

- **New problems go in a topic category, never in `Codeforces/`.** Route with
  `taxonomy.md`: the category is the technique that was needed, not the objects in
  the statement.
- **`Status` must be honest.** `hint` and `editorial` are what the harness uses to
  pick re-drills, so overwriting them with `solved` destroys the signal.

### Backfilling the archive

`Codeforces/` is ~840 files from before the harness: flat, uncategorised, and with
no metadata headers. Files there show up only as a count in `INDEX.md`. Sorting
them into categories is a deliberate project, not something to start unprompted —
it means reading each file to identify its technique, and `git mv` to keep
history. Propose it in technique-sized batches and confirm before moving anything.
Adding a header to an existing file must never change a line of its code.

## Repository hygiene

There is no `.gitignore`, and ~50 compiled binaries (`.exe`, `.bin`, `.class`) plus the cph
extension's 267 hidden `.cph/*.prob` metadata files are **tracked in git**. When compiling for a
check, write the output to the scratchpad or `$TEMP` rather than next to the source, so you do not
add more artifacts. Do not delete the existing tracked binaries or `.cph` files, and do not add a
`.gitignore`, unless asked — that is a cleanup decision for the repo owner.
