# ifndef AliAnalysisTaskEtaReconstruction_H
# define AliAnalysisTaskEtaReconstruction_H

//###########################################################
//#                                                         #
//#             Single Electron Efficiency Task             #
//#             Pair Efficiency Task                        #
//#             Track Resolution                            #
//#                                                         #
//#  Authors:                                               #
//#   Carsten Klein, Uni Frankfurt / Carsten.Klein@cern.ch  #
//#                                                         #
//###########################################################

#include "AliAnalysisTaskSE.h"
#include "AliDielectronSignalMC.h"
#include "AliDielectronCutGroup.h"
#include "AliDielectronVarCuts.h"
#include "TString.h"
#include "THnSparse.h"
class TH1F;
class TH2F;
class TH3D;


class AliTriggerAnalysis;
class AliAnalysisFilter;
class AliPIDResponse;

class TFile;
class TTree;
class TList;

class TObjArray;


class AliAnalysisTaskEtaReconstruction : public AliAnalysisTaskSE {
public:
  // two class constructors
  AliAnalysisTaskEtaReconstruction() ;

  AliAnalysisTaskEtaReconstruction(const char* name);
  // class destructor
   virtual ~AliAnalysisTaskEtaReconstruction();
   // called once at beginning or runtime
   virtual void UserCreateOutputObjects();
   // called for each event
   virtual void UserExec(Option_t* option);
   // called at end of analysis
   virtual void Terminate(Option_t* option);

   void SetRun1Analysis(Bool_t answer){ run1analysis = answer; }

   enum Detector {kITS, kTPC, kTOF};
   Bool_t               GetEnablePhysicsSelection() const   {return fSelectPhysics; }
   Int_t                GetTriggerMask() const              {return fTriggerMask; }
   AliAnalysisCuts*     GetEventFilter()                    {return fEventFilter;}

   std::vector<double>  GetPtBins() const                   {return fPtBins;}
   std::vector<double>  GetEtaBins() const                  {return fEtaBins;}
   std::vector<double>  GetPhiBins() const                  {return fPhiBins;}

   // Debug Variable
   void   SetDebug(bool debug) {fdebug = debug;}


   // MC Signal setter
   void   AddSinglePrimaryLegMCSignal(AliDielectronSignalMC signal1)         {fSinglePrimaryLegMCSignal.push_back(signal1);}
   void   AddSingleSecondaryLegMCSignal(AliDielectronSignalMC signal1)       {fSingleSecondaryLegMCSignal.push_back(signal1);}
   void   AddPrimaryPairMCSignal(AliDielectronSignalMC signal1)              {fPrimaryPairMCSignal.push_back(signal1);}
   void   AddSecondaryPairMCSignal(AliDielectronSignalMC signal1)            {fSecondaryPairMCSignal.push_back(signal1);}
   void   AddFourPairMCSignal(AliDielectronSignalMC signal1)          {fFourPairMCSignal.push_back(signal1);}
   void   AddMCSignalsWherePrimaryDielectronPairNotFromSameMother(std::vector<bool> vec) {fPrimaryDielectronPairNotFromSameMother = vec;}
   void   AddMCSignalsWhereSecondaryDielectronPairNotFromSameMother(std::vector<bool> vec) {fSecondaryDielectronPairNotFromSameMother = vec;}

   // PID correction functions
   void   SetCentroidCorrFunction(Detector det, TObject *fun, UInt_t varx, UInt_t vary=0, UInt_t varz=0);
   void   SetWidthCorrFunction   (Detector det, TObject *fun, UInt_t varx, UInt_t vary=0, UInt_t varz=0);


   // Generator
   void   SetGeneratorName         (TString generatorName) { fGeneratorName = generatorName;}
   void   SetGeneratorMCSignalName (TString generatorName) { fGeneratorMCSignalName  = generatorName;}
   void   SetGeneratorULSSignalName(TString generatorName) { fGeneratorULSSignalName = generatorName;}

   // Event setter
   void   SetEnablePhysicsSelection(Bool_t selectPhysics)   {fSelectPhysics = selectPhysics;}
   void   SetTriggerMask(Int_t triggermask)                 {fTriggerMask = triggermask;}
   void   SetEventFilter(AliAnalysisCuts * const filter)    {fEventFilter = filter;}
   void   SetCentrality(double cent_min, double cent_max)             {fMinCentrality = cent_min; fMaxCentrality = cent_max;} //For pp and pPb analysis use SetCentrality(-1, -1)

