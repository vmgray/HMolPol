/********************************************
 * \author <b>Programmer:</b> Valerie Gray
 * \author <b>Assisted By:</b> Wouter Deconinck
 *
 * \brief <b>Purpose:</b> This is the file is in charge creating the HMolPol
 *      ROOT version (hmolpolroot).  This is so that ROOT can handle
 *      the tree structure.
 *      Functionality of the command prompt.
 *
 * \date <b>Date:</b> 07-16-2013
 * \date <b>Modified:</b> 07-26-2013
 *
 * \note <b>Entry Conditions:</b> none
 *
 ********************************************/

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

/********************************************
 * Programmer: Valerie Gray
 * Function: HMolPolRint
 *
 * Purpose:  Construct the Root prompts and ROOT version
 *
 *
 * Global:
 * Entry Conditions: appClassName,
 *      argc,
 *      argv,
 *      options,
 *      numOptions,
 *      noLogo
 * Return:
 * Called By:
 * Date: 07-16-2013
 * Modified:
 ********************************************/
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

/********************************************
 * Programmer: Valerie Gray
 * Function: ~HMolPolRint
 *
 * Purpose:  destructor of HMolPolRint
 *
 *
 * Global:
 * Entry Conditions: none
 * Return:
 * Called By:
 * Date: 07-16-2013
 * Modified:
 ********************************************/
//---------------------------------------------------------------------------
HMolPolRint::~HMolPolRint()
{
  // Reset point to self
  if (fExists == this)
    fExists = NULL;
}



