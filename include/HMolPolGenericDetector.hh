/********************************************
 * \author <b>Programmer:<\b> Valerie Gray
 * \author <b>Assisted By:<\b> Wouter Deconinck
 *
 * \brief <b>Purpose:</b> This is header file for the generic detector.
 *      What it does no one knows
 *
 * \date <b>Date:</b> 06-15-2013
 * \date <b>Modified:</b> 07-06-2013
 *
 * \note <b>Entry Conditions:</b> none
 *
********************************************/

#ifndef HMOLPOLGENERICDETECTOR_HH_
#define HMOLPOLGENERICDETECTOR_HH_

// System includes
#include <map>

// Geant4 includes
#include <G4VSensitiveDetector.hh>

// HMolPol includes
#include "HMolPolGenericDetectorHit.hh"

//Geant4 classes
class G4HCofThisEvent;
class G4Step;
class G4TouchableHistory;

class HMolPolGenericDetector : public G4VSensitiveDetector
{
  public:
    HMolPolGenericDetector( G4String name);   ///< Constructor for the
      /// HMolPolGenericDetector
    virtual ~HMolPolGenericDetector();   ///< destructor for the
    /// HMolPolGenericDetector

    virtual void Initialize(G4HCofThisEvent* );   ///< Initialize function
    virtual G4bool ProcessHits(G4Step*,G4TouchableHistory* );  ///< process hits function
    virtual void EndOfEvent(G4HCofThisEvent* );   ///, End of event function

    const G4String& GetVolumeName() const {
      return fVolumeName;
    }  ///< Get volume name function - does just what it implies

    void SetVolumeName(const G4String& volumeName) {
      fVolumeName = volumeName;
    }  ///< Set volume name function - does just what it implies

  private:

    static G4int fTotalNumberOfDetectors;   ///< integer total number of detectors
    static std::map<G4String,G4int> fNumberOfDetectorsWithName;  ///< Map of
      /// the name of the detector to a number associated with that detector

    G4String fVolumeName;   ///< the volume

    HMolPolGenericDetectorHitsCollection *fHitsCollection;     ///< Hit collection
    G4int fHitsCollectionID;                                   ///< Hit collection ID

};

#endif /* HMOLPOLGENERICDETECTOR_HH_ */

