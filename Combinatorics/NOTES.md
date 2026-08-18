# Combinatorics - Concepts & Intuition

Techniques met in this folder. Problem list: [README.md](README.md).
Format: [NOTES-entry.md](../.claude/dsa/templates/NOTES-entry.md).

## Index

- [Factorial tables and nCr mod p](#factorial-tables-and-ncr-mod-p)
- [modint: wrapping the modulus in a type](#modint-wrapping-the-modulus-in-a-type)

---

## Factorial tables and nCr mod p

**Recognise it** -- any counting answer "modulo 1e9+7" or "modulo 998244353" that comes
out as a product of binomials. Precompute once, answer each query in O(1).

**The idea**

```cpp
fact[0] = 1;
for (int i = 1; i < N; i++) fact[i] = fact[i-1] * i % M;
inv[N-1] = pw(fact[N-1], M - 2);                       // one exponentiation
for (int i = N-1; i > 0; i--) inv[i-1] = inv[i] * i % M;  // then walk down
ncr(n, k) = n < k ? 0 : fact[n] * inv[k] % M * inv[n-k] % M;
```

**Why the backward walk works** -- `inv[i-1] = inv[i] * i` because
`1/(i-1)! = (1/i!) * i`. That turns `N` exponentiations into `N` multiplications:
O(N + log M) instead of O(N log M). Cross-linked from
[../Number Theory/NOTES.md](../Number%20Theory/NOTES.md#modular-inverse).

**Complexity** -- O(N) precompute, O(1) per binomial.

**Pitfalls**
- Guard `n < k` and negative `n` -- returning garbage instead of 0 here is a very
  common source of wrong answers on counting problems.
- `N` must exceed the largest `n` you will ever ask for, including intermediate values
  like `n + k` in stars-and-bars.
- Two `%` operations in `ncr` are needed, not one: three multiplied residues overflow
  otherwise.
- The modulus must be **prime** for the Fermat inverse. `998244353` is prime and is the
  one used in `cf.cpp` -- it is the NTT-friendly modulus, so seeing it hints the
  intended solution may involve polynomial multiplication.

**Template** -- [cf.cpp](cf.cpp) (`init()`), which also carries a BIT for inversion
counting -- see [../Data Structure/NOTES.md](../Data%20Structure/NOTES.md#bit--fenwick-tree).

**Drill next** -- CSES "Binomial Coefficients", then "Distributing Apples"
(stars and bars), then "Christmas Party" (derangements) to practise
inclusion-exclusion.

---

## modint: wrapping the modulus in a type

**Recognise it** -- a problem where modular arithmetic appears in enough places that
hand-written `% M` starts producing bugs, especially subtraction going negative.

**The idea** -- a `template <int32_t MOD> struct modint` overloading `+ - * /` so the
reduction happens inside the type. `operator/` multiplies by `inv()`, which is
`pow(MOD - 2)`. The companion `Combi` struct precomputes factorials, inverse
factorials and linear inverses as `mint`s and exposes `ncr`.

**Why it is worth the boilerplate** -- it eliminates the whole class of "forgot the
`% M`", "subtraction went negative", and "overflowed before reducing" bugs by making
them unrepresentable. The cost is that a stray `int` in an expression may silently
leave the type, so keep values as `mint` end to end.

**The linear inverse trick** -- `invs[i] = invs[MOD % i] * (-MOD / i)`. This computes
inverses of `1..n` in O(n) with no exponentiation at all, from
`MOD = (MOD/i)*i + MOD%i` rearranged mod `MOD`. Neat, and worth deriving once by hand
rather than copying.

**Pitfalls**
- `inv()` uses Fermat, so **MOD must be prime** -- the file says so in a comment.
- Division is a full exponentiation; inside a hot loop, precompute instead.
- `mod` is a separate global from the template parameter here (`using mint = modint<mod>`),
  so switching modulus means changing it in one place -- check which one is actually in
  use before trusting a result.

**Template** -- [modint.cpp](modint.cpp)

**Drill next** -- redo one problem already solved with raw `% M` using `mint` instead,
and compare how much of the code stops being arithmetic bookkeeping.
