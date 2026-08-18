---
name: dsa-index
description: Refresh the generated problem tables and INDEX.md, backfill metadata headers on files that have none, and report where practice is thin. Use for "update the index", "what should I practise next", "which problems are unlogged", "show my progress", "/dsa-index".
---

# Refresh and audit the index

```sh
python .claude/dsa/index.py            # rewrite the tables
python .claude/dsa/index.py --check    # report what would change, touch nothing
```

The script rewrites only the region between the `BEGIN:INDEX` / `END:INDEX`
markers in each category `README.md`, plus the top-level `INDEX.md`. Hand-written
content outside the markers -- the `## Coverage` checklist, prose, links -- is
preserved. Never hand-edit inside the markers; the next run overwrites it.

## Backfilling headers

Files that predate the harness have no metadata header, so they appear under
"Files with no header yet" in their category table rather than in the numbered
index. To fix one: read it, work out what it does, and add the header block from
[cp.cpp](../../dsa/templates/cp.cpp) at the top.

Do this only for files the user asks about, or a handful at a time when they ask
for a sweep. Do not guess at `Link` or `Judge` -- filenames like `cf(449B)` and
`LOJ-1074(Extended Traffic)` do carry the id, but `Pathetic_Interview_II.cpp`
does not. Leave a field blank rather than inventing a URL; a wrong link is worse
than a missing one. Adding the header must not change a single line of code.

## Reporting progress

When asked what to practise next, read the tables and say plainly:

- categories with **no** concepts written up -- the real gaps
- problems with `Status: hint`, `editorial` or `revisit` -- the ones to re-drill,
  which is exactly why those statuses must be honest
- unticked boxes in each `README.md`'s `## Coverage` checklist
- categories that do not exist yet, from [taxonomy.md](../../dsa/taxonomy.md)

Recommend two or three concrete next steps, not a survey of everything.

## The archives

`Codeforces/`, `SRBD Code Contest 2024/` and `python_programm/` are counted but
never indexed problem-by-problem -- `Codeforces/` alone is ~840 uncategorised
files and a table that long is unreadable. If the user wants those sorted into
categories, that is a deliberate backfill project: propose doing it in batches by
technique, moving files with `git mv` so history survives, and confirm before
moving anything.
