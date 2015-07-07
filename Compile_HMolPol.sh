#!/bin/bash

#Programmer: Valerie Gray
#Purpose:
# This makes and complies HMolPol.
# It doesn't create any of the needed files to make a ROOT Tree
# therefore it's purpose is to make the exicutables so that one
# can use the HMolPol Geant4 simulation and Root version
#
#Date: 07-06-2015
#Modified:
#Assisted By:

######################################
#now build HMolPol
#####################################

echo -e "Make HMolPol \n"
#build and make ValerianROOT
mkdir -p build
cd build
cmake ..
make

echo -e "\nDone. If there is no errors, great!! If so, sorry try again."


: <<'COMMENT'

#echo "done begin comment"
#also works for perl :)
# print <<<COMMENT
#COMMENT

COMMENT
