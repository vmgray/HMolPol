// Generic Primary Hit class loosely based on HMolPolGenericDetectorHit

// geant4 includes
#include "G4VVisManager.hh"
#include "G4VisAttributes.hh"
#include "G4Circle.hh"
#include "G4Color.hh"

// user includes
#include "HMolPolPrimaryHit.hh"

/// Hit allocator instance
G4Allocator<HMolPolPrimaryHit> HMolPolPrimaryHitAllocator;

/********************************************
 * Default constructor
 *
 * Initialize a new instance of a Primary Hit
 *
 ********************************************/
HMolPolPrimaryHit::HMolPolPrimaryHit()
{
  fVolumeName = "NULL";
  fPosition = G4ThreeVector(0.0,0.0,0.0);
  fMomentum = G4ThreeVector(0.0,0.0,0.0);
}

/********************************************
 * Copy constructor
 *  -- copies contents from passed instance of primary hit class.
 *
 ********************************************/
HMolPolPrimaryHit::HMolPolPrimaryHit(const HMolPolPrimaryHit& right)
{
  //debugging
  G4cout << G4endl << "# In the HMolPolPrimaryHit::HMolPolPrimaryHit #"<< G4endl;
  G4cout << "Copy constructor for HMolPolPrimaryHit called" << G4endl;

  fVolumeName = right.fVolumeName;
  fPosition     = right.fPosition;
  fMomentum     = right.fMomentum;
}

/********************************************
 * Default destructor
 ********************************************/
HMolPolPrimaryHit::~HMolPolPrimaryHit()
{

}
