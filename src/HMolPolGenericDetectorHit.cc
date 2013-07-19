/********************************************
 * \author <b>Programmer:</b> Valerie Gray
 * \author <b>Assisted By:</b> Wouter Deconinck
 *
 * \brief <b>Purpose:</b> This is the file is in charge of I have no clue
 * \bug I really would like to know why this is here
 *
 * \date <b>Date:</b> 07-15-2013
 * \date <b>Modified:</b> 07-18-2013
 *
 * \note <b>Entry Conditions:</b>
 *
 * \todo comment the comment blocks to get the how this is all put together
 *
 ********************************************/

// this does nothing - just taking up space

//user includes
#include "HMolPolGenericDetectorHit.hh"

/********************************************
 * Programmer: Valerie Gray
 * Function: HMolPolGenericDetectorHit
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
HMolPolGenericDetectorHit::HMolPolGenericDetectorHit()
{
  //initialization
  fDetectorID  = -1;
  fTrackID = -1;

  fPosition = G4ThreeVector(0.0,0.0,-1000.0);

  G4cout << "Created hit" << G4endl;
}

HMolPolGenericDetectorHit::HMolPolGenericDetectorHit(const HMolPolGenericDetectorHit& right)
{
  G4cout << "Copy constructor for HMolPolGenericDetectorHit called" << G4endl;
  fDetectorID = right.fDetectorID;
  fTrackID = right.fTrackID;
}
/********************************************
 * Programmer: Valerie Gray
 * Function: ~HMolPolGenericDetectorHit
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
HMolPolGenericDetectorHit::~HMolPolGenericDetectorHit()
{

}
