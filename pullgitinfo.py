#!/usr/bin/python

# import (includes)
import os  # OS stuff
import sys  # System
from html5lib.treewalkers.lxmletree import Root

# Get working directory (build directory)
presentcwd = os.getcwd()
# Change current working directory to the first command-line argument
# (as a string) supplied to the script in question.
# Main simulation directory
os.chdir(sys.argv[1])

print presentcwd
print sys.argv[1]

# Grab All the information needed for the ROOT File
HMOLPOL_REVISION = os.popen("git log --pretty=format:'%H' -n 1").read()
HMOLPOL_ROOT = os.popen("root-config --version").read()
HMOLPOL_LAST_CHANGED_AUTHOR = os.popen("git log --pretty=format:'%an' -n 1").read()
HMOLPOL_LAST_CHANGED_DATE = os.popen("git log --pretty=format:'%cd' -n 1").read()
HMOLPOL_CURRENT_BRANCH = os.popen("git symbolic-ref -q --short HEAD").read()
HMOLPOL_LAST_MASTER_REVISION = os.popen("git log HEAD --pretty=format:'%H' -n 1").read()
HMOLPOL_LAST_MASTER_CHANGED_AUTHOR = os.popen("git log HEAD --pretty=format:'%an' -n 1").read()
HMOLPOL_LAST_MASTER_CHANGED_DATE = os.popen("git log HEAD --pretty=format:'%cd' -n 1").read()
# HMOLPOL_URL =

# create the lines for the file
Revision = "#define HMOLPOL_REVISION \t\"%s\"" % HMOLPOL_REVISION
Root = "#define HMOLPOL_ROOT \t\"%s\"" % HMOLPOL_ROOT
Last_Author = "#define HMOLPOL_LAST_CHANGED_AUTHOR \t\"%s\"" % HMOLPOL_LAST_CHANGED_AUTHOR
Last_Date = "#define HMOLPOL_LAST_CHANGED_DATE \t\"%s\"" % HMOLPOL_LAST_CHANGED_DATE
Branch = "#define HMOLPOL_CURRENT_BRANCH \t\"%s\"" % HMOLPOL_CURRENT_BRANCH
Master_Revision = "#define HMOLPOL_LAST_MASTER_REVISION \t\"%s\"" % HMOLPOL_LAST_MASTER_REVISION
Master_Author = "#define HMOLPOL_LAST_MASTER_CHANGED_AUTHOR \t\"%s\"" % HMOLPOL_LAST_MASTER_CHANGED_AUTHOR
Master_Date = "#define HMOLPOL_LAST_MASTER_CHANGED_DATE \t\"%s\"" % HMOLPOL_LAST_MASTER_CHANGED_DATE

# debugging
print Revision
print Root
print Last_Author
print Last_Date
print Branch
print Master_Revision
print Master_Author
print Master_Date

f = os.popen("git log -n 1 && git status -bs && " \
             "echo \"\nGeant4 version\" `geant4-config --version` && " \
             "echo \"  ROOT version\" `root-config --version` && "\
             "echo \" `cmake --version`\" && echo \"\nGenerated at `date`\"")

# define 2 empty stings
boringstring = "";
fullstring = "";

# if f exits
if (f != 0):
  # for each line in f add it to the boringstring
  for line in f:
    boringstring += line
else:  # if f doesn't exist
  boringstring = "git information unavailable"

# variable, for max lengh of boringstring
maxlen = 2048

# add in the source directory location to boringstring
boringstring += "Source dir " + os.getcwd()
# add in the build directory location to boringstring
boringstring += "\nBuild  dir " + presentcwd + "\n"

# If the lenght of the boringstring is greater than maxlen give warning
if  len(boringstring) > maxlen:
  print "WARNING:  Truncating info from git";
  # Cut the boringstring lenght and value to only the charaters numbering 0 to maxlen-1
  boringstring = boringstring[0:maxlen - 1]

# for each charater in the boringstring and encode in hex store in fullstring
for x in boringstring:
  fullstring += '\\x' + x.encode('hex')

# define a new srting  which will be written to the screen when buliding
# simulation. Includes:
# Last commit #, author, date, message
# branch using, of local simulation to that on the repository
# Geantt 4 info
# Cmake info
# Complie time, simulation directory, build directory
newheadertext = """#ifndef __GITINFO_HH
  #define __GITINFO_HH

/*
  Generated automatically by cmake process
  Encoding:
-------------------------------------------------------------
""" + boringstring + """
-------------------------------------------------------------
*/

#define __GITMAXINFO_SIZE 2048

#define gGitInfoStr \"""" + fullstring + '\"' \
+ \
"""

#endif//__GITINFO_HH
"""

# Change directory to working directory (main simulation directory)
#os.chdir(presentcwd)

# set an output directory
outdir = "include/"
# If this diectroy doesn't exist, creat it
if not os.path.exists(outdir):
  os.makedirs(outdir)

headertext = "// This file is generated automatically.All changes will be lost." \
  "\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s" % (Revision, Root, Last_Author,
                                        Last_Date, Branch, Master_Revision,
                                        Master_Author, Master_Date)

#debugging
#print "headertext: %s" % headertext


# write this run information to HMolPolVersion
newheader = open(outdir + "/HMolPolVersion.hh", "w")
# Write to the HMolPolVersion.hh file
newheader.write(headertext)
# Close file
newheader.close()

# Open a writeable file in outdir called gitinfo.hh
# newheader = open( outdir + "/gitinfo.hh", "w")
# write newheadertext to this file
# newheader.write(newheadertext)
# Close file
# newheader.close()

# print thie information to the terminal
print "Repository information\n", boringstring
