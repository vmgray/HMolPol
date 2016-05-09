/********************************************
 * \author <b>Programmer:<\b> Wouter Deconinck
 * \author <b>Assisted By:<\b>
 *
 * \brief <b>Purpose:</b> This is header file for the quadrupole magnetic field
 *
 * \date <b>Date:</b> 05-04-2016
 * \date <b>Modified:</b>
 *
 * \note <b>Entry Conditions:</b> none
 *
********************************************/

//HMolPol specific includes
#include <HMolPolMagneticField.hh>

class HMolPolMagFieldQuadrupole: public HMolPolMagneticField {

  public:

    /// constructor for the HMolPolMagFieldQuadrupole
    HMolPolMagFieldQuadrupole(): K(0.0 * CLHEP::tesla / CLHEP::m) { };

    /// destructor for the HMolPolMagFieldQuadrupole
    virtual ~HMolPolMagFieldQuadrupole() { };

    /// function to get the field value
    void GetFieldValue(const G4double Point[4], G4double* Bfield) const {
      Bfield[0] = K*Point[0];  //x direction of B field
      Bfield[1] = K*Point[1];  //y direction of B field
      Bfield[2] = 0.;          //z direction of B field
    };

  private:
    G4double K; ///< constant magnetic field gradient

};
