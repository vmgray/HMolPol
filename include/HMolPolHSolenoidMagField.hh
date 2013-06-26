/**
 * Programmer: Valerie Gray
 * Purpose:
 *
 * This is header file which for the Solenoid magnetic field around
 * the H Moller target.
 *
 * Entry Conditions:
 * Date: 06-25-2013
 * Modified:
 * Assisted By: Wouter Deconinck
********************************************/
#include <G4MagneticField.hh>

class HMolPolSolenoidMagField: public G4MagneticField {

  public:

    HMolPolSolenoidMagField();
    virtual ~HMolPolSolenoidMagField() { };

    void GetFieldValue(const G4double Point[4], G4double* Bfield) const;

  private:
    G4double Bz; //constant magnetic field in Z direction
    G4double rmax_sq;
    G4double zmax;

};