   void   SetCentralityFile(std::string filename) {fCentralityFilename = filename; }

   // Support Histos
   void   SetSupportHistoMCSignalAndCutsetting(int nMCSignal, int nCutsetting) {fSupportMCSignal = nMCSignal; fSupportCutsetting = nCutsetting;}

   // Resolution setter
   void   SetResolutionFile(std::string filename) {fResoFilename = filename; }
   void   SetResolutionFileFromAlien(std::string filename) {fResoFilenameFromAlien = filename; }
   void   SetSmearGenerated(bool setSmearingGen) { fDoGenSmearing = setSmearingGen; }
   void   SetResolutionDeltaPtBinsLinear (const double min, const double max, const unsigned int steps){SetBinsLinear("ptDelta_reso", min, max, steps);}
   void   SetResolutionRelPtBinsLinear   (const double min, const double max, const unsigned int steps){SetBinsLinear("ptRel_reso", min, max, steps);}
   void   SetResolutionEtaBinsLinear  (const double min, const double max, const unsigned int steps){SetBinsLinear("eta_reso", min, max, steps);}
   void   SetResolutionPhiBinsLinear  (const double min, const double max, const unsigned int steps){SetBinsLinear("phi_reso", min, max, steps);}
   void   SetResolutionThetaBinsLinear(const double min, const double max, const unsigned int steps){SetBinsLinear("theta_reso", min, max, steps);}

   // single electron binning setter
   void   SetPtBins(std::vector<double> ptBins)             {fPtBins = ptBins;}
   void   SetPtBinsLinear(const double min, const double max, const unsigned int steps) {SetBinsLinear("pt", min, max, steps);}
   void   SetEtaBins(std::vector<double> etaBins)           {fEtaBins = etaBins;}
   void   SetEtaBinsLinear(const double min, const double max, const unsigned int steps){SetBinsLinear("eta", min, max, steps);}
   void   SetPhiBins(std::vector<double> phiBins)           {fEtaBins = phiBins;}
   void   SetPhiBinsLinear(const double min, const double max, const unsigned int steps){SetBinsLinear("phi", min, max, steps);}
   void   SetThetaBinsLinear(const double min, const double max, const unsigned int steps){SetBinsLinear("theta", min, max, steps);}

   // pair binning setter
   void   SetMassBins(std::vector<double> massBins){fMassBins=massBins;}
   void   SetMassBinsLinear(const double min, const double max, const unsigned int steps){SetBinsLinear("mass", min, max, steps);}
   void   SetPairPtBins(std::vector<double> pairptBins){ fPairPtBins = pairptBins;}
   void   SetPairPtBinsLinear(const double min, const double max, const unsigned int steps){SetBinsLinear("pairpt", min, max, steps);}

   // Pair related setter
   void   SetDoPairing(Bool_t doPairing) {fDoPairing = doPairing;}
   void   SetDoFourPairing(Bool_t doFourPairing) {fDoFourPairing = doFourPairing;}
   void   SetULSandLS(Bool_t doULSandLS) {fDoULSandLS = doULSandLS;}
   void   SetDeactivateLS(Bool_t deactivateLS) {fDeactivateLS = deactivateLS;}
   void   SetKinematicCuts(double ptMin, double ptMax, double etaMin, double etaMax) {fPtMin = ptMin; fPtMax = ptMax; fEtaMin = etaMin; fEtaMax = etaMax;}

