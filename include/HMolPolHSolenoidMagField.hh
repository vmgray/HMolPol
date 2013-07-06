/********************************************
* Programmer: Valerie Gray
 * Purpose:
 *
 * This is header file which for the Solenoid magnetic field around
 * the H Moller target.
 *
 * Entry Conditions:
 * Date: 06-25-2013
 * Modified: 07-06-2013
 * Assisted By: Wouter Deconinck
********************************************/

#include <G4MagneticField.hh>

class HMolPolSolenoidMagField: public G4MagneticField {

  public:

    //create the constructor and deconstructor for the detector construction
    HMolPolSolenoidMagField();
    virtual ~HMolPolSolenoidMagField() { };

    //function to get the field value
    void GetFieldValue(const G4double Point[4], G4double* Bfield) const;

  private:
    G4double Bz; ///< constant magnetic field in Z direction
    G4double rmax_sq; ///< constant the radius of the magnetic field
    G4double zmax;  ///< constant for the max z length of the magnetic field

};
