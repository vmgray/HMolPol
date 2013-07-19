/*
 * HMolPolGenericDetectorHit.hh
 *
 *  Created on: Jul 15, 2013
 *      Author: vmgray
 */

#ifndef HMolPolGenericDetectorHit_h
#define HMolPolGenericDetectorHit_h

//Geant4 includes
#include <G4VHit.hh>
#include <G4THitsCollection.hh>
#include <G4ThreeVector.hh>

// user includes

class HMolPolGenericDetectorHit : public G4VHit
{
  public:
    HMolPolGenericDetectorHit();
    HMolPolGenericDetectorHit(const HMolPolGenericDetectorHit& right);
    virtual ~HMolPolGenericDetectorHit();

    G4int GetDetectorID() const {
      return fDetectorID;
    }

    void SetDetectorID(G4int detectorID) {
      fDetectorID = detectorID;
    }

    G4int GetTrackID() const {
      return fTrackID;
    }

    void SetTrackID(G4int trackID) {
      fTrackID = trackID;
    }

    const G4ThreeVector& GetPosition() const {
      return fPosition;
    }

    void SetPosition(const G4ThreeVector& position) {
      fPosition = position;
    }

    const G4ThreeVector& GetMomentum() const {
      return fMomentum;
    }

    void SetMomentum(const G4ThreeVector& momentum) {
      fMomentum = momentum;
    }

  private:
    G4int fDetectorID;  ///< the Detector ID number (What ever this is and for?)
    G4int fTrackID;     ///< Track ID number
      // - the number of the track within an Event

    G4ThreeVector fPosition;
    G4ThreeVector fMomentum;

};

// Define the hit collection type
typedef G4THitsCollection<HMolPolGenericDetectorHit> HMolPolGenericDetectorHitsCollection;
// Define the hit allocator
/// \todo Do we need this?
//extern G4Allocator<HMolPolGenericDetectorHit> HMolPolGenericDetectorHitAllocator;

#endif // HMolPolGenericDetectorHit_h