   // Single leg from Pair related setter
   void   SetWriteLegsFromPair(bool enable){fWriteLegsFromPair = enable;}
   void   SetPtMinLegsFromPair(const double ptMin){fPtMinLegsFromPair = ptMin;}
   void   SetPtMaxLegsFromPair(const double ptMax){fPtMaxLegsFromPair = ptMax;}
   void   SetEtaMinLegsFromPair(const double etaMin){fEtaMinLegsFromPair = etaMin;}
   void   SetEtaMaxLegsFromPair(const double etaMax){fEtaMaxLegsFromPair = etaMax;}
   void   SetPhiMinLegsFromPair(const double phiMin){fPhiMinLegsFromPair = phiMin;}
   void   SetPhiMaxLegsFromPair(const double phiMax){fPhiMaxLegsFromPair = phiMax;}
   void   SetOpAngleMinLegsFromPair(const double opAngleMin){fOpAngleMinLegsFromPair = opAngleMin;}
   void   SetOpAngleMaxLegsFromPair(const double opAngleMax){fOpAngleMaxLegsFromPair = opAngleMax;}
   void   SetPtNBinsLegsFromPair(const int ptNBins){fPtNBinsLegsFromPair = ptNBins;}
   void   SetEtaNBinsLegsFromPair(const int etaNBins){fEtaNBinsLegsFromPair = etaNBins;}
   void   SetPhiNBinsLegsFromPair(const int phiNBins){fPhiNBinsLegsFromPair = phiNBins;}
   void   SetOpAngleNBinsLegsFromPair(const int opAngleNBins){fOpAngleNBinsLegsFromPair = opAngleNBins;}

   // Set Cocktail waiting
   void SetDoCocktailWeighting(bool doCocktailWeight) { fDoCocktailWeighting = doCocktailWeight; }
   void SetCocktailWeighting(std::string cocktailFilename) { fCocktailFilename = cocktailFilename; }
   void SetCocktailWeightingFromAlien(std::string cocktailFilenameFromAlien) { fCocktailFilenameFromAlien = cocktailFilenameFromAlien; }

   // Generator related setter
   void   SetMinPtGen(double ptMin)   {fPtMinGen = ptMin;}; // Look only at particles which are above a threshold. (reduces computing time/less tracks when looking at secondaries)
   void   SetMaxPtGen(double ptMax)   {fPtMaxGen = ptMax;};
   void   SetMinEtaGen(double etaMin) {fEtaMinGen = etaMin;}; // Look only at particles which are above a threshold. (reduces computing time/less tracks when looking at secondaries)
   void   SetMaxEtaGen(double etaMax) {fEtaMaxGen = etaMax;};

   // Set mass cuts
   void   SetLowerMassCutPrimaries(double massCut) {fLowerMassCutPrimaries = massCut;};
   void   SetUpperMassCutPrimaries(double massCut) {fUpperMassCutPrimaries = massCut;};
   void   SetMassCutSecondaries(double massCut) {fMassCutSecondaries = massCut;};
   void   SetMassCut(Bool_t DoMassCut) {fDoMassCut = DoMassCut;};

   // Track cuts setter
   void   AddTrackCuts_primary  (AliAnalysisFilter* filter) {fTrackCuts_primary.push_back(filter);}
   void   AddTrackCuts_secondary(AliAnalysisFilter* filter) {fTrackCuts_secondary.push_back(filter);}



  class Particle{
  public:
    Particle() :
      fPt(-99), fEta(-99), fPhi(-99), fCharge(-99), fPt_smeared(0.), fEta_smeared(0.), fPhi_smeared(0.), fTrackID(0), fMotherID(0), fGrandMotherID(0), fMCSignalPair(false), fULSSignalPair(false), isMCSignal_primary(), isMCSignal_secondary(), isReconstructed_primary(), isReconstructed_secondary(), primaryDielectronPairFromSameMother(), secondaryDielectronPairFromSameMother() {}
    Particle(double pt, double eta, double phi, short charge) :
      fPt(pt), fEta(eta), fPhi(phi), fCharge(charge), fPt_smeared(0.), fEta_smeared(0.), fPhi_smeared(0.), fTrackID(0), fMotherID(0), fGrandMotherID(0), fMCSignalPair(false), fULSSignalPair(false), isMCSignal_primary(), isMCSignal_secondary(), isReconstructed_primary(), isReconstructed_secondary(), primaryDielectronPairFromSameMother(), secondaryDielectronPairFromSameMother() {}

    void SetTrackID(int id) {fTrackID = id;}
    void SetMotherID(int id) {fMotherID = id;}
    void SetGrandMotherID(int id) {fGrandMotherID = id;}
    void SetMCSignalPair (bool value) {fMCSignalPair = value;}
    void SetULSSignalPair(bool value) {fULSSignalPair = value;}
    void SetprimaryDielectronPairFromSameMother(std::vector<Bool_t> vec){primaryDielectronPairFromSameMother = vec;}
    void SetsecondaryDielectronPairFromSameMother(std::vector<Bool_t> vec){secondaryDielectronPairFromSameMother = vec;}


