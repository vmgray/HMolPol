/********************************************
 * \author <b>Programmer:</b> Valerie Gray
 * \author <b>Assisted By:</b>
 *
 * \brief <b>Purpose:</b> his is header file which for the primary action generator.
 * Or the generator of the primary particle
 *
 * \date <b>Date:</b> 03-29-2016
 * \date <b>Modified:</b> 07-18-2016
 *
 * \note <b>Entry Conditions:</b>
 *
 ********************************************/

//standard includes
#include <math.h>

// geant4 includes
#include <CLHEP/Units/SystemOfUnits.h>
#include <G4PhysicalConstants.hh>

#include "HMolPolEventPrimaryConditions.hh"

/// Register this class with ROOT. This adds some standard ROOT functions to
/// the class, and makes sure that on the command line in ROOT it can do
/// tab-completion. This is associated with the ClassDef() line at the end
/// of the class definition in the .hh file.
ClassImp(HMolPolEventPrimaryConditions)

HMolPolEventPrimaryConditions::HMolPolEventPrimaryConditions() :    fBeamE(0 * CLHEP::GeV), fThetaCM_Min(0 * CLHEP::deg),
    fThetaCM_Max(0 * CLHEP::deg), fDelta_Theta(0 * CLHEP::deg),
    fDelta_CosTheta(0), fPhiCM_Min(0 * CLHEP::rad), fPhiCM_Max(0 * CLHEP::rad),
    fDelta_Phi(0 * CLHEP::rad), fDelta_Omega(0 * CLHEP::steradian),
    fRasterX(0 * CLHEP::mm), fRasterY(0 * CLHEP::mm), fZGenMin(-10 * CLHEP::cm),
    fZGenMax(10 * CLHEP::cm), fNb(6.241e18 * CLHEP::hertz / CLHEP::ampere), fMass_Gamma(0.0 * CLHEP::MeV)

{
  //fNb will only fill if there is no units
  fNb = 6.241e12;  //Hz/muA Found via definition of Coulumb
//  fNb = 6.241e18 * CLHEP::hertz / CLHEP::ampere; // Found via definition of Coulumb

  return;
}

/*
 HMolPolEventPrimaryConditions::~HMolPolEventPrimaryConditions()
 {
 // TODO Auto-generated destructor stub
 }
 */

/********************************************
 * Programmer: Valerie Gray
 * Function: UpdateDeltaOmega
 *
 * Purpose:
 *
 * Calculate the solid angle that is tossed over
 *
 * Global:
 * Entry Conditions:
 * Return:
 * Called By:
 * Date: 03-29-2016
 * Modified: 03-29-2016
 ********************************************/
void HMolPolEventPrimaryConditions::UpdateDeltaOmega()
{
  fDelta_Omega = (fPhiCM_Max - fPhiCM_Min)
      * (cos(fThetaCM_Min) - cos(fThetaCM_Max)) * CLHEP::steradian;
  return;
}

