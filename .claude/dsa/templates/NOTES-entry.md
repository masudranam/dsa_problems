## <Technique name>

**Recognise it** -- the signal in the statement that should make you reach for
this. Write it as the thing you *saw*, not the thing you knew: "q queries mixing
point updates with range sums" beats "segment tree problem".

**The idea** -- two or three sentences. For DP, give the state and the
transition explicitly:
- state: `dp[i][j]` = ...
- transition: ...
- base case: ...
- answer: ...

**Why it works** -- the one sentence that makes it obvious rather than
memorised. If you cannot write this line, you have not understood it yet; say so
and leave a `TODO` rather than faking it.

**Complexity** -- time and memory, and the `n` it survives.

**Pitfalls** -- what actually bit you. Off-by-one in the range, overflow before
the mod, forgetting `#define int long long` makes `int main()` illegal, a
recursion depth that blows the stack. Concrete beats general.

**Template** -- path to the reusable file in this repo, if there is one.

**Seen in** -- problems in this category that use it, as links.

**Drill next** -- two or three unsolved problems on other judges that need the
same idea, easiest first. This is the part that turns a note into practice.
