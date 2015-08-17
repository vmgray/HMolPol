/********************************************
 * \author <b>Programmer:</b> Valerie Gray
 * \author <b>Assisted By:</b> Wouter Deconinck
 *
 * \brief <b>Purpose:</b> This is the file is in charge of I have no clue
 * \bug I really would like to know why this is here
 *
 * \date <b>Date:</b> 07-15-2013
 * \date <b>Modified:</b> 08-11-2013
 *
 * \note <b>Entry Conditions:</b>
 *
 * \todo comment the comment blocks to get the how this is all put together
 *
 ********************************************/

// /todo does this link to ROOT tree?? double check

// geant4 includes
#include "G4VVisManager.hh"
#include "G4VisAttributes.hh"
#include "G4Circle.hh"
#include "G4Color.hh"

// user includes
#include "HMolPolGenericDetectorHit.hh"

/// In order to speed up the creation and deletion of new hits (something that
/// happens a lot in a typical geant4 simulation), geant4 uses a special class
/// that allocates (reserves) and deallocates (frees up) memory efficiency.
/// This custom allocator class has to be setup using G4Allocator, as in the
/// next line. You can give this allocator variable any name you like, as long
/// you create it somewhere.
G4Allocator<HMolPolGenericDetectorHit> HMolPolGenericDetectorHitAllocator;

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

  return;
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
// This is a copy constructor that take the object 'right' and copies into the
// current object, element by element (in this case).
HMolPolGenericDetectorHit::HMolPolGenericDetectorHit(const HMolPolGenericDetectorHit& right)
{
  //debugging
  G4cout << G4endl << "# In the HMolPolGenericDetectorHit::HMolPolGenericDetectorHit #"<< G4endl;
  G4cout << "Copy constructor for HMolPolGenericDetectorHit called" << G4endl;

  fDetectorID = right.fDetectorID;
  fTrackID    = right.fTrackID;

  fKineticEnergy = right.fKineticEnergy;
  fTotalEnergy   = right.fTotalEnergy;

  fParticleName = right.fParticleName;
  fParticleType = right.fParticleType;

  return;
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


/********************************************
 * Programmer: Wouter Deconinck
 * Function: Draw
 *
 * Purpose:
 *
 * Draw the hit on the display
 *
 * Global:
 * Entry Conditions: HMolPolGenericDetectorHit
 * Return:
 * Called By:
 * Date: 09-25-2013
 * Modified:
 ********************************************/
void HMolPolGenericDetectorHit::Draw()
{
  //debugging
  G4cout << G4endl << "# In the HMolPolGenericDetectorHit::Draw() #"<< G4endl;
  // Get the visualization manager
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();

  // If there is a visualization manager
  if (pVVisManager) {
    // Create a little filled circle at the position of the hit
    G4Circle circle(fPosition);
    circle.SetWorldSize(5.0*CLHEP::mm);
    circle.SetFillStyle(G4Circle::filled);
    // Create the visibility attributes
    G4Color color(1.,1.,0.);
    G4VisAttributes attribs(color);
    // Set the visibility attributes for the circle
    circle.SetVisAttributes(attribs);
    // Draw the circle
    pVVisManager->Draw(circle);
  }

  return;
}

/********************************************
 * Programmer: Wouter Deconinck
 * Function: Print
 *
 * Purpose:
 *
 * Print info about the hit
 *
 * Global:
 * Entry Conditions: HMolPolGenericDetectorHit
 * Return:
 * Called By:
 * Date: 09-25-2013
 * Modified:
 ********************************************/
void HMolPolGenericDetectorHit::Print()
{
  //debugging
  G4cout << G4endl << "# In the HMolPolGenericDetectorHit::Print #"<< G4endl;
  G4cout << "  Hit: fDetectorID[" << fDetectorID << "], fTrackID[" << fTrackID << "]: "
      << " --- position (x,y,z) [cm] "
      << fPosition.x()/CLHEP::cm << ", "
      << fPosition.y()/CLHEP::cm << ", "
      << fPosition.z()/CLHEP::cm << ", "
      << G4endl;

  return;
}
