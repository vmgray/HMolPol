/***********************************************************
 * \author <b>Programmer:</b> Valerie Gray
 * \author <b>Assisted By:</b> Wouter Deconinck

\brief <b>Purpose:</b> Create the hmolpolroot executable, aka the version of ROOT
that know about HMolPol and can work with the HMolPol structure

//< TODO: Active only when hmolpolroot is running?
// But it doesn't seem to do anything, there is no meat to it?

\date <b>Date:</b> 07-16-2013
\date <b>Modified:</b> 04-20-2015

\note <b>Entry Conditions:</b> none
 *********************************************************/

// ROOT headers
#include <TSystem.h>
#include <TROOT.h>
#include <TString.h>

// HMolPol headers
#include "HMolPolRint.hh"

//< TODO: How really does this work> I have no idea what so ever
//Just a copy and paste job from Qweak

int main(int argc, char** argv)
{
  // Start CINT command which then is the HMolPol command prompt
  HMolPolRint* hmolpolrint =
    new HMolPolRint("HMolPol ROOT Analyzer", &argc, argv);

  //< TODO: what does this really mean? work with installed version
  // So that it can work with the installed version too

  //get all the includes for HMolPol, this is so it can tell what
  //the parts of the HMolPol ROOT tree are and the structure
  //< TODO: Uses HMolPolRint?? where??
  gROOT->ProcessLine(".include include");
  // Run the interface
  hmolpolrint->Run();
  //delete the object
  delete hmolpolrint;

  //< TODO: is this called only when using hmolpolroot?
  // 100% independent of HMolPol.cc? and running the simulation?
}



