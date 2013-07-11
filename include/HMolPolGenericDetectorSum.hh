#ifndef __HMOLPOLGENERICDETECTORSUM_HH
#define __HMOLPOLGENERICDETECTORSUM_HH

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"

class HMolPolGenericDetectorSum : public G4VHit {
  public:
    HMolPolGenericDetectorSum(G4int, G4int);
    ~HMolPolGenericDetectorSum();

    HMolPolGenericDetectorSum(const HMolPolGenericDetectorSum &right);
    const HMolPolGenericDetectorSum& operator=(const HMolPolGenericDetectorSum &right);
    G4int operator==(const HMolPolGenericDetectorSum &right) const;

    inline void *operator new(size_t);
    inline void operator delete(void *aHit);
    void *operator new(size_t,void*p){return p;}

  public:
    G4int    fDetID;
    G4int    fCopyID;
    G4double fEdep;
};


typedef G4THitsCollection<HMolPolGenericDetectorSum> HMolPolGenericDetectorSumCollection;

extern G4Allocator<HMolPolGenericDetectorSum> HMolPolGenericDetectorSumAllocator;

inline void* HMolPolGenericDetectorSum::operator new(size_t){
  void *aHit;
  aHit = (void *) HMolPolGenericDetectorSumAllocator.MallocSingle();
  return aHit;
}

inline void HMolPolGenericDetectorSum::operator delete(void *aHit){
  HMolPolGenericDetectorSumAllocator.FreeSingle( (HMolPolGenericDetectorSum*) aHit);
}

#endif//__HMOLPOLGENERICDETECTORSUM_HH
