/********************************************
 * \author <b>Programmer:<\b> Valerie Gray
 * \author <b>Assisted By:<\b>
 *
 * \brief <b>Purpose:</b> This is header file which for the Solenoid
 * magnetic field around the H Moller target.
 *
 * \ref FROM: FROM: http://geant4.web.cern.ch/geant4/UserDocumentation/Doxygen/examples_doc/html/classF04FocusSolenoid.html
 * http://geant4.web.cern.ch/geant4/UserDocumentation/Doxygen/examples_doc/html/F04FocusSolenoid_8hh_source.html
 *
 * \date <b>Date:</b> 09-28-2015
 * \date <b>Modified:</b>
 *
 * \note <b>Entry Conditions:</b> none
 *
 ********************************************/

/*
 * HMolPolHSolenoidMagFieldWithFringe.hh
 *
 *  Created on: Sep 28, 2015
 *      Author: vmgray
 */

#ifndef INCLUDE_HMOLPOLHSOLENOIDMAGFIELDWITHFRINGE_HH_
#define INCLUDE_HMOLPOLHSOLENOIDMAGFIELDWITHFRINGE_HH_

#include <G4MagneticField.hh>

class HMolPolHSolenoidMagFieldWithFringe:
    public G4MagneticField
{
  public:
    HMolPolHSolenoidMagFieldWithFringe();
    virtual ~HMolPolHSolenoidMagFieldWithFringe(){};

    void GetFieldValue(const G4double Point[4], G4double* Bfield) const;  ///< function
    /// to get the field value


  private:
    G4double Bz;  ///< constant magnetic field in Z direction
    G4double rmax_sq;  ///< constant the radius of the magnetic field
    G4double zmax;  ///< constant for the max z length of the magnetic field
    G4double fringeZ;  //The fringe z length (UNIT of length)

};

#endif /* INCLUDE_HMOLPOLHSOLENOIDMAGFIELDWITHFRINGE_HH_ */
