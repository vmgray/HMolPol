#ifndef __HMOLPOLGENERICDETECTORHIT_HH
#define __HMOLPOLGENERICDETECTORHIT_HH

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

class HMolPolGenericDetectorHit : public G4VHit {
  public:
    HMolPolGenericDetectorHit(G4int, G4int);
    ~HMolPolGenericDetectorHit();

    HMolPolGenericDetectorHit(const HMolPolGenericDetectorHit &right);
  //what is this operator= or == thingy mabobber
    const HMolPolGenericDetectorHit& operator=(const HMolPolGenericDetectorHit &right);
    G4int operator==(const HMolPolGenericDetectorHit &right) const;

    inline void *operator new(size_t);
    inline void operator delete(void *aHit);
    void *operator new(size_t,void*p){return p;}

  private:

  public:
    G4int fDetID;
    G4int fCopyID;

    // Position and momentum in lab coordinates
    G4ThreeVector f3X;
    G4ThreeVector f3P;
    // Total momentum, energy, mass
    G4double fP, fE, fM;
    // Origin
    G4ThreeVector f3V;
    // Geant4 track ID, particle type, and mother ID
    G4int    fTrID, fPID, fmTrID;
    // Process generator type
    G4int    fGen;
};


typedef G4THitsCollection<HMolPolGenericDetectorHit> HMolPolGenericDetectorHitsCollection;

extern G4Allocator<HMolPolGenericDetectorHit> HMolPolGenericDetectorHitAllocator;

inline void* HMolPolGenericDetectorHit::operator new(size_t){
  void *aHit;
  aHit = (void *) HMolPolGenericDetectorHitAllocator.MallocSingle();
  return aHit;
}

inline void HMolPolGenericDetectorHit::operator delete(void *aHit){
  HMolPolGenericDetectorHitAllocator.FreeSingle( (HMolPolGenericDetectorHit*) aHit);
}
// \todo why is this commented out
#endif/*__HMOLPOLGENERICDETECTORHIT_HH */
