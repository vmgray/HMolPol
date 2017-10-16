#!/usr/bin/python

# import (includes)
import os  # OS stuff
import sys  # System
from html5lib.treewalkers.etree_lxml import Root

# Get working directory (build directory)
presentcwd = os.getcwd()
# Change current working directory to the first command-line argument
# (as a string) supplied to the script in question.
# Main simulation directory
os.chdir(sys.argv[1])

#debugging
#print presentcwd
#print sys.argv[1]

# Grab All the information needed for the ROOT File
# String name = run a command and read the output of the command and strip any
# new line
# 
# os.open = Open a pipe to or from command. The return value is an open file object connected to the pipe,
# read() = read the output of the command (this would be stored)
# remove any new line charaters
HMOLPOL_REVISION = os.popen("git log --pretty=format:'%H' -n 1").read().rstrip('\n')
HMOLPOL_ROOT = os.popen("root-config --version").read().rstrip('\n')
HMOLPOL_LAST_CHANGED_AUTHOR = os.popen("git log --pretty=format:'%an' -n 1").read().rstrip('\n')
HMOLPOL_LAST_CHANGED_DATE = os.popen("git log --pretty=format:'%cd' -n 1").read().rstrip('\n')
HMOLPOL_CURRENT_BRANCH = os.popen("git symbolic-ref -q --short HEAD").read().rstrip('\n')
HMOLPOL_LAST_MASTER_REVISION = os.popen("git log master --pretty=format:'%H' -n 1").read().rstrip('\n')
HMOLPOL_LAST_MASTER_CHANGED_AUTHOR = os.popen("git log master --pretty=format:'%an' -n 1").read().rstrip('\n')
HMOLPOL_LAST_MASTER_CHANGED_DATE = os.popen("git log master --pretty=format:'%cd' -n 1").read().rstrip('\n')
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
if False:
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


# define empty stings, this will be printed when compiled will hold 
boringstring = "";

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

# define a new srting  which will be written to the screen when buliding
# simulation. Includes:
# Last commit #, author, date, message
# branch using, of local simulation to that on the repository
# Complie time, simulation directory, build directory
#
# Not included:
# Geant 4 info
# Cmake info


# Change directory to working directory (main simulation directory)
#os.chdir(presentcwd)

# set an output directory
outfile = sys.argv[2]

headertext = "// This file is generated automatically.All changes will be lost." \
  "\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s" % (Revision, Root, Last_Author,
                                        Last_Date, Branch, Master_Revision,
                                        Master_Author, Master_Date)


#debugging
#print "headertext:\n%s" % headertext

# write this run information to HMolPolVersion
newheader = open(outfile, "w")
# Write to the HMolPolVersion.hh file
newheader.write(headertext)
# Close file
newheader.close()

# print thie information to the terminal
#print "Repository information\n", boringstring
