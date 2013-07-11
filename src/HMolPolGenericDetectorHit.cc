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

#include "HMolPolGenericDetectorHit.hh"

G4Allocator<HMolPolGenericDetectorHit> HMolPolGenericDetectorHitAllocator;

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

HMolPolGenericDetectorHit::HMolPolGenericDetectorHit(G4int det, G4int copy){
  fDetID  = det;
  fCopyID = copy;

  f3X = G4ThreeVector(-1e9, -1e9, -1e9);
  f3P = G4ThreeVector(-1e9, -1e9, -1e9);
  f3V = G4ThreeVector(-1e9, -1e9, -1e9);

  fP  = -1.0;
  fE  = -1.0;
  fM  = -1.0;

  fTrID  = -1;
  fPID   = (G4int) 1e9;
  fmTrID = -1;

  fGen   = 1;
}

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

HMolPolGenericDetectorHit::~HMolPolGenericDetectorHit(){
}

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

HMolPolGenericDetectorHit::HMolPolGenericDetectorHit(const HMolPolGenericDetectorHit &right) : G4VHit(){
  // copy constructor

  fDetID  = right.fDetID;
  fCopyID = right.fCopyID;
  f3X     = right.f3X;
  f3P     = right.f3P;
  f3V     = right.f3V;

  fP      = right.fP;
  fE      = right.fE;
  fM      = right.fM;

  fTrID   = right.fTrID;
  fPID    = right.fPID;
  fmTrID  = right.fmTrID;
  fGen    = right.fGen;
}

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

const HMolPolGenericDetectorHit& HMolPolGenericDetectorHit::operator =(const HMolPolGenericDetectorHit &right){
  (*this) = right;
  return *this;
}

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

G4int HMolPolGenericDetectorHit::operator==(const HMolPolGenericDetectorHit &right ) const {
  return (this==&right) ? 1 : 0;
}
