#!/usr/bin/env python
# Display a given source file to a user if it is in the allowed list
# Shaun Meyer, Mar 2010
print "Content-Type: text/html"
print ""
print ""

import  cgitb, os
cgitb.enable()

base = '/home/meyersh/src/'

contents=""

allowed = open("allowed.txt").read().split('\n')

def printCode():
    for line in open("template.html"):
        line = line.replace('{{CODE}}', contents)
        line = line.replace('{{FILENAME}}', filename)
        print line

if os.environ['QUERY_STRING']:
    if os.environ['QUERY_STRING'].startswith('file='):
        filename = os.environ['QUERY_STRING'].split('=')[1]
        if (filename in allowed):
            if os.path.exists(base + filename):
                contents = open(base + filename).read().replace('<', '&lt;').replace('>', '&gt;')
                printCode()
                    
        else:
            print "You don't have permission to access this."
    else:
        print filename, 'does not exist!'
else:
    for link in allowed:
        print "<a href='?file=%s'>%s</a><br>" % (link, link)


    
