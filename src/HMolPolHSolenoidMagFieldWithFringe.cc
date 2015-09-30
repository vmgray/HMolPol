/********************************************
 * \author <b>Programmer:</b> Valerie Gray
 * \author <b>Assisted By:</b> Wouter Deconinck
 *
 * \brief <b>Purpose:</b> This file is to generate the magnetic field
 * of the solenoid around the atomic hydrogen target. This field has
 * a frige field.
 *
 * \ref FROM: http://geant4.web.cern.ch/geant4/UserDocumentation/Doxygen/examples_doc/html/classF04FocusSolenoid.html
 * http://geant4.web.cern.ch/geant4/UserDocumentation/Doxygen/examples_doc/html/F04FocusSolenoid_8cc_source.html
 *
 * \date 0<b>Date:</b> 09-28-2015
 * \date <b>Modified:</b>
 *
 * \note <b>Entry Conditions:</b>
 ********************************************/

//Geant4 specific includes
#include <G4SystemOfUnits.hh>

//HMolPol includes
#include "HMolPolHSolenoidMagFieldWithFringe.hh"

/********************************************
 * Programmer: Valerie Gray
 * Function: HMolPolHSolenoidMagFieldWithFringe
 *
 * Purpose:
 *
 * To construct the solenoid with fringe
 * Sets the limits of were the field is and how strong it is
 *
 * Global:
 * Entry Conditions: none
 * Return: nothing
 * Called By:
 * Date: 09-28-2015
 * Modified:
 ********************************************/
HMolPolHSolenoidMagFieldWithFringe::HMolPolHSolenoidMagFieldWithFringe()
{
  //debugging
  G4cout << G4endl<< "######## In the HMolPolHSolenoidMagFieldWithFringe::"
  "HMolPolSolenoidMagFieldWithFringe ########"<< G4endl;

  //NOTE all distances must be in METERS here for the equation to work

  //debugging
//  Bz = 3000.0*tesla;  // this is the strength of the magnetic
  Bz = 8.0 * CLHEP::tesla;// this is the strength of the magnetic
  /** \ingroup see
   * /\brief Z B field value of 8 Tesla
   * as defined in Moller Polarimetry with Atomic Hydrogen Targets
   * by E.~Chudakov and V.~Luppov(long) pg 9
   */
  rmax_sq = std::pow(0.03 * CLHEP::m,2);  // square of the radius for the field
  zmax = 0.25 * CLHEP::m;// this is half of the z length that the field is in

  fringeZ = 0.05 * CLHEP::m;// length of fringe, beyond the end of the
  //physical solenoid. This should extend field and allow for a fringe field

  return;
}

// destuctor defined in the .hh file since it is simple so don't need to
// also define it here

/********************************************
 * Programmer: Valerie Gray
 * Function: GetFieldValue
 *
 * Purpose:
 *
 * to create the magnetic field at each point
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
void HMolPolHSolenoidMagFieldWithFringe::GetFieldValue(
                                                      const G4double Point[4],
                                                        G4double* Bfield) const
{
  //Using the following to calculate
  // \ref https://www.google.com/url?sa=t&rct=j&q=&esrc=s&source=web&cd=3&ved=0CCkQFjACahUKEwjP2PaTt5zIAhVENT4KHekzAZo&url=http%3A%2F%2Ficecube.wisc.edu%2F~shiu%2FPHY248_S07%2FExams%2Fproblems_magneticfields_sol.pdf&usg=AFQjCNFB3oUFoZQ1cpD_0-spTzycEGqiSA&sig2=MBMgDssTxnZBTK8c42A-0w&bvm=bv.103627116,d.cWw&cad=rja

  //Total length of magnetic field
  G4double TotalMagLength = zmax * 2 + fringeZ;

  /*
   * That has the following equation:
   *   B_z = (1/2)(mu_0)nI [ (b/sqrt(b^2 + R^2) + (a/sqrt(a^2 + R^2) ]
   *
   *   B_z = magnetic field strength along the Z axis
   *   mu_0 = magnetic permeability
   *   n = number of turn
   *   I = current
   *   a = location from one end of solenoid
   *   b = location from the other end of solenoid
   *   R = radius
   *
   *   Work it out so that at the center B_z = 8 T
   *
   *   Let C = (1/2)(mu_0)nI
   *   B_z = Bz
   *   R^2 = rmax_sq
   *   a = TotalMagLength / 2
   *   b = TotalMagLength / 2
   *
   *   Then C = B_z / [ (b/sqrt(b^2 + R^2) + (a/sqrt(a^2 + R^2) ]
   *   - OR -
   *   C = B_z / [ 2 * (b/sqrt(b^2 + R^2) ]
   *   - OR -
   *   C = Bz / { 2 * [(TotalMagLength / 2)/sqrt((TotalMagLength / 2)^2 + rmax_sq) }
   *
   *   NOTE all lengths must be in METERS!!
   *
   *   Using the zmax rather then total length here so that there is a
   *   better fringe?? But this is a WAG
   */

  G4double C = Bz
      / (2 * ((zmax / 2)
          / std::sqrt(std::pow((zmax/ 2), 2) + rmax_sq)));

  Bfield[0] = 0.0;  //x direction of B field
  Bfield[1] = 0.0;  //y direction of B field
  //Where is point[i] set?
  /// \todo What if field is not located at the origin

  //if you are in with in +/- zmax AND you are located
  // a radius of X^2 + y^2 or less from the z axis where the filed is placed
  // the set the B field to Bz otherwise leave it be.
  if (std::abs(Point[2]) < TotalMagLength && (std::pow(Point[0], 2)
      + std::pow(Point[1], 2))
                                   < rmax_sq)
  { //radius within the solenoid
    //B_z = (1/2)(mu_0)nI [ (b/sqrt(b^2 + R^2) + (a/sqrt(a^2 + R^2) ]
    // B_z = C [ (b/sqrt(b^2 + R^2) + (a/sqrt(a^2 + R^2) ]
    //Point[2] has a 0 point in the center of the solenoid
    //upstream is at - zmax / 2
    //downstream is at +zmax / 2
    G4double a = Point[2] - (-zmax / 2); //where we are in one point of the solenoid
    G4double b = zmax - a; //where we are in one point of the solenoid

    //radius for this point
    G4double r_sq = std::pow(Point[0],2)+ std::pow(Point[1],2);

    Bfield[2] = C * ( ( b / std::sqrt( std::pow(b,2) + r_sq) ) + ( a / std::sqrt( std::pow(a,2) + r_sq) ) );

    //Ideal field
    //Bfield[2] = Bz;
  } else
  { //radius outside the solenoid
    Bfield[2] = 0.;
  }

  return;
}
