/*
 * HMolPolRunInformation.cc
 *
 *  Created on: Jul 16, 2013
 *      Author: vmgray
 */

// root includes
#include <TObjString.h>

//HMolPol includes
#include "HMolPolRunInformation.hh"
#include "HMolPolVersion.hh"

ClassImp(HMolPolRunInformation)

HMolPolRunInformation::HMolPolRunInformation()
{
  //Conditions.AddLast(new TObjString("Revision: " + TString(HMOLPOL_REVISION)));
  //Conditions.AddLast(new TObjString("Root: " + TString(HMOLPOL_ROOT)));
  //Conditions.AddLast(new TObjString("URL: " + TString(HMOLPOL_URL)));
  //Conditions.AddLast(new TObjString("Last changed author: " + TString(HMOLPOL_LAST_CHANGED_AUTHOR)));
  //Conditions.AddLast(new TObjString("Last changed date: " + TString(HMOLPOL_LAST_CHANGED_DATE)));
  //Conditions.AddLast(new TObjString("Last changed revision: " + TString(HMOLPOL_LAST_CHANGED_REV)));
}

HMolPolRunInformation::~HMolPolRunInformation()
{

}
