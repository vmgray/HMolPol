//FILL IN ALL THESE COMMENTS

/********************************************
 * \author <b>Programmer:</b> Valerie Gray
 * \author <b>Assisted By:</b> Wouter Deconinck
 *
 * \brief <b>Purpose:</b> This is the file is in charge of I have no clue
 * \bug I really would like to know why this is here
 *
 * \date <b>Date:</b> 07-10-2013
 * \date <b>Modified:</b> 07-11-2013
 *
 * \note <b>Entry Conditions:</b>
 *
 * \todo comment the comment blocks to get the how this is all put together
 *
 ********************************************/

//Geant4 specific includes
#include <G4SDManager.hh>

//HMolPol includes
#include "HMolPolGenericDetector.hh"

/********************************************
 * Programmer: Valerie Gray
 * Function: G4VSensitiveDetector(name)
 *
 * Purpose:
 *
 * To do something with sensitive detectors
 *
 * Global:
 * Entry Conditions: none
 * Return:
 * Called By:
 * Date: 07-10-2013
 * Modified:
 ********************************************/
HMolPolGenericDetector::HMolPolGenericDetector(G4String name, G4int detnum)
:G4VSensitiveDetector(name)
{

}

/********************************************
 * Programmer: Valerie Gray
 * Function: ~HMolPolGenericDetector
 *
 * Purpose:
 *
 * To do the opposite of what ever the above does with sensitive detectors
 *
 * Global:
 * Entry Conditions: none
 * Return:
 * Called By:
 * Date: 07-10-2013
 * Modified:
 ********************************************/

HMolPolGenericDetector::~HMolPolGenericDetector()
{

}

/********************************************
 * Programmer: Valerie Gray
 * Function: Initialize
 *
 * Purpose:
 *
 * To do to initialize stuff I think
 *
 * Global:
 * Entry Conditions: none
 * Return:
 * Called By:
 * Date: 07-10-2013
 * Modified:
 ********************************************/

void HMolPolGenericDetector::Initialize(G4HCofThisEvent* HCE)
{

}

/********************************************
 * Programmer: Valerie Gray
 * Function: ProcessHits
 *
 * Purpose:
 *
 * To do to process the hits
 * \bug how?  But that is really for this whole file for
 *
 * Global:
 * Entry Conditions: none
 * Return:
 * Called By:
 * Date: 07-10-2013
 * Modified:
 ********************************************/

///////////////////////////////////////////////////////////////////////

G4bool HMolPolGenericDetector::ProcessHits(G4Step *step, G4TouchableHistory* )
{

}


/********************************************
 * Programmer: Valerie Gray
 * Function: EndOfEvent
 *
 * Purpose:
 *
 * To do to get end of Event information
 *
 * Global:
 * Entry Conditions: none
 * Return:
 * Called By:
 * Date: 07-10-2013
 * Modified:
 ********************************************/

///////////////////////////////////////////////////////////////////////

void HMolPolGenericDetector::EndOfEvent(G4HCofThisEvent* )
{

}


