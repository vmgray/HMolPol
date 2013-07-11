//FILL IN ALL THESE COMMENTS

/********************************************
 * Programmer: Valerie Gray
 * Purpose:
 *
 * This is header file for the generic detector.  What it does no one knows
 *
 * Entry Conditions:
 * Date: 06-15-2013
 * Modified: 07-06-2013
 * Assisted By: Wouter Deconinck
 ********************************************/

#ifndef HMOLPOLGENERICDETECTOR_HH_
#define HMOLPOLGENERICDETECTOR_HH_

#include "G4VSensitiveDetector.hh"
#include "HMolPolGenericDetectorHit.hh"
#include "HMolPolGenericDetectorSum.hh"

#include <map>

/*! 
      Default detector class.  This will record information on:

      - Primary generated hit information
      - Secondary hit information on particles not produced within the volume
      - Calorimetric data (total energy deposited)

      This requires two hit collections, one for "hits" and one for "sum"
      Summing occurs uniquely over copyIDs
 */

class G4HCofThisEvent;
class G4Step;
class G4TouchableHistory;

class HMolPolGenericDetector : public G4VSensitiveDetector {
  public:
    HMolPolGenericDetector( G4String name, G4int detnum );
    virtual ~HMolPolGenericDetector();

    virtual void Initialize(G4HCofThisEvent*);
    virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);
    virtual void EndOfEvent(G4HCofThisEvent*);

  private:
    HMolPolGenericDetectorHitsCollection *fHitColl;
    HMolPolGenericDetectorSumCollection  *fSumColl;
    G4int fHCID, fSCID;

    std::map<int, HMolPolGenericDetectorSum *> fSumMap;

    G4bool fTrackSecondaries;
    G4int fDetNo;
};

#endif /* HMOLPOLGENERICDETECTOR_HH_ */

