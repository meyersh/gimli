#!/usr/bin/env python
# Calculate the formula so I make fewer mistakes.
# Shaun Meyer, Oct 2012
#

import sys

def L(p,q,k):
    k -= 1
    print "r(%d,%d,%d) = r(%d,%d,%d) + r(%d,%d,%d)r(%d,%d,%d)*r(%d,%d,%d)" % (
        p, q, k+1,
        p, q, k,
        p, k+1, k,
        k+1,k+1, k,
        k+1,q, k
        )


if len(sys.argv) != 4:
    print "USAGE: %s p q k" % sys.argv[0]
else:
    L(int(sys.argv[1]),
      int(sys.argv[2]),
      int(sys.argv[3]))

