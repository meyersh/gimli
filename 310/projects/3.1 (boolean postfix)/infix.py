#!/usr/bin/python

ops = []
nums = []

#equation = "0 & (1 & 1)|1|0~"
equation = "~(0&0)"
def clean_equation(eq):
    return eq.replace(" ", "")

equation = clean_equation(equation)
postfix_out = ""

print "Equation: ", equation
print "---"
print "i:      ops stack:                          num stack: "

def precedence(opcode):
    if (opcode == '&'):
        return 3
    elif (opcode == '|'):
        return 2
    elif (opcode == '~'):
        return 4
    else:
        return 0

def do_op():
    op2 = nums.pop()

    
    opcode = ops.pop()
    global postfix_out
    postfix_out += opcode

    if (opcode != '~'):
        op1 = nums.pop()

    if (opcode == '&'):
        nums.append(op1 & op2)
    elif (opcode == '|'):
        nums.append(op1 | op2)
    elif (opcode == '~'):
        nums.append(int(not op2))

    print "Do ", opcode

def process_equation(eq):
    global postfix_out
    for i in xrange(len(eq)):
        token = eq[i]

        if (token == ' '):
            continue
        elif (token == '('):
            ops.append(token)
        elif (token in ('&', '|', '~')):
            if (len(ops) == 0 
                or precedence(token) > precedence(ops[-1])):
                ops.append(token)
            else:
                do_op()
                ops.append(token)
        elif (token.isdigit()):
            nums.append(int(token))
            postfix_out += token
        elif (token == ')'):
            while (len(ops) != 0 and ops[-1] != '('):
                do_op()
            ops.pop() # remove that )

        if (i == len(eq)-1):
            while (len(ops) != 0):
                do_op()

        print "%2d ops: %s num: %s" % (i, str(ops).ljust(30), str(nums))

    print "Answer: ", nums.pop()
    print "Postfix:", postfix_out


process_equation(equation)

