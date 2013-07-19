/********************************************
 * \author <b>Programmer:</b> Valerie Gray
 * \author <b>Assisted By:</b> Wouter Deconinck
 *
 * \brief <b>Purpose:</b> This file is in charge of Who knows what?!
 *
 * \date <b>Date:</b> 07-17-2013
 * \date <b>Modified:</b>
 *
 * \note <b>Entry Conditions:</b>
 *
 * \todo comment the comment blocks to get the how this is all put together
 *
 ********************************************/

// geant4 includes
#include <G4SystemOfUnits.hh>

//HMolPol includes
#include "HMolPolEvent.hh"

ClassImp(HMolPolEvent)

/********************************************
 * Programmer: Valerie Gray
 * Function: HMolPolEvent
 *
 * Purpose: to construct the HMolPolEvent
 *
 * \bug Somehow I think it makes the units for the ROOT tree
 *
 * Global:
 * Entry Conditions:
 * Return:
 * Called By:
 * Date: 07-11-2013
 * Modified:
 ********************************************/
HMolPolEvent::HMolPolEvent()
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
  mbarn(CLHEP::millibarn),
  nsec(CLHEP::ns),
  msec(CLHEP::ms),
  sec(CLHEP::s),
  Tesla(CLHEP::tesla)
{
  // Initialization
  fEventNumber = 0;
}

/********************************************
 * Programmer: Valerie Gray
 * Function: ~HMolPolEvent
 *
 * Purpose: to destruct the HMolPolEvent
 *
 * Global:
 * Entry Conditions:
 * Return:
 * Called By:
 * Date: 07-11-2013
 * Modified:
 ********************************************/
HMolPolEvent::~HMolPolEvent()
{

}
