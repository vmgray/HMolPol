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

#include <HMolPolMagneticField.hh>

class HMolPolMagFieldSolenoid: public HMolPolMagneticField {

  public:

    /// constructor for the HMolPolMagFieldSolenoid
    HMolPolMagFieldSolenoid(): Bz(0.0 * CLHEP::tesla) { };

    /// destructor for the HMolPolMagFieldSolenoid
    virtual ~HMolPolMagFieldSolenoid() { };

    /// function to get the field value
    void GetFieldValue(const G4double Point[4], G4double* Bfield) const {
      Bfield[0] = 0.0;  //x direction of B field
      Bfield[1] = 0.0;  //y direction of B field
      Bfield[2] = Bz;   //z direction of B field
    }

  private:
    G4double Bz; ///< constant magnetic field in Z direction

};