    int  GetTrackID() {return fTrackID;}
    int  GetMotherID() {return fMotherID;}
    int  GetGrandMotherID() {return fGrandMotherID;}
    bool GetMCSignalPair() {return fMCSignalPair;}
    bool GetULSSignalPair() {return fULSSignalPair;}

    double  fPt;
    double  fEta;
    double  fPhi;
    short   fCharge;
    double  fPt_smeared;
    double  fEta_smeared;
    double  fPhi_smeared;
    int     fTrackID;
    int     fMotherID;
    int     fGrandMotherID;
    bool    fMCSignalPair;
    bool    fULSSignalPair;
    std::vector<Bool_t> isMCSignal_primary;
    std::vector<Bool_t> isMCSignal_secondary;
    std::vector<Bool_t> isReconstructed_primary;
    std::vector<Bool_t> isReconstructed_secondary;
    std::vector<Bool_t> primaryDielectronPairFromSameMother;
    std::vector<Bool_t> secondaryDielectronPairFromSameMother;
  };

  class TwoPair{
  public:
    TwoPair() :
      fPt(-99), fEta(-99), fPhi(-99), fMass(-99), fCharge(-99), fDaughterTrackID_1(0), fDaughterTrackID_2(0), fMCTwoSignal_acc_prim(), fMCTwoSignal_acc_sec(), fFirstPartIsReconstructed(), fSecondPartIsReconstructed() {}
    TwoPair(double pt, double eta, double phi, double mass, short charge) :
      fPt(pt), fEta(eta), fPhi(phi), fMass(mass), fCharge(charge), fDaughterTrackID_1(0), fDaughterTrackID_2(0), fMCTwoSignal_acc_prim(), fMCTwoSignal_acc_sec(), fFirstPartIsReconstructed(), fSecondPartIsReconstructed(){}

    void SetMCTwoSignal_acc_prim (std::vector<Bool_t> vec) {fMCTwoSignal_acc_prim = vec;}
    void SetMCTwoSignal_acc_sec  (std::vector<Bool_t> vec) {fMCTwoSignal_acc_sec = vec;}
    void SetDautherTrackID(int id1, int id2) {fDaughterTrackID_1 = id1; fDaughterTrackID_2 = id2;}
    void SetDauthersAreReconstructed(std::vector<Bool_t> vec1, std::vector<Bool_t> vec2) {fFirstPartIsReconstructed = vec1, fSecondPartIsReconstructed = vec2;}

    std::vector<Bool_t> GetMCTwoSignal_acc_prim () {return fMCTwoSignal_acc_prim;}
    std::vector<Bool_t> GetMCTwoSignal_acc_sec  () {return fMCTwoSignal_acc_sec;}
    int GetFirstDaughter() {return fDaughterTrackID_1;}
    int GetSecondDaughter() {return fDaughterTrackID_2;}

    double  fPt;
    double  fEta;
    double  fPhi;
    double  fMass;
    short   fCharge;
    int fDaughterTrackID_1;
    int fDaughterTrackID_2;
    std::vector<Bool_t> fMCTwoSignal_acc_prim;
    std::vector<Bool_t> fMCTwoSignal_acc_sec;
    std::vector<Bool_t> fFirstPartIsReconstructed;
    std::vector<Bool_t> fSecondPartIsReconstructed;

  };

private:
  enum {kAllEvents=0, kPhysicsSelectionEvents, kFilteredEvents , kCentralityEvents, kLastBin};

  void    SetBinsLinear(const std::string variable, const double min, const double max, const unsigned int steps);

  void    SetPIDResponse(AliPIDResponse *fPIDRespIn)        {fPIDResponse = fPIDRespIn;}
  void    CheckSinglePrimaryLegMCsignals(std::vector<Bool_t>& vec, const int track);
  void    CheckSingleSecondaryLegMCsignals(std::vector<Bool_t>& vec, const int track);
  void    CheckPairMCsignals(std::vector<Bool_t>& vec, AliVParticle* part1, AliVParticle* part2);
  bool    CheckGenerator(int trackID, std::vector<unsigned int> vecHashes);
  void    CheckIfFromMotherWithDielectronAsDaughter(Particle& part);
  Bool_t  CheckIfOneIsTrue(std::vector<Bool_t>& vec);
  Bool_t  CheckIfOneIsTrue(std::vector<Bool_t>& vec, std::vector<Bool_t>& vec1);

