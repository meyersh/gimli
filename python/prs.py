#!/usr/bin/python 
# paper - rock - scissors
# 
# Shaun Meyer, Sept 2010

#
GENDER = 'M'

#
import cmd

def prs_cmp(a, b):
    if a == 'p':
        if b == 'p':
            return 0
        elif b == 'r':
            return 1
        elif b == 's':
            return -1
    elif a == 'r':
        if b == 'p':
            return -1
        elif b == 'r':
            return 0
        elif b == 's':
            return 1
    elif a == 's':
        if b == 'p':
            return 1
        elif b == 'r':
            return -1
        elif b == 's':
            return 0

# return whatever beats a.
def what_beats(a):
    if a == 'p':
        return 's'
    if a == 'r':
        return 'p'
    if a == 's':
        return 'r'

def long(a):
    if a == 'p':
        return 'Paper'
    elif a == 'r':
        return 'Rock'
    elif a == 's':
        return 'Scissors'

human_history = list()
machine_history = list()

# Generate a throw. human would be what human
# threw last.
def throw():
    """ if we're starting, throw paper... """
    if len(human_history) == 0:
        print 'Machine: Paper'
        return 'p'

    """ if the same move is thrown twice in a row
        counter with what beats it... """
    if len(human_history) > 1:
        """ If the human plays the same move 
        twice in a row - play what beats it. """
        if human_history[-1] == human_history[-2]:
            r =  what_beats(human_history[-1])
            print 'Machine: %s' % long(r)
            return r
        
        elif ( prs_cmp( human_history[-1], machine_history[-1])
               + prs_cmp(human_history[-2], machine_history[-2]) == -2):
            ''' We're in a losing streak. Try a third look ahead. '''
            r = what_beats(what_beats(what_beats(human_history[-1])))
            print 'Machine: %s' % long(r)
            return r

        elif ( human_history[-1] == machine_history[-1] and
               human_history[-2] == machine_history[-2] ):
            ''' If we experience two stalemates in a row dumb
            down the strategy and play what would beat them... '''
            r = what_beats( human_history[-1] )
            print 'Machine: %s' % long(r)
            return r
        
        elif ( human_history[-1] == machine_history[-1] ):
            """ deal with (only) one stalemate. This is tricky because 
            they may  either take the high road or the low road.. """
            print "Dealing with stalemate..."
            r = what_beats(  human_history[-1] )
            print 'Machine: %s' % long(r)
            return r

        else:
            ''' Otherwise, play what beats what beat you. '''
            r = what_beats(what_beats(human_history[-1]))
            print 'Machine: %s' % long(r)
            return r

    else:
        r = what_beats(human_history[-1])
        print 'Machine: %s' % long(r)
        return r

class prs(cmd.Cmd):
    """ Paper Rock Scissors! """

    def do_p(self, line):
        print "Human:  Paper."
        machine_history.append(throw())
        human_history.append('p')
        print prs_cmp('p', machine_history[-1])

    def do_r(self, line):
        print "Human:  Rock."
        machine_history.append(throw())
        human_history.append('r')
        print prs_cmp('r', machine_history[-1])

    def do_s(self, line):
        print "Human:  Scissors."
        machine_history.append(throw())
        human_history.append('s')
        print prs_cmp('s', machine_history[-1])

    def do_history(self, line):
        print "Human:   ", human_history
        print "Machine: ", machine_history

    def do_score(self, line):
        score = 0
        for i in range(len(human_history)):
            score += prs_cmp(human_history[i], machine_history[i])
        print "Score: %d" % score

    def do_EOF(self, line):
        return True

    def do_q(self, line):
        return True

    def do_reset(self, line):
        del human_history[:]
        del machine_history[:]
        print "Game reset."

if __name__ == '__main__':
    print """ Welcome to Paper - Rock - Scissors. A game of strategy 
    To throw, type either p, r, or s and press Enter. The computers 
    throw and the results will be displayed. 

    To view the history you may enter 'history'. 'score' will show the 
    combined score to this point. You may also 'reset' the game and 'q' 
    to quit. 

    The computer does _not_ cheat and, in this case, does _not_ use 
    random numbers.

    Good luck.
    """
    prs().cmdloop()

