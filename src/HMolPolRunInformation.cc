/********************************************
 * \author <b>Programmer:</b> Valerie Gray
 * \author <b>Assisted By:</b> Wouter Deconinck
 *
 * \brief <b>Purpose:</b> This is the file is to get the run information
 *
 * \date <b>Date:</b> 07-16-2013
 * \date <b>Modified:</b> 07-26-2013
 *
 * \note <b>Entry Conditions:</b>
 *
 ********************************************/

// root includes
#include <TObjString.h>

//HMolPol includes
#include "HMolPolRunInformation.hh"
#include "HMolPolVersion.hh"

///Root classes
ClassImp(HMolPolRunInformation)

/********************************************
 * Programmer: Valerie Gray
 * Function: HMolPolRunInformation
 *
 * Purpose: Construct the HMolPolRunInformation, and output the
 *      information to the ROOT tree
 *
 * Global:
 * Entry Conditions: none
 * Return:
 * Called By:
 * Date: 07-16-2013
 * Modified:
 ********************************************/
HMolPolRunInformation::HMolPolRunInformation()
{
  //Conditions.AddLast(new TObjString("Revision: " + TString(HMOLPOL_REVISION)));
  //Conditions.AddLast(new TObjString("Root: " + TString(HMOLPOL_ROOT)));
  //Conditions.AddLast(new TObjString("URL: " + TString(HMOLPOL_URL)));
  //Conditions.AddLast(new TObjString("Last changed author: " + TString(HMOLPOL_LAST_CHANGED_AUTHOR)));
  //Conditions.AddLast(new TObjString("Last changed date: " + TString(HMOLPOL_LAST_CHANGED_DATE)));
  //Conditions.AddLast(new TObjString("Last changed revision: " + TString(HMOLPOL_LAST_CHANGED_REV)));
}

/********************************************
 * Programmer: Valerie Gray
 * Function: ~HMolPolRunInformation
 *
 * Purpose: Destruct the HMolPolRunInformation
 *
 * Global:
 * Entry Conditions: none
 * Return:
 * Called By:
 * Date: 07-16-2013
 * Modified:
 ********************************************/
HMolPolRunInformation::~HMolPolRunInformation()
{

}
