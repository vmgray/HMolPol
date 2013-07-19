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

// System includes
#include <map>

// Geant4 includes
#include <G4VSensitiveDetector.hh>

// HMolPol include
#include "HMolPolGenericDetectorHit.hh"

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

class HMolPolGenericDetector : public G4VSensitiveDetector
{
  public:
    HMolPolGenericDetector( G4String name);
    virtual ~HMolPolGenericDetector();

    virtual void Initialize(G4HCofThisEvent* );
    virtual G4bool ProcessHits(G4Step*,G4TouchableHistory* );
    virtual void EndOfEvent(G4HCofThisEvent* );

  private:

    static G4int fNumberOfDetectors;

    HMolPolGenericDetectorHitsCollection *fHitColl;     ///< Hit collection
    G4int fHitCollID;                                   ///< Hit collection ID

};

#endif /* HMOLPOLGENERICDETECTOR_HH_ */

