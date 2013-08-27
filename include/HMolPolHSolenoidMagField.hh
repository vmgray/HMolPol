/********************************************
 * \author <b>Programmer:<\b> Valerie Gray
 * \author <b>Assisted By:<\b> Wouter Deconinck
 *
 * \brief <b>Purpose:</b> This is header file which for the Solenoid
 * magnetic field around the H Moller target.
 *
 * \date <b>Date:</b> 06-25-2013
 * \date <b>Modified:</b> 07-06-2013
 *
 * \note <b>Entry Conditions:</b> none
 *
********************************************/

#include <G4MagneticField.hh>

class HMolPolSolenoidMagField: public G4MagneticField {

  public:

    HMolPolSolenoidMagField();   ///< constructor for the HMolPolSolenoidMagField
      /// function
    virtual ~HMolPolSolenoidMagField() { };  ///< destructor for the
      /// HMolPolSolenoidMagField function

    void GetFieldValue(const G4double Point[4], G4double* Bfield) const; ///< function
      /// to get the field value

  private:
    G4double Bz; ///< constant magnetic field in Z direction
    G4double rmax_sq; ///< constant the radius of the magnetic field
    G4double zmax;  ///< constant for the max z length of the magnetic field

};
