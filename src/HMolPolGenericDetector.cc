//FILL IN ALL THESE COMMENTS
// this does nothing - just taking up space

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
#include "HMolPolGenericDetectorHit.hh"

// Initialize static variables
G4int HMolPolGenericDetector::fNumberOfDetectors = 0;

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
HMolPolGenericDetector::HMolPolGenericDetector(G4String name)
:G4VSensitiveDetector(name)
{
  // Increment number of detectors
  fNumberOfDetectors++;

  // Insert name of this detector hits into the list of hit collections
  collectionName.insert(G4String("hit_") + fNumberOfDetectors);

  // No hit collection yet
  fHitCollID = -1;
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
  fNumberOfDetectors--;
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

  fHitColl = new HMolPolGenericDetectorHitsCollection
      (SensitiveDetectorName, collectionName[0]);
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
  // Get touchable volume info
  G4TouchableHistory *hist =
      (G4TouchableHistory*)(step->GetPreStepPoint()->GetTouchable());
  G4int copyID = hist->GetReplicaNumber();

  G4StepPoint *prestep = step->GetPreStepPoint();
  G4Track     *track   = step->GetTrack();

  G4double edep = step->GetTotalEnergyDeposit();


  // We're just going to record primary particles and things
  // that have just entered our boundary
  G4bool badhit = true;
  if (track->GetCreatorProcess() == 0 ||
      (prestep->GetStepStatus() == fGeomBoundary))
  {
    badhit = false;
  }

  //  Make pointer to new hit if it's a valid track
  HMolPolGenericDetectorHit *thishit;
  if (!badhit) {
      thishit = new HMolPolGenericDetectorHit();
      thishit->SetPosition(prestep->GetPosition());
      thishit->SetMomentum(prestep->GetMomentum());
      fHitColl->insert(thishit);
  }
  return !badhit;
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

void HMolPolGenericDetector::EndOfEvent(G4HCofThisEvent* HCE)
{
  // Get pointer to the sensitive detector manager
  G4SDManager *sdman = G4SDManager::GetSDMpointer();

  // If no ID for this hit collection
  if (fHitCollID < 0)
  {
    // Get hit collection ID
    fHitCollID = sdman->GetCollectionID(collectionName[0]);
  }

  HCE->AddHitsCollection(fHitCollID, fHitColl);
}


