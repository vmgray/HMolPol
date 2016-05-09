/********************************************
 * \author <b>Programmer:<\b> Wouter Deconinck
 * \author <b>Assisted By:<\b>s
 *
 * \brief <b>Purpose:</b> This is header file for the mapped magnetic field
 *
 * \date <b>Date:</b> 05-04-2016
 * \date <b>Modified:</b>
 *
 * \note <b>Entry Conditions:</b> none
 *
********************************************/

//HMolPol specific includes
#include <HMolPolMagField.hh>

class HMolPolMagFieldMap: public HMolPolMagField {

  public:

    /// constructor for the HMolPolMagFieldMap
    HMolPolMagFieldMap(): fieldmap(0) { };

    /// destructor for the HMolPolMagFieldMap
    virtual ~HMolPolMagFieldMap() {
      if (fieldmap) delete fieldmap;
    };

    /// function to get the field value
    void GetFieldValue(const G4double Point[4], G4double* Bfield) const;

  private:
    HMolPolMagFieldMapBasic *fieldmap; ///< magnetic field map

};
