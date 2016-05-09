/********************************************
 * \author <b>Programmer:<\b> Wouter Deconinck
 * \author <b>Assisted By:<\b>s
 *
 * \brief <b>Purpose:</b> This is header file for the dipole magnetic field
 *
 * \date <b>Date:</b> 05-04-2016
 * \date <b>Modified:</b>
 *
 * \note <b>Entry Conditions:</b> none
 *
********************************************/

//HMolPol specific includes
#include <HMolPolMagneticField.hh>

class HMolPolMagFieldDipole: public HMolPolMagneticField {

  public:

    /// constructor for the HMolPolMagFieldDipole
    HMolPolMagFieldDipole(): Bx(0.0 * CLHEP::tesla) { };

    /// destructor for the HMolPolMagFieldDipole
    virtual ~HMolPolMagFieldDipole() { };

    /// function to get the field value
    void GetFieldValue(const G4double Point[4], G4double* Bfield) const {
      Bfield[0] = Bx;   //x direction of B field
      Bfield[1] = 0.0;  //y direction of B field
      Bfield[2] = 0.0;  //z direction of B field
    }

  private:
    G4double Bx; ///< constant magnetic field in x direction

};
