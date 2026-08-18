# DP - Concepts & Intuition

Techniques met in this folder, written up as they came. Problem list:
[README.md](README.md). Format: [NOTES-entry.md](../.claude/dsa/templates/NOTES-entry.md).

## Index

- [Bounded knapsack (limited copies per item)](#bounded-knapsack-limited-copies-per-item)
- [Coin change: the three questions](#coin-change-the-three-questions)
- [Digit DP](#digit-dp)

---

## Bounded knapsack (limited copies per item)

**Recognise it** -- a knapsack where each item may be used **at most `b[i]` times**,
not once and not unlimited. `Coin_Change.cpp` and
`Coin_Change (III)_(LOJ-1233).cpp` are both this.

**The idea** -- plain unbounded knapsack sweeps `j` upward and lets an item be
reused freely. To cap reuse, carry a second array counting how many copies of the
*current* item were spent reaching `j`:

- `dp[j]` = is sum `j` reachable
- `cnt[j]` = copies of the current item used to reach `j`
- for each item, sweep `j` upward; reach `j` from `j - a[i]` only if that would
  keep `cnt[j-a[i]] < b[i]`
- **reset `cnt` to zero between items** -- it is per-item scratch, not state

**Why it works** -- the upward sweep is what allows reuse, so the count is exactly
"how many times have I already stepped by `a[i]` to get here". Capping the count
caps the reuse, without adding a dimension to `dp`.

**Complexity** -- O(n * m), same as unbounded. The alternative -- binary-splitting
each item into powers of two and running 0/1 knapsack -- costs an extra
`log b[i]` but generalises to *value* knapsack rather than just reachability.

**Pitfalls**
- Forgetting the `cnt` reset between items silently carries the previous item's
  budget over.
- `dp[j] || !dp[j - a[i]]` short-circuits: skip if already reachable, or if the
  predecessor is not. Easy to invert while transcribing.
- This formulation answers *reachability*, not *minimum coins*. For the min-coin
  version, `dp[j]` holds a cost and the cap needs the binary-splitting trick.

**Seen in** -- [Coin_Change.cpp](Coin_Change.cpp),
[Coin_Change (III)_(LOJ-1233).cpp](Coin_Change%20%28III%29_%28LOJ-1233%29.cpp)

**Drill next** -- CSES "Money Sums" (reachability, unbounded) then CSES
"Minimizing Coins" to feel the difference; then a bounded version to force the
binary-splitting rewrite.

---

## Coin change: the three questions

**Recognise it** -- "coin change" names four different DP problems, and the loop
order decides which one you wrote. Before coding, answer: *unbounded or bounded?*
and *are `{1,2}` and `{2,1}` the same answer?*

**The idea**

| Question | Loop order | Answer shape |
|----------|-----------|--------------|
| count **combinations** (order-insensitive) | items outer, sum inner | `dp[0]=1`, `dp[j] += dp[j-a[i]]` |
| count **permutations** (order matters) | sum outer, items inner | same recurrence, swapped loops |
| **minimum coins** | either, with `min` | `dp[j] = min(dp[j], dp[j-a[i]] + 1)` |
| **reachability** | items outer | boolean `dp` |

**Why it works** -- with items on the outside, item `i` is only ever considered
after every use of items `< i` is settled, so each multiset is built in one fixed
order and counted once. Putting the sum outside lets any item extend any prefix,
which counts orderings.

**Pitfalls** -- this is the single most common silent wrong answer in DP: both loop
orders compile, run fast, and produce a plausible number. Decide which question is
being asked *before* writing the loops, and sanity-check on `coins={1,2}, target=3`
-- combinations gives 2, permutations gives 3.

**Seen in** -- [Coin_Change.cpp](Coin_Change.cpp),
[cf_687C(coin_change).cpp](cf_687C%28coin_change%29.cpp),
[Coin_Change (III)_(LOJ-1233).cpp](Coin_Change%20%28III%29_%28LOJ-1233%29.cpp)

**Drill next** -- LeetCode 518 (combinations) and 377 (permutations) back to back;
they are the same recurrence with the loops swapped, which is the whole lesson.

---

## Digit DP

**Recognise it** -- "how many numbers in `[a, b]` have property P", with bounds up
to `1e18`. Far too many numbers to enumerate, and P depends on the *digits*.

**The idea** -- count over `[0, x]` with a digit-by-digit recursion, then take the
difference:

```
answer(a, b) = f(b) - f(a - 1)
```

State, building the number left to right:
- `idx` -- digit position
- `small` (often called `less`/`free`) -- has a strictly smaller digit already been
  placed? If yes, remaining digits range over `0..9`; if no, they are capped at
  `s[idx]`.
- `started` -- has a nonzero digit appeared? Needed to stop leading zeros counting
  as real digits.

**Why it works** -- `small` is the entire reason this is polynomial. Once you have
gone strictly below the bound at any position, *which* bound you came from stops
mattering, so all those prefixes collapse into one state.

**Complexity** -- O(digits * 2 * 2 * 10) per query, i.e. instant.

**Pitfalls**
- `f(a-1)`, not `f(a)`. Off by one here is the classic digit-DP bug, and `a = 0`
  needs a guard -- [Palindromic_Numbers.cpp](Palindromic_Numbers.cpp) has
  `if (x < 0) return 0`.
- `memset(dp, -1, ...)` must happen **per query**, since the memo is keyed on the
  bound string `s`.
- **Only memoise on state that is actually in the key.** The local file carries
  `cur` (the number built so far) as a recursion argument but does *not* include
  it in `dp[idx][small][st]`, and it guards the cache read with `&& st`. Any
  property that depends on the digits chosen so far -- being a palindrome does --
  needs those digits reflected in the state, or the memo can serve a value
  computed for a different prefix. Worth re-deriving before reusing this file as
  a template; treat the shape as the lesson, not the caching.

**Seen in** -- [Palindromic_Numbers.cpp](Palindromic_Numbers.cpp)

**Drill next** -- CSES "Counting Numbers"; Codeforces 1560D-style digit
constructions; then a digit DP carrying a modulus in the state (count numbers
divisible by `k`) to practise adding a dimension.

---

## Still to write up

Files in this folder with no entry yet -- run `/dsa-note` on one after re-reading it:

`cf118D.cpp`, `cf_682D.cpp`, `D_Caesar_s_Legions.cpp`, `E_3_Team_Division.cpp`,
`E_Fire.cpp`, `IUPC_IUT_C.cpp`, `Orbitaxian.cpp`, `Pathetic_Interview_II.cpp`,
`Pawn.cpp`, `Select_Edges.cpp`, `Sequence_Matching.cpp`, `Word_Combinations.cpp`
