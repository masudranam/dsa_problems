---
name: dsa-note
description: Write a newly learned concept, technique or algorithm into its category NOTES.md with recognition signals, intuition, pitfalls and follow-up problems to drill, and stamp the solved problem's metadata header. Use after solving something ("I just learned X", "log this", "explain this and save it", "/dsa-note") or when the user hits a technique they had not seen before.
---

# Write up a concept

Turn something just learned into a note that will still be useful in six months.
This is the highest-value part of the harness: the solved file records *what* was
done, the note records *why it worked and how to recognise it next time*.

## 1. Close out the problem file

If this came from solving a problem, fill in the header fields left blank by
`/dsa-add`:

- `Technique` -- short, lowercase, comma-separated: `binary search on answer, greedy check`
- `Status` -- one of `solved` / `hint` / `editorial` / `revisit`. Be honest;
  `hint` and `editorial` are the ones worth re-drilling later, so silently
  writing `solved` destroys the signal the user needs.
- `Insight` -- one line, the thing that unlocked it.

## 2. Add the entry to the category NOTES.md

Use the shape in [NOTES-entry.md](../../dsa/templates/NOTES-entry.md):
**Recognise it / The idea / Why it works / Complexity / Pitfalls / Template /
Seen in / Drill next**.

Insert alphabetically among the existing `##` entries and add the anchor link to
the `## Index` list at the top of the file.

Rules that keep these notes worth reading:

- **Recognise it** is the field that makes them stronger. Write the signal they
  actually saw in the statement, not a restatement of the technique's name.
- **Why it works** must be a real argument, not a label. If you cannot write the
  sentence that makes it obvious, write `TODO: still feels like magic` and say so
  to the user. A fake explanation is worse than an admitted gap.
- **Pitfalls** should be what actually bit them this session, plus this repo's
  standing traps where relevant (`#define int long long int` making `int main()`
  illegal, `%d` on a redefined `int`, recursion depth, overflow before the mod).
- **Drill next** needs two or three real, checkable problems on other judges,
  easiest first. Only list problems you are confident exist with those exact
  ids -- a wrong link is worse than none. If unsure, describe the problem shape
  to search for instead of inventing a number.

If the concept already has an entry, **extend that entry** rather than adding a
second one: add the new recognition signal, the new pitfall, the new problem
under *Seen in*.

## 3. Cross-link

If the technique leans on one from another category (segment tree inside a DP
optimisation, DSU inside a greedy), add a one-line pointer in the other
category's `NOTES.md`. Cross-links are how the wiki stops being a pile of
isolated notes.

Update the `## Coverage` checklist in the category's `README.md` if this entry
ticks a box.

## 4. Refresh the index

```sh
python .claude/dsa/index.py
```

The concept count in [INDEX.md](../../../INDEX.md) comes from the `##` headings
in each `NOTES.md`, so it updates itself.
