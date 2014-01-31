#!/usr/bin/env python
# Try to implement thm. 3.30
#
# Shaun Meyer, Oct 2012
#

transition_table = dict()
transition_table[1] = {'a': 3, 'b':1}
transition_table[2] = {'a': 1, 'b':3}
transition_table[3] = {'a': 2, 'b':4}
transition_table[4] = {'a': 4, 'b':2}

# for state in transition_table:
#     for sigma in transition_table[state]:
#         print "%d -%s-> %d" % (state, sigma, transition_table[state][sigma])

def r(p, q, k):
    if k == 0:
        for sigma in transition_table[p]:
            if transition_table[p][sigma] == q:
                return sigma
            else: return "0"
    k -= 1

    ret = ""
    r1 = r(p, q, k)
    
    if r1 != "0":
        ret = "%s + " % r1
    
    r2 = r(p,k+1,k)

    if r2 != "0":
        ret = "%s(%s)" % (ret, r2)

    r3 = r(k+1,k+1, k)

    if r3 != "0":
        ret = "%s(%s)*" % (ret, r3)

    r4 = r(k+1,q, k)

    if r4 != "0":
        ret = "%s(%s)" % (ret, r4)

    
    #return r1 + " + (" + r2 + ")(" + r2 + ")*(" + r4 + ")"
    return ret

import re
rregex = re.compile("r\(([0-9]+),([0-9]+),([0-9]+)\)")
            
def print_r(p, q, k):
    result = ""
    
    if k == 0:
        if p == q:
            result = "^ + "
        for sigma in transition_table[p]:
            if transition_table[p][sigma] == q:
                if p==q:
                    result = "%s + %s" % (result, sigma)
                    return result
                else:
                    result = sigma
                    return result
        return ""
                    

    k -= 1

    result = "r(%d,%d,%d) + r(%d,%d,%d)r(%d,%d,%d)*r(%d,%d,%d)" % (
        p, q, k,
        p, k+1, k,
        k+1,k+1, k,
        k+1,q, k
        )
    


    res = rregex.search(result)
    
    while (res):
        print result, res.groups()
        (p, q, k) = (res.groups())
        print "r(%s,%s,%s)" % (p,q,k) 
        result = result.replace("r(%s,%s,%s)" % (p,q,k), "(%s)" %
                                print_r(int(p), 
                                        int(q),
                                        int(k)))
        res = rregex.search(result)

    return result

def recurse_r(s):
    res = rregex.match(s)
    if res:
        s.replace("r(%s,%s,%s)", print_r(int(res.groups(0)), 
                                         int(res.groups(1)),
                                         int(res.groups(2))))

import sys
if len(sys.argv) != 4:
    print "USAGE: %s p q k" % sys.argv[0]
else:
    print print_r(int(sys.argv[1]),
            int(sys.argv[2]),
            int(sys.argv[3])).replace(' +  + ', ' + ').replace('()', '').replace('( +', '(').replace('( ', '(')

