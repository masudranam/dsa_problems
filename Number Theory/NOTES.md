# Number Theory - Concepts & Intuition

Techniques met in this folder. Problem list: [README.md](README.md).
Format: [NOTES-entry.md](../.claude/dsa/templates/NOTES-entry.md).

## Index

- [Binary exponentiation (bigmod)](#binary-exponentiation-bigmod)
- [Euler's totient](#eulers-totient)
- [Miller-Rabin primality](#miller-rabin-primality)
- [Modular inverse](#modular-inverse)
- [Mulmod: multiplying without overflow](#mulmod-multiplying-without-overflow)

---

## Binary exponentiation (bigmod)

**Recognise it** -- `a^b mod M` with `b` up to `1e18`. Also the building block for
modular inverse, matrix power, and anything with a `log` in the exponent.

**The idea** -- square the base, halve the exponent, multiply the answer in whenever
the current bit is set:

```cpp
int pw(int a, int b) {
    int res = 1;
    while (b) { if (b & 1) res = res * a % M; a = a * a % M; b >>= 1; }
    return res;
}
```

**Why it works** -- `b` in binary is a sum of powers of two, so `a^b` is a product of
`a^(2^k)` for exactly the set bits. The squaring chain generates each `a^(2^k)` in one
step from the last.

**Complexity** -- O(log b).

**Pitfalls**
- `res * a` with both near `M ~ 1e9` needs 64-bit. The repo-wide
  `#define int long long int` covers this by accident -- on a judge without it, cast
  explicitly.
- If `M` itself approaches `1e18`, even 64-bit multiplication overflows: use
  [mulmod](#mulmod-multiplying-without-overflow).
- `pw(0, 0)` returns 1 here; check what the problem wants.

**Template** -- [BigMod/power](BigMod/power)

**Drill next** -- CSES "Exponentiation" then "Exponentiation II" (needs Fermat on the
exponent), then matrix exponentiation for a linear recurrence.

---

## Euler's totient

**Recognise it** -- counting integers in `[1, n]` coprime to `n`; also the exponent in
Euler's theorem, which is what "reduce `a^b mod m` for non-prime `m`" needs.

**The idea** -- three versions in the local file, and choosing between them is the
actual skill:

| Need | Method | Cost |
|------|--------|------|
| `phi(n)` for one `n` | factorise by trial division, `res -= res / p` per prime | O(sqrt n) |
| `phi(1..n)` for all | sieve: for each prime `i`, `phi[j] -= phi[j]/i` | O(n log log n) |
| `phi(1..n)`, simpler to recall | initialise `phi[i] = i-1`, subtract `phi[i]` from multiples | O(n log n) |

**Why the sieve form works** -- `phi[]` starts as the identity; each distinct prime
factor `p` of `j` multiplies the running value by `(1 - 1/p)`, which is exactly
`phi[j] -= phi[j] / p`. The guard `if (phi[i] == i)` detects "still untouched", i.e.
`i` is prime -- a neat way to avoid a separate sieve.

**Pitfalls**
- The single-`n` version must handle the leftover `if (n > 1) res -= res / n` for a
  remaining prime factor above `sqrt(n)`. Dropping it is a silent wrong answer on
  semiprimes.
- `phi(1) = 1` by convention.
- Euler's theorem needs `gcd(a, m) = 1`; without that, the general reduction is the
  lifting-the-exponent / CRT route, not `a^(b mod phi(m))`.

**Template** -- [Totient_phi/totient_phi_function](Totient_phi/totient_phi_function)

**Seen in** -- [Totient_phi/cf (1295D)](Totient_phi/cf%20%281295D%29)

**Drill next** -- CSES "Counting Coprime Pairs", then CF 1295D (already here -- redo
it), then a Mobius-function problem to see totient as one case of a family.

---

## Miller-Rabin primality

**Recognise it** -- "is `n` prime" for `n` up to `1e18`, where trial division to
`sqrt(n) = 1e9` is far too slow.

**The idea** -- write `n - 1 = d * 2^s`. For a witness `a`, either `a^d == 1` or some
`a^(d * 2^r)` equals `n - 1`; any `a` failing both proves `n` composite. Testing a
fixed set of small prime bases makes it deterministic for all 64-bit `n`.

**Why it works** -- Fermat's little theorem gives `a^(n-1) == 1 (mod n)` for prime `n`,
and in a field the only square roots of 1 are `±1`. Finding a square root of 1 that is
neither is therefore a proof of compositeness -- which is why this catches Carmichael
numbers that a plain Fermat test does not.

**Pitfalls**
- Needs [mulmod](#mulmod-multiplying-without-overflow) at 64-bit moduli, or every
  multiplication silently wraps.
- Small `n` (< 4) and even `n` need special-casing before the loop.
- Random bases make it probabilistic; the standard deterministic base set
  `{2,3,5,7,11,13,17,19,23,29,31,37}` covers everything below `2^64`.

**Template** -- [BigMod/millerRabin(spoj_PON)](BigMod/millerRabin%28spoj_PON%29)

**Drill next** -- SPOJ PON (already here), then Pollard's rho for actual
factorisation of `1e18` numbers, which pairs with this.

---

## Modular inverse

**Recognise it** -- dividing under a modulus. There is no division mod `M`; you
multiply by an inverse.

**The idea** -- two routes:
- `M` prime: Fermat gives `a^(M-2) mod M` via binary exponentiation. One line.
- `M` not prime: extended Euclid, which exists iff `gcd(a, M) = 1`.

For a whole factorial table, do **not** call `pw` per element. Invert the largest
factorial once and walk backwards:

```cpp
inv[N-1] = pw(fact[N-1], M - 2);
for (int i = N - 1; i > 0; i--) inv[i-1] = inv[i] * i % M;
```

**Why the backward walk works** -- `1/(i-1)! = (1/i!) * i`. One exponentiation plus
`N` multiplications instead of `N` exponentiations: O(N + log M) rather than
O(N log M).

**Pitfalls**
- Fermat requires `M` prime **and** `a` not a multiple of `M`.
- `inv[0]` must be 1.
- Check whether the problem's modulus is `1e9+7` or `998244353`; both appear in this
  repo and both are prime, but hardcoding the wrong one is a hard bug to see.

**Template** -- see [../Combinatorics/cf.cpp](../Combinatorics/cf.cpp) `init()`, and
the linear-inverse variant in [../Combinatorics/modint.cpp](../Combinatorics/modint.cpp).
Cross-linked from [../Combinatorics/NOTES.md](../Combinatorics/NOTES.md).

**Drill next** -- CSES "Binomial Coefficients", then "Creating Strings II"
(multinomials), then a problem where `M` is composite so Fermat is unavailable.

---

## Mulmod: multiplying without overflow

**Recognise it** -- a modulus near `1e18`, where `a * b` overflows 64 bits before the
`%` can run. Shows up inside Miller-Rabin and Pollard's rho.

**The idea** -- the same trick as binary exponentiation, one level down: reduce
multiplication to doubling and addition, which cannot overflow if `a, b < M`.

```cpp
int mul(int a, int b, int M) {
    int res = 0;
    while (b) { if (b & 1) res = (res + a) % M; a = (a + a) % M; b >>= 1; }
    return res;
}
```

**Why it works** -- `a * b = sum of a * 2^k` over set bits `k` of `b`, and each term is
reached by repeated doubling with a `%` after every step, so nothing ever exceeds `2M`.

**Complexity** -- O(log b) per multiply, so it makes Miller-Rabin O(log^2 n). Where
available, `__int128` does the same job in one instruction; this version is the
portable fallback.

**Also in this file** -- string multiplication in an arbitrary base (grade-school
long multiplication on digit strings), for when the numbers exceed 64 bits entirely.

**Template** -- [BigMod/power](BigMod/power)

**Drill next** -- implement Pollard's rho on top of this and factor a few `1e18`
numbers.

---

## Still to write up

`GCD/gcd` -- Euclid plus extended Euclid. Worth an entry connecting extended Euclid
to [modular inverse](#modular-inverse) and to linear Diophantine equations.
