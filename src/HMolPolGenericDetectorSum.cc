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

#include "HMolPolGenericDetectorSum.hh"

G4Allocator<HMolPolGenericDetectorSum> HMolPolGenericDetectorSumAllocator;

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

HMolPolGenericDetectorSum::HMolPolGenericDetectorSum(int detid, int copyid){
  fDetID  = detid;
  fCopyID = copyid;
  fEdep   = 0.0;
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

HMolPolGenericDetectorSum::~HMolPolGenericDetectorSum(){
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

HMolPolGenericDetectorSum::HMolPolGenericDetectorSum(
    const HMolPolGenericDetectorSum &right) : G4VHit(){
  // copy constructor
  fDetID  = right.fDetID;
  fCopyID = right.fCopyID;
  fEdep   = right.fEdep;
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

const HMolPolGenericDetectorSum& HMolPolGenericDetectorSum
::operator =(const HMolPolGenericDetectorSum &right){
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

G4int HMolPolGenericDetectorSum::operator==(const HMolPolGenericDetectorSum &right ) const {
  return (this==&right) ? 1 : 0;
}
