/********************************************
 * \author <b>Programmer:</b> Valerie Gray
 * \author <b>Assisted By:</b> Wouter Deconinck
 *
 * \brief <b>Purpose:</b> This file is to generate the magnetic field
 * of the solenoid around the atomic hydrogen target.
 *
 * \date 0<b>Date:</b> 6-25-2013
 * \date <b>Modified:</b> 07-06-2013
 *
 * \note <b>Entry Conditions:</b>
 ********************************************/

//Geant4 specific includes
#include <G4SystemOfUnits.hh>

//HMolPol specific includes
#include "HMolPolHSolenoidMagField.hh"

/********************************************
 * Programmer: Valerie Gray
 * Function: HMolPolSolenoidMagField
 *
 * Purpose:
 *
 * To construct the solenoid
 * Sets the limits of were the field is and how strong it is
 *
 * Global:
 * Entry Conditions: none
 * Return: nothing
 * Called By:
 * Date: 06-25-2013
 * Modified:
********************************************/
HMolPolSolenoidMagField::HMolPolSolenoidMagField ()
{
  Bz = 0.0*tesla;  // this is the stenght of the magnetic
  /** \ingroup see
   * /\brief Z B field value of 8 Tesla
   * as defined in Moller Polarimetry with Atomic Hydrogen Targets
   * by E.~Chudakov and V.~Luppov(long) pg 9
   */
  rmax_sq = std::pow(30.0*cm,2); // square of the radius for the field
  zmax = 25.0*cm;  // this is half of the z length that the field is in
}

// destuctor defined in the .hh file since it is simple so don't need to
// also define it here

/********************************************
 * Programmer: Valerie Gray
 * Function: GetFieldValue
 *
 * Purpose:
 *
 * to create the magnetic field
 *
 * Global:
 * Entry Conditions:
 *      point[] - the point in the mother volume in which one is located
 *      Bfield - the magnetic filed value
 * Return:
 * Called By:
 * Date: 06-25-2013
 * Modified:
********************************************/
void HMolPolSolenoidMagField::GetFieldValue(const G4double Point[4],
                                            G4double* Bfield) const
{
  Bfield[0] = 0.0;  //x direction of B field
  Bfield[1] = 0.0;  //y direction of B field
  //Where is point[i] set?
  /// \todo What if field is not located at the origin

  //if you are in with in +/- zmax AND you are located
  // a radius of X^2 + y^2 or less from the z axis where the filed is placed
  // the set the B field to Bz otherwise leave it be.
  if(std::abs(Point[2])<zmax &&
      (std::pow(Point[0],2) + std::pow(Point[1],2))<rmax_sq)
  {
    Bfield[2] = Bz;
  }
  else
  {
    Bfield[2] = 0.;
  }
}