  Particle    CreateParticle(AliVParticle* part);

  void    CreateSupportHistos();

  // Function to do reconstructed two pairing and filling histogramms
  void   DoGenAndGenSmearTwoPairing(std::vector<Particle>* vec_negParticle, std::vector<Particle>* vec_posParticle, Bool_t PartPrimary, Bool_t SmearedPair, double centralityWeight);
  void   DoRecTwoPairing(std::vector<Particle> fRecNegPart, std::vector<Particle> fRecPosPart, std::vector<AliDielectronSignalMC> fPairMCSignal, Bool_t PartPrimary, double centralityWeight);
  void   DoFourPairing(std::vector<TwoPair> fPairVec_primary, std::vector<TwoPair> fPairVec_secondary, Bool_t ReconstructedPair, Bool_t SmearedPair, double centralityWeight);

  void    FillTrackHistograms_Primary(AliVParticle* track, AliVParticle* mcTrack, int iCutList);
  void    FillTrackHistograms_Secondary(AliVParticle* track, AliVParticle* mcTrack, int iCutList);

  TLorentzVector ApplyResolution(double pt, double eta, double phi, short ch);
  Double_t GetSmearing(TObjArray *arr, Double_t x);

  double GetWeight(Particle part1, Particle part2, double motherpt);

  AliAnalysisCuts*  fEventFilter; // event filter

  bool   fdebug;  // debug variable

  Bool_t run1analysis;

  TFile* fResoFile;
  std::string fResoFilename;
  std::string fResoFilenameFromAlien;
  TObjArray* fArrResoPt;
  TObjArray* fArrResoEta;
  TObjArray* fArrResoPhi_Pos;
  TObjArray* fArrResoPhi_Neg;

  TList* fOutputList;
  TList* fSingleElectronList;
  TList* fPairList;
  TList* fFourPairList;
  TList* fResolutionList;

  TH2D* fPGen_DeltaP;
  TH2D* fPGen_PrecOverPGen;
  TH2D* fPtGen_DeltaPt;
  TH2D* fPtGen_DeltaPtOverPtGen;
  TH2D* fPtGen_PtRecOverPtGen;
  TH2D* fPtGen_DeltaPt_wGenSmeared;
  TH2D* fPtGen_DeltaPtOverPtGen_wGenSmeared;
  TH2D* fPtGen_PtRecOverPtGen_wGenSmeared;
  TH2D* fPGen_DeltaEta;
  TH2D* fPtGen_DeltaEta;
  TH2D* fPGen_DeltaTheta;
  TH2D* fPGen_DeltaPhi_Ele;
  TH2D* fPGen_DeltaPhi_Pos;
  TH2D* fPtGen_DeltaPhi_Ele;
  TH2D* fPtGen_DeltaPhi_Pos;
  TH2D* fThetaGen_DeltaTheta;
  TH2D* fPhiGen_DeltaPhi;


  std::vector<double> fPtBins;
  std::vector<double> fEtaBins;
  std::vector<double> fPhiBins;
  std::vector<double> fThetaBins;
  std::vector<double> fResolutionDeltaPtBins;
  std::vector<double> fResolutionRelPtBins;
  std::vector<double> fResolutionEtaBins;
  std::vector<double> fResolutionPhiBins;
  std::vector<double> fResolutionThetaBins;
  std::vector<double> fMassBins;
  std::vector<double> fPairPtBins;
  bool fDoGenSmearing;

  double  fPtMin; // Kinematic cut for pairing
  double  fPtMax; // Kinematic cut for pairing
  double  fEtaMin; // Kinematic cut for pairing
  double  fEtaMax; // Kinematic cut for pairing

  double  fPtMinGen;
  double  fPtMaxGen;
  double  fEtaMinGen;
  double  fEtaMaxGen;

  double fLowerMassCutPrimaries; // Mass cut for primary pair
  double fUpperMassCutPrimaries; // Mass cut for primary pair
  double fMassCutSecondaries; // Mass cut for secondary pair

