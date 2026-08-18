---
name: dsa-add
description: Start a new practice problem from a URL or a pasted statement. Picks the category, creates the solution file from the right template with a filled-in metadata header, opens the category if it is new, and refreshes the index. Use for "add this problem", "I want to solve this", "/dsa-add <link>", or when the user pastes a problem statement to work on.
---

# Add a practice problem

Scaffold a problem into the right category and get the user solving it. **Do not
solve it for them unless they ask** -- see *Hints, not solutions* below. This
repo exists so they get stronger; handing over a finished solution defeats it.

## 1. Get the statement

If given a URL, fetch it with WebFetch. Judges that commonly block automated
fetches (Codeforces sometimes does) will fail -- if so, say the fetch failed and
ask the user to paste the statement. Do not guess at a problem from its URL slug
or from memory of the problem ID; a wrong statement wastes their session.

Pull out: title, judge + problem id, constraints (the `n` bound decides the
intended complexity), and whether it is multi-test.

## 2. Pick the category

Read [taxonomy.md](../../dsa/taxonomy.md) and apply its routing rule: the
category is **the technique they had to know**, not the objects in the statement.

State the category and the one-line reason before creating anything. If it is
genuinely a toss-up between two, say so and pick the harder-to-see one -- the
user can move it later, and hesitating here stalls the session for nothing.

## 3. Create the file

Copy the matching template from [.claude/dsa/templates/](../../dsa/templates/):

| Source | Template | Why |
|--------|----------|-----|
| Codeforces, AtCoder, CSES, SPOJ, LOJ -- anything stdin/stdout | `cp.cpp` | matches the ~800 existing files |
| LeetCode | `leetcode.cpp` | class-based, and deliberately **without** `#define int long long` so the method signature still matches LeetCode's |
| Python practice | `solution.py` | |

Filename conventions, matching what is already in the repo:

- Codeforces / AtCoder: keep the Competitive Programming Helper shape --
  contest index, then the title in `Snake_Case`: `C_Spiral_Rotation.cpp`
- LeetCode: `LC<number>_Title.cpp` -- `LC0136_Single_Number.cpp`
- Anything else: `Title.cpp`

Strip apostrophes and punctuation from titles the way the existing files do
(`A_Doremy_s_Paint_3.cpp`, `B_osu_mania.cpp`).

Fill in every header field except `Technique` and `Insight` -- those get written
after solving, by `/dsa-note`. Set `Status: unsolved` and `Date` to today.
Set the multi-test line: uncomment `cin >> t;` for multi-test problems, comment
it out (`int t = 1;   //cin >> t;`) for single-test ones.

If the category folder does not exist, create it along with a `README.md` and a
`NOTES.md` seeded from the shape of an existing category's files.

## 4. Refresh the index

```sh
python .claude/dsa/index.py
```

## 5. Hints, not solutions

Default to a **hint ladder**. Offer level 1 and wait; escalate only when asked:

1. **Category and recognition** -- "this is binary search on the answer; what is
   the monotone predicate?" Nothing more.
2. **The state or the invariant** -- for DP the state but not the transition;
   for greedy the exchange argument to try but not the proof.
3. **Full approach in prose**, still no code.
4. **Code**, only on an explicit ask.

If they ask outright for the solution, give it -- but then write the concept up
with `/dsa-note` so the idea is captured rather than just the answer.

## Verify before reporting done

Compile it. The local compiler is MinGW g++ 6.3.0, pre-C++17:

```sh
g++ -std=c++14 -O2 "DP/Some_Problem.cpp" -o "$TEMP/a.exe"
```

For a LeetCode file add `-DLOCAL` to build the test `main()`. Run the sample
input by hand and compare against the expected output -- there is no test runner
in this repo, so "it compiles" is not "it is correct", and say which of the two
you actually checked.
