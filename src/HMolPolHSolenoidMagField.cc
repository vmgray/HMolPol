/********************************************
 * Programmer: Valerie Gray
 * Purpose:
 *
 * This file is to generate the magnetic field of the solenoid around the
 * atomic hydrogen target
 *
 * Entry Conditions:
 * Date: 06-23-2013
 * Modified: 06-25-2013
 * Assisted By: Wouter Deconinck
********************************************/

//Geant4 specific includes
#include <G4SystemOfUnits.hh>

//HMolPol specific includes
#include "HMolPolHSolenoidMagField.hh"

/********************************************
 * Function: HMolPolSolenoidMagField
 *
 * Purpose:
 *
 * To construct the solenoid
 * Sets the limits of were the field is and how strong it is
 *
 * Global:
 * Return: nothing
 * Called By:
 * Date: 06-25-2013
 * Modified:
********************************************/
HMolPolSolenoidMagField::HMolPolSolenoidMagField ()
{
  Bz = 8.0*tesla;
  //Z B field value of 8 Tesla
  //as defined in Moller Polarimetry with Atomic Hydrogen Targets
  //by E.~Chudakov and V.~Luppov(long) pg 9
  rmax_sq = std::pow(30.0*cm,2);
  zmax = 25.0*cm;  //this is half of the z length that the field is in
}

/********************************************
 * Function: ~HMolPolSolenoidMagField
 *
 * Purpose:
 *
 * To destruct the solenoid field
 *
 * Global:
 * Return: nothing
 * Called By:
 * Date: 06-25-2013
 * Modified:
********************************************/
//WHY do I NOT need this?!?!
/*HMolPolSolenoidMagField::~HMolPolSolenoidMagField()
{
  ;
}*/

/********************************************
 * Function: HMolPolPrimaryGeneratorAction
 *
 * Purpose:
 *
 * Global:
 * Return:
 * Called By:
 * Date: 06-25-2013
 * Modified:
********************************************/
void HMolPolSolenoidMagField::GetFieldValue(const G4double Point[4], G4double* Bfield) const
{
  Bfield[0] = 0.0;  //x direction of B field
  Bfield[1] = 0.0;  //y direction of B field
  //Where is point[i] set?
  //What if field is not located at the origin
  //if you are in with in +/- zmax AND you are located
  // a radius of X^2 + y^2 or less from the z axis where the filed is placed
  // the set the B field to Bz otherwise leave it be.
  if(std::abs(Point[2])<zmax && (std::pow(Point[0],2)+std::pow(Point[1],2))<rmax_sq)
  { Bfield[2] = Bz; }
  else
  { Bfield[2] = 0.; }
}
