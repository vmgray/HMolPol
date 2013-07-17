/*
 * HMolPolRint.cc
 *
 *  Created on: Jul 16, 2013
 *      Author: vmgray
 */

#include "HMolPolRint.hh"

// Standard C and C++ headers
#include <iostream>
#include <cstdlib>

// ROOT headers
#include <TROOT.h>

// Global pointers
HMolPolRint* gHMolPolRint = NULL;

// Pointer to self
HMolPolRint* HMolPolRint::fExists = NULL;


//--------------------------------------------------------------------------
HMolPolRint::HMolPolRint (const char* appClassName, int* argc, char** argv,
                      void* options, int numOptions, bool noLogo)
: TRint (appClassName, argc, argv, options, numOptions, noLogo)
{
  gHMolPolRint = this;

  // HMolPol-Root command prompt
  SetPrompt("HMolPol [%d] ");

  // Pointer to self
  fExists = this;
}

//---------------------------------------------------------------------------
HMolPolRint::~HMolPolRint()
{
  // Reset point to self
  if (fExists == this)
    fExists = NULL;
}



