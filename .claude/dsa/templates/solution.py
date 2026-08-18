# ------------------------------------------------------------------
# Problem   :
# Judge     :
# Link      :
# Category  :
# Technique :
# Difficulty:
# Status    : unsolved
# Date      :
# Insight   :
# ------------------------------------------------------------------

import sys


def solve(data):
    pass


def main():
    data = sys.stdin.buffer.read().split()
    it = iter(data)
    t = int(next(it))          # drop this line for single-test problems
    out = []
    for _ in range(t):
        out.append(str(solve(it)))
    sys.stdout.write("\n".join(out) + "\n")


if __name__ == "__main__":
    main()
