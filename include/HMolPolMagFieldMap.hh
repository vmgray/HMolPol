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

#ifndef HMOLPOLMAGFIELDMAP_HH_
#define HMOLPOLMAGFIELDMAP_HH_

//HMolPol specific includes
#include <HMolPolMagField.hh>
#include <HMolPolMagFieldMapBasic.hh>

class HMolPolMagFieldMap: public HMolPolMagField {

  public:

    /// constructor for the HMolPolMagFieldMap
    HMolPolMagFieldMap(const std::string& filename) {
      fieldmap = new HMolPolMagFieldMapBasic<float,3>(filename);
    };

    /// destructor for the HMolPolMagFieldMap
    virtual ~HMolPolMagFieldMap() {
      if (fieldmap) delete fieldmap;
    };

    /// function to get the field value
    void GetFieldValue(const G4double four_point[4], G4double* bfield) const {
      G4double three_point[3] = {four_point[0],four_point[1],four_point[2]};
      if (fieldmap) fieldmap->GetValue(three_point, bfield);
    }

  private:
    HMolPolMagFieldMapBasic<float,3> *fieldmap; ///< magnetic field map

};

#endif // HMOLPOLMAGFIELDMAP_HH_
