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
  // Start HMolPol command prompt
  HMolPolRint* hmolpolrint = new HMolPolRint("HMolPol ROOT Analyzer", &argc, argv);
  gROOT->ProcessLine(".include include");
  hmolpolrint->Run();
  delete hmolpolrint;
}



