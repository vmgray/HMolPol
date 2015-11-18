/*****************************************
 * Information of each step taken by a primary
 ****************************************/
#ifndef HMOLPOLEVENTPRIMARYHIT_HH_
#define HMOLPOLEVENTPRIMARYHIT_HH_

// ROOT includes
#include <TObject.h>
#include <TVector3.h>

class HMolPolEventPrimaryHit: public TObject {
  public:
    HMolPolEventPrimaryHit() { };  ///< Constructor for
      /// HMolPolEventPrimaryHit
    virtual ~HMolPolEventPrimaryHit() { };  ///< Destrutor for
    /// HMolPolEventPrimaryHit

    Int_t fPrimaryID;   ///< Primary ID
    TString fVolumeName; ///< The physical volume name where primary stepped

    TVector3 fPosition; ///< Current primary position
    TVector3 fMomentum; ///< Current primary momentum

    /// Register this class with ROOT. This adds some standard ROOT functions to
    /// the class, and makes sure that on the command line in ROOT it can do
    /// tab-completion. This is associated with the ClassImp() line at the top
    /// of the .cc file.
    ClassDef(HMolPolEventPrimaryHit,1)
};

#endif /* HMOLPOLEVENTPRIMARYHIT_HH_ */
