/********************************************
 * \author <b>Programmer:</b> Valerie Gray
 * \author <b>Assisted By:</b> Wouter Deconinck
 *
 * \brief <b>Purpose:</b> This is the file is in charge of getting all the
 * detectors out of and then processing the hits of in each of these detectors
 *
 * \date <b>Date:</b> 07-10-2013
 * \date <b>Modified:</b> 07-11-2013
 *
 * \note <b>Entry Conditions:</b>
 *
 * \todo comment the comment blocks to get the how this is all put together
 *
 ********************************************/

// System includes
#include <sstream>

//Geant4 specific includes
#include <G4SDManager.hh>

//HMolPol includes
#include "HMolPolGenericDetector.hh"
#include "HMolPolGenericDetectorHit.hh"

// Initialize static variables
G4int HMolPolGenericDetector::fTotalNumberOfDetectors = 0;
std::map<G4String,G4int> HMolPolGenericDetector::fNumberOfDetectorsWithName;

/********************************************
 * Programmer: Valerie Gray
 * Function: HMolPolGenericDetector
 *
 * Purpose:
 *
 * To get the name of the generic detectors and number then.
 *
 * Global:
 * Entry Conditions: name
 * Return:
 * Called By:
 * Date: 07-10-2013
 * Modified:
 ********************************************/
HMolPolGenericDetector::HMolPolGenericDetector(G4String name)
: G4VSensitiveDetector(name),
  fVolumeName(""),
  fHitsCollection(0),
  fHitsCollectionID(-1)
{
  // Have we encountered a detector of this type yet?
  if (fNumberOfDetectorsWithName.count(SensitiveDetectorName) == 0) {
    // No, then this is the first one
    fNumberOfDetectorsWithName[SensitiveDetectorName] = 0;
  }

  // Insert name of this detector hits collection into the list of hit collections
  std::stringstream number;
  number << fNumberOfDetectorsWithName[SensitiveDetectorName];
  G4cout << "Inserting collection " << SensitiveDetectorName + "_" + number.str()
         << " in SD " << name << "." << G4endl;
  collectionName.insert(SensitiveDetectorName + "_" + number.str());

  // Increment the number of detectors with this name
  fNumberOfDetectorsWithName[SensitiveDetectorName]++;
  // Increment total number of detectors
  fTotalNumberOfDetectors++;
}

/********************************************
 * Programmer: Valerie Gray
 * Function: ~HMolPolGenericDetector
 *
 * Purpose:
 *
 * To delete all the generic detector and de-increment the count of
 *  generic detectors
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
  fTotalNumberOfDetectors--;
  fNumberOfDetectorsWithName[SensitiveDetectorName]--;
}

/********************************************
 * Programmer: Valerie Gray
 * Function: Initialize
 *
 * Purpose:
 *
 * To do to initialize stuff
 *      - the hits collection
 *      - and its ID
 *
 * Global:
 * Entry Conditions: G4HCofThisEvent
 * Return:
 * Called By:
 * Date: 07-10-2013
 * Modified:
 ********************************************/

void HMolPolGenericDetector::Initialize(G4HCofThisEvent* HCE)
{
  // Create new hits collection
  fHitsCollection = new HMolPolGenericDetectorHitsCollection
      (SensitiveDetectorName, collectionName[0]);

  // Get the hits collection ID number
  fHitsCollectionID =
      G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);

  // Add the hits collection ID number to the hits collections of this event
  HCE->AddHitsCollection(fHitsCollectionID, fHitsCollection);
}

/********************************************
 * Programmer: Valerie Gray
 * Function: ProcessHits
 *
 * Purpose:
 *
 * To do to process the hits for each detector
 *
 * Global:
 * Entry Conditions: G4Step, G4TouchableHistory
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
  G4TouchableHistory* touchable = (G4TouchableHistory*) prestep->GetTouchable();

  G4double edep = step->GetTotalEnergyDeposit();

  G4ParticleDefinition*  particleDefinition = step->GetTrack()->GetDefinition();


  // We're just going to record primary particles and things
  // that have just entered our boundary
  G4bool badhit = true;
  if (track->GetCreatorProcess() == 0 ||
      (prestep->GetStepStatus() == fGeomBoundary))
  {
    badhit = false;
  }

  //  Make pointer to new hit if it's a valid track
  HMolPolGenericDetectorHit *thisHit;
  if (!badhit) {
      thisHit = new HMolPolGenericDetectorHit();

      //Really I want this to be the name of the "detector" (with left and right
      // the closest I get is in detector construction line 238
      // How do I get that here?? or even better what I want??
      thisHit->SetTrackID(step->GetTrack()->GetTrackID());

      /// \todo Fix volume numbering for detector ID
      G4cout << touchable->GetVolume(0)->GetCopyNo() << G4endl;
      G4cout << touchable->GetVolume(1)->GetCopyNo() << G4endl;
      thisHit->SetDetectorID(touchable->GetVolume(1)->GetCopyNo());

      thisHit->SetPosition(prestep->GetPosition());
      thisHit->SetMomentum(prestep->GetMomentum());

      thisHit->SetKineticEnergy(step->GetTrack()->GetKineticEnergy());
      thisHit->SetTotalEnergy(step->GetTrack()->GetTotalEnergy());

      thisHit->SetParticleName(particleDefinition->GetParticleName());
      thisHit->SetParticleType(particleDefinition->GetPDGEncoding());

      // add hit to hit collection
      fHitsCollection->insert(thisHit);

      // delete this hit (a copy continues to live in hit collection)
      //delete thisHit;
  }
  return !badhit;
}


/********************************************
 * Programmer: Valerie Gray
 * Function: EndOfEvent
 *
 * Purpose:
 *
 * To do to get end of Event information - the hits collection
 *
 * Global:
 * Entry Conditions: G4HCofThisEvent
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
  if (fHitsCollectionID < 0)
  {
    // Get hit collection ID
    fHitsCollectionID = sdman->GetCollectionID(collectionName[0]);
  }

  HCE->AddHitsCollection(fHitsCollectionID, fHitsCollection);
}