  std::vector<AliDielectronSignalMC> fSinglePrimaryLegMCSignal;
  std::vector<AliDielectronSignalMC> fSingleSecondaryLegMCSignal;
  std::vector<AliDielectronSignalMC> fPrimaryPairMCSignal;
  std::vector<AliDielectronSignalMC> fSecondaryPairMCSignal;
  std::vector<AliDielectronSignalMC> fFourPairMCSignal;
  std::vector<bool> fPrimaryDielectronPairNotFromSameMother; // this is used to get electrons from charmed mesons in a environment where GEANT is doing the decay of D mesons, like in LHC18b5a
  std::vector<bool> fSecondaryDielectronPairNotFromSameMother; // this is used to get electrons from charmed mesons in a environment where GEANT is doing the decay of D mesons, like in LHC18b5a

  TString fGeneratorName;
  TString fGeneratorMCSignalName;
  TString fGeneratorULSSignalName;
  std::vector<unsigned int> fGeneratorHashs;
  std::vector<unsigned int> fGeneratorMCSignalHashs;
  std::vector<unsigned int> fGeneratorULSSignalHashs;

  AliPIDResponse* fPIDResponse;
  AliVEvent*      fEvent;
  AliMCEvent*     fMC;
  AliVTrack*      fTrack;
  Bool_t          isAOD;

  Bool_t fSelectPhysics;
  Int_t  fTriggerMask;
  std::vector<AliAnalysisFilter*> fTrackCuts_primary;
  std::vector<AliAnalysisFilter*> fTrackCuts_secondary;
  TBits*  fUsedVars;                // used variables by AliDielectronVarManager

  int fSupportMCSignal; // Setting for which the support histograms are filled
  int fSupportCutsetting; // Setting for which the support histograms are filled

  TH1F* fHistEvents;
  TH1F* fHistEventStat;
  TH1F* fHistCentrality;
  TH1F* fHistVertex;
  TH1F* fHistVertexContibutors;
  TH1F* fHistNTracks;
  Double_t fMinCentrality;
  Double_t fMaxCentrality;

  TFile* fCentralityFile;
  std::string fCentralityFilename;
  TH1F* fHistCentralityCorrection;
  TList* fOutputListSupportHistos;

  std::vector<TList*> fCutListVecPrim;    // Vector filled with each applied primary cutsetting
  std::vector<TList*> fCutListVecSec;     // Vector filled with each applied secondary cutsetting
  // TList* fPrimarySupportHistos;
  // TList* fSecondarySupportHistos;

  std::vector<TH3D*> fHistGenPrimaryPosPart;
  std::vector<TH3D*> fHistGenPrimaryNegPart;
  std::vector<TH3D*> fHistGenSecondaryPosPart;
  std::vector<TH3D*> fHistGenSecondaryNegPart;
  std::vector<TH3D*> fHistGenSmearedPrimaryPosPart;
  std::vector<TH3D*> fHistGenSmearedPrimaryNegPart;
  std::vector<TH3D*> fHistGenSmearedSecondaryPosPart;
  std::vector<TH3D*> fHistGenSmearedSecondaryNegPart;
  std::vector<TH3D*> fHistRecPrimaryPosPart;
  std::vector<TH3D*> fHistRecPrimaryNegPart;
  std::vector<TH3D*> fHistRecSecondaryPosPart;
  std::vector<TH3D*> fHistRecSecondaryNegPart;

  std::vector<TH2D*> fHistGenPrimaryPair;
  std::vector<TH2D*> fHistGenSecondaryPair;
  std::vector<TH2D*> fHistGenSmearedPrimaryPair;
  std::vector<TH2D*> fHistGenSmearedSecondaryPair;
  std::vector<TH2D*> fHistRecPrimaryPair;
  std::vector<TH2D*> fHistRecSecondaryPair;
  std::vector<TH2D*> fHistGenFourPair;
  std::vector<TH2D*> fHistGenSmearedFourPair;
  std::vector<TH2D*> fHistRecFourPair;
  std::vector<TH2D*> fHistGenPair_ULSandLS;
  std::vector<TH2D*> fHistGenFourPair_ULSandLS;
  std::vector<TH2D*> fHistGenSmearedPair_ULSandLS;
  std::vector<TH2D*> fHistGenSmearedFourPair_ULSandLS;
  std::vector<TH2D*> fHistRecPair_ULSandLS;
  std::vector<TH2D*> fHistRecFourPair_ULSandLS;

