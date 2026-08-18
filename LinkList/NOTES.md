# LinkList - Concepts & Intuition

Pointer-based structures and stack/queue mechanics. Problem list:
[README.md](README.md). Format: [NOTES-entry.md](../.claude/dsa/templates/NOTES-entry.md).

**Note on idiom** -- unlike the rest of the repo, these files do **not** use the
`Codeforces/cp.cpp` template. They use plain `#include<iostream>`, `using ll = long long`
and a real `int main()`. That is correct here: without the repo-wide
`#define int long long int`, `int main()` is legal. Do not "modernise" them into the
CP template -- they are implementation exercises, not contest submissions.

## Index

- [Double pointer for head modification](#double-pointer-for-head-modification)
- [In-place reversal (three pointers)](#in-place-reversal-three-pointers)
- [Circular lists: the termination condition](#circular-lists-the-termination-condition)

---

## Double pointer for head modification

**Recognise it** -- any list operation that might change the head: insert at front,
delete the first node, or insert into an empty list.

**The idea** -- pass `node **hr` rather than `node *h`. Assigning `*hr = nnode` changes
the caller's head; assigning to a plain `node *h` parameter only rebinds the local copy
and the change vanishes at return.

**Why it works** -- the parameter is itself a variable. To mutate the caller's pointer
you need its address, exactly as with any other by-reference argument. `node *&h` is the
C++ alternative and reads better; the `**` form is what these files use.

**Pitfalls**
- The empty-list branch (`if (!(*hr))`) is separate and is the one most often forgotten.
- A stale global `tail` is a real hazard: `fullLinkList.cpp` sets `tail` only in the
  non-empty branch, so after inserting into an *empty* list `tail` still points at
  whatever it held before. Any pattern that caches a second pointer must update it on
  every path.

**Seen in** -- [fullLinkList.cpp](fullLinkList.cpp), [InsertEnd.cpp](InsertEnd.cpp),
[InsertInPos.cpp](InsertInPos.cpp), [addValueNthPosition.cpp](addValueNthPosition.cpp),
[DeleteFromBegEndPos.cpp](DeleteFromBegEndPos.cpp)

**Drill next** -- LeetCode 203 (Remove Elements) and 83, both of which are much shorter
with a dummy head node than with a special case for the head. Worth writing both ways
once to see why the dummy-node trick is standard.

---

## In-place reversal (three pointers)

**Recognise it** -- reverse a list, or any problem needing the list walked backwards
without extra memory. Also the core of "is this a palindrome list" and "reorder list".

**The idea** -- carry `prev`, `cur`, `next`; at each step save `next`, flip
`cur->next` to `prev`, then slide both forward. Return `prev`, which ends on the old
tail.

**Why the save matters** -- overwriting `cur->next` destroys the only reference to the
rest of the list. Saving `next` first is the entire trick, and it is why the loop needs
three names and not two.

**Complexity** -- O(n) time, O(1) extra space. The recursive version is O(n) stack, which
blows up around `n = 1e5`.

**Pitfalls**
- `prev` starts as `NULL`, which correctly becomes the new tail's `next`.
- For a doubly linked list, both `next` **and** `prev` must be swapped per node, and the
  list's `tail` needs updating too.

**Seen in** -- [reverseLinkList.cpp](reverseLinkList.cpp),
[doubleLinkListEmplementation.cpp](doubleLinkListEmplementation.cpp)

**Drill next** -- LeetCode 206, then 92 (reverse a sublist -- the same loop with
boundary bookkeeping), then 25 (reverse in k-groups).

---

## Circular lists: the termination condition

**Recognise it** -- a list whose last node points back to the first. Round-robin
scheduling and Josephus-style elimination problems.

**The idea** -- every traversal changes shape: `while (cur != NULL)` never terminates.
Use `do { ... } while (cur != head)`, so the body runs once before the check.

**Why it must be `do/while`** -- with a plain `while (cur != head)` the loop exits
immediately, since `cur` starts at `head`. The first node would never be visited.

**Pitfalls**
- Insertion must close the loop again -- a single missed `last->next = head` turns a
  circular list into a linear one, and the bug only shows on the *next* traversal.
- Deleting the head requires finding the node that points to it, which is O(n) in a
  singly linked circular list.

**Seen in** -- [createCircularLinkList.cpp](createCircularLinkList.cpp),
[dataInsertInCircularLinkList.cpp](dataInsertInCircularLinkList.cpp)

**Drill next** -- LeetCode 141/142 (cycle detection with Floyd's tortoise-and-hare,
which is a different question about cycles and worth its own entry when you get there),
then the Josephus problem.

---

## Still to write up

**Monotonic stack** is the contest-relevant stack technique and is **not covered
anywhere in this repo yet** -- no file implements it. It is the tool for "next greater
element", "largest rectangle in histogram", and "count subarrays where this is the
maximum". This is the biggest single gap in this category; start with LeetCode 496,
then 84, then CF 1156C-style counting.

Also unwritten: `ImplementationStack.cpp`, `stactWithLink.cpp` (stack over a linked
list -- push/pop at the head, both O(1)), `addOneWithNumber.cpp` (digits in a list,
carry propagation), `deDatalFromBegEndPosLinkList.cpp`.
