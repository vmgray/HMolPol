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

HMolPolGenericDetector::HMolPolGenericDetector( G4String name, G4int detnum )
: G4VSensitiveDetector(name){

  char colname[255];

  fDetNo = detnum;
  assert( fDetNo > 0 );

  //    fTrackSecondaries = false;
  fTrackSecondaries = true;

  sprintf(colname, "genhit_%d", detnum);
  collectionName.insert(G4String(colname));

  sprintf(colname, "gensum_%d", detnum);
  collectionName.insert(G4String(colname));

  fHCID = -1;
  fSCID = -1;
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

HMolPolGenericDetector::~HMolPolGenericDetector(){
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

void HMolPolGenericDetector::Initialize(G4HCofThisEvent *){

  fHitColl = new HMolPolGenericDetectorHitsCollection
      ( SensitiveDetectorName, collectionName[0] );

  fSumColl = new HMolPolGenericDetectorSumCollection
      ( SensitiveDetectorName, collectionName[1] );

  fSumMap.clear();
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

G4bool HMolPolGenericDetector::ProcessHits( G4Step *step, G4TouchableHistory *){
  G4bool badedep = false;
  G4bool badhit  = false;

  // Get touchable volume info
  G4TouchableHistory *hist =
      (G4TouchableHistory*)(step->GetPreStepPoint()->GetTouchable());
  G4int  copyID = hist->GetReplicaNumber();

  G4StepPoint *prestep = step->GetPreStepPoint();
  G4Track     *track   = step->GetTrack();

  G4double edep = step->GetTotalEnergyDeposit();


  // We're just going to record primary particles and things
  // that have just entered our boundary
  badhit = true;
  if( track->GetCreatorProcess() == 0 ||
      (prestep->GetStepStatus() == fGeomBoundary && fTrackSecondaries)
  ){
    badhit = false;
  }


  //  Make pointer to new hit if it's a valid track
  HMolPolGenericDetectorHit *thishit;
  if( !badhit ){
    thishit = new HMolPolGenericDetectorHit(fDetNo, copyID);
    fHitColl->insert( thishit );
  }

  //  Get pointer to our sum  /////////////////////////
  HMolPolGenericDetectorSum *thissum = NULL;

  if( !fSumMap.count(copyID) ){
    if( edep > 0.0 ){
      thissum = new HMolPolGenericDetectorSum(fDetNo, copyID);
      fSumMap[copyID] = thissum;
      fSumColl->insert( thissum );
    } else {
      badedep = true;
    }
  } else {
    thissum = fSumMap[copyID];
  }
  /////////////////////////////////////////////////////

  // Do the actual data grabbing

  if( !badedep ){
    // This is all we need to do for the sum
    thissum->fEdep += edep;
  }

  if( !badhit ){
    // Hit
    thishit->f3X = prestep->GetPosition();
    thishit->f3V = track->GetVertexPosition();
    thishit->f3P = track->GetMomentum();

    thishit->fP = track->GetMomentum().mag();
    thishit->fE = track->GetTotalEnergy();
    thishit->fM = track->GetDefinition()->GetPDGMass();

    thishit->fTrID  = track->GetTrackID();
    thishit->fmTrID = track->GetParentID();
    thishit->fPID   = track->GetDefinition()->GetPDGEncoding();

    // FIXME - Enumerate encodings
    thishit->fGen   = (long int) track->GetCreatorProcess();
  }

  return !badedep && !badhit;
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

void HMolPolGenericDetector::EndOfEvent(G4HCofThisEvent*HCE) {
  G4SDManager *sdman = G4SDManager::GetSDMpointer();

  if(fHCID<0){ fHCID = sdman->GetCollectionID(collectionName[0]); }
  if(fSCID<0){ fSCID = sdman->GetCollectionID(collectionName[1]); }

  HCE->AddHitsCollection( fHCID, fHitColl );
  HCE->AddHitsCollection( fSCID, fSumColl );

  return;
}


