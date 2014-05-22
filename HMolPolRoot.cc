/*
 * HMolPolRoot.cc
 *
 *  Created on: Jul 16, 2013
 *      Author: vmgray
 *
 *      This file will cause endless lose of sanity
 *
 */

// ROOT headers
#include <TSystem.h>
#include <TROOT.h>
#include <TString.h>

// HMolPol headers
#include "HMolPolRint.hh"

int main(int argc, char** argv)
{
  // Start CINT command which then is the HMolPol command prompt
  HMolPolRint* hmolpolrint =
    new HMolPolRint("HMolPol ROOT Analyzer", &argc, argv);
  // So that it can work with the installed version too
  gROOT->ProcessLine(".include include");
  // Run the interface
  hmolpolrint->Run();
  //delete the object
  delete hmolpolrint;
}



