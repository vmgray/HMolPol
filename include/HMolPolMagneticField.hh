/********************************************
 * \author <b>Programmer:<\b> Wouter Deconinck
 * \author <b>Assisted By:<\b>
 *
 * \brief <b>Purpose:</b> This is header file for a full magnetic field
 *
 * \date <b>Date:</b> 05-04-2016
 * \date <b>Modified:</b>
 *
 * \note <b>Entry Conditions:</b> none
 *
********************************************/

//System includes
#include <vector>

//Geant4 specific includes
#include <G4MagneticField.hh>

//HMolPol specific includes
#include "HMolPolMagField.hh"

class HMolPolMagneticField: public G4MagneticField {

  public:

    /// constructor for the HMolPolMagneticField
    HMolPolMagneticField() { };

    /// destructor for the HMolPolMagneticField
    virtual ~HMolPolMagneticField() {
      for (std::vector<const HMolPolMagField*>::iterator iter = fields.begin();
          iter != fields.end(); iter++) {
        delete (*iter);
      }
    };

    /// function to get the field value
    void GetFieldValue(const G4double Point[4],
                             G4double *Bfield) const { };

    /// add field to list
    void AddField(const HMolPolMagField* field) {
      fields.push_back(field);
    }

  private:

    /// list of magnetic fields
    std::vector<const HMolPolMagField*> fields;

};
