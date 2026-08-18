---
name: dsa-review
description: Review a solution the user wrote for correctness, complexity against the problem's constraints, and the repo's C++ traps, then name the pattern and suggest what to drill next. Use for "review my solution", "why is this wrong", "will this TLE", "is there a better approach", "/dsa-review <file>".
---

# Review a solution

Review a practice solution the way a strong teammate would: find the bug, check
it fits the constraints, name the pattern, and point at what to practise. Teach
through the review -- explain *why* something is wrong, not just what to change.

## What to check, in this order

**1. Correctness.** Trace the logic against the samples by hand. Look hardest at
off-by-one in loop and range bounds, the empty/`n == 1` case, and the branch that
never runs on the samples. If you find a bug, give the concrete input that breaks
it -- a failing case beats a description every time.

**2. Constraints.** Read the `n` bound from the header's `Link`/statement and
compare against the actual complexity. `n <= 2e5` with an O(n^2) loop is a TLE,
not a style note. Count total work across test cases too: the sum-of-n bound is a
standard trap when `t` is large.

**3. Overflow and the repo's own traps.** These come from the shared template, so
they recur across every file:

- `#define int long long int` is repo-wide, so `int main()` expands to
  `long long int main()` and fails to link -- it must stay `signed main()`.
  The same redefinition breaks any genuine 32-bit `int` and every `%d` format.
- Intermediate products overflowing *before* the `% M`.
- `memset(d, 0x3f, ...)` on a `long long` array does not give the value people
  expect -- check the sentinel is actually larger than any real distance.
- Recursion depth on `n = 2e5` chains.
- The LeetCode template deliberately omits the `int` redefinition; flag it if a
  LeetCode file has picked it up, because the submission will not compile there.

**4. Then, and only then, the approach.** If a fundamentally simpler or faster
idea exists, describe it -- but lead with what is wrong with the current code.
Rewriting a working solution into your preferred style teaches nothing; keep the
user's structure and idiom unless it is the actual problem.

## Verify, do not assert

Compile it, and run the samples:

```sh
g++ -std=c++14 -O2 "DP/Some_Problem.cpp" -o "$TEMP/a.exe"
echo "<sample input>" | "$TEMP/a.exe"
```

The compiler is MinGW g++ 6.3.0 and is pre-C++17 -- structured bindings
(`auto [a, b] = p`) fail here but pass on the judge, so check for C++17 syntax
before reporting a real bug. For LeetCode files compile with `-DLOCAL`.

Report what you actually ran. If you only compiled it and did not verify output
against the samples, say that.

## Close the loop

End with the pattern name and one line on what class of problem it generalises
to. If the technique is new or the review exposed a real gap, offer `/dsa-note`
to write it into the category's `NOTES.md` -- and if the user needed a hint or the
editorial, make sure `Status` in the header says so rather than `solved`.
