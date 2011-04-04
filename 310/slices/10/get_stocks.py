#!/usr/bin/env python
# Shaun Meyer, Apr 2011
# From CSV info provided online at 
# http://www.gummy-stuff.org/Yahoo-data.htm

import httplib
import sys

symbols=""
urls=[]
counter = 0;

# Compile the symbol list into a list of URL's to get.
# We also keep the number of symbols per request below
# 198 here.

for symbol in open("symbols.txt"):
    symbol = symbol.rstrip('\n')
    if (symbols != ""):
        if symbol != "":
            symbols += "+%s" % symbol 
    else:
        symbols = symbol
    counter += 1

    if counter > 198:
        urls.append("/d/quotes.csv?s=%s&f=sh" % symbols)
        symbols = ""
        counter=0

#
# download all csv.
#
    
csvres = ""
conn = httplib.HTTPConnection("download.finance.yahoo.com")
for url in urls:
    conn.request("GET", url)
    res = conn.getresponse()
    csvres += res.read()

# clean up csvres (remove quotes and change commas to colons.)
csvres = csvres.replace(',', ':').replace('"', '')

# Save to a file (if specified) otherwise, stdout.
if (len(sys.argv) > 1):
    ofile = open(sys.argv[1], 'w')
    ofile.write(csvres)
    ofile.close()
else:
    print csvres


