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


HMolPolEvent::HMolPolEvent()
: mm(CLHEP::mm),
  MeV(CLHEP::MeV),
  rad(CLHEP::rad),
  deg(CLHEP::deg)
{
  // Create primary vertex information object
  fPrimary = new HMolPolPrimaryEvent();

  // Initialization
  fEventNumber = 0;
}

HMolPolEvent::~HMolPolEvent()
{
  // Delete objects
  if (fPrimary) delete fPrimary;
}
