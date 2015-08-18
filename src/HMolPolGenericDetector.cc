/********************************************
 * \author <b>Programmer:</b> Valerie Gray
 * \author <b>Assisted By:</b> Wouter Deconinck
 *
 * \brief <b>Purpose:</b> This is the file is in charge of getting all the
 * detectors out of and then processing the hits of in each of these detectors
 *
 * \date <b>Date:</b> 07-10-2013
 * \date <b>Modified:</b> 08-17-2015
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
//#include "HMolPolGenericDetectorHit.hh"

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
  //debugging
  G4cout << G4endl << "## In the HMolPolGenericDetector::HMolPolGenericDetector() ##"<< G4endl;

  // Have we encountered a detector of this type yet?
  if (fNumberOfDetectorsWithName.count(SensitiveDetectorName) == 0) {
    // No, then this is the first one
    fNumberOfDetectorsWithName[SensitiveDetectorName] = 0;
  }

  // Insert name of this detector hits collection into the list of hit collections
  std::stringstream number;
  number << fNumberOfDetectorsWithName[SensitiveDetectorName];
  G4cout << "  Inserting collection " << SensitiveDetectorName + "_" + number.str()
         << " in SD " << name << "." << G4endl;
  collectionName.insert(SensitiveDetectorName + "_" + number.str());

  // Increment the number of detectors with this name
  fNumberOfDetectorsWithName[SensitiveDetectorName]++;
  // Increment total number of detectors
  fTotalNumberOfDetectors++;

  return;
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
  //debugging
  G4cout << G4endl << "## In the HMolPolGenericDetector::~HMolPolGenericDetector() ##"<< G4endl;
  fTotalNumberOfDetectors--;
  fNumberOfDetectorsWithName[SensitiveDetectorName]--;

  return;
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
  //debugging
  G4cout << G4endl << "## In the HMolPolGenericDetector::Initialize ##"<< G4endl;

  G4cout << "  Initializing " << SensitiveDetectorName << G4endl;

  // Create new hits collection
  fHitsCollection = new HMolPolGenericDetectorHitsCollection
      (SensitiveDetectorName, collectionName[0]);

  // Get the hits collection ID number
  fHitsCollectionID =
      G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);

  // Add the hits collection ID number to the hits collections of this event
  HCE->AddHitsCollection(fHitsCollectionID, fHitsCollection);

  return;
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
 * Modified: 07-15-2015
 ********************************************/
G4bool HMolPolGenericDetector::ProcessHits(G4Step* step, G4TouchableHistory* )
{
  //debugging
  G4cout << G4endl << "## In the HMolPolGenericDetector::ProcessHits ##"<< G4endl;
  G4cout << "  Processing hit in " << SensitiveDetectorName << G4endl;

  //get rid of unused parameter warning
  // Get touchable volume info
  //G4TouchableHistory* hist =
  //    (G4TouchableHistory*)(step->GetPreStepPoint()->GetTouchable());
  //get rid of unused parameter warning
  //G4int copyID = hist->GetReplicaNumber();

  G4StepPoint* prestep = step->GetPreStepPoint();
  G4Track* track   = step->GetTrack();
  //No longer need
  //  G4TouchableHistory* touchable = (G4TouchableHistory*) prestep->GetTouchable();

  //loop through our vector of names and make sure the physical volume
  //is registered to to this detector instance (logical Volume)
  //track has the volume name
  G4int VolumeIndex = -1;

  for(size_t i = 0; i < fPhysicalVolumeNames.size(); i++)
  {
    if( fPhysicalVolumeNames[i].compareTo(track->GetVolume()->GetName()) == 0)
    {
      VolumeIndex = i;
      break;
    }
  }
/*
  //debugging
  G4cout << "  Searching for Volume Names " << track->GetVolume()->GetName() <<
      " Found at Volume index: "<< VolumeIndex << G4endl;
  G4cout << "  Volume Stored " << fPhysicalVolumeNames[VolumeIndex] <<
        " for Volume index: "<< VolumeIndex << G4endl;
  G4cout << "  Volume Type ID: " << fHitsCollectionID << G4endl;
*/

  //if the VolumeIndex is less than 0 it it is not part of this detector instance
  //so we ignore it
  if(VolumeIndex < 0)
  {
    G4cerr << "Hit not associated with: " << SensitiveDetectorName << G4endl;
    return false;
  }


  //get rid of unused parameter warning
  //G4double edep = step->GetTotalEnergyDeposit();

  G4ParticleDefinition*  particleDefinition = step->GetTrack()->GetDefinition();

  //TODO THIS IS WRONG FIX.
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
  if (!badhit)
  {
    thisHit = new HMolPolGenericDetectorHit();

    //Really I want this to be the name of the "detector" (with left and right
    // the closest I get is in detector construction line 238
    // How do I get that here?? or even better what I want??
    thisHit->SetTrackID(track->GetTrackID());

    //Set the detector type ID, which is associated with the logical volume
    //Logical volumes have one branch this makes sure the hits land in the
    //corresponding logical detector branch
    thisHit->SetDetectorTypeID(fHitsCollectionID);

    //Detector name is the name of the physical volume
    //detector ID is the index the physical volume had in
    //fPhysicalVolumeNames vector.
    thisHit->SetDetectorID(VolumeIndex);
    thisHit->SetDetectorName(fPhysicalVolumeNames[VolumeIndex]);

    thisHit->SetPosition(prestep->GetPosition());
    thisHit->SetMomentum(prestep->GetMomentum());

    thisHit->SetKineticEnergy(track->GetKineticEnergy());
    thisHit->SetTotalEnergy(track->GetTotalEnergy());

    thisHit->SetParticleName(particleDefinition->GetParticleName());
    thisHit->SetParticleType(particleDefinition->GetPDGEncoding());

/*
    //debugging
    G4cout << G4endl << "  What is added to Hit to Collection" << G4endl;
    G4cout << "  Searching for Volume Names " << track->GetVolume()->GetName() <<
        " Found at Volume index: "<< VolumeIndex << G4endl;
    G4cout << "  Volume Stored " << thisHit->GetDetectorName() <<
          " for Volume index: "<< thisHit->GetDetectorID() << G4endl;
    G4cout << "  Volume Type ID: " << thisHit->GetDetectorTypeID() << G4endl;
*/


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
void HMolPolGenericDetector::EndOfEvent(G4HCofThisEvent* HCE)
{
  //debugging
  G4cout << G4endl << "## In the HMolPolGenericDetector::EndOfEvent ##"<< G4endl;
  G4cout << "  End of event for detector " << SensitiveDetectorName << G4endl;

  // Get pointer to the sensitive detector manager
  G4SDManager* sdman = G4SDManager::GetSDMpointer();

  // If no ID for this hit collection
  if (fHitsCollectionID < 0)
  {
    // Get hit collection ID
    fHitsCollectionID = sdman->GetCollectionID(collectionName[0]);
  }

  HCE->AddHitsCollection(fHitsCollectionID, fHitsCollection);

  return;
}
