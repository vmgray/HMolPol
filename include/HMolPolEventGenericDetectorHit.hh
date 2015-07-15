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

    //define a class for ROOT
    ClassDef(HMolPolEventGenericDetectorHit,1)
};

#endif /* HMOLPOLEVENTGENERICDETECTORHIT_HH_ */
