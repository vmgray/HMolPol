/********************************************
 * \author <b>Programmer:<\b> Valerie Gray
 * \author <b>Assisted By:<\b> Wouter Deconinck
 *
 * \brief <b>Purpose:</b> This is header file which for the primary
 *
 * \date <b>Date:</b> 03-30-2016
 * \date <b>Modified:</b> 04-18-2016
 *
 * \note <b>Entry Conditions:</b> none
 *
 ********************************************/
#ifndef HMOLPOLEVENTPRIMARYCONDITIONS_HH_
#define HMOLPOLEVENTPRIMARYCONDITIONS_HH_

//standard includes
#include <math.h>

// ROOT includes
#include <TObject.h>

class HMolPolEventPrimaryConditions:
    public TObject
{
  public:
    //the following are equivalent
    HMolPolEventPrimaryConditions();  ///< Constructor for HMolPolEventPrimaryConditions
//    HMolPolEventPrimaryConditions(HMolPolPrimaryGeneratorAction* PAG);  ///< Constructor for
    // HMolPolEventPrimaryConditions
    virtual ~HMolPolEventPrimaryConditions()
    {
    }
    ;  ///< Destructor for
    //HMolPolEventPrimaryConditions

  public:
    //getters setters
    //--------------------------------------------------
    ///<  Set Raster size
    void SetRasterX(const Double_t& RasterX)
    {
      fRasterX = RasterX;
      return;
    }  ///< set the X raster of the beam

    void SetRasterY(const Double_t& RasterY)
    {
      fRasterY = RasterY;
      return;
    }  ///< set the y raster of the beam

    //--------------------------------------------------
    ///<  Set Z generator location
    void SetZGenMin(const Double_t& ZGenMin)
    {
      fZGenMin = ZGenMin;
      return;
    }  ///< set the min Z generator location

    void SetZGenMax(const Double_t& ZGenMax)
    {
      fZGenMax = ZGenMax;
      return;
    }  ///< set the max generator location

    //--------------------------------------------------
    //<  Beam Energy
    void SetBeamE(const Double_t& BeamE)
    {
      fBeamE = BeamE;
      return;
    }  ///< set the Energy of the beam

    //--------------------------------------------------
    ///<  Theta angle ranges
    ///<  Set Theta CM range size and update differences and omega
    void SetThetaComMax(const Double_t& ThetaComMax)
    {
      fThetaCM_Max = ThetaComMax;
      UpdateDeltaTheta();
      UpdateDelta_CosTheta();
      UpdateDeltaOmega();
      return;
    }  ///< set
    /// the max Theta CM value

    void SetThetaComMin(const Double_t& ThetaComMin)
    {
      fThetaCM_Min = ThetaComMin;
      UpdateDeltaTheta();
      UpdateDelta_CosTheta();
      UpdateDeltaOmega();
      return;
    }  ///< set
    /// the min Theta CM value

    //--------------------------------------------------
    ///<  phi angle ranges
    ///<  Set Phi CM range size and update differences and omega
    void SetPhiComMax(const Double_t& PhiComMax)
    {
      fPhiCM_Max = PhiComMax;
      UpdateDeltaPhi();
      UpdateDeltaOmega();
      return;
    }  ///< set
    /// the max Phi CM value

    void SetPhiComMin(const Double_t& PhiComMin)
    {
      fPhiCM_Min = PhiComMin;
      UpdateDeltaPhi();
      UpdateDeltaOmega();
      return;
    }  ///< set
    /// the min Theta CM value

  public:
    //Variables
    Double_t fBeamE;  ///< the incoming electron beam energy

    Double_t fThetaCM_Min;  //< Min theta center of mass angle thrown over
    Double_t fThetaCM_Max;  //< Max theta center of mass angle thrown over
    Double_t fDelta_Theta;  //< range in theta center of mass angle thrown over
    Double_t fDelta_CosTheta;  //< range in cos theta center of mass angle thrown over

    Double_t fPhiCM_Min;  //< Min phi center of mass angle thrown over
    Double_t fPhiCM_Max;  //< Max phi center of mass angle thrown over
    Double_t fDelta_Phi;  //< range in phi center of mass angle thrown over

    Double_t fDelta_Omega;  //< Solid angle that is tossed over

    Double_t fRasterX;  //< Raster size in X
    Double_t fRasterY;  //< MRaster size in Y

    Double_t fZGenMin;  //< Min Z value of generator
    Double_t fZGenMax;  //< Max Z value of generator

    /*
     Double_t fSoilMagFeild;

     //Target
     Double_t fTargetDentsity;
     Double_t fTragetLenght;

     Double_t fLuminosity;
     */

    Double_t fNb; // really Hz/uA Can't get that in G4/Root
    //> fNb = 6.241e12 Hz/uA
    //Double_t fNb;  //< N_b // really Hz/uA Can't get that in G4/Root

    Double_t fMass_Gamma;  ///< mass * gamma or e_com for the simulation

  private:
    //functions for calculations
    /********
     * Function for the solid angle covered
     * Double_t Omega = (fPhiCM_Max - fPhiCM_Min)
     * (cos(fThetaCM_Min) - cos(fThetaCM_Max));
     *********/
    void UpdateDeltaOmega();

    //Calculate the differnce in the theta and phi angles
    void UpdateDeltaPhi()
    {
      fDelta_Phi = (fPhiCM_Max - fPhiCM_Min);
      return;
    }

    void UpdateDeltaTheta()
    {
      fDelta_Theta = (fThetaCM_Max - fThetaCM_Min);
      return;
    }

    void UpdateDelta_CosTheta()
    {
      fDelta_CosTheta = (cos(fThetaCM_Min) - cos(fThetaCM_Max));
      return;
    }

  public:
    /// Register this class with ROOT. This adds some standard ROOT functions to
    /// the class, and makes sure that on the command line in ROOT it can do
    /// tab-completion. This is associated with the ClassImp() line at the top
    /// of the .cc file.
  ClassDef(HMolPolEventPrimaryConditions,1)
};

#endif /* HMOLPOLEVENTPRIMARYCONDITIONS_HH_ */
