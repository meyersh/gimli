#!/usr/bin/env python

# I am sick of having to manually ZIP things for class, ugh!, huge drag.

import os
import sys

#"zip S3.P1.zip -r 3.1\ \(boolean\ postfix\)/ -x '*/*.svn*' 3.1\ \(boolean\ postfix\)/infix.py"
zip_command = "/usr/local/bin/zip"

excludes = ['"*/*.svn*"', 'zip.cfg']
zip_file = None

try:
    # this can append to `excludes`, etc
    execfile("./zip.cfg");
except:
    print "No zip.cfg, moving on..."

if (len(sys.argv) > 1):
    zip_file = sys.argv[1].rstrip('/') + '.zip'
    zip_dir = sys.argv[1]
    

if not zip_file:
    zip_file=os.path.split( os.getcwd() )[1].rstrip('/') + '.zip'
    zip_dir=os.path.split( os.getcwd() )[1]

cmd = "%s '%s' -r '%s' -x %s" % (zip_command, zip_file, zip_dir, ' '.join(excludes))
print cmd



