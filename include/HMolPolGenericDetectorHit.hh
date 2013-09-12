/********************************************
 * \author <b>Programmer:<\b> Valerie Gray
 * \author <b>Assisted By:<\b> Wouter Deconinck
 *
 * \brief <b>Purpose:</b> This is header file for the HMolPolGenericDetectorHit Class
 *
 * \date <b>Date:</b> 07-15-2013
 * \date <b>Modified:</b> 08-20-2013
 *
 * \note <b>Entry Conditions:</b> none
 *
********************************************/

#ifndef HMolPolGenericDetectorHit_h
#define HMolPolGenericDetectorHit_h

//Geant4 includes

#include <CLHEP/Vector/ThreeVector.h>
#include <G4String.hh>
#include <G4THitsCollection.hh>
#include <G4Allocator.hh>
#include <G4ThreeVector.hh>
#include <G4Types.hh>
#include <G4VHit.hh>

// user includes

class HMolPolGenericDetectorHit: public G4VHit {
  public:
    HMolPolGenericDetectorHit();  ///< constructor for HMolPolGenericDetectorHit
    HMolPolGenericDetectorHit(const HMolPolGenericDetectorHit& right);
// \bug    ///< constructor for HMolPolGenericDetectorHit I think, the second one??
    virtual ~HMolPolGenericDetectorHit();
       //< destructor for HMolPolGenericDetectorHit

    // New and delete operators for custom allocator
    inline void *operator new(size_t);
    inline void operator delete(void *aHit);
    void *operator new(size_t,void*p) { return p; }


    G4int GetDetectorID() const {
      return fDetectorID;
    } ///< function to get the detector id number (int)

    void SetDetectorID(G4int detectorID) {
      fDetectorID = detectorID;
      //fDetectorID = fColHits->GetName(); - want something like this where this is a string not an int
    } ///< function to set the detector id number (int)

    G4int GetTrackID() const {
      return fTrackID;
    } ///< function to get the track id number (int)

    void SetTrackID(G4int trackID) {
      fTrackID = trackID;
    } ///< function to set the Track id number (int)

    const G4ThreeVector& GetPosition() const {
      return fPosition;
    } ///< function to get the three vector of the position (is a constant)
      // of the track

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

    G4double GetKineticEnergy() const {
      return fKineticEnergy;
    } ///< function to get the kinetic energy (double) of the track

    void SetKineticEnergy(G4double kineticEnergy) {
      fKineticEnergy = kineticEnergy;
    } ///< function to set the kinetic energy (double) of the track

    const G4String& GetParticleName() const {
      return fParticleName;
    } ///< function to get the particle name of the particle of the track

    void SetParticleName(const G4String& particleName) {
      fParticleName = particleName;
    } ///< function to set the particle name of the particle of the track

    G4int GetParticleType() const {
      return fParticleType;
    } ///< function to get the particle type of the particle of the track
     // (this is the int value which will be in the ROOT tree

    void SetParticleType(G4int particleType) {
      fParticleType = particleType;
    } ///< function to set the particle type of the particle of the track
    // (this is the int value which will be in the ROOT tree

    G4double GetTotalEnergy() const {
      return fTotalEnergy;
    } ///< function to get the total energy (double) of the track

    void SetTotalEnergy(G4double totalEnergy) {
      fTotalEnergy = totalEnergy;
    } ///< function to set the total energy (double) of the track

  private:
    G4int fDetectorID; ///< the Detector ID number (What ever this is and for?)
    G4int fTrackID; ///< Track ID number
    // - the number of the track within an Event

    G4String fParticleName; ///< the particle name
    G4int fParticleType;    ///< the particle type (as an integer)

    G4double fTotalEnergy;   ///< the total energy of the track
    G4double fKineticEnergy; ///< the kinetic energy of the track

    G4ThreeVector fPosition; ///<the position of the track
    G4ThreeVector fMomentum; ///< the momentum of the track

};

// Define the hit collection type - this is NOT working
typedef G4THitsCollection<HMolPolGenericDetectorHit> HMolPolGenericDetectorHitsCollection;
// Define the hit allocator
extern G4Allocator<HMolPolGenericDetectorHit> HMolPolGenericDetectorHitAllocator;

/// Operator to create a new hit (for use with the custom allocator)
inline void* HMolPolGenericDetectorHit::operator new(size_t){
    void *aHit;
    aHit = (void *) HMolPolGenericDetectorHitAllocator.MallocSingle();
    return aHit;
}

/// Operator to delete a hit (for use with the custom allocator)
inline void HMolPolGenericDetectorHit::operator delete(void *aHit){
    HMolPolGenericDetectorHitAllocator.FreeSingle( (HMolPolGenericDetectorHit*) aHit);
}

#endif // HMolPolGenericDetectorHit_h