  bool fWriteLegsFromPair;
  double fPtMinLegsFromPair;
  double fPtMaxLegsFromPair;
  double fEtaMinLegsFromPair;
  double fEtaMaxLegsFromPair;
  double fPhiMinLegsFromPair;
  double fPhiMaxLegsFromPair;
  double fOpAngleMinLegsFromPair;
  double fOpAngleMaxLegsFromPair;
  int fPtNBinsLegsFromPair;
  int fEtaNBinsLegsFromPair;
  int fPhiNBinsLegsFromPair;
  int fOpAngleNBinsLegsFromPair;
  std::vector<THnSparseF*> fTHnSparseGenSmearedLegsFromPrimaryPair;
  std::vector<THnSparseF*> fTHnSparseGenSmearedLegsFromSecondaryPair;
  // std::vector<THnSparseF*> fTHnSparseRecLegsFromPair;
  std::vector<THnSparseF*> fTHnSparseRecLegsFromPrimaryPair;
  std::vector<THnSparseF*> fTHnSparseRecLegsFromSecondaryPair;

  Bool_t fDoPairing;
  Bool_t fDoFourPairing;
  Bool_t fDoULSandLS;
  Bool_t fDeactivateLS;
  Bool_t fDoMassCut;

  std::vector<Particle> fGenNegPart;
  std::vector<Particle> fGenPosPart;
  std::vector<Particle> fGenNegPart_primary;
  std::vector<Particle> fGenPosPart_primary;
  std::vector<Particle> fGenNegPart_secondary;
  std::vector<Particle> fGenPosPart_secondary;
  std::vector<Particle> fGenSmearedNegPart;
  std::vector<Particle> fGenSmearedPosPart;
  std::vector<Particle> fGenSmearedNegPart_primary;
  std::vector<Particle> fGenSmearedPosPart_primary;
  std::vector<Particle> fGenSmearedNegPart_secondary;
  std::vector<Particle> fGenSmearedPosPart_secondary;
  std::vector<Particle> fRecNegPart_primary;
  std::vector<Particle> fRecPosPart_primary;
  std::vector<Particle> fRecNegPart_secondary;
  std::vector<Particle> fRecPosPart_secondary;
  std::vector<Particle> fRecNegPart_PrimAndSec;
  std::vector<Particle> fRecPosPart_PrimAndSec;

  std::vector<TwoPair>  fGenPairVec_primary;
  std::vector<TwoPair>  fGenPairVec_secondary;
  std::vector<TwoPair>  fGenSmearedPairVec_primary;
  std::vector<TwoPair>  fGenSmearedPairVec_secondary;
  std::vector<TwoPair>  fRecPairVec_primary;
  std::vector<TwoPair>  fRecPairVec_secondary;

  bool fDoCocktailWeighting;
  std::string fCocktailFilename;
  std::string fCocktailFilenameFromAlien;
  TFile* fCocktailFile;
  TH1F* fPtPion;
  TH1F* fPtEta;
  TH1F* fPtEtaPrime;
  TH1F* fPtRho;
  TH1F* fPtOmega;
  TH1F* fPtPhi;
  TH1F* fPtJPsi;

  TH1* fPostPIDCntrdCorrTPC;     // post pid correction object for centroids in TPC
  TH1* fPostPIDWdthCorrTPC;      // post pid correction object for widths in TPC
  TH1* fPostPIDCntrdCorrITS;     // post pid correction object for centroids in ITS
  TH1* fPostPIDWdthCorrITS;      // post pid correction object for widths in ITS
  TH1* fPostPIDCntrdCorrTOF;     // post pid correction object for centroids in TOF
  TH1* fPostPIDWdthCorrTOF;      // post pid correction object for widths in TOF

  AliAnalysisTaskEtaReconstruction(const AliAnalysisTaskEtaReconstruction&); // not implemented
  AliAnalysisTaskEtaReconstruction& operator=(const AliAnalysisTaskEtaReconstruction&); // not implemented

  ClassDef(AliAnalysisTaskEtaReconstruction, 1);
};


# endif
