#ifndef HMolPolPrimaryHit_hh
#define HMolPolPrimaryHit_hh

//Geant4 includes

#include <CLHEP/Vector/ThreeVector.h>
#include <G4String.hh>
#include <G4THitsCollection.hh>
#include <G4Allocator.hh>
#include <G4ThreeVector.hh>
#include <G4Types.hh>
#include <G4VHit.hh>

// user includes

class HMolPolPrimaryHit: public G4VHit {
  public:
    HMolPolPrimaryHit();  ///< Default constructor
    HMolPolPrimaryHit(const HMolPolPrimaryHit& right); ///< Copy constructor
    virtual ~HMolPolPrimaryHit(); //< Default destructor

    // New and delete operators for custom allocator
    inline void *operator new(size_t);
    inline void operator delete(void *aHit);
    void *operator new(size_t,void*p) { return p; }

    const G4ThreeVector& GetPosition() const {
      return fPosition;
    } ///< function to get the three vector of the position (is a constant)
      // of the track

    void SetPrimaryID( G4int id) {
      fPrimaryID = id;
    } ///< function to set the primary ID

    G4int GetPrimaryID() {
      return fPrimaryID;
    } ///< function to get primary ID

    void SetPosition(const G4ThreeVector& position) {
      fPosition = position;
    } ///< function to set the three vector of the position (is a constant)
    // of the track

    const G4ThreeVector& GetMomentum() const {
      return fMomentum;
    } ///< function to get the three vector of the momentum (is a constant)
    // of the track

    void SetMomentum(const G4ThreeVector& momentum) {
      fMomentum = momentum;
    } ///< function to set the three vector of the momentum (is a constant)
    // of the track

    G4String GetVolumeName() {
      return fVolumeName;
    } ///< function to get the physical volume name

    void SetVolumeName(G4String VolumeName) {
      fVolumeName = VolumeName;
    } ///< function to set the physical volume name

  private:
    G4int fPrimaryID; ///< ID of this primary
    G4String fVolumeName; ///< Name of the physical volume
    G4ThreeVector fPosition; ///< the position of the track
    G4ThreeVector fMomentum; ///< the momentum of the track
};


/// Define special HitCollection (vector) of Primary Hits type
typedef G4THitsCollection<HMolPolPrimaryHit> HMolPolPrimaryHitsCollection;

/// A generic allocator of Primary Hits
extern G4Allocator<HMolPolPrimaryHit> HMolPolPrimaryHitAllocator;

/// Operator to create a new hit (for use with the custom allocator)
inline void* HMolPolPrimaryHit::operator new(size_t){
    void *aHit;
    aHit = (void *) HMolPolPrimaryHitAllocator.MallocSingle();
    return aHit;
}

/// Operator to delete a hit (for use with the custom allocator)
inline void HMolPolPrimaryHit::operator delete(void *aHit){
    HMolPolPrimaryHitAllocator.FreeSingle( (HMolPolPrimaryHit*) aHit);
}

#endif // HMolPolPrimaryHit_hh
