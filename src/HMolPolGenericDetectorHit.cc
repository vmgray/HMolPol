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

// /todo does this link to ROOT tree?? double check

//user includes
#include "HMolPolGenericDetectorHit.hh"

/********************************************
 * Programmer: Valerie Gray
 * Function: HMolPolGenericDetectorHit
 *
 * Purpose:
 *
 * To initialize the variables that will be stored and used from each hit in
 * a hit in a generic detector.
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

  fParticleName = "None";
  fParticleType = -1;

  fKineticEnergy = 0.;
  fTotalEnergy  = 0.;

  fPosition = G4ThreeVector(0.0,0.0,-1000.0);

  G4cout << "Created hit" << G4endl;
}

/********************************************
 * Programmer: Valerie Gray
 * Function: HMolPolGenericDetectorHit
 *
 * Purpose:
 *
 * To get a copy of a HMolPolGenericDetectorHit and put info about it in
 * variables for the ROOT tree??
 *
 * Global:
 * Entry Conditions: HMolPolGenericDetectorHit
 * Return:
 * Called By:
 * Date: 07-10-2013
 * Modified:
 ********************************************/
// /todo what is the difference between this and the above function??

HMolPolGenericDetectorHit::HMolPolGenericDetectorHit(const HMolPolGenericDetectorHit& right)
{
  G4cout << "Copy constructor for HMolPolGenericDetectorHit called" << G4endl;

  fDetectorID = right.fDetectorID;
  fTrackID    = right.fTrackID;

  fKineticEnergy = right.fKineticEnergy;
  fTotalEnergy   = right.fTotalEnergy;

  fParticleName = right.fParticleName;
  fParticleType = right.fParticleType;
}

/********************************************
 * Programmer: Valerie Gray
 * Function: ~HMolPolGenericDetectorHit
 *
 * Purpose:
 *
 * Destroy the HMolPolGenericDetectorHit
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
