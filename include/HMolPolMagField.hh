/********************************************
 * \author <b>Programmer:<\b> Wouter Deconinck
 * \author <b>Assisted By:<\b>
 *
 * \brief <b>Purpose:</b> This is header file for a magnetic field
 *
 * \date <b>Date:</b> 05-04-2016
 * \date <b>Modified:</b>
 *
 * \note <b>Entry Conditions:</b> none
 *
********************************************/

#ifndef HMOLPOLMAGFIELD_HH_
#define HMOLPOLMAGFIELD_HH_

//Geant4 specific includes
#include <G4MagneticField.hh>

class HMolPolMagField: public G4MagneticField {

  public:

    /// constructor for the HMolPolMagField
    HMolPolMagField() { };

    /// destructor for the HMolPolMagField
    virtual ~HMolPolMagField() { };

    /// pure virtual function to get the field value
    virtual void  GetFieldValue( const G4double Point[4],
                                       G4double *Bfield ) const = 0;

};

#endif // HMOLPOLMAGFIELD_HH_
