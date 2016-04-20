/********************************************
 * \author <b>Programmer:</b> Valerie Gray
 * \author <b>Assisted By:</b> Wouter Deconinck
 *
 * \brief <b>Purpose:</b> This file is in charge of defining the unit variables
 *      and passing these to the ROOT tree
 *
 *      All units MUST be internal GEANT4 units or this will not work :-(
 *
 * \date <b>Date:</b> 07-17-2013
 * \date <b>Modified:</b>
 *
 * \note <b>Entry Conditions:</b>
 *
********************************************/

// geant4 includes
#include <CLHEP/Units/SystemOfUnits.h>

//HMolPol includes
#include "HMolPolEventUnits.hh"

/// Register this class with ROOT. This adds some standard ROOT functions to
/// the class, and makes sure that on the command line in ROOT it can do
/// tab-completion. This is associated with the ClassDef() line at the end
/// of the class definition in the .hh file.
ClassImp(HMolPolEventUnits)

/********************************************
 * Programmer: Valerie Gray
 * Function: HMolPolEventUnits
 *
 * Purpose: to construct the HMolPolEventUnits
 *      - Initializes all the units to their proper value
 *
 * Global:
 * Entry Conditions: none
 * Return:
 * Called By:
 * Date: 07-11-2013
 * Modified:
 ********************************************/
HMolPolEventUnits::HMolPolEventUnits()
: mm(CLHEP::mm),
  cm(CLHEP::cm),
  m(CLHEP::m),
  mm2(CLHEP::mm2),
  cm2(CLHEP::cm2),
  m2(CLHEP::m2),
  mm3(CLHEP::mm3),
  cm3(CLHEP::cm3),
  m3(CLHEP::m3),
  keV(CLHEP::keV),
  MeV(CLHEP::MeV),
  GeV(CLHEP::GeV),
  rad(CLHEP::rad),
  deg(CLHEP::deg),
  sr(CLHEP::sr),
  barn(CLHEP::barn),
  mbarn(CLHEP::millibarn),
  ubarn(CLHEP::microbarn),
  nsec(CLHEP::ns),
  msec(CLHEP::ms),
  sec(CLHEP::s),
  Tesla(CLHEP::tesla)
{

}
