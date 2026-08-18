# String algorithm - Concepts & Intuition

Techniques met in this folder. Problem list: [README.md](README.md).
Format: [NOTES-entry.md](../.claude/dsa/templates/NOTES-entry.md).

## Index

- [Polynomial hashing (double hash)](#polynomial-hashing-double-hash)
- [Prefix function / KMP](#prefix-function--kmp)
- [Which string tool](#which-string-tool)
- [Z-function](#z-function)

---

## Polynomial hashing (double hash)

**Recognise it** -- "are these two substrings equal", palindrome checks over
arbitrary ranges, counting distinct substrings. Hashing is the blunt instrument
that replaces a suffix automaton when you only need equality.

**The idea** -- treat the string as a number in base `p` mod `M`, and precompute
prefix hashes plus powers so any substring hash comes out in O(1). Two independent
`(p, M)` pairs run in parallel; a match requires **both** to agree.

**Why two hashes** -- one 32-bit-ish modulus collides by the birthday bound once you
compare enough pairs: with `~1e9` buckets, collisions become likely around `~1e5`
comparisons. Two independent hashes square the space and make it negligible. This is
not paranoia; single-hash solutions get anti-hash tested on Codeforces.

**The palindrome trick** -- hash the string forwards and backwards, then a range is a
palindrome iff its forward hash equals its backward hash over the mirrored range.
Aligning the two requires multiplying by a power of `p` so both are normalised to the
same exponent -- that is what the `pw[l-1] * (...)` factors are doing.

**Pitfalls**
- Subtracting prefix hashes underflows: always `(a - b + M) % M`.
- Multiply two values near `1e9` and you need 64-bit, which the repo-wide
  `#define int long long int` happens to give you here.
- Compare hashes only after normalising both to the same power of `p`.
- **The local `Hash` file does not compile as-is**: `subStr()` and `pal()` reference
  `h1`/`h2`, but only `h11, h12, h21, h22` are declared. Read it as two half-merged
  versions of the same idea and reconstruct rather than pasting blindly.

**Template** -- [Hashing/Hash](Hashing/Hash),
[Hashing/Forward_Backward_Hash](Hashing/Forward_Backward_Hash)

**Seen in** -- [Hashing/Prefix_SuffixPalindromecf(1326D)](Hashing/Prefix_SuffixPalindromecf%281326D%29),
[Hashing/Rhythmic_Tune.cpp](Hashing/Rhythmic_Tune.cpp)

**Drill next** -- CF 1326D2 (already here -- redo it), then CSES "String Matching"
with hashing instead of KMP, then "Finding Borders" to compare against the prefix
function.

---

## Prefix function / KMP

**Recognise it** -- pattern matching, and anything about **borders** (a prefix that is
also a suffix): shortest period, "how many times must I repeat", "shortest string
containing this twice".

**The idea** -- `pi[i]` = length of the longest proper prefix of `s[0..i]` that is
also a suffix of it.

```cpp
for (int i = 1; i < n; i++) {
    int j = pi[i - 1];
    while (j > 0 && s[i] != s[j]) j = pi[j - 1];
    if (s[i] == s[j]) j++;
    pi[i] = j;
}
```

**Why it works** -- when the next character fails, the next-best candidate border is
the border *of the border*, which `pi[j-1]` already holds. The `while` loop is
therefore walking a chain of ever-shorter borders, and the total number of steps
across the whole string is O(n) amortised because `j` only ever decreases here and
increases at most once per position.

**The period identity** -- worth memorising: the shortest period of a string of
length `n` is `n - pi[n-1]`, and the string is a whole number of repetitions of it
iff `n % (n - pi[n-1]) == 0`. A surprising number of problems are one line once you
know this.

**Pitfalls**
- Proper prefix -- `pi[0] = 0` always.
- For matching, run the prefix function over `pattern + '#' + text` and the separator
  must be a character absent from both, or matches can straddle the join.

**Template** -- [Prefix_function.cpp](Prefix_function.cpp)

**Drill next** -- CSES "String Matching", then "Finding Periods", then CF 471D which
reduces to KMP on a difference array.

---

## Which string tool

**Recognise it** -- the choice, before writing anything.

| Need | Tool |
|------|------|
| find a pattern in a text | prefix function (KMP), or hashing |
| borders, periods, repetitions | prefix function |
| longest common prefix of `s` with each of its suffixes | Z-function |
| compare two arbitrary substrings for equality | hashing |
| all palindromic substrings | Manacher |
| many patterns at once | Aho-Corasick |
| distinct substrings, sorted suffixes, LCP | suffix array |

Hashing overlaps most of these and is usually the fastest to write; the specialised
tool is what you reach for when hashing is too slow or an exact guarantee is needed.

**Also in this folder** -- [ManachersAlgorithm.cpp](ManachersAlgorithm.cpp),
[AhoCorasick.cpp](AhoCorasick.cpp), [SuffixArray/suffixArray.cpp](SuffixArray/suffixArray.cpp),
none written up yet.

---

## Z-function

**Recognise it** -- `z[i]` = length of the longest common prefix of `s` and the suffix
starting at `i`. Reach for it when the question is "how far does the string match
itself starting here".

**The idea** -- maintain the rightmost-reaching match `[l, r]` (the "Z-box"). For a
new `i` inside that box, `z[i - l]` already tells you what to expect, so you start
from `min(r - i + 1, z[i - l])` instead of zero, then extend naively.

**Why it's linear** -- naive extension only ever pushes `r` rightwards, and `r` moves
at most `n` times in total. So the `while` loop is O(n) across the entire run even
though any single iteration may be long.

**Complexity** -- O(n).

**Pitfalls**
- `z[0]` is undefined (or `n` by convention) -- the loop starts at `i = 1`.
- `min(r - i + 1, z[i - l])` matters: without the clamp you would read past the box
  and the guarantee breaks.
- For matching, use `pattern + '#' + text` and look for `z[i] == pattern.size()`;
  the separator must not occur in either string.

**Template** -- [Z aolgorithm/Z](Z%20aolgorithm/Z) (extensionless -- compile with `-x c++`)

**Seen in** -- [Z aolgorithm/Password (cf-126B)](Z%20aolgorithm/Password%20%28cf-126B%29),
[Z aolgorithm/String_Matching.cpp](Z%20aolgorithm/String_Matching.cpp),
[Z aolgorithm/String_Functions.cpp](Z%20aolgorithm/String_Functions.cpp)

**Drill next** -- CF 126B (already here -- redo without looking), then CF 432D which
wants Z plus counting, then re-solve one KMP problem with Z to feel where each is
natural.

---

## Still to write up

`AhoCorasick.cpp`, `ManachersAlgorithm.cpp`, `SuffixArray/suffixArray.cpp`,
`SuffixArray/Automata.cpp`, `SuffixArray/Repeating_Substring.cpp`,
`SuffixArray/SPOJ _ ADAPHOTO.cpp`.

`selectionSort.cpp` is in this folder but is not a string algorithm -- it belongs
elsewhere whenever a sorting category opens.
