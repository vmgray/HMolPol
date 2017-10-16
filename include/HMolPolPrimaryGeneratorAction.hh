/********************************************
 * \author <b>Programmer:<\b> Valerie Gray
 * \author <b>Assisted By:<\b> Wouter Deconinck
 *
 * \brief <b>Purpose:</b> This is header file which for the primary action generator.
 * Or the generator of the primary particle
 *
 * Also pass all the variables that are initial conditions to the
 * PrimaryConditions branch.
 *
 * \date <b>Date:</b> 06-25-2013
 * \date <b>Modified:</b> 07-18-2016
 *
 * \note <b>Entry Conditions:</b> none
 *
 ********************************************/

#ifndef HMolPolPrimaryGeneratorAction_h
#define HMolPolPrimaryGeneratorAction_h

//Geant4 includes
#include <G4ParticleTable.hh>
#include <G4VUserPrimaryGeneratorAction.hh>
#include <G4SystemOfUnits.hh>

// HMolPol includes
#include "HMolPolEventPrimaryConditions.hh"
#include "HMolPolAnalysis.hh"

// Geant4 classes
class G4ParticleGun;

class HMolPolPrimaryGeneratorAction:
    public G4VUserPrimaryGeneratorAction
{

  public:
    //create the constructor and destructor for the primary particle generator
    HMolPolPrimaryGeneratorAction(HMolPolAnalysis* AN);  ///< The constructor
    // for the HMolPolPrimaryGeneratorAction class - it gets passed the
    // HMolPolAnalysis class so that the histogram will get filled
    virtual ~HMolPolPrimaryGeneratorAction();  ///< The destructor
    // for the HMolPolPrimaryGeneratorAction class

    //function to generate the primary electrons
    void GeneratePrimaries(G4Event* anEvent);  ///< function to generate
    // primary electrons

  private:
    HMolPolAnalysis* fAnalysis;  ///< the analysis class

  public:
    //< Getters and Setters
    //Set info for both the Primary generator and the primary conditions branch

    ///<  Set Raster size
    void SetRasterX(const G4double& XRaster)
    {
      fRasterX = XRaster;
      fAnalysis->fPrimaryConditions->SetRasterX(XRaster);
    }  ///< set
    /// the XRaster size of beam
    void SetRasterY(const G4double& YRaster)
    {
      fRasterY = YRaster;
      fAnalysis->fPrimaryConditions->SetRasterY(YRaster);
    }  ///< set
    /// the YRaster size of beam

    //--------------------------------------------------
    //Z Gen location
    ///<  Set Z generator location
    void SetZGenMin(const G4double& ZGenMin)
    {
      fZGenMin = ZGenMin;
      fAnalysis->fPrimaryConditions->SetZGenMin(ZGenMin);
    }  ///< set
    /// the min Z location of the generatior
    void SetZGenMax(const G4double& ZGenMax)
    {
      fZGenMax = ZGenMax;
      fAnalysis->fPrimaryConditions->SetZGenMax(ZGenMax);
    }  ///< set
    /// the max Z location of the generator

    //--------------------------------------------------
    //Beam Energy
    void SetBeamE(const G4double& BeamE)
    {
      fBeamE = BeamE;
      fAnalysis->fPrimaryConditions->SetBeamE(BeamE);
    }  ///< set the Energy of the beam

    //--------------------------------------------------
    ///<  Set Theta CM range size
    void SetThetaComMax(const G4double& ThetaComMax)
    {
      fTheta_com_max = ThetaComMax;
      fAnalysis->fPrimaryConditions->SetThetaComMax(ThetaComMax);
    }  ///< set
    /// the max Theta CM value
    void SetThetaComMin(const G4double& ThetaComMin)
    {
      fTheta_com_min = ThetaComMin;
      fAnalysis->fPrimaryConditions->SetThetaComMin(ThetaComMin);
    }  ///< set
    /// the min Theta CM value

    ///<  Set phi CM range size
    void SetPhiComMax(const G4double& PhiComMax)
    {
      fPhi_com_max = PhiComMax;
      fAnalysis->fPrimaryConditions->SetPhiComMax(PhiComMax);
    }  ///< set
    /// the max Phi CM value
    void SetPhiComMin(const G4double& PhiComMin)
    {
      fPhi_com_min = PhiComMin;
      fAnalysis->fPrimaryConditions->SetPhiComMin(PhiComMin);
    }  ///< set
    /// the min Phi CM value


  private: //< Valriable
    G4double fRasterX;  ///< the total raster size in X
    G4double fRasterY;  ///< the total raser size in Y
    G4double fZGenMin;  ///< the min Z generator value
    G4double fZGenMax;  ///< the max Z generator value
    G4double fBeamE;  ///< the incoming electron beam energy

    // Center of mass angles to generate particles between
    G4double fTheta_com_min;  /// < this is the minimum Theta that electrons
    /// will be generated over
    G4double fTheta_com_max;  /// < this is the minimum Theta that electrons
    /// will be generated over
    G4double fPhi_com_min;  /// < this is the minimum Theta that electrons
    /// will be generated over
    G4double fPhi_com_max;  /// < this is the minimum Theta that electrons
    /// will be generated over

    //the primary generation of the primary electrons
    /// \todo why does doxygen not want this documented?? and are these right?
    G4ParticleGun* fParticleGun;  ///< the ParticleGun class
};

#endif
