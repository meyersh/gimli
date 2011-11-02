#!/usr/bin/env python
# Infix to Postfix converter/solver using the two-stack algorithm
# prototype for 310.
# 
# Shaun Meyer, Feb 2011
#

ops = []   # operator stack
nums = []  # number stack

#equation = "0 & (1 & 1)|1|0~"
equation = "~(0&1)"
postfix_out = ""

def precedence(opcode):
    if (opcode == '~'):
        return 4
    elif (opcode == '&'):
        return 3
    elif (opcode == '|'):
        return 2
    else:
        return 0

def do_op():
    global postfix_out
    op2 = nums.pop()

    opcode = ops.pop()

    postfix_out += opcode

    if (opcode != '~'):
        op1 = nums.pop()

    if (opcode == '&'):
        nums.append(op1 & op2) # "push" result.
    elif (opcode == '|'):
        nums.append(op1 | op2) # "push" result.
    elif (opcode == '~'):
        nums.append(not op2) # "push" result.

    print "Do ", opcode

def process_equation(eq):
    global postfix_out
    for i in xrange(len(eq)):
        token = eq[i]

        if (token == ' '):
            continue
        
        if (token == '('):
            ops.append(token) # "push" token.
        elif (token in ('&', '|', '~')):
            if (len(ops) == 0 
                or precedence(token) > precedence(ops[-1])): # token > ops.top?
                ops.append(token) # "push" token.
            else:
                do_op()
                ops.append(token) # "push" token.
        elif (token.isdigit()):
            nums.append(int(token)) # "push" token.
            postfix_out += token
        elif (token == ')'):
            while (len(ops) != 0 and ops[-1] != '('):
                do_op()
            ops.pop() # remove that )

        # We're at the last token in the array, go ahead and do the 
        # rest of the ops.
        if (i == len(eq)-1):
            while (ops):
                do_op()

        print "%2d ops: %s num: %s" % (i, str(ops).ljust(30), str(nums))

    print "Answer: ", nums.pop()
    print "Postfix:", postfix_out

# Do the equation (remembering to show our work.)
print "Equation: ", equation
print "---"
print "i:      ops stack:                          num stack: "

process_equation(equation)

