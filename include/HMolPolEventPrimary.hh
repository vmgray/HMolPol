/********************************************
 * \author <b>Programmer:<\b> Valerie Gray
 * \author <b>Assisted By:<\b> Wouter Deconinck
 *
 * \brief <b>Purpose:</b> This is header file for the Primary event.
 *
 * \date <b>Date:</b> 07-17-2013
 * \date <b>Modified:</b> 04-18-2016
 *
 * \note <b>Entry Conditions:</b> none
 *
********************************************/


#ifndef HMOLPOLPRIMARYEVENT_HH_
#define HMOLPOLPRIMARYEVENT_HH_

// root include
#include <TObject.h>
#include <TVector3.h>

class HMolPolEventPrimary: public TObject {
  public:
    HMolPolEventPrimary();  ///< constructor for the primary event
    virtual ~HMolPolEventPrimary();  ///< destuctor for the primary event

    Double_t fCrossSectionCM;  ///< the cross section in CM frame

    Double_t fPhiLab1;        ///< the lab phi angle one particle(same as CM phi angle)
    Double_t fPhiLab2;        ///< the lab phi angle one particle(same as CM phi angle) Lab phi1 + 180 deg
    Double_t fThetaLab1;     ///< the lab theta angle for particle 1
    Double_t fThetaLab2;     ///< the lab theta angle for particle 2

    Double_t fPhiCenterOfMass;   ///< the CM phi angle
    Double_t fPhiCenterOfMass1;   ///< the CM phi angle for particle 1, phi_com
    Double_t fPhiCenterOfMass2;   ///< the CM phi angle for particle 2, phi_com + 180 deg

    Double_t fThetaCenterOfMass;    ///< the CM theta angle
    Double_t fThetaCenterOfMass1;   ///< the CM theta angle for particle 1, theta_com
    Double_t fThetaCenterOfMass2;   ///< the CM theta angle for particle 2, pi - theta_com

    //Have the interaction position and momentum (both particles in this case) as 3 vectors
    TVector3 fInteractionVertexPosition; ///< Interaction vertex position
    TVector3 fInteractionVertexMomentum1; ///< Interaction vertex momentum for particle 1
    TVector3 fInteractionVertexMomentum2; ///< Interaction vertex momentum for particle 2

    Double_t fA_zz; ///< Analyzing Power for this interaction where
    /// both particles polarized in the ZZ direction - this is a unitless quantity

    /// Register this class with ROOT. This adds some standard ROOT functions to
    /// the class, and makes sure that on the command line in ROOT it can do
    /// tab-completion. This is associated with the ClassImp() line at the top
    /// of the .cc file.
    ClassDef(HMolPolEventPrimary,1)

};

#endif /* HMOLPOLPRIMARYEVENT_HH_ */
