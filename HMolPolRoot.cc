/***********************************************************
 * \author <b>Programmer:</b> Valerie Gray
 * \author <b>Assisted By:</b> Wouter Deconinck

\brief <b>Purpose:</b> Create the hmolpolroot executable, aka the version of ROOT
that know about HMolPol and can work with the HMolPol structure

This executable is active run when hmolpolroot is running. All it does is
provide the HMolPol classes to the user through a ROOT interface. All other
classes are compiled and saved to a shared library which is then used by this
executable, so it does not need to explicitly include all the header files.
That is why it can be short, sweet, and to the point.

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

// When the main function is called, all we do is start a command prompt as
// an interface to ROOT. Whatever is typed on the command prompt will be
// handled by ROOT, possibly using the classes of HMolPol that have been
// compiled and loaded as a shared library.

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
  // Run the interface, the program will spend all of its time in this next line
  hmolpolrint->Run();
  //delete the object
  delete hmolpolrint;

  //< TODO: is this called only when using hmolpolroot?
  // 100% independent of HMolPol.cc? and running the simulation?
}



