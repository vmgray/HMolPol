/********************************************
 * \author <b>Programmer:<\b> Valerie Gray
 * \author <b>Assisted By:<\b> Wouter Deconinck
 *
 * \brief <b>Purpose:</b> This is header file for the header file for the
 *      HMolPolEventUnits class .
 *
 * \date <b>Date:</b> 07-15-2013
 * \date <b>Modified:</b> 07-20-2013
 *
 * \note <b>Entry Conditions:</b> none
 *
********************************************/

//CHANGE ME!!!
//=============================================================================
//
//=============================================================================
//
//   ---------------------------
//  | Doxygen Class Information |
//  ---------------------------
/**
   \class HMolPolEventUnits

   \brief Defines a branch of the ROOT tree - the Units branch

          Units - has all the units need for the simulation.

 */
//=============================================================================

#ifndef HMOLPOLEVENTUNITS_HH_
#define HMOLPOLEVENTUNITS_HH_

// root include
#include <TObject.h>

class HMolPolEventUnits : public TObject
{
  public:

    // Constructor
    HMolPolEventUnits();  ///< constructor for the HMolPolEventUnits class
    // Destructor
    virtual ~HMolPolEventUnits() { };  ///< destrutor for the HMolPolEventUnits class

    // Distance
    const Double_t mm;        /// \ingroup root Units of mm
    const Double_t cm;        /// \ingroup root Units of cm
    const Double_t m;         /// \ingroup root Units of m
    // Area
    const Double_t mm2;       /// \ingroup root Units of mm^2
    const Double_t cm2;       /// \ingroup root Units of cm^2
    const Double_t m2;        /// \ingroup root Units of m^2
    // Volume
    const Double_t mm3;       /// \ingroup root Units of mm^3
    const Double_t cm3;       /// \ingroup root Units of cm^3
    const Double_t m3;        /// \ingroup root Units of m^3
    // Energy
    const Double_t keV;       /// \ingroup root Units of keV
    const Double_t MeV;       /// \ingroup root Units of MeV
    const Double_t GeV;       /// \ingroup root Units of GeV
    // Angle
    const Double_t rad;       /// \ingroup root Units of rad
    const Double_t deg;       /// \ingroup root Units of deg
    const Double_t sr;        /// \ingroup root Units of steradian
    // Cross Section
    const Double_t barn;      /// \ingroup root Units of barn
    const Double_t mbarn;     /// \ingroup root Units of millibarn
    const Double_t ubarn;  /// \ingroup root Units of microbarn
    // Time
    const Double_t nsec;      /// \ingroup root Units of nanosec
    const Double_t msec;      /// \ingroup root Units of millisec
    const Double_t sec;       /// \ingroup root Units of sec
    // Magnetic Field
    const Double_t Tesla;     /// \ingroup root Units of tesla

    /// Register this class with ROOT. This adds some standard ROOT functions to
    /// the class, and makes sure that on the command line in ROOT it can do
    /// tab-completion. This is associated with the ClassImp() line at the top
    /// of the .cc file.
    ClassDef(HMolPolEventUnits,1)

  private:

};

#endif /* HMOLPOLEVENTUNITS_HH_ */
