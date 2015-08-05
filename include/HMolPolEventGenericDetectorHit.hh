/********************************************
 * \author <b>Programmer:<\b> Valerie Gray
 * \author <b>Assisted By:<\b> Wouter Deconinck
 *
 * \brief <b>Purpose:</b> This is header file for the
 *      HMolPolEventGenericDetectorHit class.
 *
 *      Sets up the variable for the ROOT tree
 *
 * \date <b>Date:</b> 07-19-2013
 * \date <b>Modified:</b> 07-14-2015
 *
 * \note <b>Entry Conditions:</b> none
 *
********************************************/

#ifndef HMOLPOLEVENTGENERICDETECTORHIT_HH_
#define HMOLPOLEVENTGENERICDETECTORHIT_HH_

// ROOT includes
#include <TObject.h>
#include <TVector3.h>

class HMolPolEventGenericDetectorHit: public TObject {
  public:
    HMolPolEventGenericDetectorHit() { };  ///< Constructor for
      /// HMolPolEventGenericDetectorHit
    virtual ~HMolPolEventGenericDetectorHit() { };  ///< Destrutor for
    /// HMolPolEventGenericDetectorHit

    Int_t fTrackID;     ///< Track ID
    Int_t fDetectorID;  ///< Detector ID

    TVector3 fPosition; ///< Hit position
    TVector3 fMomentum; ///< Hit momentum

    TString fParticleName;     ///< Particle Name
    Int_t fParticleType;       ///< Particle Type (PGD encoding)

    Double_t fTotalEnergy;      ///< total energy of the track
    Double_t fKineticEnergy;    ///< Kinetic energy of the track

    /// Register this class with ROOT. This adds some standard ROOT functions to
    /// the class, and makes sure that on the command line in ROOT it can do
    /// tab-completion. This is associated with the ClassImp() line at the top
    /// of the .cc file.
    ClassDef(HMolPolEventGenericDetectorHit,1)
};

#endif /* HMOLPOLEVENTGENERICDETECTORHIT_HH_ */
