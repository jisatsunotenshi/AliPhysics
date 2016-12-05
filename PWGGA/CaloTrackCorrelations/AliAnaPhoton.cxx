/**************************************************************************
 * Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 *                                                                        *
 * Author: The ALICE Off-line Project.                                    *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes hereby granted      *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

// --- ROOT system ---
#include <TH2F.h>
#include <TClonesArray.h>
#include <TObjString.h>
#include "TParticle.h"
#include "TDatabasePDG.h"

// --- Analysis system ---
#include "AliAnaPhoton.h"
#include "AliCaloTrackReader.h"
#include "AliStack.h"
#include "AliCaloPID.h"
#include "AliMCAnalysisUtils.h"
#include "AliFiducialCut.h"
#include "AliVCluster.h"
#include "AliAODMCParticle.h"
#include "AliMixedEvent.h"
#include "AliAODEvent.h"
#include "AliESDEvent.h"
#include "AliMCEvent.h"

// --- Detectors ---
#include "AliPHOSGeoUtils.h"
#include "AliEMCALGeometry.h"

/// \cond CLASSIMP
ClassImp(AliAnaPhoton) ;
/// \endcond

//____________________________
/// Default constructor.
/// Initialize parameters with default values.
//____________________________
AliAnaPhoton::AliAnaPhoton() :
AliAnaCaloTrackCorrBaseClass(),
fMinDist(0.),                 fMinDist2(0.),                fMinDist3(0.),
fRejectTrackMatch(0),         fFillTMHisto(kFALSE),
fTimeCutMin(-10000),          fTimeCutMax(10000),
fNCellsCut(0),
fNLMCutMin(-1),               fNLMCutMax(10),
fFillSSHistograms(0),         fFillEMCALRegionSSHistograms(0), 
fFillConversionVertexHisto(0),fFillOnlySimpleSSHisto(1),
fFillSSNLocMaxHisto(0),
fNOriginHistograms(9),        fNPrimaryHistograms(5),
fMomentum(),                  fMomentum2(),
fPrimaryMom(),                fProdVertex(),
fConstantTimeShift(0),        fFillEBinAcceptanceHisto(0), fNEBinCuts(0),
fStudyActivityNearCluster(0), 
// Histograms

// Control histograms
fhNCellsE(0),                 fhCellsE(0),
fhMaxCellDiffClusterE(0),     fhTimePt(0),                  fhEtaPhi(0),

fhEPhoton(0),                 fhPtPhoton(0),
fhPhiPhoton(0),               fhEtaPhoton(0),
fhEtaPhiPhoton(0),            fhEtaPhi05Photon(0),
fhPtCentralityPhoton(0),      fhPtEventPlanePhoton(0),

// Shower shape histograms
fhNLocMax(0),
fhDispE(0),                   fhDispPt(0),                  
fhLam0E(0),                   fhLam0Pt(0),        
fhLam1E(0),                   fhLam1Pt(0),
fhLam0PtNLM1(0),              fhLam0PtNLM2(0),              
fhLam1PtNLM1(0),              fhLam1PtNLM2(0),
fhDispETRD(0),                fhLam0ETRD(0),                fhLam0PtTRD(0),     fhLam1ETRD(0),
fhDispETM(0),                 fhLam0ETM(0),                 fhLam0PtTM(0),      fhLam1ETM(0),
fhDispETMTRD(0),              fhLam0ETMTRD(0),              fhLam0PtTMTRD(0),   fhLam1ETMTRD(0),

fhNCellsLam0LowE(0),          fhNCellsLam1LowE(0),          fhNCellsDispLowE(0),
fhNCellsLam0HighE(0),         fhNCellsLam1HighE(0),         fhNCellsDispHighE(0),

fhEtaLam0LowE(0),             fhPhiLam0LowE(0),
fhEtaLam0HighE(0),            fhPhiLam0HighE(0),
fhLam0DispLowE(0),            fhLam0DispHighE(0),
fhLam1Lam0LowE(0),            fhLam1Lam0HighE(0),
fhDispLam1LowE(0),            fhDispLam1HighE(0),
fhDispEtaE(0),                fhDispPhiE(0),
fhSumEtaE(0),                 fhSumPhiE(0),                 fhSumEtaPhiE(0),
fhDispEtaPhiDiffE(0),         fhSphericityE(0),
fhDispSumEtaDiffE(0),         fhDispSumPhiDiffE(0),

// MC histograms
fhMCPhotonELambda0NoOverlap(0),       fhMCPhotonELambda0TwoOverlap(0),      fhMCPhotonELambda0NOverlap(0),
// Embedding
fhEmbeddedSignalFractionEnergy(0),
fhEmbedPhotonELambda0FullSignal(0),   fhEmbedPhotonELambda0MostlySignal(0),
fhEmbedPhotonELambda0MostlyBkg(0),    fhEmbedPhotonELambda0FullBkg(0),
fhEmbedPi0ELambda0FullSignal(0),      fhEmbedPi0ELambda0MostlySignal(0),
fhEmbedPi0ELambda0MostlyBkg(0),       fhEmbedPi0ELambda0FullBkg(0),

fhTimePtPhotonNoCut(0),               fhTimePtPhotonSPD(0),
fhTimeNPileUpVertSPD(0),              fhTimeNPileUpVertTrack(0),
fhPtPhotonNPileUpSPDVtx(0),           fhPtPhotonNPileUpTrkVtx(0),
fhPtPhotonNPileUpSPDVtxTimeCut(0),    fhPtPhotonNPileUpTrkVtxTimeCut(0),
fhPtPhotonNPileUpSPDVtxTimeCut2(0),   fhPtPhotonNPileUpTrkVtxTimeCut2(0),

fhEClusterSM(0),                      fhEPhotonSM(0),
fhPtClusterSM(0),                     fhPtPhotonSM(0),
fhMCConversionVertex(0),              fhMCConversionVertexTRD(0),
fhLocalRegionClusterEtaPhi(0),
fhLocalRegionClusterEnergySum(0),     fhLocalRegionClusterMultiplicity(0),
fhLocalRegionClusterEnergySumPerCentrality(0),
fhLocalRegionClusterMultiplicityPerCentrality(0),
fhLocalRegionClusterEnergySumHijing(0),fhLocalRegionClusterMultiplicityHijing(0),
fhLocalRegionClusterEnergySumPerCentralityHijing(0),
fhLocalRegionClusterMultiplicityPerCentralityHijing(0) 
{
  for(Int_t i = 0; i < fgkNmcTypes; i++)
  {
    fhMCPt     [i] = 0;
    fhMCE      [i] = 0;
    fhMCPhi    [i] = 0;
    fhMCEta    [i] = 0;
    fhMCDeltaE [i] = 0;
    fhMCDeltaPt[i] = 0;
    fhMC2E     [i] = 0;
    fhMC2Pt    [i] = 0;
  }
  
  for(Int_t i = 0; i < fgkNmcPrimTypes; i++)
  {
    fhPtPrimMC [i] = 0;
    fhEPrimMC  [i] = 0;
    fhPhiPrimMC[i] = 0;
    fhEtaPrimMC[i] = 0;
    fhYPrimMC  [i] = 0;
    
    fhPtPrimMCAcc [i] = 0;
    fhEPrimMCAcc  [i] = 0;
    fhPhiPrimMCAcc[i] = 0;
    fhEtaPrimMCAcc[i] = 0;
    fhYPrimMCAcc  [i] = 0;
  }
  
  for(Int_t i = 0; i < 7; i++)
  {
    fhDispEtaDispPhi[i] = 0;
    fhLambda0DispPhi[i] = 0;
    fhLambda0DispEta[i] = 0;
    
    fhPtPhotonPileUp[i] = 0;
    fhClusterTimeDiffPhotonPileUp [i] = 0;
    
    for(Int_t j = 0; j < fgkNssTypes; j++)
    {
      fhMCDispEtaDispPhi[i][j] = 0;
      fhMCLambda0DispEta[i][j] = 0;
      fhMCLambda0DispPhi[i][j] = 0;
    }
  }
  
  for(Int_t i = 0; i < fgkNssTypes; i++)
  {
    fhMCELambda0    [i]                  = 0;
    fhMCPtLambda0   [i]                  = 0;
    fhMCELambda1    [i]                  = 0;
    fhMCEDispersion [i]                  = 0;
    fhMCNCellsE     [i]                  = 0;
    fhMCMaxCellDiffClusterE[i]           = 0;
    fhLambda0DispEta[i]                  = 0;
    fhLambda0DispPhi[i]                  = 0;
    
    fhMCLambda0vsClusterMaxCellDiffE0[i] = 0;
    fhMCLambda0vsClusterMaxCellDiffE2[i] = 0;
    fhMCLambda0vsClusterMaxCellDiffE6[i] = 0;
    fhMCNCellsvsClusterMaxCellDiffE0 [i] = 0;
    fhMCNCellsvsClusterMaxCellDiffE2 [i] = 0;
    fhMCNCellsvsClusterMaxCellDiffE6 [i] = 0;
    
    fhMCEDispEta       [i]               = 0;
    fhMCEDispPhi       [i]               = 0;
    fhMCESumEtaPhi     [i]               = 0;
    fhMCEDispEtaPhiDiff[i]               = 0;
    fhMCESphericity    [i]               = 0;
  }
  
  for(Int_t i = 0; i < 5; i++)
  {
    fhClusterCutsE [i] = 0;
    fhClusterCutsPt[i] = 0;
  }
  
  // Track matching residuals
  for(Int_t i = 0; i < 2; i++)
  {
    fhTrackMatchedDEta   [i] = 0;             fhTrackMatchedDPhi   [i] = 0;         fhTrackMatchedDEtaDPhi   [i] = 0;
    fhTrackMatchedDEtaNeg[i] = 0;             fhTrackMatchedDPhiNeg[i] = 0;         fhTrackMatchedDEtaDPhiNeg[i] = 0;
    fhTrackMatchedDEtaPos[i] = 0;             fhTrackMatchedDPhiPos[i] = 0;         fhTrackMatchedDEtaDPhiPos[i] = 0;
    fhTrackMatchedDEtaTRD[i] = 0;             fhTrackMatchedDPhiTRD[i] = 0;
    fhTrackMatchedDEtaMCOverlap[i] = 0;       fhTrackMatchedDPhiMCOverlap[i] = 0;
    fhTrackMatchedDEtaMCNoOverlap[i] = 0;     fhTrackMatchedDPhiMCNoOverlap[i] = 0;
    fhTrackMatchedDEtaMCConversion[i] = 0;    fhTrackMatchedDPhiMCConversion[i] = 0;
    fhTrackMatchedMCParticle[i] = 0;          fhTrackMatchedMCParticle[i] = 0;
    fhdEdx[i] = 0;                            fhEOverP[i] = 0;
    fhEOverPTRD[i] = 0;
  }
  
  for(Int_t i = 0; i < 6; i++) 
  {
    fhMCConversionLambda0Rcut   [i] = 0;            
    fhMCConversionLambda0RcutTRD[i] = 0;          
    fhMCConversionLambda1Rcut   [i] = 0;            
    fhMCConversionLambda1RcutTRD[i] = 0;            
  }
  
  for(Int_t ieta = 0; ieta < 4; ieta++) 
  {  
    for(Int_t iphi = 0; iphi < 3; iphi++) 
    {
//      fhLam0EMCALRegion   [ieta][iphi] = 0;
//      fhLam0EMCALRegionTRD[ieta][iphi] = 0;
//      
//      for(Int_t i = 0; i < 6; i++) 
//      {
//        fhLam0EMCALRegionMCConvRcut   [ieta][iphi][i] = 0;
//        fhLam0EMCALRegionTRDMCConvRcut[ieta][iphi][i] = 0;
//      }
      for(Int_t ism =0; ism < 20; ism++)
      {
        fhLam0EMCALRegionPerSM[ieta][iphi][ism] = 0; 
        fhLam1EMCALRegionPerSM[ieta][iphi][ism] = 0; 
      }
    }
  }

  for(Int_t il0 = 0; il0 < 2; il0++)
  {
    for(Int_t i = 0; i < 7; i++) 
    {
      fhEtaPhiLam0BinPtBin          [il0][i] = 0 ;
      fhEtaPhiLargeTimeInClusterCell[il0][i] = 0 ;    

      fhColRowLam0BinPtBin          [il0][i] = 0 ;              
      fhColRowLam0BinPtBinWeighted  [il0][i] = 0 ;  
      fhColRowLam0BinPtBinLargeTime [il0][i] = 0 ;  
      fhCellClusterIndexEAndTime    [il0][i] = 0 ; 
      fhCellClusterEAndTime         [il0][i] = 0 ;
      fhCellClusterEFracAndTime     [il0][i] = 0 ;
    }
    
    for(Int_t ism =0; ism < 20; ism++)
    {
      fhLam1Lam0BinPerSM                [il0][ism] = 0; 
      fhTimeLam0BinPerSM                [il0][ism] = 0;   
      fhTimeLam0BinPerSMWeighted        [il0][ism] = 0;   
      fhDTimeLam0BinPerSM               [il0][ism] = 0;   
      fhDTimeLam0BinPerSMWeighted       [il0][ism] = 0;   
      fhCellClusterEFracLam0BinPerSM    [il0][ism] = 0;
//      fhCellClusterEFracLam0BinPerSMWeighted[il0][ism] = 0;
      fhCellClusterELam0BinPerSM        [il0][ism] = 0;
      fhCellClusterELam0BinPerSMWeighted[il0][ism] = 0;
      
      fhCellClusterEFracLam0BinPerSMLargeTime [il0][ism] = 0;
      fhCellClusterEFracLam0BinPerSMLargeTimeTotal[il0][ism] = 0;
      fhCellClusterELam0BinPerSMLargeTime     [il0][ism] = 0; 
      fhCellClusterIndexELam0BinPerSMLargeTime[il0][ism] = 0;
      fhNCellsWithLargeTimeInCluster          [il0][ism] = 0;
    }
  }
  
//  for(Int_t il0 = 0; il0 < 2; il0++)
//  {
//    for(Int_t i = 0; i < 7; i++) 
//    {
//      fhEtaPhiLam0BinPtBinSMShared      [il0][i] = 0 ;
//      fhColRowLam0BinPtBinSMShared      [il0][i] = 0 ;              
//    }
//    
//    for(Int_t ism =0; ism < 12; ism++)
//    {
//      fhTimeLam0BinPerSMShared[il0][ism] = 0;   
//      fhLam0PerSMShared            [ism] = 0;
//      fhLam1PerSMShared            [ism] = 0;
//    }
//  }
  
  for(Int_t ism = 0; ism < 20; ism++)
  {
    fhLam0PerSM                      [ism] = 0;
    fhLam1PerSM                      [ism] = 0;
    fhLam0PerSMLargeTimeInClusterCell[ism] = 0;
    fhLam1PerSMLargeTimeInClusterCell[ism] = 0;
//  fhLam0PerSMSPDPileUp             [ism] = 0;
//  fhLam1PerSMSPDPileUp             [ism] = 0; 
  }
  
  for(Int_t ilarge = 0; ilarge < 5; ilarge++)
  {
    fhLam0PerNLargeTimeInClusterCell[ilarge] = 0;
    fhLam1PerNLargeTimeInClusterCell[ilarge] = 0;
  }
  
  for(Int_t i = 0; i < 14; i++)
  {
    fhEBinClusterEtaPhi[i] = 0 ;
    fhEBinClusterColRow[i] = 0 ;    
    fhEBinClusterEtaPhiPID[i] = 0 ;
    fhEBinClusterColRowPID[i] = 0 ;
  }
  
  for(Int_t igen = 0; igen < 10; igen++)
  { 
    for(Int_t ip = 0; ip < fgkNGenTypes; ip++)
    {
      fhMergeGeneratorCluster                 [igen][ip] = 0; 
      fhMergeGeneratorClusterNotHijingBkg     [igen][ip] = 0;        
      fhMergeGeneratorClusterHijingAndOtherBkg[igen][ip] = 0;
      fhMergeGeneratorClusterHijingBkg        [igen][ip] = 0;  
      fhCleanGeneratorCluster                 [igen][ip] = 0;
      
      fhMergeGeneratorClusterEPrimRecoRatio                 [igen][ip] = 0;
      fhMergeGeneratorClusterNotHijingBkgEPrimRecoRatio     [igen][ip] = 0;        
      fhMergeGeneratorClusterHijingAndOtherBkgEPrimRecoRatio[igen][ip] = 0;
      fhMergeGeneratorClusterHijingBkgEPrimRecoRatio        [igen][ip] = 0;  
      fhCleanGeneratorClusterEPrimRecoRatio                 [igen][ip] = 0;  
      
      fhMergeGeneratorClusterEPrimRecoDiff                 [igen][ip] = 0; 
      fhMergeGeneratorClusterNotHijingBkgEPrimRecoDiff     [igen][ip] = 0;        
      fhMergeGeneratorClusterHijingAndOtherBkgEPrimRecoDiff[igen][ip] = 0;
      fhMergeGeneratorClusterHijingBkgEPrimRecoDiff        [igen][ip] = 0;  
      fhCleanGeneratorClusterEPrimRecoDiff                 [igen][ip] = 0;  
    }
  }
  
  // Initialize parameters
  InitParameters();
}

//_____________________________________________________________________
/// Check the cluster activity, total energy and multiplicity around 
/// a selected cluster on EMCal acceptance within a cone of R=0.2
///
/// \param icalo: cluster index in array
/// \param en  : selected cluster energy
/// \param eta : cluster pseudo-rapidity
/// \param phi : cluster azimuthal angle (0-360 deg)
/// \param clusterList: clusters array
//__________________________________________________________________________
void AliAnaPhoton::ActivityNearCluster(Int_t icalo, Float_t en, Float_t eta, 
                                       Float_t phi, TObjArray *clusterList)
{
  Float_t radius = 0.2; // Hardcoded unless real use.
  
  // Accept cluster on EMCal limits - radius
  // CAREFUL if used for Run2 with DCal.
  if(phi < 3.15 - radius && phi > 1.4 + radius && TMath::Abs(eta) < 0.7-radius)
  {
    fhLocalRegionClusterEtaPhi ->Fill(eta,phi, GetEventWeight());
    
    Float_t sumE   = 0;
    Int_t   sumM   = 0;
    Float_t sumEHi = 0;
    Int_t   sumMHi = 0;
    for(Int_t icalo2 = 0; icalo2 < clusterList->GetEntriesFast(); icalo2++)
    {
      if ( icalo2 == icalo ) continue;
      
      AliVCluster * calo2 =  (AliVCluster*) (clusterList->At(icalo2));
      
      // Select clusters in a radius of R=0.2
      calo2->GetMomentum(fMomentum2,GetVertex(0)) ;
      
      Float_t dEta = eta-fMomentum2.Eta();
      Float_t dPhi = phi-GetPhi(fMomentum2.Phi());
      
      if(TMath::Abs(dPhi) >= TMath::Pi())
        dPhi = TMath::TwoPi()-TMath::Abs(dPhi);
      
      if (TMath::Sqrt( dEta*dEta + dPhi*dPhi ) > 0.2) continue;
      
      sumM++;
      sumE += calo2->E();
      
      if( IsDataMC() && IsStudyClusterOverlapsPerGeneratorOn() && calo2->GetNLabels() > 0)
      {
        TString genName;
        (GetReader()->GetMC())->GetCocktailGenerator(calo2->GetLabel(),genName);
        
        if(genName.Contains("ijing"))
        {
          sumMHi++;
          sumEHi += calo2->E();
        }
      }
    }
    
    fhLocalRegionClusterEnergySum   ->Fill(en,sumE,GetEventWeight());
    fhLocalRegionClusterMultiplicity->Fill(en,sumM,GetEventWeight());
    
    if(IsHighMultiplicityAnalysisOn())
    {
      fhLocalRegionClusterEnergySumPerCentrality   ->Fill(GetEventCentrality(),sumE,GetEventWeight());
      fhLocalRegionClusterMultiplicityPerCentrality->Fill(GetEventCentrality(),sumM,GetEventWeight());
    }
    
    if( IsDataMC() && IsStudyClusterOverlapsPerGeneratorOn())
    {
      fhLocalRegionClusterEnergySumHijing   ->Fill(en,sumEHi,GetEventWeight());
      fhLocalRegionClusterMultiplicityHijing->Fill(en,sumMHi,GetEventWeight());
      
      if(IsHighMultiplicityAnalysisOn())
      {
        fhLocalRegionClusterEnergySumPerCentralityHijing   ->Fill(GetEventCentrality(),sumEHi,GetEventWeight());
        fhLocalRegionClusterMultiplicityPerCentralityHijing->Fill(GetEventCentrality(),sumMHi,GetEventWeight());
      }
    }
  }
}

//_____________________________________________________________________
/// Check the particle overlaps into a cluster when originated by 
/// different generators
///
/// \param calo: cluster pointer
/// \param mctag  : mc tag label, of the originating particle
//__________________________________________________________________________
void AliAnaPhoton::CocktailGeneratorsClusterOverlaps(AliVCluster* calo, Int_t mctag)
{
  //
  // Check the generators inside the cluster
  TString genName = "", genNameBkg = "";
  Int_t genBkgTag = GetCocktailGeneratorBackgroundTag(calo, genName, genNameBkg);
  if     (genBkgTag == -1) return;
  else if(genBkgTag  >  3) printf("Bkg generator tag larger than 3\n");
  
  //
  // Get primary particle info of main particle contributing to the cluster
  Float_t eprim   = 0;
  //Float_t ptprim  = 0;
  Bool_t ok = kFALSE;
  Int_t pdg = 0, status = 0, momLabel = -1;
  
  //fPrimaryMom = GetMCAnalysisUtils()->GetMother(label,GetReader(),ok);
  fPrimaryMom = GetMCAnalysisUtils()->GetMother(calo->GetLabel(),GetReader(), pdg, status, ok, momLabel);     
  
  if(ok)
  {
    eprim   = fPrimaryMom.Energy();
    //ptprim  = fPrimaryMom.Pt();
  }
  
  //
  Float_t en = calo->E();

  if ( eprim < 0.1 || en < 0.5 ) return;
  
  //
  Int_t partType = -1;
  if     ( GetMCAnalysisUtils()->CheckTagBit(mctag,AliMCAnalysisUtils::kMCPi0)      ) partType = kmcGenPi0Merged;
  else if( GetMCAnalysisUtils()->CheckTagBit(mctag,AliMCAnalysisUtils::kMCPi0Decay) ) partType = kmcGenPi0Decay;
  else if( GetMCAnalysisUtils()->CheckTagBit(mctag,AliMCAnalysisUtils::kMCEtaDecay) ) partType = kmcGenEtaDecay;
  else if( GetMCAnalysisUtils()->CheckTagBit(mctag,AliMCAnalysisUtils::kMCPhoton)   ) partType = kmcGenPhoton;
  else if( GetMCAnalysisUtils()->CheckTagBit(mctag,AliMCAnalysisUtils::kMCElectron) ) partType = kmcGenElectron;
  else                                                                                partType = kmcGenOther;

  Int_t genType = GetNCocktailGenNamesToCheck()-1;
  for(Int_t igen = 1; igen < GetNCocktailGenNamesToCheck(); igen++)
  {
    if ( genName.Contains(GetCocktailGenNameToCheck(igen)) )
    {
      genType = igen;
      break;
    }
  }
  
  Float_t ratio = en / eprim;
  Float_t diff  = en - eprim;
  
  if ( genBkgTag > 0 ) 
  {
    fhMergeGeneratorCluster[0]      [0]        ->Fill(en,GetEventWeight());
    fhMergeGeneratorCluster[0]      [partType] ->Fill(en,GetEventWeight());
    fhMergeGeneratorCluster[genType][0]        ->Fill(en,GetEventWeight());
    fhMergeGeneratorCluster[genType][partType] ->Fill(en,GetEventWeight());

    fhMergeGeneratorClusterEPrimRecoRatio[0]      [0]        ->Fill(en,ratio,GetEventWeight());
    fhMergeGeneratorClusterEPrimRecoRatio[0]      [partType] ->Fill(en,ratio,GetEventWeight());
    fhMergeGeneratorClusterEPrimRecoRatio[genType][0]        ->Fill(en,ratio,GetEventWeight());
    fhMergeGeneratorClusterEPrimRecoRatio[genType][partType] ->Fill(en,ratio,GetEventWeight());
    
    fhMergeGeneratorClusterEPrimRecoDiff[0]      [0]        ->Fill(en,diff,GetEventWeight());
    fhMergeGeneratorClusterEPrimRecoDiff[0]      [partType] ->Fill(en,diff,GetEventWeight());
    fhMergeGeneratorClusterEPrimRecoDiff[genType][0]        ->Fill(en,diff,GetEventWeight());
    fhMergeGeneratorClusterEPrimRecoDiff[genType][partType] ->Fill(en,diff,GetEventWeight());
    
    if(!genName.Contains("ijing"))
    {
      if(genBkgTag == 2) 
      {
        fhMergeGeneratorClusterNotHijingBkg[0]      [0]        ->Fill(en,GetEventWeight());
        fhMergeGeneratorClusterNotHijingBkg[0]      [partType] ->Fill(en,GetEventWeight());
        fhMergeGeneratorClusterNotHijingBkg[genType][0]        ->Fill(en,GetEventWeight());
        fhMergeGeneratorClusterNotHijingBkg[genType][partType] ->Fill(en,GetEventWeight());
    
        fhMergeGeneratorClusterNotHijingBkgEPrimRecoRatio[0]      [0]        ->Fill(en,ratio,GetEventWeight());
        fhMergeGeneratorClusterNotHijingBkgEPrimRecoRatio[0]      [partType] ->Fill(en,ratio,GetEventWeight());
        fhMergeGeneratorClusterNotHijingBkgEPrimRecoRatio[genType][0]        ->Fill(en,ratio,GetEventWeight());
        fhMergeGeneratorClusterNotHijingBkgEPrimRecoRatio[genType][partType] ->Fill(en,ratio,GetEventWeight());
        
        fhMergeGeneratorClusterNotHijingBkgEPrimRecoDiff[0]      [0]        ->Fill(en,diff,GetEventWeight());
        fhMergeGeneratorClusterNotHijingBkgEPrimRecoDiff[0]      [partType] ->Fill(en,diff,GetEventWeight());
        fhMergeGeneratorClusterNotHijingBkgEPrimRecoDiff[genType][0]        ->Fill(en,diff,GetEventWeight());
        fhMergeGeneratorClusterNotHijingBkgEPrimRecoDiff[genType][partType] ->Fill(en,diff,GetEventWeight());
      }
      
      if(genBkgTag == 1)
      {
        fhMergeGeneratorClusterHijingBkg[0]      [0]        ->Fill(en,GetEventWeight());
        fhMergeGeneratorClusterHijingBkg[0]      [partType] ->Fill(en,GetEventWeight());
        fhMergeGeneratorClusterHijingBkg[genType][0]        ->Fill(en,GetEventWeight());
        fhMergeGeneratorClusterHijingBkg[genType][partType] ->Fill(en,GetEventWeight());
        
        fhMergeGeneratorClusterHijingBkgEPrimRecoRatio[0]      [0]        ->Fill(en,ratio,GetEventWeight());
        fhMergeGeneratorClusterHijingBkgEPrimRecoRatio[0]      [partType] ->Fill(en,ratio,GetEventWeight());
        fhMergeGeneratorClusterHijingBkgEPrimRecoRatio[genType][0]        ->Fill(en,ratio,GetEventWeight());
        fhMergeGeneratorClusterHijingBkgEPrimRecoRatio[genType][partType] ->Fill(en,ratio,GetEventWeight());
        
        fhMergeGeneratorClusterHijingBkgEPrimRecoDiff[0]      [0]        ->Fill(en,diff,GetEventWeight());
        fhMergeGeneratorClusterHijingBkgEPrimRecoDiff[0]      [partType] ->Fill(en,diff,GetEventWeight());
        fhMergeGeneratorClusterHijingBkgEPrimRecoDiff[genType][0]        ->Fill(en,diff,GetEventWeight());
        fhMergeGeneratorClusterHijingBkgEPrimRecoDiff[genType][partType] ->Fill(en,diff,GetEventWeight());
      }
      
      if ( genBkgTag == 3 )
      {      
        fhMergeGeneratorClusterHijingAndOtherBkg[0]      [0]        ->Fill(en,GetEventWeight());
        fhMergeGeneratorClusterHijingAndOtherBkg[0]      [partType] ->Fill(en,GetEventWeight());
        fhMergeGeneratorClusterHijingAndOtherBkg[genType][0]        ->Fill(en,GetEventWeight());
        fhMergeGeneratorClusterHijingAndOtherBkg[genType][partType] ->Fill(en,GetEventWeight());
        
        fhMergeGeneratorClusterHijingAndOtherBkgEPrimRecoRatio[0]      [0]        ->Fill(en,ratio,GetEventWeight());
        fhMergeGeneratorClusterHijingAndOtherBkgEPrimRecoRatio[0]      [partType] ->Fill(en,ratio,GetEventWeight());
        fhMergeGeneratorClusterHijingAndOtherBkgEPrimRecoRatio[genType][0]        ->Fill(en,ratio,GetEventWeight());
        fhMergeGeneratorClusterHijingAndOtherBkgEPrimRecoRatio[genType][partType] ->Fill(en,ratio,GetEventWeight());
        
        fhMergeGeneratorClusterHijingAndOtherBkgEPrimRecoDiff[0]      [0]        ->Fill(en,diff,GetEventWeight());
        fhMergeGeneratorClusterHijingAndOtherBkgEPrimRecoDiff[0]      [partType] ->Fill(en,diff,GetEventWeight());
        fhMergeGeneratorClusterHijingAndOtherBkgEPrimRecoDiff[genType][0]        ->Fill(en,diff,GetEventWeight());
        fhMergeGeneratorClusterHijingAndOtherBkgEPrimRecoDiff[genType][partType] ->Fill(en,diff,GetEventWeight());
      }
    }
  }
  else    
  {
    fhCleanGeneratorCluster[0]      [0]        ->Fill(en,GetEventWeight());
    fhCleanGeneratorCluster[0]      [partType] ->Fill(en,GetEventWeight());
    fhCleanGeneratorCluster[genType][0]        ->Fill(en,GetEventWeight());
    fhCleanGeneratorCluster[genType][partType] ->Fill(en,GetEventWeight());
    
    fhCleanGeneratorClusterEPrimRecoRatio[0]      [0]        ->Fill(en,ratio,GetEventWeight());
    fhCleanGeneratorClusterEPrimRecoRatio[0]      [partType] ->Fill(en,ratio,GetEventWeight());
    fhCleanGeneratorClusterEPrimRecoRatio[genType][0]        ->Fill(en,ratio,GetEventWeight());
    fhCleanGeneratorClusterEPrimRecoRatio[genType][partType] ->Fill(en,ratio,GetEventWeight());
    
    fhCleanGeneratorClusterEPrimRecoDiff[0]      [0]        ->Fill(en,diff,GetEventWeight());
    fhCleanGeneratorClusterEPrimRecoDiff[0]      [partType] ->Fill(en,diff,GetEventWeight());
    fhCleanGeneratorClusterEPrimRecoDiff[genType][0]        ->Fill(en,diff,GetEventWeight());
    fhCleanGeneratorClusterEPrimRecoDiff[genType][partType] ->Fill(en,diff,GetEventWeight());
  }
}


//_____________________________________________________________________
/// Select calorimeter clusters if they pass different cuts:
///  * Energy (if stricter cut than in AliCaloTrackReader)
///  * Time (but usually it is already done in AliCaloTrackReader)
///  * Number of cells in cluster
///  * Number of local maxima in cluster
///  * Fiducial cut, eta-phi acceptance cut via AliFiducialCut
///  * Charged clusters are rejected (if requested)
///  * Reject clusters close to a bad channel
///
/// Fill for each of the cuts a 1 dimensional histogram with either the energy
/// or the transverse momentum of the cluster. Also track-matching control histograms
/// can be filled with residuals of the matching.
/// \return kTRUE of cluster is accepted
/// \param calo: cluster pointer.
/// \param nMaxima: number of local maxima.
//_____________________________________________________________________
Bool_t  AliAnaPhoton::ClusterSelected(AliVCluster* calo, Int_t nMaxima)
{
  Float_t ptcluster  = fMomentum.Pt();
  Float_t ecluster   = fMomentum.E();
  Float_t etacluster = fMomentum.Eta();
  Float_t phicluster = fMomentum.Phi();

  if(phicluster < 0) phicluster+=TMath::TwoPi();
  
  Bool_t matched = IsTrackMatched(calo,GetReader()->GetInputEvent());
  
  AliDebug(2,Form("Current Event %d; Before selection : E %2.2f, pT %2.2f, phi %2.2f, eta %2.2f",
           GetReader()->GetEventNumber(),
           ecluster,ptcluster, phicluster*TMath::RadToDeg(),etacluster));
    
  fhClusterCutsE [1]->Fill( ecluster, GetEventWeight());
  fhClusterCutsPt[1]->Fill(ptcluster, GetEventWeight());
  
  if(ecluster > 0.5) fhEtaPhi->Fill(etacluster, phicluster, GetEventWeight());
  
  Int_t   nSM  = GetModuleNumber(calo);
  if(nSM < GetCaloUtils()->GetNumberOfSuperModulesUsed() && nSM >=0)
  {
    fhEClusterSM ->Fill(ecluster , nSM, GetEventWeight());
    fhPtClusterSM->Fill(ptcluster, nSM, GetEventWeight());
  }
  
  //.......................................
  //If too small or big energy, skip it
  if(ecluster < GetMinEnergy() || ecluster > GetMaxEnergy() ) return kFALSE ;
  
  AliDebug(2,Form("\t Cluster %d Pass E Cut",calo->GetID()));
  
  fhClusterCutsE [2]->Fill( ecluster, GetEventWeight());
  fhClusterCutsPt[2]->Fill(ptcluster, GetEventWeight());
  
  //.......................................
  // TOF cut, BE CAREFUL WITH THIS CUT
  Double_t tof = calo->GetTOF()*1e9;
  if(tof > 400) tof-=fConstantTimeShift; // only for MC, rest shift = 0
  
  if(tof < fTimeCutMin || tof > fTimeCutMax) return kFALSE;
  
  AliDebug(2,Form("\t Cluster %d Pass Time Cut",calo->GetID()));
  
  fhClusterCutsE [3]->Fill( ecluster, GetEventWeight());
  fhClusterCutsPt[3]->Fill(ptcluster, GetEventWeight());
  
  //.......................................
  if(calo->GetNCells() <= fNCellsCut && GetReader()->GetDataType() != AliCaloTrackReader::kMC) return kFALSE;
  
  AliDebug(2,Form("\t Cluster %d Pass NCell Cut",calo->GetID()));
  
  fhClusterCutsE [4]->Fill( ecluster, GetEventWeight());
  fhClusterCutsPt[4]->Fill(ptcluster, GetEventWeight());
  
  if(nMaxima < fNLMCutMin || nMaxima > fNLMCutMax) return kFALSE ;
  AliDebug(2,Form("\t Cluster %d pass NLM %d of out of range",calo->GetID(), nMaxima));
  
  fhClusterCutsE [5]->Fill( ecluster, GetEventWeight());
  fhClusterCutsPt[5]->Fill(ptcluster, GetEventWeight());
  
  //.......................................
  //Check acceptance selection
  if(IsFiducialCutOn())
  {
    Bool_t in = GetFiducialCut()->IsInFiducialCut(fMomentum.Eta(),fMomentum.Phi(),GetCalorimeter()) ;
    if(! in ) return kFALSE ;
  }
  
  AliDebug(2,Form("\t Fiducial cut passed"));
  
  fhClusterCutsE [6]->Fill( ecluster, GetEventWeight());
  fhClusterCutsPt[6]->Fill(ptcluster, GetEventWeight());
  
  //.......................................
  //Skip matched clusters with tracks
  
  // Fill matching residual histograms before PID cuts
  if(fFillTMHisto) FillTrackMatchingResidualHistograms(calo,0);
  
  if(fRejectTrackMatch)
  {
    if(matched)
    {
      AliDebug(2,"\t Reject track-matched clusters");
      return kFALSE ;
    }
    else
      AliDebug(2,"\t Track-matching cut passed");
  }// reject matched clusters
  
  fhClusterCutsE [7]->Fill( ecluster, GetEventWeight());
  fhClusterCutsPt[7]->Fill(ptcluster, GetEventWeight());
  
  //.......................................
  //Check Distance to Bad channel, set bit.
  Double_t distBad=calo->GetDistanceToBadChannel() ; //Distance to bad channel
  if(distBad < 0.) distBad=9999. ; //workout strange convension dist = -1. ;
  if(distBad < fMinDist)
  {//In bad channel (PHOS cristal size 2.2x2.2 cm), EMCAL ( cell units )
    return kFALSE ;
  }
  else AliDebug(2,Form("\t Bad channel cut passed %4.2f > %2.2f",distBad, fMinDist));
  
  fhClusterCutsE [8]->Fill( ecluster, GetEventWeight());
  fhClusterCutsPt[8]->Fill(ptcluster, GetEventWeight());
  
  AliDebug(1,Form("Current Event %d; After  selection : E %2.2f, pT %2.2f, phi %2.2f, eta %2.2f",
           GetReader()->GetEventNumber(),
           ecluster, ptcluster,fMomentum.Phi()*TMath::RadToDeg(),fMomentum.Eta()));
  
  //All checks passed, cluster selected
  return kTRUE;
  
}

//___________________________________________
/// Fill primary generated MC particles acceptance
/// histograms if MC data is available. Pure generated kinematics analysis.
/// Fill histograms for different photon particle types,
/// index defined in enum mcPTypes.
//___________________________________________
void AliAnaPhoton::FillAcceptanceHistograms()
{
  Double_t photonY   = -100 ;
  Double_t photonE   = -1 ;
  Double_t photonPt  = -1 ;
  Double_t photonPhi =  100 ;
  Double_t photonEta = -1 ;
  
  Int_t    pdg       =  0 ;
  Int_t    tag       =  0 ;
  Int_t    status    =  0 ;
  Int_t    mcIndex   =  0 ;
  Int_t    nprim     =  0 ;
  Bool_t   inacceptance = kFALSE ;
  
  TParticle        * primStack = 0;
  AliAODMCParticle * primAOD   = 0;
  
  // Get the ESD MC particles container
  AliStack * stack = 0;
  if( GetReader()->ReadStack() )
  {
    stack = GetMCStack();
    if( !stack )
    {
      AliFatal("Stack not available, is the MC handler called? STOP");
      return;
    }
    nprim = stack->GetNtrack();
  }
  
  // Get the AOD MC particles container
  TClonesArray * mcparticles = 0;
  if( GetReader()->ReadAODMCParticles() )
  {
    mcparticles = GetReader()->GetAODMCParticles();
    if( !mcparticles )
    {
      AliFatal("Standard MCParticles not available!");
      return;
    }
    nprim = mcparticles->GetEntriesFast();
  }
  
  for(Int_t i=0 ; i < nprim; i++)
  {
    if ( !GetReader()->AcceptParticleMCLabel( i ) ) continue ;
    
    if(GetReader()->ReadStack())
    {
      primStack = stack->Particle(i) ;
      if(!primStack)
      {
        AliWarning("ESD primaries pointer not available!!");
        continue;
      }
      
      pdg    = primStack->GetPdgCode();
      status = primStack->GetStatusCode();
      
      // Protection against floating point exception
      if ( primStack->Energy() == TMath::Abs(primStack->Pz()) || 
          (primStack->Energy() - primStack->Pz()) < 1e-3      ||
          (primStack->Energy() + primStack->Pz()) < 0           )  continue ; 
      
      //printf("i %d, %s %d  %s %d \n",i, stack->Particle(i)->GetName(), stack->Particle(i)->GetPdgCode(),
      //       prim->GetName(), prim->GetPdgCode());
      
      // Photon kinematics
      primStack->Momentum(fMomentum);
      
      photonY = 0.5*TMath::Log((primStack->Energy()+primStack->Pz())/(primStack->Energy()-primStack->Pz())) ;
    }
    else
    {
      primAOD = (AliAODMCParticle *) mcparticles->At(i);
      if(!primAOD)
      {
        AliWarning("AOD primaries pointer not available!!");
        continue;
      }
      
      pdg    = primAOD->GetPdgCode();
      status = primAOD->GetStatus();
      
      // Protection against floating point exception
      if ( primAOD->E() == TMath::Abs(primAOD->Pz()) || 
          (primAOD->E() - primAOD->Pz()) < 1e-3      || 
          (primAOD->E() + primAOD->Pz()) < 0           )  continue ; 
      
      // Photon kinematics
      fMomentum.SetPxPyPzE(primAOD->Px(),primAOD->Py(),primAOD->Pz(),primAOD->E());
      
      photonY = 0.5*TMath::Log((primAOD->E()+primAOD->Pz())/(primAOD->E()-primAOD->Pz())) ;
    }

    // Select only photons in the final state
    if(pdg != 22 ) continue ;
    
    // If too small or too large pt, skip, same cut as for data analysis
    photonPt  = fMomentum.Pt () ;
    
    if(photonPt < GetMinPt() || photonPt > GetMaxPt() ) continue ;
    
    photonE   = fMomentum.E  () ;
    photonEta = fMomentum.Eta() ;
    photonPhi = fMomentum.Phi() ;
    
    if(photonPhi < 0) photonPhi+=TMath::TwoPi();
    
    // Check if photons hit desired acceptance
    inacceptance = kTRUE;
    
    // Check same fidutial borders as in data analysis on top of real acceptance if real was requested.
    if( IsFiducialCutOn() && !GetFiducialCut()->IsInFiducialCut(fMomentum.Eta(),fMomentum.Phi(),GetCalorimeter())) inacceptance = kFALSE ;
    
    // Check if photons hit the Calorimeter acceptance
    if(IsRealCaloAcceptanceOn()) // defined on base class
    {
      if(GetReader()->ReadStack()          &&
         !GetCaloUtils()->IsMCParticleInCalorimeterAcceptance(GetCalorimeter(), primStack)) inacceptance = kFALSE ;
      if(GetReader()->ReadAODMCParticles() &&
         !GetCaloUtils()->IsMCParticleInCalorimeterAcceptance(GetCalorimeter(), primAOD  )) inacceptance = kFALSE ;
    }
    
    // Get tag of this particle photon from fragmentation, decay, prompt ...
    // Set the origin of the photon.
    tag = GetMCAnalysisUtils()->CheckOrigin(i,GetReader(),GetCalorimeter());
    
    if(!GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCPhoton))
    {
      // A conversion photon from a hadron, skip this kind of photon
      // printf("AliAnaPhoton::FillAcceptanceHistograms() - not a photon, weird!\n ");
      // GetMCAnalysisUtils()->PrintMCTag(tag);
      
      continue;
    }
    
    // Consider only final state particles, but this depends on generator,
    // status 1 is the usual one, in case of not being ok, leave the possibility
    // to not consider this.
    if(status > 1) continue ; // Avoid "partonic" photons
    
    Bool_t takeIt  = kFALSE ;
    if(status == 1 && GetMCAnalysisUtils()->GetMCGenerator() != AliMCAnalysisUtils::kBoxLike ) takeIt = kTRUE ;

    if     (GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCConversion)) continue;
    
    // Origin of photon
    if     (GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCPrompt))
    {
      mcIndex = kmcPPrompt;
    }
    else if(GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCFragmentation))
    {
      mcIndex = kmcPFragmentation ;
    }
    else if(GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCISR))
    {
      mcIndex = kmcPISR;
    }
    else if(GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCPi0Decay))
    {
      mcIndex = kmcPPi0Decay;
    }
    else if(GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCEtaDecay))
    {
      mcIndex = kmcPEtaDecay;
    }
    else if(GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCOtherDecay))
    {
      mcIndex = kmcPOtherDecay;
    }
    else
    {
      // Other decay but from non final state particle
      mcIndex = kmcPOtherDecay;
    } // Other origin
    
    if(!takeIt &&  (mcIndex == kmcPPi0Decay || mcIndex == kmcPOtherDecay)) takeIt = kTRUE ;

    if(!takeIt) continue ;
      
    // Fill histograms for all photons
    fhYPrimMC[kmcPPhoton]->Fill(photonPt, photonY, GetEventWeight()) ;
    if(TMath::Abs(photonY) < 1.0)
    {
      fhEPrimMC  [kmcPPhoton]->Fill(photonE , GetEventWeight()) ;
      fhPtPrimMC [kmcPPhoton]->Fill(photonPt, GetEventWeight()) ;
      fhPhiPrimMC[kmcPPhoton]->Fill(photonE , photonPhi, GetEventWeight()) ;
      fhEtaPrimMC[kmcPPhoton]->Fill(photonE , photonEta, GetEventWeight()) ;
    }
    
    if(inacceptance)
    {
      fhEPrimMCAcc  [kmcPPhoton]->Fill(photonE , GetEventWeight()) ;
      fhPtPrimMCAcc [kmcPPhoton]->Fill(photonPt, GetEventWeight()) ;
      fhPhiPrimMCAcc[kmcPPhoton]->Fill(photonE , photonPhi, GetEventWeight()) ;
      fhEtaPrimMCAcc[kmcPPhoton]->Fill(photonE , photonEta, GetEventWeight()) ;
      fhYPrimMCAcc  [kmcPPhoton]->Fill(photonE , photonY  , GetEventWeight()) ;
    } // Accepted
    
    // Fill histograms for photons origin
    if(mcIndex < fNPrimaryHistograms)
    {
      fhYPrimMC[mcIndex]->Fill(photonPt, photonY, GetEventWeight()) ;
      if(TMath::Abs(photonY) < 1.0)
      {
        fhEPrimMC  [mcIndex]->Fill(photonE , GetEventWeight()) ;
        fhPtPrimMC [mcIndex]->Fill(photonPt, GetEventWeight()) ;
        fhPhiPrimMC[mcIndex]->Fill(photonE , photonPhi, GetEventWeight()) ;
        fhEtaPrimMC[mcIndex]->Fill(photonE , photonEta, GetEventWeight()) ;
      }
      
      if(inacceptance)
      {
        fhEPrimMCAcc  [mcIndex]->Fill(photonE , GetEventWeight()) ;
        fhPtPrimMCAcc [mcIndex]->Fill(photonPt, GetEventWeight()) ;
        fhPhiPrimMCAcc[mcIndex]->Fill(photonE , photonPhi, GetEventWeight()) ;
        fhEtaPrimMCAcc[mcIndex]->Fill(photonE , photonEta, GetEventWeight()) ;
        fhYPrimMCAcc  [mcIndex]->Fill(photonE , photonY  , GetEventWeight()) ;
      } // Accepted
    }
  } // loop on primaries
}

//________________________________________________________________________________
/// Fill some histograms to understand effect of pile-up.
//________________________________________________________________________________
void AliAnaPhoton::FillPileUpHistograms(AliVCluster* cluster, AliVCaloCells *cells,
                                        Int_t absIdMax)
{
  Float_t pt   = fMomentum.Pt();
  Float_t time = cluster->GetTOF()*1.e9;
  if ( time > 400 ) time-=fConstantTimeShift; // Only for MC, rest shift = 0
  
  AliVEvent * event = GetReader()->GetInputEvent();
  
  if(GetReader()->IsPileUpFromSPD())               fhPtPhotonPileUp[0]->Fill(pt, GetEventWeight()) ;
  if(GetReader()->IsPileUpFromEMCal())             fhPtPhotonPileUp[1]->Fill(pt, GetEventWeight()) ;
  if(GetReader()->IsPileUpFromSPDOrEMCal())        fhPtPhotonPileUp[2]->Fill(pt, GetEventWeight()) ;
  if(GetReader()->IsPileUpFromSPDAndEMCal())       fhPtPhotonPileUp[3]->Fill(pt, GetEventWeight()) ;
  if(GetReader()->IsPileUpFromSPDAndNotEMCal())    fhPtPhotonPileUp[4]->Fill(pt, GetEventWeight()) ;
  if(GetReader()->IsPileUpFromEMCalAndNotSPD())    fhPtPhotonPileUp[5]->Fill(pt, GetEventWeight()) ;
  if(GetReader()->IsPileUpFromNotSPDAndNotEMCal()) fhPtPhotonPileUp[6]->Fill(pt, GetEventWeight()) ;
  
  fhTimePtPhotonNoCut->Fill(pt, time, GetEventWeight());
  if(GetReader()->IsPileUpFromSPD()) fhTimePtPhotonSPD->Fill(pt, time, GetEventWeight());
  
  //
  // Cells inside the cluster
  //
    
  // Loop on cells inside cluster, max cell must be over 100 MeV and time in BC=0
  if(cells->GetCellAmplitude(absIdMax) > 0.1 && TMath::Abs(time) < 30)
  {
    for (Int_t ipos = 0; ipos < cluster->GetNCells(); ipos++)
    {
      Int_t absId  = cluster->GetCellsAbsId()[ipos];
      
      if( absId == absIdMax ) continue ;
      
      Double_t tcell = cells->GetCellTime(absId);
      Float_t  amp   = cells->GetCellAmplitude(absId);
      Int_t    bc    = GetReader()->GetInputEvent()->GetBunchCrossNumber();
      
      GetCaloUtils()->GetEMCALRecoUtils()->AcceptCalibrateCell(absId,bc,amp,tcell,cells);
      tcell*=1e9;
      tcell-=fConstantTimeShift;
      
      Float_t diff = (time-tcell);
      
      if( cells->GetCellAmplitude(absIdMax) < 0.1 ) continue ;
      
      if(GetReader()->IsPileUpFromSPD())               fhClusterTimeDiffPhotonPileUp[0]->Fill(pt, diff, GetEventWeight());
      if(GetReader()->IsPileUpFromEMCal())             fhClusterTimeDiffPhotonPileUp[1]->Fill(pt, diff, GetEventWeight());
      if(GetReader()->IsPileUpFromSPDOrEMCal())        fhClusterTimeDiffPhotonPileUp[2]->Fill(pt, diff, GetEventWeight());
      if(GetReader()->IsPileUpFromSPDAndEMCal())       fhClusterTimeDiffPhotonPileUp[3]->Fill(pt, diff, GetEventWeight());
      if(GetReader()->IsPileUpFromSPDAndNotEMCal())    fhClusterTimeDiffPhotonPileUp[4]->Fill(pt, diff, GetEventWeight());
      if(GetReader()->IsPileUpFromEMCalAndNotSPD())    fhClusterTimeDiffPhotonPileUp[5]->Fill(pt, diff, GetEventWeight());
      if(GetReader()->IsPileUpFromNotSPDAndNotEMCal()) fhClusterTimeDiffPhotonPileUp[6]->Fill(pt, diff, GetEventWeight());

    } // loop
  }
  
  AliESDEvent* esdEv = dynamic_cast<AliESDEvent*> (event);
  AliAODEvent* aodEv = dynamic_cast<AliAODEvent*> (event);
  
  //
  // N pile up vertices
  //
  
  Int_t nVtxSPD = -1;
  Int_t nVtxTrk = -1;
  
  if      (esdEv)
  {
    nVtxSPD = esdEv->GetNumberOfPileupVerticesSPD();
    nVtxTrk = esdEv->GetNumberOfPileupVerticesTracks();
    
  } // ESD
  else if (aodEv)
  {
    nVtxSPD = aodEv->GetNumberOfPileupVerticesSPD();
    nVtxTrk = aodEv->GetNumberOfPileupVerticesTracks();
  } // AOD
  
  if(pt < 8)
  {
    fhTimeNPileUpVertSPD  ->Fill(time, nVtxSPD, GetEventWeight());
    fhTimeNPileUpVertTrack->Fill(time, nVtxTrk, GetEventWeight());
  }
  
  fhPtPhotonNPileUpSPDVtx->Fill(pt, nVtxSPD, GetEventWeight());
  fhPtPhotonNPileUpTrkVtx->Fill(pt, nVtxTrk, GetEventWeight());
	
  if(TMath::Abs(time) < 25)
  {
    fhPtPhotonNPileUpSPDVtxTimeCut->Fill(pt, nVtxSPD, GetEventWeight());
    fhPtPhotonNPileUpTrkVtxTimeCut->Fill(pt, nVtxTrk, GetEventWeight());
  }
	
  if(time < 75 && time > -25)
  {
    fhPtPhotonNPileUpSPDVtxTimeCut2->Fill(pt, nVtxSPD, GetEventWeight());
    fhPtPhotonNPileUpTrkVtxTimeCut2->Fill(pt, nVtxTrk, GetEventWeight());
  }
}

//_________________________________________________________________________________
/// Fill cluster Shower Shape histograms.
//_________________________________________________________________________________
void  AliAnaPhoton::FillShowerShapeHistograms(AliVCluster* cluster, Int_t mcTag, Int_t nlm,
                                              Float_t maxCellFraction, Int_t & largeTime)
{
  if(!fFillSSHistograms || GetMixedEvent()) return;
  
  Float_t energy  = cluster->E();
  Int_t   ncells  = cluster->GetNCells();
  Float_t lambda0 = cluster->GetM02();
  Float_t lambda1 = cluster->GetM20();
  Float_t disp    = cluster->GetDispersion()*cluster->GetDispersion();
  
  Float_t pt  = fMomentum.Pt();
  Float_t eta = fMomentum.Eta();
  Float_t phi = fMomentum.Phi();
  if(phi < 0) phi+=TMath::TwoPi();
  
  fhLam0E ->Fill(energy, lambda0, GetEventWeight());
  fhLam0Pt->Fill(pt    , lambda0, GetEventWeight());
  fhLam1E ->Fill(energy, lambda1, GetEventWeight());
  fhLam1Pt->Fill(pt    , lambda1, GetEventWeight());
  fhDispE ->Fill(energy, disp   , GetEventWeight());
  fhDispPt->Fill(pt    , disp   , GetEventWeight());
  
  if(fFillSSNLocMaxHisto)
  {
    if(nlm==1) 
    {
      fhLam0PtNLM1->Fill(pt, lambda0, GetEventWeight());
      fhLam1PtNLM1->Fill(pt, lambda1, GetEventWeight());
    }
    else if(nlm==2)
    {
      fhLam0PtNLM2->Fill(pt, lambda0, GetEventWeight());
      fhLam1PtNLM2->Fill(pt, lambda1, GetEventWeight());
    }
  }
  
  if(GetCalorimeter() == kEMCAL &&  GetFirstSMCoveredByTRD() >= 0 &&
     GetModuleNumber(cluster) >= GetFirstSMCoveredByTRD()  )
  {
    fhLam0ETRD ->Fill(energy, lambda0, GetEventWeight());
    fhLam0PtTRD->Fill(pt    , lambda0, GetEventWeight());
    fhLam1ETRD ->Fill(energy, lambda1, GetEventWeight());
    fhDispETRD ->Fill(energy, disp,    GetEventWeight());
  }
  
  //
  // EMCal SM regions
  //
  if(cluster->IsEMCAL() && fFillEMCALRegionSSHistograms)
  {
    Int_t sm = GetModuleNumber(cluster);
    
//    Bool_t shared = GetCaloUtils()->IsClusterSharedByTwoSuperModules(GetEMCALGeometry(),cluster);
    
    fhLam0PerSM[sm]->Fill(pt, lambda0, GetEventWeight());
    fhLam1PerSM[sm]->Fill(pt, lambda1, GetEventWeight());
    
//    if(GetReader()->IsPileUpFromSPD())
//    {
//      fhLam0PerSMSPDPileUp[sm]->Fill(pt, lambda0, GetEventWeight());
//      fhLam1PerSMSPDPileUp[sm]->Fill(pt, lambda1, GetEventWeight());      
//    }
    
    Int_t etaRegion = -1, phiRegion = -1;
    GetCaloUtils()->GetEMCALSubregion(cluster,GetReader()->GetEMCALCells(),etaRegion,phiRegion);
    if(etaRegion >= 0 && etaRegion < 4 && phiRegion >=0 && phiRegion < 3) 
    {
      
      //      fhLam0EMCALRegion[etaRegion][phiRegion]->Fill(pt,lambda0, GetEventWeight());
      //      
      //      if(GetFirstSMCoveredByTRD() >= 0 && sm >= GetFirstSMCoveredByTRD()  )
      //      {
      //        fhLam0EMCALRegionTRD[etaRegion][phiRegion]->Fill(pt, lambda0, GetEventWeight());
      //      }
      
      fhLam0EMCALRegionPerSM[etaRegion][phiRegion][sm]->Fill(pt,lambda0, GetEventWeight());
      fhLam1EMCALRegionPerSM[etaRegion][phiRegion][sm]->Fill(pt,lambda1, GetEventWeight());
      
//      if(shared)
//      {
//        fhLam0PerSMShared[sm]->Fill(pt,lambda0, GetEventWeight());
//        fhLam1PerSMShared[sm]->Fill(pt,lambda1, GetEventWeight());
//      }
    }
    
    // Define l0 bin and cluster pt bin 
    Int_t l0bin = -1;
    if     ( lambda0 >=0.30 && lambda0 <= 0.40 ) l0bin = 1;
    else if( lambda0 >=0.23 && lambda0 <= 0.26 ) l0bin = 0;
    
    Float_t ptLimit[] = {2,3,4,5,6,8,10,12};
    Int_t ptbin = -1;
    for(Int_t ipt = 0; ipt < 7; ipt++)  
    {
      if( pt >= ptLimit[ipt] && pt < ptLimit[ipt+1]  )
      {
        ptbin = ipt;
        break;
      }
    }
    
    if(l0bin!=-1)
    {
      fhLam1Lam0BinPerSM[l0bin][sm]->Fill(pt,lambda1); 
      
      if(ptbin >= 0) 
      {
        fhEtaPhiLam0BinPtBin[l0bin][ptbin]->Fill(eta, phi, GetEventWeight());
//        if(shared) fhEtaPhiLam0BinPtBinSMShared[l0bin][ptbin]->Fill(eta, phi, GetEventWeight());
      }
    }
    
    // 
    // Histograms with time of secondary cells, only if they contribute with a non null 
    // weight to the shower shape
    //
    AliVCaloCells* cells = GetReader()->GetEMCALCells();
    
    Int_t    bc          = GetReader()->GetInputEvent()->GetBunchCrossNumber();
    
    //
    // Sort the cluster cells in energy, find maximum energy cell
    //
    Int_t ncell = cluster->GetNCells();
    Int_t    * sortList = new Int_t   [ncell];
    Float_t  * enerList = new Float_t [ncell];
    Double_t * timeList = new Double_t[ncell];

    for(Int_t icell = 0; icell < ncell; icell++)
    {
      Int_t absId = cluster->GetCellAbsId(icell);
      Float_t  cellE    = cells->GetCellAmplitude(absId);
      Double_t cellTime = cells->GetCellTime(absId);

      GetCaloUtils()->GetEMCALRecoUtils()->AcceptCalibrateCell(absId,bc,cellE,cellTime,cells);
      cellTime*=1e9;
      cellTime-=fConstantTimeShift;
      
      enerList[icell] = cellE;
      timeList[icell] = cellTime;
    }

    TMath::Sort(ncell,enerList,sortList); 

    //
    // Location of max cell
    // First in sorted list.
    //
    Int_t    absIdMax    = cluster->GetCellAbsId(sortList[0]);
    //    Float_t  cellEMax    = enerList[sortList[0]];
    Double_t cellTimeMax = timeList[sortList[0]];    
    
    //
    // Cluster cell loop, select only secondary cells with enough contribution to cluster
    // Start from second highest energy cell
    //
    largeTime  = 0;
    Float_t largeTimeE = 0;
    for(Int_t icell = 1; icell < ncell; icell++)
    {
      Int_t    absId    = cluster->GetCellAbsId(sortList[icell]);
      Float_t  cellE    = enerList[sortList[icell]];
      Double_t cellTime = timeList[sortList[icell]];

      //printf("\t icell %d, absId %d, ener %2.2f, time %2.2f, sort %d\n",icell,absId,cellE,cellTime,sortList[icell]);

      if ( absId == absIdMax ) continue;
      
      Float_t weight = GetCaloUtils()->GetEMCALRecoUtils()->GetCellWeight(cellE,cluster->E());
      
      //printf("Cluster E %2.2f, cell E %2.2f, time %2.2f, weight %2.3f\n",cluster->E(),cellE,cellTime,weight);
      if(weight < 0.01) continue;
      
      if ( TMath::Abs(cellTime) > 50 ) 
      {
        largeTime++;
        largeTimeE += cellE;
        //printf(" cluster pT %2.2f, cell E %2.2f, large time %d; ",pt, cellE, largeTime);
      }
      if ( l0bin == -1 ) continue;
          
      fhTimeLam0BinPerSM        [l0bin][sm]->Fill(pt, cellTime,        GetEventWeight());
      fhTimeLam0BinPerSMWeighted[l0bin][sm]->Fill(pt, cellTime, weight*GetEventWeight());
      
      fhDTimeLam0BinPerSM        [l0bin][sm]->Fill(pt, cellTimeMax-cellTime,        GetEventWeight());
      fhDTimeLam0BinPerSMWeighted[l0bin][sm]->Fill(pt, cellTimeMax-cellTime, weight*GetEventWeight());
      
      fhCellClusterEFracLam0BinPerSM        [l0bin][sm]->Fill(pt, cellE/cluster->E(),        GetEventWeight());
//      fhCellClusterEFracLam0BinPerSMWeighted[l0bin][sm]->Fill(pt, cellE/cluster->E(), weight*GetEventWeight());        
      
      fhCellClusterELam0BinPerSM        [l0bin][sm]->Fill(pt, cellE,        GetEventWeight());
      fhCellClusterELam0BinPerSMWeighted[l0bin][sm]->Fill(pt, cellE, weight*GetEventWeight());
      
      if(ptbin >= 0) 
      {
        fhCellClusterIndexEAndTime[l0bin][ptbin] ->Fill(cellTime, icell             , GetEventWeight());
        fhCellClusterEAndTime     [l0bin][ptbin] ->Fill(cellTime, cellE             , GetEventWeight());
        fhCellClusterEFracAndTime [l0bin][ptbin] ->Fill(cellTime, cellE/cluster->E(), GetEventWeight());
      }
      
      if ( TMath::Abs(cellTime) > 50 )
      {        
        fhCellClusterEFracLam0BinPerSMLargeTime [l0bin][sm]->Fill(pt, cellE/cluster->E(), GetEventWeight());
        fhCellClusterELam0BinPerSMLargeTime     [l0bin][sm]->Fill(pt, cellE             , GetEventWeight());
        fhCellClusterIndexELam0BinPerSMLargeTime[l0bin][sm]->Fill(pt, icell             , GetEventWeight());        
      }
//      if(shared)
//        fhTimeLam0BinPerSMShared[l0bin][sm]->Fill(pt, cellTime, GetEventWeight());
      
      if(ptbin >= 0) 
      {
        Int_t   icol     = -1, icolAbs = -1;
        Int_t   irow     = -1, irowAbs = -1;
        Int_t   iRCU     = -1;
        GetModuleNumberCellIndexesAbsCaloMap(absId,GetCalorimeter(), icol, irow, iRCU, icolAbs, irowAbs);
        
        fhColRowLam0BinPtBin        [l0bin][ptbin]->Fill(icolAbs, irowAbs,        GetEventWeight());
        fhColRowLam0BinPtBinWeighted[l0bin][ptbin]->Fill(icolAbs, irowAbs, weight*GetEventWeight());
        
        if ( TMath::Abs(cellTime) > 50 )
          fhColRowLam0BinPtBinLargeTime[l0bin][ptbin]->Fill(icolAbs, irowAbs, GetEventWeight());
        
//        if(shared)  fhColRowLam0BinPtBinSMShared[l0bin][ptbin]->Fill(icolAbs, irowAbs, GetEventWeight());
      }
      
      
    } // cluster cell loop
      //if(largeTime > 0) printf("\n");
    
    if ( largeTime > 0 ) 
    {
      if ( l0bin != -1 ) 
      {
        if ( ptbin > 0 ) 
          fhEtaPhiLargeTimeInClusterCell[l0bin][ptbin]->Fill(eta, phi, GetEventWeight());
        
        fhCellClusterEFracLam0BinPerSMLargeTimeTotal[l0bin][sm]->Fill(pt, largeTimeE/cluster->E(), GetEventWeight());
        fhNCellsWithLargeTimeInCluster              [l0bin][sm]->Fill(pt, largeTime              , GetEventWeight());
      }
      
      fhLam0PerSMLargeTimeInClusterCell[sm]->Fill(pt, lambda0, GetEventWeight());
      fhLam1PerSMLargeTimeInClusterCell[sm]->Fill(pt, lambda1, GetEventWeight());
      
      if(largeTime < 6)
      {
        fhLam0PerNLargeTimeInClusterCell[largeTime-1]->Fill(pt, lambda0, GetEventWeight());
        fhLam1PerNLargeTimeInClusterCell[largeTime-1]->Fill(pt, lambda1, GetEventWeight());
      }
    }
    
    delete [] sortList ;
    delete [] enerList ;
    delete [] timeList ;
    //printf("Cluster %d, E %2.2f, sm %d, eta %2.2f, phi %2.2f ---> region %d %d\n",cluster->GetID(),cluster->E(),GetModuleNumber(cluster),eta,RadToDeg(phi),etaRegion,phiRegion);
  } // Check EMCal SM and regions
  
  Float_t l0   = 0., l1   = 0.;
  Float_t dispp= 0., dEta = 0., dPhi    = 0.;
  Float_t sEta = 0., sPhi = 0., sEtaPhi = 0.;
  if(GetCalorimeter() == kEMCAL && !fFillOnlySimpleSSHisto)
  {
    GetCaloUtils()->GetEMCALRecoUtils()->RecalculateClusterShowerShapeParameters(GetEMCALGeometry(), GetReader()->GetInputEvent()->GetEMCALCells(), cluster,
                                                                                 l0, l1, dispp, dEta, dPhi, sEta, sPhi, sEtaPhi);
    //printf("AliAnaPhoton::FillShowerShapeHistogram - l0 %2.6f, l1 %2.6f, disp %2.6f, dEta %2.6f, dPhi %2.6f, sEta %2.6f, sPhi %2.6f, sEtaPhi %2.6f \n",
    //       l0, l1, dispp, dEta, dPhi, sEta, sPhi, sEtaPhi );
    //printf("AliAnaPhoton::FillShowerShapeHistogram - dispersion %f, dispersion eta+phi %f \n",
    //       disp, dPhi+dEta );
    fhDispEtaE        -> Fill(energy, dEta     , GetEventWeight());
    fhDispPhiE        -> Fill(energy, dPhi     , GetEventWeight());
    fhSumEtaE         -> Fill(energy, sEta     , GetEventWeight());
    fhSumPhiE         -> Fill(energy, sPhi     , GetEventWeight());
    fhSumEtaPhiE      -> Fill(energy, sEtaPhi  , GetEventWeight());
    fhDispEtaPhiDiffE -> Fill(energy, dPhi-dEta, GetEventWeight());
      
    if(dEta+dPhi>0)fhSphericityE     -> Fill(energy,(dPhi-dEta)/(dEta+dPhi)     , GetEventWeight());
    if(dEta+sEta>0)fhDispSumEtaDiffE -> Fill(energy,(dEta-sEta)/((dEta+sEta)/2.), GetEventWeight());
    if(dPhi+sPhi>0)fhDispSumPhiDiffE -> Fill(energy,(dPhi-sPhi)/((dPhi+sPhi)/2.), GetEventWeight());
    
    Int_t ebin = -1;
    if      (energy < 2 ) ebin = 0;
    else if (energy < 4 ) ebin = 1;
    else if (energy < 6 ) ebin = 2;
    else if (energy < 10) ebin = 3;
    else if (energy < 15) ebin = 4;
    else if (energy < 20) ebin = 5;
    else                  ebin = 6;
    
    fhDispEtaDispPhi[ebin]->Fill(dEta   , dPhi, GetEventWeight());
    fhLambda0DispEta[ebin]->Fill(lambda0, dEta, GetEventWeight());
    fhLambda0DispPhi[ebin]->Fill(lambda0, dPhi, GetEventWeight());
  }
  
  // If track-matching was off, check effect of track-matching residual cut
  
  if(!fRejectTrackMatch)
  {
    Float_t dZ  = cluster->GetTrackDz();
    Float_t dR  = cluster->GetTrackDx();
//    if(cluster->IsEMCAL() && GetCaloUtils()->IsRecalculationOfClusterTrackMatchingOn())
//    {
//      dR = 2000., dZ = 2000.;
//      GetCaloUtils()->GetEMCALRecoUtils()->GetMatchedResiduals(cluster->GetID(),dZ,dR);
//    }
    
    if(TMath::Abs(dZ) < 0.05 && TMath::Abs(dR) < 0.05)
    {
      fhLam0ETM ->Fill(energy, lambda0, GetEventWeight());
      fhLam0PtTM->Fill(pt    , lambda0, GetEventWeight());
      fhLam1ETM ->Fill(energy, lambda1, GetEventWeight());
      fhDispETM ->Fill(energy, disp   , GetEventWeight());
      
      if(GetCalorimeter() == kEMCAL &&  GetFirstSMCoveredByTRD() >= 0 &&
         GetModuleNumber(cluster) >= GetFirstSMCoveredByTRD()  )
      {
        fhLam0ETMTRD ->Fill(energy, lambda0, GetEventWeight());
        fhLam0PtTMTRD->Fill(pt    , lambda0, GetEventWeight());
        fhLam1ETMTRD ->Fill(energy, lambda1, GetEventWeight());
        fhDispETMTRD ->Fill(energy, disp   , GetEventWeight());
      }
    }
  } // If track-matching was off, check effect of matching residual cut
  
  if(!fFillOnlySimpleSSHisto)
  {
    if(energy < 2)
    {
      fhNCellsLam0LowE ->Fill(ncells, lambda0, GetEventWeight());
      fhNCellsLam1LowE ->Fill(ncells, lambda1, GetEventWeight());
      fhNCellsDispLowE ->Fill(ncells, disp   , GetEventWeight());
      
      fhLam1Lam0LowE  ->Fill(lambda1, lambda0, GetEventWeight());
      fhLam0DispLowE  ->Fill(lambda0, disp   , GetEventWeight());
      fhDispLam1LowE  ->Fill(disp   , lambda1, GetEventWeight());
      fhEtaLam0LowE   ->Fill(eta    , lambda0, GetEventWeight());
      fhPhiLam0LowE   ->Fill(phi    , lambda0, GetEventWeight());
    }
    else
    {
      fhNCellsLam0HighE ->Fill(ncells, lambda0, GetEventWeight());
      fhNCellsLam1HighE ->Fill(ncells, lambda1, GetEventWeight());
      fhNCellsDispHighE ->Fill(ncells, disp   , GetEventWeight());
      
      fhLam1Lam0HighE  ->Fill(lambda1, lambda0, GetEventWeight());
      fhLam0DispHighE  ->Fill(lambda0, disp   , GetEventWeight());
      fhDispLam1HighE  ->Fill(disp   , lambda1, GetEventWeight());
      fhEtaLam0HighE   ->Fill(eta    , lambda0, GetEventWeight());
      fhPhiLam0HighE   ->Fill(phi    , lambda0, GetEventWeight());
    }
  }
  
  if(IsDataMC())
  {
    AliVCaloCells* cells = 0;
    if(GetCalorimeter() == kEMCAL) cells = GetEMCALCells();
    else                           cells = GetPHOSCells();
    
    // Fill histograms to check shape of embedded clusters
    Float_t fraction = 0;
    // printf("check embedding %i\n",GetReader()->IsEmbeddedClusterSelectionOn());
    
    if(GetReader()->IsEmbeddedClusterSelectionOn())
    {
      // Only working for EMCAL
      // 	printf("embedded\n");
        
      Float_t clusterE = 0; // recalculate in case corrections applied.
      Float_t cellE    = 0;
      for(Int_t icell  = 0; icell < cluster->GetNCells(); icell++)
      {
        cellE    = cells->GetCellAmplitude(cluster->GetCellAbsId(icell));
        clusterE+=cellE;
        fraction+=cellE*cluster->GetCellAmplitudeFraction(icell);
      }
      
      //Fraction of total energy due to the embedded signal
      fraction/=clusterE;
      
      AliDebug(1,Form("Energy fraction of embedded signal %2.3f, Energy %2.3f",fraction, clusterE));
      
      fhEmbeddedSignalFractionEnergy->Fill(clusterE, fraction, GetEventWeight());
    }  // embedded fraction
        
    // Check the origin and fill histograms
    
    Int_t mcIndex = -1;
    
    if( GetMCAnalysisUtils()->CheckTagBit(mcTag,AliMCAnalysisUtils::kMCPhoton)     &&
       !GetMCAnalysisUtils()->CheckTagBit(mcTag,AliMCAnalysisUtils::kMCConversion) &&
       !GetMCAnalysisUtils()->CheckTagBit(mcTag,AliMCAnalysisUtils::kMCPi0)        &&
       !GetMCAnalysisUtils()->CheckTagBit(mcTag,AliMCAnalysisUtils::kMCEta))
    {
      mcIndex = kmcssPhoton ;
      
      if(!GetReader()->IsEmbeddedClusterSelectionOn())
      {
        //Check particle overlaps in cluster
        
        // Compare the primary depositing more energy with the rest,
        // if no photon/electron as comon ancestor (conversions), count as other particle
        const UInt_t nlabels = cluster->GetNLabels();
        Int_t overpdg[nlabels];
        Int_t overlab[nlabels];
        Int_t noverlaps = GetMCAnalysisUtils()->GetNOverlaps(cluster->GetLabels(), nlabels,mcTag,-1,GetReader(),overpdg,overlab);

        //printf("N overlaps %d \n",noverlaps);
        
        if(noverlaps == 0)
        {
          fhMCPhotonELambda0NoOverlap  ->Fill(energy, lambda0, GetEventWeight());
        }
        else if(noverlaps == 1)
        {
          fhMCPhotonELambda0TwoOverlap ->Fill(energy, lambda0, GetEventWeight());
        }
        else if(noverlaps > 1)
        {
          fhMCPhotonELambda0NOverlap   ->Fill(energy, lambda0, GetEventWeight());
        }
        else
        {
          AliWarning(Form("n overlaps = %d!!", noverlaps));
        }
      } // No embedding
      
      // Fill histograms to check shape of embedded clusters
      if(GetReader()->IsEmbeddedClusterSelectionOn())
      {
        if     (fraction > 0.9)
        {
          fhEmbedPhotonELambda0FullSignal   ->Fill(energy, lambda0, GetEventWeight());
        }
        else if(fraction > 0.5)
        {
          fhEmbedPhotonELambda0MostlySignal ->Fill(energy, lambda0, GetEventWeight());
        }
        else if(fraction > 0.1)
        {
          fhEmbedPhotonELambda0MostlyBkg    ->Fill(energy, lambda0, GetEventWeight());
        }
        else
        {
          fhEmbedPhotonELambda0FullBkg      ->Fill(energy, lambda0, GetEventWeight());
        }
      } // embedded
    } // photon no conversion
    else if  ( GetMCAnalysisUtils()->CheckTagBit(mcTag,AliMCAnalysisUtils::kMCPhoton)     &&
               GetMCAnalysisUtils()->CheckTagBit(mcTag,AliMCAnalysisUtils::kMCConversion) &&
              !GetMCAnalysisUtils()->CheckTagBit(mcTag,AliMCAnalysisUtils::kMCPi0)        &&
              !GetMCAnalysisUtils()->CheckTagBit(mcTag,AliMCAnalysisUtils::kMCEta))
    {
      mcIndex = kmcssConversion ;
    }//conversion photon

    else if  ( GetMCAnalysisUtils()->CheckTagBit(mcTag,AliMCAnalysisUtils::kMCElectron))
    {
      mcIndex = kmcssElectron ;
    }//electron
    else if  ( GetMCAnalysisUtils()->CheckTagBit(mcTag,AliMCAnalysisUtils::kMCPi0)  )
    {
      mcIndex = kmcssPi0 ;
      
      //Fill histograms to check shape of embedded clusters
      if(GetReader()->IsEmbeddedClusterSelectionOn())
      {
        if     (fraction > 0.9)
        {
          fhEmbedPi0ELambda0FullSignal   ->Fill(energy, lambda0, GetEventWeight());
        }
        else if(fraction > 0.5)
        {
          fhEmbedPi0ELambda0MostlySignal ->Fill(energy, lambda0, GetEventWeight());
        }
        else if(fraction > 0.1)
        {
          fhEmbedPi0ELambda0MostlyBkg    ->Fill(energy, lambda0, GetEventWeight());
        }
        else
        {
          fhEmbedPi0ELambda0FullBkg      ->Fill(energy, lambda0, GetEventWeight());
        }
      } // embedded
      
    }//pi0
    else if  ( GetMCAnalysisUtils()->CheckTagBit(mcTag,AliMCAnalysisUtils::kMCEta)  )
    {
      mcIndex = kmcssEta ;
    }//eta
    else
    {
      mcIndex = kmcssOther ;
    }//other particles
    
    fhMCELambda0           [mcIndex]->Fill(energy, lambda0, GetEventWeight());
    fhMCPtLambda0          [mcIndex]->Fill(pt    , lambda0, GetEventWeight());
    fhMCELambda1           [mcIndex]->Fill(energy, lambda1, GetEventWeight());
    fhMCEDispersion        [mcIndex]->Fill(energy, disp   , GetEventWeight());
    fhMCNCellsE            [mcIndex]->Fill(energy, ncells , GetEventWeight());
    fhMCMaxCellDiffClusterE[mcIndex]->Fill(energy, maxCellFraction, GetEventWeight());
    
    if(!fFillOnlySimpleSSHisto)
    {
      if     (energy < 2.)
      {
        fhMCLambda0vsClusterMaxCellDiffE0[mcIndex]->Fill(lambda0, maxCellFraction, GetEventWeight());
        fhMCNCellsvsClusterMaxCellDiffE0 [mcIndex]->Fill(ncells,  maxCellFraction, GetEventWeight());
      }
      else if(energy < 6.)
      {
        fhMCLambda0vsClusterMaxCellDiffE2[mcIndex]->Fill(lambda0, maxCellFraction, GetEventWeight());
        fhMCNCellsvsClusterMaxCellDiffE2 [mcIndex]->Fill(ncells,  maxCellFraction, GetEventWeight());
      }
      else
      {
        fhMCLambda0vsClusterMaxCellDiffE6[mcIndex]->Fill(lambda0, maxCellFraction, GetEventWeight());
        fhMCNCellsvsClusterMaxCellDiffE6 [mcIndex]->Fill(ncells,  maxCellFraction, GetEventWeight());
      }
      
      if(GetCalorimeter() == kEMCAL)
      {
        fhMCEDispEta        [mcIndex]-> Fill(energy, dEta     , GetEventWeight());
        fhMCEDispPhi        [mcIndex]-> Fill(energy, dPhi     , GetEventWeight());
        fhMCESumEtaPhi      [mcIndex]-> Fill(energy, sEtaPhi  , GetEventWeight());
        fhMCEDispEtaPhiDiff [mcIndex]-> Fill(energy, dPhi-dEta, GetEventWeight());
          
        if( dEta+dPhi > 0 ) fhMCESphericity[mcIndex]-> Fill(energy, (dPhi-dEta)/(dEta+dPhi), GetEventWeight());
        
        Int_t ebin = -1;
        if      (energy < 2 ) ebin = 0;
        else if (energy < 4 ) ebin = 1;
        else if (energy < 6 ) ebin = 2;
        else if (energy < 10) ebin = 3;
        else if (energy < 15) ebin = 4;
        else if (energy < 20) ebin = 5;
        else                  ebin = 6;
        
        fhMCDispEtaDispPhi[ebin][mcIndex]->Fill(dEta   , dPhi, GetEventWeight());
        fhMCLambda0DispEta[ebin][mcIndex]->Fill(lambda0, dEta, GetEventWeight());
        fhMCLambda0DispPhi[ebin][mcIndex]->Fill(lambda0, dPhi, GetEventWeight());
      }
    }
  }// MC data
}

//__________________________________________________________________________
/// If selected, fill histograms with residuals of matched clusters,
/// help to define track matching cut.
/// Residual filled for different cuts:
/// \param cluster: pointer to cluster info.
/// \param cut: 0, No cut, 1, after cluster selection cuts.
//__________________________________________________________________________
void AliAnaPhoton::FillTrackMatchingResidualHistograms(AliVCluster* cluster,
                                                       Int_t cut)
{
  Float_t dZ = cluster->GetTrackDz();
  Float_t dR = cluster->GetTrackDx();
  
//  if(cluster->IsEMCAL() && GetCaloUtils()->IsRecalculationOfClusterTrackMatchingOn())
//  {
//    dR = 2000., dZ = 2000.;
//    GetCaloUtils()->GetEMCALRecoUtils()->GetMatchedResiduals(cluster->GetID(),dZ,dR);
//  }
  
  AliVTrack *track = GetCaloUtils()->GetMatchedTrack(cluster, GetReader()->GetInputEvent());
  
  Bool_t positive = kFALSE;
  if(track) positive = (track->Charge()>0);
  
  if(fhTrackMatchedDEta[cut] && TMath::Abs(dR) < 999)
  {
    fhTrackMatchedDEta[cut]->Fill(cluster->E(), dZ, GetEventWeight());
    fhTrackMatchedDPhi[cut]->Fill(cluster->E(), dR, GetEventWeight());
    if(cluster->E() > 0.5) fhTrackMatchedDEtaDPhi[cut]->Fill(dZ, dR, GetEventWeight());

    if(track)
    {
      if(positive)
      {
        fhTrackMatchedDEtaPos[cut]->Fill(cluster->E(), dZ, GetEventWeight());
        fhTrackMatchedDPhiPos[cut]->Fill(cluster->E(), dR, GetEventWeight());
        if(cluster->E() > 0.5) fhTrackMatchedDEtaDPhiPos[cut]->Fill(dZ,dR, GetEventWeight());
      }
      else
      {
        fhTrackMatchedDEtaNeg[cut]->Fill(cluster->E(), dZ, GetEventWeight());
        fhTrackMatchedDPhiNeg[cut]->Fill(cluster->E(), dR, GetEventWeight());
        if(cluster->E() > 0.5) fhTrackMatchedDEtaDPhiNeg[cut]->Fill(dZ, dR, GetEventWeight());
      }
    }
    
    Int_t nSMod = GetModuleNumber(cluster);
    
    if( GetCalorimeter() == kEMCAL &&   GetFirstSMCoveredByTRD() >= 0 &&
       nSMod >= GetFirstSMCoveredByTRD()   )
    {
      fhTrackMatchedDEtaTRD[cut]->Fill(cluster->E(), dZ, GetEventWeight());
      fhTrackMatchedDPhiTRD[cut]->Fill(cluster->E(), dR, GetEventWeight());
    }
    
    // Check dEdx and E/p of matched clusters

    if(TMath::Abs(dZ) < 0.05 && TMath::Abs(dR) < 0.05)
    {
      if(track)
      {
        Float_t dEdx   = track->GetTPCsignal();
        Float_t eOverp = cluster->E()/track->P();
        
        fhdEdx[cut]  ->Fill(cluster->E(), dEdx  , GetEventWeight());
        fhEOverP[cut]->Fill(cluster->E(), eOverp, GetEventWeight());
        
        if(GetCalorimeter()==kEMCAL &&  GetFirstSMCoveredByTRD() >= 0 &&
           nSMod >= GetFirstSMCoveredByTRD()  )
          fhEOverPTRD[cut]->Fill(cluster->E(), eOverp, GetEventWeight());
        
        
      }
      else
        AliWarning(Form("Residual OK but (dR, dZ)= (%2.4f,%2.4f) no track associated WHAT?", dR,dZ));
      
      if(IsDataMC())
      {
        
        Int_t tag = GetMCAnalysisUtils()->CheckOrigin(cluster->GetLabels(),cluster->GetNLabels(),GetReader(),GetCalorimeter());
        
        if  ( !GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCConversion)  )
        {
          if       ( GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCPi0)      ||
                     GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCEta)       )
              fhTrackMatchedMCParticle[cut]->Fill(cluster->E(), 2.5, GetEventWeight());
          else if  ( GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCPhoton)    )
              fhTrackMatchedMCParticle[cut]->Fill(cluster->E(), 0.5, GetEventWeight());
          else if  ( GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCElectron)  )
              fhTrackMatchedMCParticle[cut]->Fill(cluster->E(), 1.5, GetEventWeight());
          else
              fhTrackMatchedMCParticle[cut]->Fill(cluster->E(), 3.5, GetEventWeight());
          
          // Check if several particles contributed to cluster and discard overlapped mesons
          if(!GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCPi0) ||
             !GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCEta))
          {
            if(cluster->GetNLabels()==1)
            {
              fhTrackMatchedDEtaMCNoOverlap[cut]->Fill(cluster->E(), dZ, GetEventWeight());
              fhTrackMatchedDPhiMCNoOverlap[cut]->Fill(cluster->E(), dR, GetEventWeight());
            }
            else
            {
              fhTrackMatchedDEtaMCOverlap[cut]->Fill(cluster->E(), dZ, GetEventWeight());
              fhTrackMatchedDPhiMCOverlap[cut]->Fill(cluster->E(), dR, GetEventWeight());
            }
            
          }// Check overlaps
          
        }
        else
        {
          if       ( GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCPi0)      ||
                    GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCEta)       )
              fhTrackMatchedMCParticle[cut]->Fill(cluster->E(), 6.5, GetEventWeight());
          else if  ( GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCPhoton)    )
              fhTrackMatchedMCParticle[cut]->Fill(cluster->E(), 4.5, GetEventWeight());
          else if  ( GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCElectron)  )
              fhTrackMatchedMCParticle[cut]->Fill(cluster->E(), 5.5, GetEventWeight());
          else
              fhTrackMatchedMCParticle[cut]->Fill(cluster->E(), 7.5, GetEventWeight());
          
          // Check if several particles contributed to cluster
          if(!GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCPi0) ||
             !GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCEta))
          {
            fhTrackMatchedDEtaMCConversion[cut]->Fill(cluster->E(), dZ, GetEventWeight());
            fhTrackMatchedDPhiMCConversion[cut]->Fill(cluster->E(), dR, GetEventWeight());
            
          }// Check overlaps
          
        }
        
      } // MC
      
    } // residuals window
    
  } // Small residual
}

//___________________________________________
/// Save parameters used for analysis in a string.
//___________________________________________
TObjString *  AliAnaPhoton::GetAnalysisCuts()
{
  TString parList ; //this will be list of parameters used for this analysis.
  const Int_t buffersize = 255;
  char onePar[buffersize] ;
  
  snprintf(onePar,buffersize,"--- AliAnaPhoton ---:") ;
  parList+=onePar ;
  snprintf(onePar,buffersize,"Calorimeter: %s;",GetCalorimeterString().Data()) ;
  parList+=onePar ;
  snprintf(onePar,buffersize,"fMinDist =%2.2f (Minimal distance to bad channel to accept cluster);",fMinDist) ;
  parList+=onePar ;
  snprintf(onePar,buffersize,"fMinDist2=%2.2f (Cuts on Minimal distance to study acceptance evaluation);",fMinDist2) ;
  parList+=onePar ;
  snprintf(onePar,buffersize,"fMinDist3=%2.2f (One more cut on distance used for acceptance-efficiency study);",fMinDist3) ;
  parList+=onePar ;
  snprintf(onePar,buffersize,"fRejectTrackMatch: %d",fRejectTrackMatch) ;
  parList+=onePar ;
  
  // Get parameters set in base class.
  parList += GetBaseParametersList() ;
  
  // Get parameters set in PID class.
  parList += GetCaloPID()->GetPIDParametersList() ;
  
  // Get parameters set in FiducialCut class (not available yet)
  //parlist += GetFidCut()->GetFidCutParametersList()
  
  return new TObjString(parList) ;
}

//_____________________________________________
/// Create histograms to be saved in output file and
/// store them in outputContainer.
//_____________________________________________
TList *  AliAnaPhoton::GetCreateOutputObjects()
{
  TList * outputContainer = new TList() ;
  outputContainer->SetName("PhotonHistos") ;
	
  Int_t nptbins  = GetHistogramRanges()->GetHistoPtBins();  Float_t ptmax  = GetHistogramRanges()->GetHistoPtMax();  Float_t ptmin  = GetHistogramRanges()->GetHistoPtMin();
  Int_t nphibins = GetHistogramRanges()->GetHistoPhiBins(); Float_t phimax = GetHistogramRanges()->GetHistoPhiMax(); Float_t phimin = GetHistogramRanges()->GetHistoPhiMin();
  Int_t netabins = GetHistogramRanges()->GetHistoEtaBins(); Float_t etamax = GetHistogramRanges()->GetHistoEtaMax(); Float_t etamin = GetHistogramRanges()->GetHistoEtaMin();
  Int_t ssbins   = GetHistogramRanges()->GetHistoShowerShapeBins();  Float_t ssmax   = GetHistogramRanges()->GetHistoShowerShapeMax();  Float_t ssmin   = GetHistogramRanges()->GetHistoShowerShapeMin();
  Int_t nbins    = GetHistogramRanges()->GetHistoNClusterCellBins(); Int_t   nmax    = GetHistogramRanges()->GetHistoNClusterCellMax(); Int_t   nmin    = GetHistogramRanges()->GetHistoNClusterCellMin();
  Int_t ntimebins= GetHistogramRanges()->GetHistoTimeBins();         Float_t timemax = GetHistogramRanges()->GetHistoTimeMax();         Float_t timemin = GetHistogramRanges()->GetHistoTimeMin();
  
  Int_t   nresetabins = GetHistogramRanges()->GetHistoTrackResidualEtaBins();
  Float_t resetamax   = GetHistogramRanges()->GetHistoTrackResidualEtaMax();
  Float_t resetamin   = GetHistogramRanges()->GetHistoTrackResidualEtaMin();
  Int_t   nresphibins = GetHistogramRanges()->GetHistoTrackResidualPhiBins();
  Float_t resphimax   = GetHistogramRanges()->GetHistoTrackResidualPhiMax();
  Float_t resphimin   = GetHistogramRanges()->GetHistoTrackResidualPhiMin();
  
  Int_t   ndedxbins   = GetHistogramRanges()->GetHistodEdxBins();
  Float_t dedxmax     = GetHistogramRanges()->GetHistodEdxMax();
  Float_t dedxmin     = GetHistogramRanges()->GetHistodEdxMin();
  Int_t   nPoverEbins = GetHistogramRanges()->GetHistoPOverEBins();
  Float_t pOverEmax   = GetHistogramRanges()->GetHistoPOverEMax();
  Float_t pOverEmin   = GetHistogramRanges()->GetHistoPOverEMin();
  
  Int_t bin[] = {0,2,4,6,10,15,20,100}; // energy bins for SS studies
  
  TString cut[] = {"Open","Reader","E","Time","NCells","NLM","Fidutial","Matching","Bad","PID"};
  for (Int_t i = 0; i < 10 ;  i++)
  {
    fhClusterCutsE[i] = new TH1F(Form("hE_Cut_%d_%s", i, cut[i].Data()),
                                Form("Number of clusters that pass cuts <= %d, %s", i, cut[i].Data()),
                                nptbins,ptmin,ptmax);
    fhClusterCutsE[i]->SetYTitle("d#it{N}/d#it{E} ");
    fhClusterCutsE[i]->SetXTitle("#it{E} (GeV)");
    outputContainer->Add(fhClusterCutsE[i]) ;
    
    fhClusterCutsPt[i] = new TH1F(Form("hPt_Cut_%d_%s", i, cut[i].Data()),
                                Form("Number of clusters that pass cuts <= %d, %s", i, cut[i].Data()),
                                nptbins,ptmin,ptmax);
    fhClusterCutsPt[i]->SetYTitle("d#it{N}/d#it{E} ");
    fhClusterCutsPt[i]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
    outputContainer->Add(fhClusterCutsPt[i]) ;
  }
  
  fhEClusterSM = new TH2F("hEClusterSM","Raw clusters E and super-module number",
                          nptbins,ptmin,ptmax,
                          GetCaloUtils()->GetNumberOfSuperModulesUsed(),0,GetCaloUtils()->GetNumberOfSuperModulesUsed());
  fhEClusterSM->SetYTitle("SuperModule ");
  fhEClusterSM->SetXTitle("#it{E} (GeV)");
  outputContainer->Add(fhEClusterSM) ;

  fhPtClusterSM = new TH2F("hPtClusterSM","Raw clusters #it{p}_{T} and super-module number",
                          nptbins,ptmin,ptmax,
                          GetCaloUtils()->GetNumberOfSuperModulesUsed(),0,GetCaloUtils()->GetNumberOfSuperModulesUsed());
  fhPtClusterSM->SetYTitle("SuperModule ");
  fhPtClusterSM->SetXTitle("#it{E} (GeV)");
  outputContainer->Add(fhPtClusterSM) ;
  
  fhEPhotonSM = new TH2F("hEPhotonSM","Selected clusters E and super-module number",
                          nptbins,ptmin,ptmax,
                          GetCaloUtils()->GetNumberOfSuperModulesUsed(),0,GetCaloUtils()->GetNumberOfSuperModulesUsed());
  fhEPhotonSM->SetYTitle("SuperModule ");
  fhEPhotonSM->SetXTitle("#it{E} (GeV)");
  outputContainer->Add(fhEPhotonSM) ;
  
  fhPtPhotonSM = new TH2F("hPtPhotonSM","Selected clusters #it{p}_{T} and super-module number",
                           nptbins,ptmin,ptmax,
                           GetCaloUtils()->GetNumberOfSuperModulesUsed(),0,GetCaloUtils()->GetNumberOfSuperModulesUsed());
  fhPtPhotonSM->SetYTitle("SuperModule ");
  fhPtPhotonSM->SetXTitle("#it{p}_{T} (GeV/#it{c})");
  outputContainer->Add(fhPtPhotonSM) ;
  
  fhNCellsE  = new TH2F ("hNCellsE","# of cells in cluster vs E of clusters", nptbins,ptmin,ptmax, nbins,nmin,nmax);
  fhNCellsE->SetXTitle("#it{E} (GeV)");
  fhNCellsE->SetYTitle("# of cells in cluster");
  outputContainer->Add(fhNCellsE);
  
  fhCellsE  = new TH2F ("hCellsE","energy of cells in cluster vs E of clusters", nptbins,ptmin,ptmax, nptbins*2,ptmin,ptmax);
  fhCellsE->SetXTitle("#it{E}_{cluster} (GeV)");
  fhCellsE->SetYTitle("#it{E}_{cell} (GeV)");
  outputContainer->Add(fhCellsE);
  
  fhTimePt  = new TH2F ("hTimePt","time of cluster vs pT of clusters", nptbins,ptmin,ptmax, ntimebins,timemin,timemax);
  fhTimePt->SetXTitle("#it{p}_{T} (GeV/#it{c})");
  fhTimePt->SetYTitle("#it{time} (ns)");
  outputContainer->Add(fhTimePt);
  
  fhMaxCellDiffClusterE  = new TH2F ("hMaxCellDiffClusterE","energy vs difference of cluster energy - max cell energy / cluster energy, good clusters",
                                     nptbins,ptmin,ptmax, 500,0,1.);
  fhMaxCellDiffClusterE->SetXTitle("#it{E}_{cluster} (GeV) ");
  fhMaxCellDiffClusterE->SetYTitle("(#it{E}_{cluster} - #it{E}_{cell max})/ #it{E}_{cluster}");
  outputContainer->Add(fhMaxCellDiffClusterE);
  
  fhEPhoton  = new TH1F("hEPhoton","Number of #gamma over calorimeter vs energy",nptbins,ptmin,ptmax);
  fhEPhoton->SetYTitle("#it{counts}");
  fhEPhoton->SetXTitle("#it{E}_{#gamma}(GeV)");
  outputContainer->Add(fhEPhoton) ;
  
  fhPtPhoton  = new TH1F("hPtPhoton","Number of #gamma over calorimeter vs #it{p}_{T}",nptbins,ptmin,ptmax);
  fhPtPhoton->SetYTitle("#it{counts}");
  fhPtPhoton->SetXTitle("p_{T #gamma}(GeV/#it{c})");
  outputContainer->Add(fhPtPhoton) ;
  
  if(IsHighMultiplicityAnalysisOn())
  {
    fhPtCentralityPhoton  = new TH2F("hPtCentralityPhoton","centrality vs #it{p}_{T}",nptbins,ptmin,ptmax, 100,0,100);
    fhPtCentralityPhoton->SetYTitle("Centrality");
    fhPtCentralityPhoton->SetXTitle("#it{p}_{T}(GeV/#it{c})");
    outputContainer->Add(fhPtCentralityPhoton) ;
    
    fhPtEventPlanePhoton  = new TH2F("hPtEventPlanePhoton","centrality vs #it{p}_{T}",nptbins,ptmin,ptmax, 100,0,TMath::Pi());
    fhPtEventPlanePhoton->SetYTitle("Event plane angle (rad)");
    fhPtEventPlanePhoton->SetXTitle("#it{p}_{T} (GeV/#it{c})");
    outputContainer->Add(fhPtEventPlanePhoton) ;
  }
  
  fhEtaPhi  = new TH2F
  ("hEtaPhi","cluster,#it{E} > 0.5 GeV, #eta vs #phi",netabins,etamin,etamax,nphibins,phimin,phimax);
  fhEtaPhi->SetYTitle("#phi (rad)");
  fhEtaPhi->SetXTitle("#eta");
  outputContainer->Add(fhEtaPhi) ;
  
  fhPhiPhoton  = new TH2F
  ("hPhiPhoton","#phi_{#gamma} vs #it{p}_{T}",nptbins,ptmin,ptmax,nphibins,phimin,phimax);
  fhPhiPhoton->SetYTitle("#phi (rad)");
  fhPhiPhoton->SetXTitle("p_{T #gamma} (GeV/#it{c})");
  outputContainer->Add(fhPhiPhoton) ;
  
  fhEtaPhoton  = new TH2F
  ("hEtaPhoton","#eta_{#gamma} vs #it{p}_{T}",nptbins,ptmin,ptmax,netabins,etamin,etamax);
  fhEtaPhoton->SetYTitle("#eta");
  fhEtaPhoton->SetXTitle("p_{T #gamma} (GeV/#it{c})");
  outputContainer->Add(fhEtaPhoton) ;
  
  fhEtaPhiPhoton  = new TH2F
  ("hEtaPhiPhoton","#eta vs #phi",netabins,etamin,etamax,nphibins,phimin,phimax);
  fhEtaPhiPhoton->SetYTitle("#phi (rad)");
  fhEtaPhiPhoton->SetXTitle("#eta");
  outputContainer->Add(fhEtaPhiPhoton) ;
  if(GetMinPt() < 0.5)
  {
    fhEtaPhi05Photon  = new TH2F
    ("hEtaPhi05Photon","#eta vs #phi, E < 0.5",netabins,etamin,etamax,nphibins,phimin,phimax);
    fhEtaPhi05Photon->SetYTitle("#phi (rad)");
    fhEtaPhi05Photon->SetXTitle("#eta");
    outputContainer->Add(fhEtaPhi05Photon) ;
  }
  
  fhNLocMax = new TH2F("hNLocMax","Number of local maxima in cluster",
                       nptbins,ptmin,ptmax,10,0,10);
  fhNLocMax ->SetYTitle("N maxima");
  fhNLocMax ->SetXTitle("#it{E} (GeV)");
  outputContainer->Add(fhNLocMax) ;
  
  //Shower shape
  if(fFillSSHistograms)
  {
    fhLam0E  = new TH2F ("hLam0E","#lambda_{0}^{2} vs E", nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
    fhLam0E->SetYTitle("#lambda_{0}^{2}");
    fhLam0E->SetXTitle("#it{E} (GeV)");
    outputContainer->Add(fhLam0E);

    fhLam0Pt  = new TH2F ("hLam0Pt","#lambda_{0}^{2} vs #it{p}_{T}", nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
    fhLam0Pt->SetYTitle("#lambda_{0}^{2}");
    fhLam0Pt->SetXTitle("#it{p}_{T} (GeV/#it{c})");
    outputContainer->Add(fhLam0Pt);
    
    fhLam1E  = new TH2F ("hLam1E","#lambda_{1}^{2} vs E", nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
    fhLam1E->SetYTitle("#lambda_{1}^{2}");
    fhLam1E->SetXTitle("#it{E} (GeV)");
    outputContainer->Add(fhLam1E);

    fhLam1Pt  = new TH2F ("hLam1Pt","#lambda_{1}^{2} vs E", nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
    fhLam1Pt->SetYTitle("#lambda_{1}^{2}");
    fhLam1Pt->SetXTitle("#it{p}_{T} (GeV/#it{c})");
    outputContainer->Add(fhLam1Pt);

    fhDispE  = new TH2F ("hDispE"," dispersion^{2} vs E", nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
    fhDispE->SetYTitle("D^{2}");
    fhDispE->SetXTitle("#it{E} (GeV) ");
    outputContainer->Add(fhDispE);

    fhDispPt  = new TH2F ("hDispPt"," dispersion^{2} vs #it{p}_{T}", nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
    fhDispPt->SetYTitle("D^{2}");
    fhDispPt->SetXTitle("#it{p}_{T} (GeV/#it{c}) ");
    outputContainer->Add(fhDispPt);
    
    if(fFillSSNLocMaxHisto)
    {
      fhLam0PtNLM1  = new TH2F ("hLam0PtNLM1","#lambda_{0}^{2} vs #it{p}_{T}, #it{n}_{LM}=1", nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
      fhLam0PtNLM1->SetYTitle("#lambda_{0}^{2}");
      fhLam0PtNLM1->SetXTitle("#it{p}_{T} (GeV/#it{c})");
      outputContainer->Add(fhLam0PtNLM1);
      
      fhLam0PtNLM2  = new TH2F ("hLam0PtNLM2","#lambda_{0}^{2} vs #it{p}_{T}, #it{n}_{LM}=2", nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
      fhLam0PtNLM2->SetYTitle("#lambda_{0}^{2}");
      fhLam0PtNLM2->SetXTitle("#it{p}_{T} (GeV/#it{c})");
      outputContainer->Add(fhLam0PtNLM2);

      fhLam1PtNLM1  = new TH2F ("hLam1PtNLM1","#lambda_{1}^{2} vs #it{p}_{T}, #it{n}_{LM}=1", nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
      fhLam1PtNLM1->SetYTitle("#lambda_{1}^{2}");
      fhLam1PtNLM1->SetXTitle("#it{p}_{T} (GeV/#it{c})");
      outputContainer->Add(fhLam1PtNLM1);
      
      fhLam1PtNLM2  = new TH2F ("hLam1PtNLM2","#lambda_{1}^{2} vs #it{p}_{T}, #it{n}_{LM}=2", nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
      fhLam1PtNLM2->SetYTitle("#lambda_{1}^{2}");
      fhLam1PtNLM2->SetXTitle("#it{p}_{T} (GeV/#it{c})");
      outputContainer->Add(fhLam1PtNLM2);      
    }
    
    if(!fRejectTrackMatch)
    {
      fhLam0ETM  = new TH2F ("hLam0ETM","#lambda_{0}^{2} vs E, cut on track-matching residual |#Delta #eta| < 0.05,  |#Delta #phi| < 0.05", nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
      fhLam0ETM->SetYTitle("#lambda_{0}^{2}");
      fhLam0ETM->SetXTitle("#it{E} (GeV)");
      outputContainer->Add(fhLam0ETM);

      fhLam0PtTM  = new TH2F ("hLam0PtTM","#lambda_{0}^{2} vs #it{p}_{T}, cut on track-matching residual |#Delta #eta| < 0.05,  |#Delta #phi| < 0.05", nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
      fhLam0PtTM->SetYTitle("#lambda_{0}^{2}");
      fhLam0PtTM->SetXTitle("#it{p}_{T} (GeV/#it{c})");
      outputContainer->Add(fhLam0PtTM);
      
      fhLam1ETM  = new TH2F ("hLam1ETM","#lambda_{1}^{2} vs E, cut on track-matching residual |#Delta #eta| < 0.05,  |#Delta #phi| < 0.05", nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
      fhLam1ETM->SetYTitle("#lambda_{1}^{2}");
      fhLam1ETM->SetXTitle("#it{E} (GeV)");
      outputContainer->Add(fhLam1ETM);
      
      fhDispETM  = new TH2F ("hDispETM"," dispersion^{2} vs E, cut on track-matching residual |#Delta #eta| < 0.05,  |#Delta #phi| < 0.05", nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
      fhDispETM->SetYTitle("D^{2}");
      fhDispETM->SetXTitle("#it{E} (GeV) ");
      outputContainer->Add(fhDispETM);
    }
    
    if(GetCalorimeter() == kEMCAL &&  GetFirstSMCoveredByTRD() >= 0)
    {
      fhLam0ETRD  = new TH2F ("hLam0ETRD","#lambda_{0}^{2} vs E, EMCAL SM covered by TRD", nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
      fhLam0ETRD->SetYTitle("#lambda_{0}^{2}");
      fhLam0ETRD->SetXTitle("#it{E} (GeV)");
      outputContainer->Add(fhLam0ETRD);
      
      fhLam0PtTRD  = new TH2F ("hLam0PtTRD","#lambda_{0}^{2} vs #it{p}_{T}, EMCAL SM covered by TRD", nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
      fhLam0PtTRD->SetYTitle("#lambda_{0}^{2}");
      fhLam0PtTRD->SetXTitle("#it{p}_{T} (GeV/#it{c})");
      outputContainer->Add(fhLam0PtTRD);
      
      fhLam1ETRD  = new TH2F ("hLam1ETRD","#lambda_{1}^{2} vs E, EMCAL SM covered by TRD", nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
      fhLam1ETRD->SetYTitle("#lambda_{1}^{2}");
      fhLam1ETRD->SetXTitle("#it{E} (GeV)");
      outputContainer->Add(fhLam1ETRD);
      
      fhDispETRD  = new TH2F ("hDispETRD"," dispersion^{2} vs E, EMCAL SM covered by TRD", nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
      fhDispETRD->SetYTitle("Dispersion^{2}");
      fhDispETRD->SetXTitle("#it{E} (GeV) ");
      outputContainer->Add(fhDispETRD);
      
      if(!fRejectTrackMatch &&  GetFirstSMCoveredByTRD() >=0 )
      {
        fhLam0ETMTRD  = new TH2F ("hLam0ETMTRD","#lambda_{0}^{2} vs E, EMCAL SM covered by TRD, cut on track-matching residual |#Delta #eta| < 0.05,  |#Delta #phi| < 0.05", nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
        fhLam0ETMTRD->SetYTitle("#lambda_{0}^{2}");
        fhLam0ETMTRD->SetXTitle("#it{E} (GeV)");
        outputContainer->Add(fhLam0ETMTRD);
        
        fhLam0PtTMTRD  = new TH2F ("hLam0PtTMTRD","#lambda_{0}^{2} vs #it{p}_{T}, EMCAL SM covered by TRD, cut on track-matching residual |#Delta #eta| < 0.05,  |#Delta #phi| < 0.05", nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
        fhLam0PtTMTRD->SetYTitle("#lambda_{0}^{2}");
        fhLam0PtTMTRD->SetXTitle("#it{p}_{T} (GeV/#it{c})");
        outputContainer->Add(fhLam0PtTMTRD);
        
        fhLam1ETMTRD  = new TH2F ("hLam1ETMTRD","#lambda_{1}^{2} vs E, EMCAL SM covered by TRD, cut on track-matching residual |#Delta #eta| < 0.05,  |#Delta #phi| < 0.05", nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
        fhLam1ETMTRD->SetYTitle("#lambda_{1}^{2}");
        fhLam1ETMTRD->SetXTitle("#it{E} (GeV)");
        outputContainer->Add(fhLam1ETMTRD);
        
        fhDispETMTRD  = new TH2F ("hDispETMTRD"," dispersion^{2} vs E, EMCAL SM covered by TRD, cut on track-matching residual |#Delta #eta| < 0.05,  |#Delta #phi| < 0.05", nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
        fhDispETMTRD->SetYTitle("Dispersion^{2}");
        fhDispETMTRD->SetXTitle("#it{E} (GeV) ");
        outputContainer->Add(fhDispETMTRD);
      }
    }
    
    if(!fFillOnlySimpleSSHisto)
    {
      fhNCellsLam0LowE  = new TH2F ("hNCellsLam0LowE","N_{cells} in cluster vs #lambda_{0}^{2}, E < 2 GeV", nbins,nmin, nmax, ssbins,ssmin,ssmax);
      fhNCellsLam0LowE->SetXTitle("N_{cells}");
      fhNCellsLam0LowE->SetYTitle("#lambda_{0}^{2}");
      outputContainer->Add(fhNCellsLam0LowE);
      
      fhNCellsLam0HighE  = new TH2F ("hNCellsLam0HighE","N_{cells} in cluster vs #lambda_{0}^{2}, #it{E} > 2 GeV", nbins,nmin, nmax, ssbins,ssmin,ssmax);
      fhNCellsLam0HighE->SetXTitle("N_{cells}");
      fhNCellsLam0HighE->SetYTitle("#lambda_{0}^{2}");
      outputContainer->Add(fhNCellsLam0HighE);
      
      fhNCellsLam1LowE  = new TH2F ("hNCellsLam1LowE","N_{cells} in cluster vs #lambda_{1}^{2}, E < 2 GeV", nbins,nmin, nmax, ssbins,ssmin,ssmax);
      fhNCellsLam1LowE->SetXTitle("N_{cells}");
      fhNCellsLam1LowE->SetYTitle("#lambda_{0}^{2}");
      outputContainer->Add(fhNCellsLam1LowE);
      
      fhNCellsLam1HighE  = new TH2F ("hNCellsLam1HighE","N_{cells} in cluster vs #lambda_{1}^{2}, #it{E} > 2 GeV", nbins,nmin, nmax, ssbins,ssmin,ssmax);
      fhNCellsLam1HighE->SetXTitle("N_{cells}");
      fhNCellsLam1HighE->SetYTitle("#lambda_{0}^{2}");
      outputContainer->Add(fhNCellsLam1HighE);
      
      fhNCellsDispLowE  = new TH2F ("hNCellsDispLowE","N_{cells} in cluster vs dispersion^{2}, E < 2 GeV", nbins,nmin, nmax, ssbins,ssmin,ssmax);
      fhNCellsDispLowE->SetXTitle("N_{cells}");
      fhNCellsDispLowE->SetYTitle("D^{2}");
      outputContainer->Add(fhNCellsDispLowE);
      
      fhNCellsDispHighE  = new TH2F ("hNCellsDispHighE","N_{cells} in cluster vs dispersion^{2}, E < 2 GeV", nbins,nmin, nmax, ssbins,ssmin,ssmax);
      fhNCellsDispHighE->SetXTitle("N_{cells}");
      fhNCellsDispHighE->SetYTitle("D^{2}");
      outputContainer->Add(fhNCellsDispHighE);
      
      fhEtaLam0LowE  = new TH2F ("hEtaLam0LowE","#eta vs #lambda_{0}^{2}, E < 2 GeV", netabins,etamin,etamax, ssbins,ssmin,ssmax);
      fhEtaLam0LowE->SetYTitle("#lambda_{0}^{2}");
      fhEtaLam0LowE->SetXTitle("#eta");
      outputContainer->Add(fhEtaLam0LowE);
      
      fhPhiLam0LowE  = new TH2F ("hPhiLam0LowE","#phi vs #lambda_{0}^{2}, E < 2 GeV", nphibins,phimin,phimax, ssbins,ssmin,ssmax);
      fhPhiLam0LowE->SetYTitle("#lambda_{0}^{2}");
      fhPhiLam0LowE->SetXTitle("#phi");
      outputContainer->Add(fhPhiLam0LowE);
      
      fhEtaLam0HighE  = new TH2F ("hEtaLam0HighE","#eta vs #lambda_{0}^{2}, #it{E} > 2 GeV", netabins,etamin,etamax, ssbins,ssmin,ssmax);
      fhEtaLam0HighE->SetYTitle("#lambda_{0}^{2}");
      fhEtaLam0HighE->SetXTitle("#eta");
      outputContainer->Add(fhEtaLam0HighE);
      
      fhPhiLam0HighE  = new TH2F ("hPhiLam0HighE","#phi vs #lambda_{0}^{2}, #it{E} > 2 GeV", nphibins,phimin,phimax, ssbins,ssmin,ssmax);
      fhPhiLam0HighE->SetYTitle("#lambda_{0}^{2}");
      fhPhiLam0HighE->SetXTitle("#phi");
      outputContainer->Add(fhPhiLam0HighE);
      
      fhLam1Lam0LowE  = new TH2F ("hLam1Lam0LowE","#lambda_{0}^{2} vs #lambda_{1}^{2} in cluster of E < 2 GeV",  ssbins,ssmin,ssmax, ssbins,ssmin,ssmax);
      fhLam1Lam0LowE->SetYTitle("#lambda_{0}^{2}");
      fhLam1Lam0LowE->SetXTitle("#lambda_{1}^{2}");
      outputContainer->Add(fhLam1Lam0LowE);
      
      fhLam1Lam0HighE  = new TH2F ("hLam1Lam0HighE","#lambda_{0}^{2} vs #lambda_{1}^{2} in cluster of #it{E} > 2 GeV",  ssbins,ssmin,ssmax, ssbins,ssmin,ssmax);
      fhLam1Lam0HighE->SetYTitle("#lambda_{0}^{2}");
      fhLam1Lam0HighE->SetXTitle("#lambda_{1}^{2}");
      outputContainer->Add(fhLam1Lam0HighE);
      
      fhLam0DispLowE  = new TH2F ("hLam0DispLowE","#lambda_{0}^{2} vs dispersion^{2} in cluster of E < 2 GeV",  ssbins,ssmin,ssmax, ssbins,ssmin,ssmax);
      fhLam0DispLowE->SetXTitle("#lambda_{0}^{2}");
      fhLam0DispLowE->SetYTitle("D^{2}");
      outputContainer->Add(fhLam0DispLowE);
      
      fhLam0DispHighE  = new TH2F ("hLam0DispHighE","#lambda_{0}^{2} vs dispersion^{2} in cluster of #it{E} > 2 GeV",  ssbins,ssmin,ssmax, ssbins,ssmin,ssmax);
      fhLam0DispHighE->SetXTitle("#lambda_{0}^{2}");
      fhLam0DispHighE->SetYTitle("D^{2}");
      outputContainer->Add(fhLam0DispHighE);
      
      fhDispLam1LowE  = new TH2F ("hDispLam1LowE","Dispersion^{2} vs #lambda_{1}^{2} in cluster of E < 2 GeV",  ssbins,ssmin,ssmax, ssbins,ssmin,ssmax);
      fhDispLam1LowE->SetXTitle("D^{2}");
      fhDispLam1LowE->SetYTitle("#lambda_{1}^{2}");
      outputContainer->Add(fhDispLam1LowE);
      
      fhDispLam1HighE  = new TH2F ("hDispLam1HighE","Dispersion^{2} vs #lambda_{1^{2}} in cluster of #it{E} > 2 GeV",  ssbins,ssmin,ssmax, ssbins,ssmin,ssmax);
      fhDispLam1HighE->SetXTitle("D^{2}");
      fhDispLam1HighE->SetYTitle("#lambda_{1}^{2}");
      outputContainer->Add(fhDispLam1HighE);
      
      if(GetCalorimeter() == kEMCAL)
      {
        fhDispEtaE  = new TH2F ("hDispEtaE","#sigma^{2}_{#eta #eta} = #Sigma w_{i}(#eta_{i} - <#eta>)^{2}/ #Sigma w_{i} vs E",  nptbins,ptmin,ptmax, ssbins,ssmin,ssmax);
        fhDispEtaE->SetXTitle("#it{E} (GeV)");
        fhDispEtaE->SetYTitle("#sigma^{2}_{#eta #eta}");
        outputContainer->Add(fhDispEtaE);
        
        fhDispPhiE  = new TH2F ("hDispPhiE","#sigma^{2}_{#phi #phi} = #Sigma w_{i}(#phi_{i} - <#phi>)^{2} / #Sigma w_{i} vs E",  nptbins,ptmin,ptmax, ssbins,ssmin,ssmax);
        fhDispPhiE->SetXTitle("#it{E} (GeV)");
        fhDispPhiE->SetYTitle("#sigma^{2}_{#phi #phi}");
        outputContainer->Add(fhDispPhiE);
        
        fhSumEtaE  = new TH2F ("hSumEtaE","#delta^{2}_{#eta #eta} = #Sigma w_{i}(#eta_{i})^{2} / #Sigma w_{i} - <#eta>^{2} vs E",  nptbins,ptmin,ptmax, ssbins,ssmin,ssmax);
        fhSumEtaE->SetXTitle("#it{E} (GeV)");
        fhSumEtaE->SetYTitle("#delta^{2}_{#eta #eta}");
        outputContainer->Add(fhSumEtaE);
        
        fhSumPhiE  = new TH2F ("hSumPhiE","#delta^{2}_{#phi #phi} = #Sigma w_{i}(#phi_{i})^{2}/ #Sigma w_{i} - <#phi>^{2} vs E",
                               nptbins,ptmin,ptmax, ssbins,ssmin,ssmax);
        fhSumPhiE->SetXTitle("#it{E} (GeV)");
        fhSumPhiE->SetYTitle("#delta^{2}_{#phi #phi}");
        outputContainer->Add(fhSumPhiE);
        
        fhSumEtaPhiE  = new TH2F ("hSumEtaPhiE","#delta^{2}_{#eta #phi} = #Sigma w_{i}(#phi_{i} #eta_{i} ) / #Sigma w_{i} - <#phi><#eta> vs E",
                                  nptbins,ptmin,ptmax, 2*ssbins,-ssmax,ssmax);
        fhSumEtaPhiE->SetXTitle("#it{E} (GeV)");
        fhSumEtaPhiE->SetYTitle("#delta^{2}_{#eta #phi}");
        outputContainer->Add(fhSumEtaPhiE);
        
        fhDispEtaPhiDiffE  = new TH2F ("hDispEtaPhiDiffE","#sigma^{2}_{#phi #phi} - #sigma^{2}_{#eta #eta} vs E",
                                       nptbins,ptmin,ptmax,200, -10,10);
        fhDispEtaPhiDiffE->SetXTitle("#it{E} (GeV)");
        fhDispEtaPhiDiffE->SetYTitle("#sigma^{2}_{#phi #phi}-#sigma^{2}_{#eta #eta}");
        outputContainer->Add(fhDispEtaPhiDiffE);
        
        fhSphericityE  = new TH2F ("hSphericityE","(#sigma^{2}_{#phi #phi} - #sigma^{2}_{#eta #eta}) / (#sigma^{2}_{#eta #eta} + #sigma^{2}_{#phi #phi}) vs E",
                                   nptbins,ptmin,ptmax, 200, -1,1);
        fhSphericityE->SetXTitle("#it{E} (GeV)");
        fhSphericityE->SetYTitle("s = (#sigma^{2}_{#phi #phi} - #sigma^{2}_{#eta #eta}) / (#sigma^{2}_{#eta #eta} + #sigma^{2}_{#phi #phi})");
        outputContainer->Add(fhSphericityE);
        
        fhDispSumEtaDiffE  = new TH2F ("hDispSumEtaDiffE","#sigma^{2}_{#eta #eta} - #delta^{2}_{#eta #eta} / average vs E",  nptbins,ptmin,ptmax, 200,-0.01,0.01);
        fhDispSumEtaDiffE->SetXTitle("#it{E} (GeV)");
        fhDispSumEtaDiffE->SetYTitle("#sigma^{2}_{#eta #eta} - #delta^{2}_{#eta #eta} / average");
        outputContainer->Add(fhDispSumEtaDiffE);
        
        fhDispSumPhiDiffE  = new TH2F ("hDispSumPhiDiffE","#sigma^{2}_{#phi #phi} - #delta^{2}_{#phi #phi} / average vs E",  nptbins,ptmin,ptmax, 200,-0.01,0.01);
        fhDispSumPhiDiffE->SetXTitle("#it{E} (GeV)");
        fhDispSumPhiDiffE->SetYTitle("#sigma^{2}_{#phi #phi} - #delta^{2}_{#phi #phi} / average");
        outputContainer->Add(fhDispSumPhiDiffE);
        
        for(Int_t i = 0; i < 7; i++)
        {
          fhDispEtaDispPhi[i] = new TH2F (Form("hDispEtaDispPhi_EBin%d",i),Form("#sigma^{2}_{#phi #phi} vs #sigma^{2}_{#eta #eta} for %d < E < %d GeV",bin[i],bin[i+1]),
                                          ssbins,ssmin,ssmax , ssbins,ssmin,ssmax);
          fhDispEtaDispPhi[i]->SetXTitle("#sigma^{2}_{#eta #eta}");
          fhDispEtaDispPhi[i]->SetYTitle("#sigma^{2}_{#phi #phi}");
          outputContainer->Add(fhDispEtaDispPhi[i]);
          
          fhLambda0DispEta[i] = new TH2F (Form("hLambda0DispEta_EBin%d",i),Form("#lambda^{2}_{0} vs #sigma^{2}_{#eta #eta} for %d < E < %d GeV",bin[i],bin[i+1]),
                                          ssbins,ssmin,ssmax , ssbins,ssmin,ssmax);
          fhLambda0DispEta[i]->SetXTitle("#lambda^{2}_{0}");
          fhLambda0DispEta[i]->SetYTitle("#sigma^{2}_{#eta #eta}");
          outputContainer->Add(fhLambda0DispEta[i]);
          
          fhLambda0DispPhi[i] = new TH2F (Form("hLambda0DispPhi_EBin%d",i),Form("#lambda^{2}_{0}} vs #sigma^{2}_{#phi #phi} for %d < E < %d GeV",bin[i],bin[i+1]),
                                          ssbins,ssmin,ssmax , ssbins,ssmin,ssmax);
          fhLambda0DispPhi[i]->SetXTitle("#lambda^{2}_{0}");
          fhLambda0DispPhi[i]->SetYTitle("#sigma^{2}_{#phi #phi}");
          outputContainer->Add(fhLambda0DispPhi[i]);
        }
      }
    }
  } // Shower shape
  
  // Track Matching
  
  if(fFillTMHisto)
  {
    TString cutTM [] = {"NoCut",""};
    
    for(Int_t i = 0; i < 2; i++)
    {
      fhTrackMatchedDEta[i]  = new TH2F
      (Form("hTrackMatchedDEta%s",cutTM[i].Data()),
       Form("d#eta of cluster-track vs cluster energy, %s",cutTM[i].Data()),
       nptbins,ptmin,ptmax,nresetabins,resetamin,resetamax);
      fhTrackMatchedDEta[i]->SetYTitle("d#eta");
      fhTrackMatchedDEta[i]->SetXTitle("#it{E}_{cluster} (GeV)");
      
      fhTrackMatchedDPhi[i]  = new TH2F
      (Form("hTrackMatchedDPhi%s",cutTM[i].Data()),
       Form("d#phi of cluster-track vs cluster energy, %s",cutTM[i].Data()),
       nptbins,ptmin,ptmax,nresphibins,resphimin,resphimax);
      fhTrackMatchedDPhi[i]->SetYTitle("d#phi (rad)");
      fhTrackMatchedDPhi[i]->SetXTitle("#it{E}_{cluster} (GeV)");
      
      fhTrackMatchedDEtaDPhi[i]  = new TH2F
      (Form("hTrackMatchedDEtaDPhi%s",cutTM[i].Data()),
       Form("d#eta vs d#phi of cluster-track vs cluster energy, %s",cutTM[i].Data()),
       nresetabins,resetamin,resetamax,nresphibins,resphimin,resphimax);
      fhTrackMatchedDEtaDPhi[i]->SetYTitle("d#phi (rad)");
      fhTrackMatchedDEtaDPhi[i]->SetXTitle("d#eta");
      
      fhTrackMatchedDEtaPos[i]  = new TH2F
      (Form("hTrackMatchedDEtaPos%s",cutTM[i].Data()),
       Form("d#eta of cluster-track vs cluster energy, %s",cutTM[i].Data()),
       nptbins,ptmin,ptmax,nresetabins,resetamin,resetamax);
      fhTrackMatchedDEtaPos[i]->SetYTitle("d#eta");
      fhTrackMatchedDEtaPos[i]->SetXTitle("#it{E}_{cluster} (GeV)");
      
      fhTrackMatchedDPhiPos[i]  = new TH2F
      (Form("hTrackMatchedDPhiPos%s",cutTM[i].Data()),
       Form("d#phi of cluster-track vs cluster energy, %s",cutTM[i].Data()),
       nptbins,ptmin,ptmax,nresphibins,resphimin,resphimax);
      fhTrackMatchedDPhiPos[i]->SetYTitle("d#phi (rad)");
      fhTrackMatchedDPhiPos[i]->SetXTitle("#it{E}_{cluster} (GeV)");
      
      fhTrackMatchedDEtaDPhiPos[i]  = new TH2F
      (Form("hTrackMatchedDEtaDPhiPos%s",cutTM[i].Data()),
       Form("d#eta vs d#phi of cluster-track vs cluster energy, %s",cutTM[i].Data()),
       nresetabins,resetamin,resetamax,nresphibins,resphimin,resphimax);
      fhTrackMatchedDEtaDPhiPos[i]->SetYTitle("d#phi (rad)");
      fhTrackMatchedDEtaDPhiPos[i]->SetXTitle("d#eta");
      
      fhTrackMatchedDEtaNeg[i]  = new TH2F
      (Form("hTrackMatchedDEtaNeg%s",cutTM[i].Data()),
       Form("d#eta of cluster-track vs cluster energy, %s",cutTM[i].Data()),
       nptbins,ptmin,ptmax,nresetabins,resetamin,resetamax);
      fhTrackMatchedDEtaNeg[i]->SetYTitle("d#eta");
      fhTrackMatchedDEtaNeg[i]->SetXTitle("#it{E}_{cluster} (GeV)");
      
      fhTrackMatchedDPhiNeg[i]  = new TH2F
      (Form("hTrackMatchedDPhiNeg%s",cutTM[i].Data()),
       Form("d#phi of cluster-track vs cluster energy, %s",cutTM[i].Data()),
       nptbins,ptmin,ptmax,nresphibins,resphimin,resphimax);
      fhTrackMatchedDPhiNeg[i]->SetYTitle("d#phi (rad)");
      fhTrackMatchedDPhiNeg[i]->SetXTitle("#it{E}_{cluster} (GeV)");
      
      fhTrackMatchedDEtaDPhiNeg[i]  = new TH2F
      (Form("hTrackMatchedDEtaDPhiNeg%s",cutTM[i].Data()),
       Form("d#eta vs d#phi of cluster-track vs cluster energy, %s",cutTM[i].Data()),
       nresetabins,resetamin,resetamax,nresphibins,resphimin,resphimax);
      fhTrackMatchedDEtaDPhiNeg[i]->SetYTitle("d#phi (rad)");
      fhTrackMatchedDEtaDPhiNeg[i]->SetXTitle("d#eta");
      
      fhdEdx[i]  = new TH2F (Form("hdEdx%s",cutTM[i].Data()),Form("matched track <dE/dx> vs cluster E, %s",cutTM[i].Data()),
                             nptbins,ptmin,ptmax,ndedxbins, dedxmin, dedxmax);
      fhdEdx[i]->SetXTitle("#it{E} (GeV)");
      fhdEdx[i]->SetYTitle("<dE/dx>");
      
      fhEOverP[i]  = new TH2F (Form("hEOverP%s",cutTM[i].Data()),Form("matched track E/p vs cluster E, %s",cutTM[i].Data()),
                               nptbins,ptmin,ptmax,nPoverEbins,pOverEmin,pOverEmax);
      fhEOverP[i]->SetXTitle("#it{E} (GeV)");
      fhEOverP[i]->SetYTitle("E/p");
      
      outputContainer->Add(fhTrackMatchedDEta[i]) ;
      outputContainer->Add(fhTrackMatchedDPhi[i]) ;
      outputContainer->Add(fhTrackMatchedDEtaDPhi[i]) ;
      outputContainer->Add(fhTrackMatchedDEtaPos[i]) ;
      outputContainer->Add(fhTrackMatchedDPhiPos[i]) ;
      outputContainer->Add(fhTrackMatchedDEtaDPhiPos[i]) ;
      outputContainer->Add(fhTrackMatchedDEtaNeg[i]) ;
      outputContainer->Add(fhTrackMatchedDPhiNeg[i]) ;
      outputContainer->Add(fhTrackMatchedDEtaDPhiNeg[i]) ;
      outputContainer->Add(fhdEdx[i]);
      outputContainer->Add(fhEOverP[i]);
      
      if(GetCalorimeter()==kEMCAL &&  GetFirstSMCoveredByTRD() >=0 )
      {
        fhTrackMatchedDEtaTRD[i]  = new TH2F
        (Form("hTrackMatchedDEtaTRD%s",cutTM[i].Data()),
         Form("d#eta of cluster-track vs cluster energy, SM behind TRD, %s",cutTM[i].Data()),
         nptbins,ptmin,ptmax,nresetabins,resetamin,resetamax);
        fhTrackMatchedDEtaTRD[i]->SetYTitle("d#eta");
        fhTrackMatchedDEtaTRD[i]->SetXTitle("#it{E}_{cluster} (GeV)");
        
        fhTrackMatchedDPhiTRD[i]  = new TH2F
        (Form("hTrackMatchedDPhiTRD%s",cutTM[i].Data()),
         Form("d#phi of cluster-track vs cluster energy, SM behing TRD, %s",cutTM[i].Data()),
         nptbins,ptmin,ptmax,nresphibins,resphimin,resphimax);
        fhTrackMatchedDPhiTRD[i]->SetYTitle("d#phi (rad)");
        fhTrackMatchedDPhiTRD[i]->SetXTitle("#it{E}_{cluster} (GeV)");
        
        fhEOverPTRD[i]  = new TH2F
        (Form("hEOverPTRD%s",cutTM[i].Data()),
         Form("matched track E/p vs cluster E, behind TRD, %s",cutTM[i].Data()),
         nptbins,ptmin,ptmax,nPoverEbins,pOverEmin,pOverEmax);
        fhEOverPTRD[i]->SetXTitle("#it{E} (GeV)");
        fhEOverPTRD[i]->SetYTitle("E/p");
        
        outputContainer->Add(fhTrackMatchedDEtaTRD[i]) ;
        outputContainer->Add(fhTrackMatchedDPhiTRD[i]) ;
        outputContainer->Add(fhEOverPTRD[i]);
      }
      
      if(IsDataMC())
      {
        fhTrackMatchedDEtaMCNoOverlap[i]  = new TH2F
        (Form("hTrackMatchedDEtaMCNoOverlap%s",cutTM[i].Data()),
         Form("d#eta of cluster-track vs cluster energy, no other MC particles overlap %s",cutTM[i].Data()),
         nptbins,ptmin,ptmax,nresetabins,resetamin,resetamax);
        fhTrackMatchedDEtaMCNoOverlap[i]->SetYTitle("d#eta");
        fhTrackMatchedDEtaMCNoOverlap[i]->SetXTitle("#it{E}_{cluster} (GeV)");
        
        fhTrackMatchedDPhiMCNoOverlap[i]  = new TH2F
        (Form("hTrackMatchedDPhiMCNoOverlap%s",cutTM[i].Data()),
         Form("d#phi of cluster-track vs cluster energy, no other MC particles overlap %s",cutTM[i].Data()),
         nptbins,ptmin,ptmax,nresphibins,resphimin,resphimax);
        fhTrackMatchedDPhiMCNoOverlap[i]->SetYTitle("d#phi (rad)");
        fhTrackMatchedDPhiMCNoOverlap[i]->SetXTitle("#it{E}_{cluster} (GeV)");
        
        outputContainer->Add(fhTrackMatchedDEtaMCNoOverlap[i]) ;
        outputContainer->Add(fhTrackMatchedDPhiMCNoOverlap[i]) ;
        fhTrackMatchedDEtaMCOverlap[i]  = new TH2F
        (Form("hTrackMatchedDEtaMCOverlap%s",cutTM[i].Data()),
         Form("d#eta of cluster-track vs cluster energy, several MC particles overlap %s",cutTM[i].Data()),
         nptbins,ptmin,ptmax,nresetabins,resetamin,resetamax);
        fhTrackMatchedDEtaMCOverlap[i]->SetYTitle("d#eta");
        fhTrackMatchedDEtaMCOverlap[i]->SetXTitle("#it{E}_{cluster} (GeV)");
        
        fhTrackMatchedDPhiMCOverlap[i]  = new TH2F
        (Form("hTrackMatchedDPhiMCOverlap%s",cutTM[i].Data()),
         Form("d#phi of cluster-track vs cluster energy, several MC particles overlap %s",cutTM[i].Data()),
         nptbins,ptmin,ptmax,nresphibins,resphimin,resphimax);
        fhTrackMatchedDPhiMCOverlap[i]->SetYTitle("d#phi (rad)");
        fhTrackMatchedDPhiMCOverlap[i]->SetXTitle("#it{E}_{cluster} (GeV)");
        
        outputContainer->Add(fhTrackMatchedDEtaMCOverlap[i]) ;
        outputContainer->Add(fhTrackMatchedDPhiMCOverlap[i]) ;
        
        fhTrackMatchedDEtaMCConversion[i]  = new TH2F
        (Form("hTrackMatchedDEtaMCConversion%s",cutTM[i].Data()),
         Form("d#eta of cluster-track vs cluster energy, no other MC particles overlap appart from conversions %s",cutTM[i].Data()),
         nptbins,ptmin,ptmax,nresetabins,resetamin,resetamax);
        fhTrackMatchedDEtaMCConversion[i]->SetYTitle("d#eta");
        fhTrackMatchedDEtaMCConversion[i]->SetXTitle("#it{E}_{cluster} (GeV)");
        
        fhTrackMatchedDPhiMCConversion[i]  = new TH2F
        (Form("hTrackMatchedDPhiMCConversion%s",cutTM[i].Data()),
         Form("d#phi of cluster-track vs cluster energy, no other MC particles overlap appart from conversions %s",cutTM[i].Data()),
         nptbins,ptmin,ptmax,nresphibins,resphimin,resphimax);
        fhTrackMatchedDPhiMCConversion[i]->SetYTitle("d#phi (rad)");
        fhTrackMatchedDPhiMCConversion[i]->SetXTitle("#it{E}_{cluster} (GeV)");
        
        outputContainer->Add(fhTrackMatchedDEtaMCConversion[i]) ;
        outputContainer->Add(fhTrackMatchedDPhiMCConversion[i]) ;
        
        fhTrackMatchedMCParticle[i]  = new TH2F
        (Form("hTrackMatchedMCParticle%s",cutTM[i].Data()),
         Form("Origin of particle vs energy %s",cutTM[i].Data()),
         nptbins,ptmin,ptmax,8,0,8);
        fhTrackMatchedMCParticle[i]->SetXTitle("#it{E} (GeV)");
        //fhTrackMatchedMCParticle[i]->SetYTitle("Particle type");
        
        fhTrackMatchedMCParticle[i]->GetYaxis()->SetBinLabel(1 ,"Photon");
        fhTrackMatchedMCParticle[i]->GetYaxis()->SetBinLabel(2 ,"Electron");
        fhTrackMatchedMCParticle[i]->GetYaxis()->SetBinLabel(3 ,"Meson Merged");
        fhTrackMatchedMCParticle[i]->GetYaxis()->SetBinLabel(4 ,"Rest");
        fhTrackMatchedMCParticle[i]->GetYaxis()->SetBinLabel(5 ,"Conv. Photon");
        fhTrackMatchedMCParticle[i]->GetYaxis()->SetBinLabel(6 ,"Conv. Electron");
        fhTrackMatchedMCParticle[i]->GetYaxis()->SetBinLabel(7 ,"Conv. Merged");
        fhTrackMatchedMCParticle[i]->GetYaxis()->SetBinLabel(8 ,"Conv. Rest");
        
        outputContainer->Add(fhTrackMatchedMCParticle[i]);
      }
    }
  }
  
  if(IsPileUpAnalysisOn())
  {
    TString pileUpName[] = {"SPD","EMCAL","SPDOrEMCAL","SPDAndEMCAL","SPDAndNotEMCAL","EMCALAndNotSPD","NotSPDAndNotEMCAL"} ;
    
    for(Int_t i = 0 ; i < 7 ; i++)
    {
      fhPtPhotonPileUp[i]  = new TH1F(Form("hPtPhotonPileUp%s",pileUpName[i].Data()),
                                      Form("Selected photon #it{p}_{T} distribution, %s Pile-Up event",pileUpName[i].Data()), nptbins,ptmin,ptmax);
      fhPtPhotonPileUp[i]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
      outputContainer->Add(fhPtPhotonPileUp[i]);
      
      fhClusterTimeDiffPhotonPileUp[i]  = new TH2F(Form("hClusterTimeDiffPhotonPileUp%s",pileUpName[i].Data()),
                                             Form("Photon cluster E vs #it{t}_{max}-#it{t}_{cell} in cluster, %s Pile-Up event",pileUpName[i].Data()),
                                             nptbins,ptmin,ptmax,400,-200,200);
      fhClusterTimeDiffPhotonPileUp[i]->SetXTitle("#it{E} (GeV)");
      fhClusterTimeDiffPhotonPileUp[i]->SetYTitle("#it{t}_{max}-#it{t}_{cell} (ns)");
      outputContainer->Add(fhClusterTimeDiffPhotonPileUp[i]);
    }
    
    fhTimePtPhotonNoCut  = new TH2F ("hTimePtPhoton_NoCut","time of photon cluster vs pT of clusters, no cut", nptbins,ptmin,ptmax, ntimebins,timemin,timemax);
    fhTimePtPhotonNoCut->SetXTitle("#it{p}_{T} (GeV/#it{c})");
    fhTimePtPhotonNoCut->SetYTitle("#it{time} (ns)");
    outputContainer->Add(fhTimePtPhotonNoCut);
    
    fhTimePtPhotonSPD  = new TH2F ("hTimePtPhoton_SPD","time of  photon cluster vs pT of clusters, SPD cut", nptbins,ptmin,ptmax, ntimebins,timemin,timemax);
    fhTimePtPhotonSPD->SetXTitle("#it{p}_{T} (GeV/#it{c})");
    fhTimePtPhotonSPD->SetYTitle("#it{time} (ns)");
    outputContainer->Add(fhTimePtPhotonSPD);
    
    fhTimeNPileUpVertSPD  = new TH2F ("hTime_NPileUpVertSPD","time of cluster vs N pile-up SPD vertex", ntimebins,timemin,timemax,20,0,20);
    fhTimeNPileUpVertSPD->SetYTitle("# vertex ");
    fhTimeNPileUpVertSPD->SetXTitle("#it{time} (ns)");
    outputContainer->Add(fhTimeNPileUpVertSPD);
    
    fhTimeNPileUpVertTrack  = new TH2F ("hTime_NPileUpVertTracks","time of cluster vs N pile-up Tracks vertex", ntimebins,timemin,timemax, 20,0,20 );
    fhTimeNPileUpVertTrack->SetYTitle("# vertex ");
    fhTimeNPileUpVertTrack->SetXTitle("#it{time} (ns)");
    outputContainer->Add(fhTimeNPileUpVertTrack);
    
    fhPtPhotonNPileUpSPDVtx  = new TH2F ("hPtPhoton_NPileUpVertSPD","pT of cluster vs N pile-up SPD vertex",
                                         nptbins,ptmin,ptmax,20,0,20);
    fhPtPhotonNPileUpSPDVtx->SetYTitle("# vertex ");
    fhPtPhotonNPileUpSPDVtx->SetXTitle("#it{p}_{T} (GeV/#it{c})");
    outputContainer->Add(fhPtPhotonNPileUpSPDVtx);
	  
    fhPtPhotonNPileUpTrkVtx  = new TH2F ("hPtPhoton_NPileUpVertTracks","pT of cluster vs N pile-up Tracks vertex",
                                         nptbins,ptmin,ptmax, 20,0,20 );
    fhPtPhotonNPileUpTrkVtx->SetYTitle("# vertex ");
    fhPtPhotonNPileUpTrkVtx->SetXTitle("#it{p}_{T} (GeV/#it{c})");
    outputContainer->Add(fhPtPhotonNPileUpTrkVtx);
	  
    fhPtPhotonNPileUpSPDVtxTimeCut  = new TH2F ("hPtPhoton_NPileUpVertSPD_TimeCut","pT of cluster vs N pile-up SPD vertex, |tof| < 25 ns",
                                                nptbins,ptmin,ptmax,20,0,20);
    fhPtPhotonNPileUpSPDVtxTimeCut->SetYTitle("# vertex ");
    fhPtPhotonNPileUpSPDVtxTimeCut->SetXTitle("#it{p}_{T} (GeV/#it{c})");
    outputContainer->Add(fhPtPhotonNPileUpSPDVtxTimeCut);
	  
    fhPtPhotonNPileUpTrkVtxTimeCut  = new TH2F ("hPtPhoton_NPileUpVertTracks_TimeCut","pT of cluster vs N pile-up Tracks vertex, |tof| < 25 ns",
                                                nptbins,ptmin,ptmax, 20,0,20 );
    fhPtPhotonNPileUpTrkVtxTimeCut->SetYTitle("# vertex ");
    fhPtPhotonNPileUpTrkVtxTimeCut->SetXTitle("#it{p}_{T} (GeV/#it{c})");
    outputContainer->Add(fhPtPhotonNPileUpTrkVtxTimeCut);
	  
    fhPtPhotonNPileUpSPDVtxTimeCut2  = new TH2F ("hPtPhoton_NPileUpVertSPD_TimeCut2","pT of cluster vs N pile-up SPD vertex, -25 < tof < 75 ns",
                                                 nptbins,ptmin,ptmax,20,0,20);
    fhPtPhotonNPileUpSPDVtxTimeCut2->SetYTitle("# vertex ");
    fhPtPhotonNPileUpSPDVtxTimeCut2->SetXTitle("#it{p}_{T} (GeV/#it{c})");
    outputContainer->Add(fhPtPhotonNPileUpSPDVtxTimeCut2);
	  
    fhPtPhotonNPileUpTrkVtxTimeCut2  = new TH2F ("hPtPhoton_NPileUpVertTracks_TimeCut2","pT of cluster vs N pile-up Tracks vertex,  -25 < tof < 75 ns",
                                                 nptbins,ptmin,ptmax, 20,0,20 );
    fhPtPhotonNPileUpTrkVtxTimeCut2->SetYTitle("# vertex ");
    fhPtPhotonNPileUpTrkVtxTimeCut2->SetXTitle("#it{p}_{T} (GeV/#it{c})");
    outputContainer->Add(fhPtPhotonNPileUpTrkVtxTimeCut2);
    
  }

  if(GetCalorimeter() == kEMCAL && fFillEMCALRegionSSHistograms && fFillSSHistograms)
  {
    for(Int_t ieta = 0; ieta < 4; ieta++) 
    {  
      for(Int_t iphi = 0; iphi < 3; iphi++) 
      {
//        fhLam0EMCALRegion[ieta][iphi] = 
//        new TH2F(Form("hLam0_eta%d_phi%d",ieta,iphi),
//                 Form("cluster from converted photon, #it{p}_{T} vs #lambda_{0}^{2}, region eta %d, phi %d",ieta,iphi),
//                 nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
//        fhLam0EMCALRegion[ieta][iphi]->SetYTitle("#lambda_{0}^{2}");
//        fhLam0EMCALRegion[ieta][iphi]->SetXTitle("#it{p}_{T} (GeV)");
//        outputContainer->Add(fhLam0EMCALRegion[ieta][iphi]) ;
//        
//        if(GetFirstSMCoveredByTRD() >= 0)
//        {
//          fhLam0EMCALRegionTRD[ieta][iphi] = 
//          new TH2F(Form("hLam0TRD_eta%d_phi%d",ieta,iphi),
//                   Form("cluster from converted photon, #it{p}_{T} vs #lambda_{0}^{2}, region eta %d, phi %d, SM covered by TRD",ieta,iphi),
//                   nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
//          fhLam0EMCALRegionTRD[ieta][iphi]->SetYTitle("#lambda_{0}^{2}");
//          fhLam0EMCALRegionTRD[ieta][iphi]->SetXTitle("#it{p}_{T} (GeV)");
//          outputContainer->Add(fhLam0EMCALRegionTRD[ieta][iphi]) ;
//        } // TRD
        
        for(Int_t ism = 0; ism < GetCaloUtils()->GetNumberOfSuperModulesUsed(); ism++) 
        {
          fhLam0EMCALRegionPerSM[ieta][iphi][ism] = 
          new TH2F(Form("hLam0_eta%d_phi%d_sm%d",ieta,iphi,ism),
                   Form("cluster from converted photon, #it{p}_{T} vs #lambda_{0}^{2}, sm %d, region eta %d, phi %d",ism,ieta,iphi),
                   nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
          fhLam0EMCALRegionPerSM[ieta][iphi][ism]->SetYTitle("#lambda_{0}^{2}");
          fhLam0EMCALRegionPerSM[ieta][iphi][ism]->SetXTitle("#it{p}_{T} (GeV)");
          outputContainer->Add(fhLam0EMCALRegionPerSM[ieta][iphi][ism]) ;
          
          fhLam1EMCALRegionPerSM[ieta][iphi][ism] = 
          new TH2F(Form("hLam1_eta%d_phi%d_sm%d",ieta,iphi,ism),
                   Form("cluster from converted photon, #it{p}_{T} vs #lambda_{1}^{2}, sm %d, region eta %d, phi %d",ism,ieta,iphi),
                   nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
          fhLam1EMCALRegionPerSM[ieta][iphi][ism]->SetYTitle("#lambda_{1}^{2}");
          fhLam1EMCALRegionPerSM[ieta][iphi][ism]->SetXTitle("#it{p}_{T} (GeV)");
          outputContainer->Add(fhLam1EMCALRegionPerSM[ieta][iphi][ism]) ;
        }
      } // iphi 
    } // ieta
    
    Float_t ptLimit[] = {2,3,4,5,6,8,10,12};
    TString l0bin  [] = {"0.23<#lambda^{2}_{0}<0.26","0.3<#lambda^{2}_{0}<0.4"};
    
    for(Int_t il0 = 0; il0 < 2; il0++)
    {
      for(Int_t ipt = 0; ipt < 7; ipt++)
      {
        fhEtaPhiLam0BinPtBin[il0][ipt]  = new TH2F
        (Form("hEtaPhiLam0Bin%d_PtBin%d",il0,ipt),
         Form("#eta vs #phi in #it{p}_{T}=[%2.1f,%2.1f] GeV/#it{c}, %s",
              ptLimit[ipt],ptLimit[ipt+1],l0bin[il0].Data()),
         netabins,etamin,etamax,nphibins,phimin,phimax);
        fhEtaPhiLam0BinPtBin[il0][ipt]->SetYTitle("#phi (rad)");
        fhEtaPhiLam0BinPtBin[il0][ipt]->SetXTitle("#eta");
        outputContainer->Add(fhEtaPhiLam0BinPtBin[il0][ipt]) ;
        
        fhColRowLam0BinPtBin[il0][ipt]  = new TH2F
        (Form("hColRowLam0Bin%d_PtBin%d",il0,ipt),
         Form("row vs column in #it{p}_{T}=[%2.1f,%2.1f] GeV/#it{c}, %s, w > 0",
              ptLimit[ipt],ptLimit[ipt+1],l0bin[il0].Data()),
         96+2,-1.5,96+0.5,(8*24+2*8)+2,-1.5,(8*24+2*8)+0.5); // fix to generalize to other periods
        fhColRowLam0BinPtBin[il0][ipt]->SetYTitle("row");
        fhColRowLam0BinPtBin[il0][ipt]->SetXTitle("column");
        outputContainer->Add(fhColRowLam0BinPtBin[il0][ipt]) ;   
        
        fhColRowLam0BinPtBinWeighted[il0][ipt]  = new TH2F
        (Form("hColRowLam0Bin%d_PtBin%dWeighted",il0,ipt),
         Form("cluster cell row vs column weighted in #it{p}_{T}=[%2.1f,%2.1f] GeV/#it{c}, %s",
              ptLimit[ipt],ptLimit[ipt+1],l0bin[il0].Data()),
         96+2,-1.5,96+0.5,(8*24+2*8)+2,-1.5,(8*24+2*8)+0.5); // fix to generalize to other periods
        fhColRowLam0BinPtBinWeighted[il0][ipt]->SetYTitle("row");
        fhColRowLam0BinPtBinWeighted[il0][ipt]->SetXTitle("column");
        outputContainer->Add(fhColRowLam0BinPtBinWeighted[il0][ipt]) ;
        
        fhColRowLam0BinPtBinLargeTime[il0][ipt]  = new TH2F
        (Form("hColRowLam0Bin%d_PtBin%d_LargeTimeInClusterCell",il0,ipt),
         Form("row vs column in #it{p}_{T}=[%2.1f,%2.1f] GeV/#it{c}, %s, |t| > 50 ns, w > 0",
              ptLimit[ipt],ptLimit[ipt+1],l0bin[il0].Data()),
         96+2,-1.5,96+0.5,(8*24+2*8)+2,-1.5,(8*24+2*8)+0.5); // fix to generalize to other periods
        fhColRowLam0BinPtBinLargeTime[il0][ipt]->SetYTitle("row");
        fhColRowLam0BinPtBinLargeTime[il0][ipt]->SetXTitle("column");
        outputContainer->Add(fhColRowLam0BinPtBinLargeTime[il0][ipt]) ;  
        
//        fhEtaPhiLam0BinPtBinSMShared[il0][ipt]  = new TH2F
//        (Form("hEtaPhiLam0Bin%d_PtBin%d_SMShared",il0,ipt),
//         Form("#eta vs #phi in #it{p}_{T}=[%2.1f,%2.1f] GeV/#it{c}, %s",
//              ptLimit[ipt],ptLimit[ipt+1],l0bin[il0].Data()),
//         netabins,etamin,etamax,nphibins,phimin,phimax);
//        fhEtaPhiLam0BinPtBinSMShared[il0][ipt]->SetYTitle("#phi (rad)");
//        fhEtaPhiLam0BinPtBinSMShared[il0][ipt]->SetXTitle("#eta");
//        outputContainer->Add(fhEtaPhiLam0BinPtBinSMShared[il0][ipt]) ;
//        
//        fhColRowLam0BinPtBinSMShared[il0][ipt]  = new TH2F
//        (Form("hColRowLam0Bin%d_PtBin%d_SMShared",il0,ipt),
//         Form("row vs column in #it{p}_{T}=[%2.1f,%2.1f] GeV/#it{c}, %s, w > 0",
//              ptLimit[ipt],ptLimit[ipt+1],l0bin[il0].Data()),
//         96,0,96,5*24,0,5*24); // fix to generalize to other periods
//        fhColRowLam0BinPtBinSMShared[il0][ipt]->SetYTitle("row");
//        fhColRowLam0BinPtBinSMShared[il0][ipt]->SetXTitle("column");
//        outputContainer->Add(fhColRowLam0BinPtBinSMShared[il0][ipt]) ;  
        
        fhEtaPhiLargeTimeInClusterCell[il0][ipt]  = new TH2F
        (Form("hEtaPhiLam0Bin%d_PtBin%d_LargeTimeInClusterCell",il0,ipt),
         Form("#eta vs #phi in #it{p}_{T}=[%2.1f,%2.1f] GeV/#it{c}, w > 0, t > 50 ns, %s",
              ptLimit[ipt],ptLimit[ipt+1],l0bin[il0].Data()),
         netabins,etamin,etamax,nphibins,phimin,phimax);
        fhEtaPhiLargeTimeInClusterCell[il0][ipt]->SetYTitle("#phi (rad)");
        fhEtaPhiLargeTimeInClusterCell[il0][ipt]->SetXTitle("#eta");
        outputContainer->Add(fhEtaPhiLargeTimeInClusterCell[il0][ipt]) ;
        
        fhCellClusterIndexEAndTime[il0][ipt]  = new TH2F
        (Form("hCellClusterIndexEAndTimeLam0Bin%d_PtBin%d",il0,ipt),
         Form("#it{t}_{cell} vs cell index (E sorted) in #it{p}_{T}=[%2.1f,%2.1f] GeV/#it{c}, w > 0, %s",
              ptLimit[ipt],ptLimit[ipt+1],l0bin[il0].Data()),
         ntimebins,timemin,timemax,30,0,30);
        fhCellClusterIndexEAndTime[il0][ipt] ->SetYTitle("Index (sorted #it{E})");
        fhCellClusterIndexEAndTime[il0][ipt] ->SetXTitle("#it{time}_{cell} (ns)");
        outputContainer->Add(fhCellClusterIndexEAndTime[il0][ipt] ) ;    

        fhCellClusterEAndTime[il0][ipt]  = new TH2F
        (Form("hCellClusterEAndTimeLam0Bin%d_PtBin%d",il0,ipt),
         Form("#it{E}_{cell} vs #it{t}_{cell}  in #it{p}_{T}=[%2.1f,%2.1f] GeV/#it{c}, w > 0, %s",
              ptLimit[ipt],ptLimit[ipt+1],l0bin[il0].Data()),
         ntimebins,timemin,timemax,100,0,5);
        fhCellClusterEAndTime[il0][ipt] ->SetYTitle("#it{E}_{cell} (GeV)");
        fhCellClusterEAndTime[il0][ipt] ->SetXTitle("#it{time}_{cell} (ns)");
        outputContainer->Add(fhCellClusterEAndTime[il0][ipt] ) ;    

        fhCellClusterEFracAndTime[il0][ipt]  = new TH2F
        (Form("hCellClusterEFracAndTimeLam0Bin%d_PtBin%d",il0,ipt),
         Form("#it{E}_{cell} vs #it{t}_{cell}  in #it{p}_{T}=[%2.1f,%2.1f] GeV/#it{c}, w > 0, %s",
              ptLimit[ipt],ptLimit[ipt+1],l0bin[il0].Data()),
         ntimebins,timemin,timemax,100,0,1);
        fhCellClusterEFracAndTime[il0][ipt] ->SetYTitle("#it{E}_{cell} (GeV)");
        fhCellClusterEFracAndTime[il0][ipt] ->SetXTitle("#it{time}_{cell} (ns)");
        outputContainer->Add(fhCellClusterEFracAndTime[il0][ipt] ) ;            
      } // pt bin
      
      for(Int_t ism = 0; ism < GetCaloUtils()->GetNumberOfSuperModulesUsed(); ism++)
      {
        fhLam1Lam0BinPerSM[il0][ism] = new TH2F
        (Form("hLam1Lam0Bin%d_sm%d",il0,ism),
         Form("#it{p}_{T} vs #lambda^{2}_{1} in sm %d, %s",ism,l0bin[il0].Data()),
         nptbins,ptmin,ptmax,40,0,0.4);
        fhLam1Lam0BinPerSM[il0][ism]->SetYTitle("#lambda^{2}_{1}");
        fhLam1Lam0BinPerSM[il0][ism]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
        outputContainer->Add(fhLam1Lam0BinPerSM[il0][ism]) ;   
        
        fhTimeLam0BinPerSM[il0][ism] = new TH2F
        (Form("hTimeLam0Bin%d_sm%d",il0,ism),
         Form("#it{p}_{T} vs cluster cell time in sm %d, %s, w > 0",ism,l0bin[il0].Data()),
         nptbins,ptmin,ptmax,ntimebins,timemin,timemax);
        fhTimeLam0BinPerSM[il0][ism]->SetYTitle("cell time (ns)");
        fhTimeLam0BinPerSM[il0][ism]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
        outputContainer->Add(fhTimeLam0BinPerSM[il0][ism]) ;   
        
        fhTimeLam0BinPerSMWeighted[il0][ism] = new TH2F
        (Form("hTimeLam0Bin%d_sm%d_Weighted",il0,ism),
         Form("#it{p}_{T} vs cluster cell time weighted in sm %d, %s",ism,l0bin[il0].Data()),
         nptbins,ptmin,ptmax,ntimebins,timemin,timemax);
        fhTimeLam0BinPerSMWeighted[il0][ism]->SetYTitle("cell time (ns)");
        fhTimeLam0BinPerSMWeighted[il0][ism]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
        outputContainer->Add(fhTimeLam0BinPerSMWeighted[il0][ism]) ;   
        
        fhDTimeLam0BinPerSM[il0][ism] = new TH2F
        (Form("hDTimeLam0Bin%d_sm%d",il0,ism),
         Form("#it{p}_{T} vs t_{cluster}-t_{cell} in sm %d, %s, w > 0",ism,l0bin[il0].Data()),
         nptbins,ptmin,ptmax,ntimebins,timemin,timemax);
        fhDTimeLam0BinPerSM[il0][ism]->SetYTitle("t_{cluster}-t_{cell} (ns)");
        fhDTimeLam0BinPerSM[il0][ism]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
        outputContainer->Add(fhDTimeLam0BinPerSM[il0][ism]) ;   
        
        fhDTimeLam0BinPerSMWeighted[il0][ism] = new TH2F
        (Form("hDTimeLam0Bin%d_sm%d_Weighted",il0,ism),
         Form("#it{p}_{T} vs t_{cluster}-t_{cell} weighted in sm %d, %s",ism,l0bin[il0].Data()),
         nptbins,ptmin,ptmax,ntimebins,timemin,timemax);
        fhDTimeLam0BinPerSMWeighted[il0][ism]->SetYTitle("t_{cluster}-t_{cell} (ns)");
        fhDTimeLam0BinPerSMWeighted[il0][ism]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
        outputContainer->Add(fhDTimeLam0BinPerSMWeighted[il0][ism]) ;   
        
        fhCellClusterEFracLam0BinPerSM[il0][ism] = new TH2F
        (Form("hCellClusterEFracLam0Bin%d_sm%d",il0,ism),
         Form("#it{p}_{T} vs cell E / cluster E in sm %d, %s, w > 0",ism,l0bin[il0].Data()),
         nptbins,ptmin,ptmax,100,0,1);
        fhCellClusterEFracLam0BinPerSM[il0][ism]->SetYTitle("cell E / cluster E ");
        fhCellClusterEFracLam0BinPerSM[il0][ism]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
        outputContainer->Add(fhCellClusterEFracLam0BinPerSM[il0][ism]) ;         
        
//        fhCellClusterEFracLam0BinPerSMWeighted[il0][ism] = new TH2F
//        (Form("hCellClusterEFracLam0Bin%d_sm%d_Weighted",il0,ism),
//         Form("#it{p}_{T} vs cell E / cluster E weighted in sm %d, %s",ism,l0bin[il0].Data()),
//         nptbins,ptmin,ptmax,100,0,1);
//        fhCellClusterEFracLam0BinPerSMWeighted[il0][ism]->SetYTitle("cell E / cluster E ");
//        fhCellClusterEFracLam0BinPerSMWeighted[il0][ism]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
//        outputContainer->Add(fhCellClusterEFracLam0BinPerSMWeighted[il0][ism]) ;         
        
        fhCellClusterEFracLam0BinPerSMLargeTime[il0][ism] = new TH2F
        (Form("hCellClusterEFracLam0Bin%d_sm%d_LargeTimeInClusterCell",il0,ism),
         Form("#it{p}_{T} vs cell E / cluster E in sm %d, %s, w > 0, |t_{cell}| > 50 ns",ism,l0bin[il0].Data()),
         nptbins,ptmin,ptmax,100,0,1);
        fhCellClusterEFracLam0BinPerSMLargeTime[il0][ism]->SetYTitle("cell E / cluster E ");
        fhCellClusterEFracLam0BinPerSMLargeTime[il0][ism]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
        outputContainer->Add(fhCellClusterEFracLam0BinPerSMLargeTime[il0][ism]) ;         
        
        fhCellClusterEFracLam0BinPerSMLargeTimeTotal[il0][ism] = new TH2F
        (Form("hCellClusterEFracLam0Bin%d_sm%d_LargeTimeInClusterCell_Total",il0,ism),
         Form("#it{p}_{T} vs cell E / cluster E in sm %d, %s, w > 0, all |t_{cell}| > 50 ns",ism,l0bin[il0].Data()),
         nptbins,ptmin,ptmax,100,0,1);
        fhCellClusterEFracLam0BinPerSMLargeTimeTotal[il0][ism]->SetYTitle("cell E / cluster E ");
        fhCellClusterEFracLam0BinPerSMLargeTimeTotal[il0][ism]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
        outputContainer->Add(fhCellClusterEFracLam0BinPerSMLargeTimeTotal[il0][ism]) ;
        
        fhCellClusterELam0BinPerSM[il0][ism] = new TH2F
        (Form("hCellClusterELam0Bin%d_sm%d",il0,ism),
         Form("#it{p}_{T} vs cell E in sm %d, %s, w > 0",ism,l0bin[il0].Data()),
         nptbins,ptmin,ptmax,500,0,10);
        fhCellClusterELam0BinPerSM[il0][ism]->SetYTitle("cell E (GeV)");
        fhCellClusterELam0BinPerSM[il0][ism]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
        outputContainer->Add(fhCellClusterELam0BinPerSM[il0][ism]) ;         
        
        fhCellClusterELam0BinPerSMWeighted[il0][ism] = new TH2F
        (Form("hCellClusterELam0Bin%d_sm%d_Weighted",il0,ism),
         Form("#it{p}_{T} vs cell E  weighted in sm %d, %s",ism,l0bin[il0].Data()),
         nptbins,ptmin,ptmax,500,0,10);
        fhCellClusterELam0BinPerSMWeighted[il0][ism]->SetYTitle("cell E (GeV)");
        fhCellClusterELam0BinPerSMWeighted[il0][ism]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
        outputContainer->Add(fhCellClusterELam0BinPerSMWeighted[il0][ism]) ;   
        
        fhCellClusterELam0BinPerSMLargeTime[il0][ism] = new TH2F
        (Form("hCellClusterELam0Bin%d_sm%d_LargeTimeInClusterCell",il0,ism),
         Form("#it{p}_{T} vs cell E in sm %d, %s, w > 0, |t_{cell}| > 50 ns",ism,l0bin[il0].Data()),
         nptbins,ptmin,ptmax,500,0,10);
        fhCellClusterELam0BinPerSMLargeTime[il0][ism]->SetYTitle("cell E (GeV)");
        fhCellClusterELam0BinPerSMLargeTime[il0][ism]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
        outputContainer->Add(fhCellClusterELam0BinPerSMLargeTime[il0][ism]) ;      

        fhCellClusterIndexELam0BinPerSMLargeTime[il0][ism] = new TH2F
        (Form("hCellClusterIndexELam0Bin%d_sm%d_LargeTimeInClusterCell",il0,ism),
         Form("#it{p}_{T} vs cell index (E sorted) in sm %d, %s, w > 0, |t_{cell}| > 50 ns",ism,l0bin[il0].Data()),
         nptbins,ptmin,ptmax,30,0,30);
        fhCellClusterIndexELam0BinPerSMLargeTime[il0][ism]->SetYTitle("Index");
        fhCellClusterIndexELam0BinPerSMLargeTime[il0][ism]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
        outputContainer->Add(fhCellClusterIndexELam0BinPerSMLargeTime[il0][ism]) ;      
        
        fhNCellsWithLargeTimeInCluster[il0][ism] = new TH2F
        (Form("hNCellsWithLargeTimeInClusterLam0Bin%d_sm%d",il0,ism),
         Form("#it{p}_{T} vs number of cells in sm %d, %s, w > 0, |t_{cell}| > 50 ns",ism,l0bin[il0].Data()),
         nptbins,ptmin,ptmax,30,0,30);
        fhNCellsWithLargeTimeInCluster[il0][ism]->SetYTitle("#it{n}_{cells}");
        fhNCellsWithLargeTimeInCluster[il0][ism]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
        outputContainer->Add(fhNCellsWithLargeTimeInCluster[il0][ism]) ;      
        
//        if(ism < 12)
//        {
//          fhTimeLam0BinPerSMShared[il0][ism] = new TH2F
//          (Form("hTimeLam0Bin%d_sm%d_SMShared",il0,ism),
//           Form("#it{p}_{T} vs cluster cell time in sm %d, %s, w > 0",ism,l0bin[il0].Data()),
//           nptbins,ptmin,ptmax,ntimebins,timemin,timemax);
//          fhTimeLam0BinPerSMShared[il0][ism]->SetYTitle("cell time (ns)");
//          fhTimeLam0BinPerSMShared[il0][ism]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
//          outputContainer->Add(fhTimeLam0BinPerSMShared[il0][ism]) ;             
//        } // Run 1 SM
      } // sm
    } // l0 bin 
    
    for(Int_t ism = 0; ism < GetCaloUtils()->GetNumberOfSuperModulesUsed(); ism++)
    {
      fhLam0PerSM[ism] = new TH2F
      (Form("hLam0_sm%d",ism),
       Form("#it{p}_{T} vs #lambda^{2}_{0} in sm %d",ism),
       nptbins,ptmin,ptmax,40,0,0.4);
      fhLam0PerSM[ism]->SetYTitle("#lambda^{2}_{0}");
      fhLam0PerSM[ism]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
      outputContainer->Add(fhLam0PerSM[ism]) ;             
      
      fhLam1PerSM[ism] = new TH2F
      (Form("hLam1_sm%d",ism),
       Form("#it{p}_{T} vs #lambda^{2}_{1} in sm %d",ism),
       nptbins,ptmin,ptmax,40,0,0.4);
      fhLam1PerSM[ism]->SetYTitle("#lambda^{2}_{1}");
      fhLam1PerSM[ism]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
      outputContainer->Add(fhLam1PerSM[ism]) ;   
      
      fhLam0PerSMLargeTimeInClusterCell[ism] = new TH2F
      (Form("hLam0_sm%d_LargeTimeInClusterCell",ism),
       Form("#it{p}_{T} vs #lambda^{2}_{0} in sm %d,|t_{secondary cell}| > 50 ns",ism),
       nptbins,ptmin,ptmax,40,0,0.4);
      fhLam0PerSMLargeTimeInClusterCell[ism]->SetYTitle("#lambda^{2}_{0}");
      fhLam0PerSMLargeTimeInClusterCell[ism]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
      outputContainer->Add(fhLam0PerSMLargeTimeInClusterCell[ism]) ;             
      
      fhLam1PerSMLargeTimeInClusterCell[ism] = new TH2F
      (Form("hLam1_sm%d_LargeTimeInClusterCell",ism),
       Form("#it{p}_{T} vs #lambda^{2}_{1} in sm %d, |t_{secondary cell}| > 50 ns",ism),
       nptbins,ptmin,ptmax,40,0,0.4);
      fhLam1PerSMLargeTimeInClusterCell[ism]->SetYTitle("#lambda^{2}_{1}");
      fhLam1PerSMLargeTimeInClusterCell[ism]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
      outputContainer->Add(fhLam1PerSMLargeTimeInClusterCell[ism]) ;   
      
//      fhLam0PerSMSPDPileUp[ism] = new TH2F
//      (Form("hLam0_sm%d_SPDPileUp",ism),
//       Form("#it{p}_{T} vs #lambda^{2}_{0} in sm %d, Pile-up event SPD",ism),
//       nptbins,ptmin,ptmax,40,0,0.4);
//      fhLam0PerSMSPDPileUp[ism]->SetYTitle("#lambda^{2}_{0}");
//      fhLam0PerSMSPDPileUp[ism]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
//      outputContainer->Add(fhLam0PerSMSPDPileUp[ism]) ;             
//      
//      fhLam1PerSMSPDPileUp[ism] = new TH2F
//      (Form("hLam1_sm%d_SPDPileUp",ism),
//       Form("#it{p}_{T} vs #lambda^{2}_{1} in sm %d, Pile-up event SPD",ism),
//       nptbins,ptmin,ptmax,40,0,0.4);
//      fhLam1PerSMSPDPileUp[ism]->SetYTitle("#lambda^{2}_{1}");
//      fhLam1PerSMSPDPileUp[ism]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
//      outputContainer->Add(fhLam1PerSMSPDPileUp[ism]) ;   
      
//      if(ism < 12)
//      {
//        fhLam0PerSMShared[ism] = new TH2F
//        (Form("hLam0_sm%d_SMShared",ism),
//         Form("#it{p}_{T} vs #lambda^{2}_{0} in sm %d, SM shared",ism),
//         nptbins,ptmin,ptmax,40,0,0.4);
//        fhLam0PerSMShared[ism]->SetYTitle("#lambda^{2}_{0}");
//        fhLam0PerSMShared[ism]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
//        outputContainer->Add(fhLam0PerSMShared[ism]) ;             
//        
//        fhLam1PerSMShared[ism] = new TH2F
//        (Form("hLam1_sm%d_SMShared",ism),
//         Form("#it{p}_{T} vs #lambda^{2}_{1} in sm %d, SM shared",ism),
//         nptbins,ptmin,ptmax,40,0,0.4);
//        fhLam1PerSMShared[ism]->SetYTitle("#lambda^{2}_{1}");
//        fhLam1PerSMShared[ism]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
//        outputContainer->Add(fhLam1PerSMShared[ism]) ;   
//      } // run1
    } // sm
    
    for(Int_t ilarge = 0; ilarge < 5; ilarge++)
    {
      fhLam0PerNLargeTimeInClusterCell[ilarge] = new TH2F
      (Form("hLam0_NLargeTimeInClusterCell%d",ilarge),
       Form("#it{p}_{T} vs #lambda^{2}_{0} in sm %d,|t_{secondary cell}| > 50 ns",ilarge),
       nptbins,ptmin,ptmax,40,0,0.4);
      fhLam0PerNLargeTimeInClusterCell[ilarge]->SetYTitle("#lambda^{2}_{0}");
      fhLam0PerNLargeTimeInClusterCell[ilarge]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
      outputContainer->Add(fhLam0PerNLargeTimeInClusterCell[ilarge]) ;             
      
      fhLam1PerNLargeTimeInClusterCell[ilarge] = new TH2F
      (Form("hLam1_NLargeTimeInClusterCell%d",ilarge),
       Form("#it{p}_{T} vs #lambda^{2}_{1} in sm %d, |t_{secondary cell}| > 50 ns",ilarge),
       nptbins,ptmin,ptmax,40,0,0.4);
      fhLam1PerNLargeTimeInClusterCell[ilarge]->SetYTitle("#lambda^{2}_{1}");
      fhLam1PerNLargeTimeInClusterCell[ilarge]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
      outputContainer->Add(fhLam1PerNLargeTimeInClusterCell[ilarge]) ;   
    }
    
  } // regions in EMCal

  
  if(IsDataMC())
  {
    TString ptype[] = { "#gamma"         , "#gamma_{#pi decay}"    , "#gamma_{#eta decay}", "#gamma_{other decay}",
                        "#pi^{0}"        , "#eta"                  , "e^{#pm}"            , "#gamma->e^{#pm}"     ,
                        "hadron?"        , "Anti-N"                , "Anti-P"             , 
                        "Neutron"        , "Proton"                , "#pi^{#pm}"          ,
                        "#gamma_{prompt}", "#gamma_{fragmentation}", "#gamma_{ISR}"       , "String"               } ;
    
    TString pname[] = { "Photon"      , "PhotonPi0Decay"     , "PhotonEtaDecay", "PhotonOtherDecay",
                        "Pi0"         , "Eta"                , "Electron"      , "Conversion"      ,
                        "Hadron"      , "AntiNeutron"        , "AntiProton"    , 
                        "Neutron"     , "Proton"             , "ChPion"        ,
                        "PhotonPrompt", "PhotonFragmentation", "PhotonISR"     , "String"           } ;
    
    for(Int_t i = 0; i < fNOriginHistograms; i++)
    {
      fhMCE[i]  = new TH1F(Form("hE_MC%s",pname[i].Data()),
                           Form("cluster from %s : E ",ptype[i].Data()),
                           nptbins,ptmin,ptmax);
      fhMCE[i]->SetXTitle("#it{E} (GeV)");
      outputContainer->Add(fhMCE[i]) ;
      
      fhMCPt[i]  = new TH1F(Form("hPt_MC%s",pname[i].Data()),
                            Form("cluster from %s : #it{p}_{T} ",ptype[i].Data()),
                            nptbins,ptmin,ptmax);
      fhMCPt[i]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
      outputContainer->Add(fhMCPt[i]) ;
      
      fhMCEta[i]  = new TH2F(Form("hEta_MC%s",pname[i].Data()),
                             Form("cluster from %s : #eta ",ptype[i].Data()),
                             nptbins,ptmin,ptmax,netabins,etamin,etamax);
      fhMCEta[i]->SetYTitle("#eta");
      fhMCEta[i]->SetXTitle("#it{E} (GeV)");
      outputContainer->Add(fhMCEta[i]) ;
      
      fhMCPhi[i]  = new TH2F(Form("hPhi_MC%s",pname[i].Data()),
                             Form("cluster from %s : #phi ",ptype[i].Data()),
                             nptbins,ptmin,ptmax,nphibins,phimin,phimax);
      fhMCPhi[i]->SetYTitle("#phi (rad)");
      fhMCPhi[i]->SetXTitle("#it{E} (GeV)");
      outputContainer->Add(fhMCPhi[i]) ;
      
      
      fhMCDeltaE[i]  = new TH2F (Form("hDeltaE_MC%s",pname[i].Data()),
                                 Form("MC - Reco E from %s",pname[i].Data()),
                                 nptbins,ptmin,ptmax, 200,-50,50);
      fhMCDeltaE[i]->SetYTitle("#Delta #it{E} (GeV)");
      fhMCDeltaE[i]->SetXTitle("#it{E} (GeV)");
      outputContainer->Add(fhMCDeltaE[i]);
      
      fhMCDeltaPt[i]  = new TH2F (Form("hDeltaPt_MC%s",pname[i].Data()),
                                  Form("MC - Reco #it{p}_{T} from %s",pname[i].Data()),
                                  nptbins,ptmin,ptmax, 200,-50,50);
      fhMCDeltaPt[i]->SetXTitle("p_{T,rec} (GeV/#it{c})");
      fhMCDeltaPt[i]->SetYTitle("#Delta #it{p}_{T} (GeV/#it{c})");
      outputContainer->Add(fhMCDeltaPt[i]);
      
      fhMC2E[i]  = new TH2F (Form("h2E_MC%s",pname[i].Data()),
                             Form("E distribution, reconstructed vs generated from %s",pname[i].Data()),
                             nptbins,ptmin,ptmax,nptbins,ptmin,ptmax);
      fhMC2E[i]->SetXTitle("#it{E}_{rec} (GeV)");
      fhMC2E[i]->SetYTitle("#it{E}_{gen} (GeV)");
      outputContainer->Add(fhMC2E[i]);
      
      fhMC2Pt[i]  = new TH2F (Form("h2Pt_MC%s",pname[i].Data()),
                              Form("p_T distribution, reconstructed vs generated from %s",pname[i].Data()),
                              nptbins,ptmin,ptmax,nptbins,ptmin,ptmax);
      fhMC2Pt[i]->SetXTitle("p_{T,rec} (GeV/#it{c})");
      fhMC2Pt[i]->SetYTitle("p_{T,gen} (GeV/#it{c})");
      outputContainer->Add(fhMC2Pt[i]);
    }
    
    TString pptype[] = { "#gamma"             , "#gamma_{#pi decay}"    ,
                         "#gamma_{#eta decay}", "#gamma_{other decay}"  ,
                         "#gamma_{prompt}"    , "#gamma_{fragmentation}", "#gamma_{ISR}" } ;
    
    TString ppname[] = { "Photon"        , "PhotonPi0Decay"     ,
                         "PhotonEtaDecay", "PhotonOtherDecay"   ,
                         "PhotonPrompt"  , "PhotonFragmentation", "PhotonISR" } ;
    
    for(Int_t i = 0; i < fNPrimaryHistograms; i++)
    {
      fhEPrimMC[i]  = new TH1F(Form("hEPrim_MC%s",ppname[i].Data()),
                               Form("primary photon %s : E ",pptype[i].Data()),
                               nptbins,ptmin,ptmax);
      fhEPrimMC[i]->SetXTitle("#it{E} (GeV)");
      outputContainer->Add(fhEPrimMC[i]) ;
      
      fhPtPrimMC[i]  = new TH1F(Form("hPtPrim_MC%s",ppname[i].Data()),
                                Form("primary photon %s : #it{p}_{T} ",pptype[i].Data()),
                                nptbins,ptmin,ptmax);
      fhPtPrimMC[i]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
      outputContainer->Add(fhPtPrimMC[i]) ;
      
      fhYPrimMC[i]  = new TH2F(Form("hYPrim_MC%s",ppname[i].Data()),
                               Form("primary photon %s : Rapidity ",pptype[i].Data()),
                               nptbins,ptmin,ptmax,200,-2,2);
      fhYPrimMC[i]->SetYTitle("Rapidity");
      fhYPrimMC[i]->SetXTitle("#it{E} (GeV)");
      outputContainer->Add(fhYPrimMC[i]) ;
      
      fhEtaPrimMC[i]  = new TH2F(Form("hEtaPrim_MC%s",ppname[i].Data()),
                               Form("primary photon %s : #eta",pptype[i].Data()),
                               nptbins,ptmin,ptmax,200,-2,2);
      fhEtaPrimMC[i]->SetYTitle("#eta");
      fhEtaPrimMC[i]->SetXTitle("#it{E} (GeV)");
      outputContainer->Add(fhEtaPrimMC[i]) ;
      
      fhPhiPrimMC[i]  = new TH2F(Form("hPhiPrim_MC%s",ppname[i].Data()),
                                 Form("primary photon %s : #phi ",pptype[i].Data()),
                                 nptbins,ptmin,ptmax,nphibins,0,TMath::TwoPi());
      fhPhiPrimMC[i]->SetYTitle("#phi (rad)");
      fhPhiPrimMC[i]->SetXTitle("#it{E} (GeV)");
      outputContainer->Add(fhPhiPrimMC[i]) ;
      
      
      fhEPrimMCAcc[i]  = new TH1F(Form("hEPrimAcc_MC%s",ppname[i].Data()),
                                  Form("primary photon %s in acceptance: E ",pptype[i].Data()),
                                  nptbins,ptmin,ptmax);
      fhEPrimMCAcc[i]->SetXTitle("#it{E} (GeV)");
      outputContainer->Add(fhEPrimMCAcc[i]) ;
      
      fhPtPrimMCAcc[i]  = new TH1F(Form("hPtPrimAcc_MC%s",ppname[i].Data()),
                                   Form("primary photon %s in acceptance: #it{p}_{T} ",pptype[i].Data()),
                                   nptbins,ptmin,ptmax);
      fhPtPrimMCAcc[i]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
      outputContainer->Add(fhPtPrimMCAcc[i]) ;
      
      fhYPrimMCAcc[i]  = new TH2F(Form("hYPrimAcc_MC%s",ppname[i].Data()),
                                  Form("primary photon %s in acceptance: Rapidity ",pptype[i].Data()),
                                  nptbins,ptmin,ptmax,100,-1,1);
      fhYPrimMCAcc[i]->SetYTitle("Rapidity");
      fhYPrimMCAcc[i]->SetXTitle("#it{E} (GeV)");
      outputContainer->Add(fhYPrimMCAcc[i]) ;

      fhEtaPrimMCAcc[i]  = new TH2F(Form("hEtaPrimAcc_MC%s",ppname[i].Data()),
                                  Form("primary photon %s in acceptance: #eta ",pptype[i].Data()),
                                  nptbins,ptmin,ptmax,netabins,etamin,etamax);
      fhEtaPrimMCAcc[i]->SetYTitle("#eta");
      fhEtaPrimMCAcc[i]->SetXTitle("#it{E} (GeV)");
      outputContainer->Add(fhEtaPrimMCAcc[i]) ;
      
      fhPhiPrimMCAcc[i]  = new TH2F(Form("hPhiPrimAcc_MC%s",ppname[i].Data()),
                                    Form("primary photon %s in acceptance: #phi ",pptype[i].Data()),
                                    nptbins,ptmin,ptmax,nphibins,phimin,phimax);
      fhPhiPrimMCAcc[i]->SetYTitle("#phi (rad)");
      fhPhiPrimMCAcc[i]->SetXTitle("#it{E} (GeV)");
      outputContainer->Add(fhPhiPrimMCAcc[i]) ;
    }
    
    if(fFillSSHistograms)
    {
      TString ptypess[] = { "#gamma","hadron?","#pi^{0}","#eta","#gamma->e^{#pm}","e^{#pm}"} ;
      
      TString pnamess[] = { "Photon","Hadron","Pi0","Eta","Conversion","Electron"} ;
      
      for(Int_t i = 0; i < 6; i++)
      {
        fhMCELambda0[i]  = new TH2F(Form("hELambda0_MC%s",pnamess[i].Data()),
                                    Form("cluster from %s : E vs #lambda_{0}^{2}",ptypess[i].Data()),
                                    nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
        fhMCELambda0[i]->SetYTitle("#lambda_{0}^{2}");
        fhMCELambda0[i]->SetXTitle("#it{E} (GeV)");
        outputContainer->Add(fhMCELambda0[i]) ;

        fhMCPtLambda0[i]  = new TH2F(Form("hPtLambda0_MC%s",pnamess[i].Data()),
                                    Form("cluster from %s : #it{p}_{T} vs #lambda_{0}^{2}",ptypess[i].Data()),
                                    nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
        fhMCPtLambda0[i]->SetYTitle("#lambda_{0}^{2}");
        fhMCPtLambda0[i]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
        outputContainer->Add(fhMCPtLambda0[i]) ;
        
        fhMCELambda1[i]  = new TH2F(Form("hELambda1_MC%s",pnamess[i].Data()),
                                    Form("cluster from %s : E vs #lambda_{1}^{2}",ptypess[i].Data()),
                                    nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
        fhMCELambda1[i]->SetYTitle("#lambda_{1}^{2}");
        fhMCELambda1[i]->SetXTitle("#it{E} (GeV)");
        outputContainer->Add(fhMCELambda1[i]) ;
        
        fhMCEDispersion[i]  = new TH2F(Form("hEDispersion_MC%s",pnamess[i].Data()),
                                       Form("cluster from %s : E vs dispersion^{2}",ptypess[i].Data()),
                                       nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
        fhMCEDispersion[i]->SetYTitle("D^{2}");
        fhMCEDispersion[i]->SetXTitle("#it{E} (GeV)");
        outputContainer->Add(fhMCEDispersion[i]) ;
        
        fhMCNCellsE[i]  = new TH2F (Form("hNCellsE_MC%s",pnamess[i].Data()),
                                    Form("# of cells in cluster from %s vs E of clusters",ptypess[i].Data()),
                                    nptbins,ptmin,ptmax, nbins,nmin,nmax);
        fhMCNCellsE[i]->SetXTitle("#it{E} (GeV)");
        fhMCNCellsE[i]->SetYTitle("# of cells in cluster");
        outputContainer->Add(fhMCNCellsE[i]);
        
        fhMCMaxCellDiffClusterE[i]  = new TH2F (Form("hMaxCellDiffClusterE_MC%s",pnamess[i].Data()),
                                                Form("energy vs difference of cluster energy from %s - max cell energy / cluster energy, good clusters",ptypess[i].Data()),
                                                nptbins,ptmin,ptmax, 500,0,1.);
        fhMCMaxCellDiffClusterE[i]->SetXTitle("#it{E}_{cluster} (GeV) ");
        fhMCMaxCellDiffClusterE[i]->SetYTitle("(#it{E}_{cluster} - #it{E}_{cell max})/ #it{E}_{cluster}");
        outputContainer->Add(fhMCMaxCellDiffClusterE[i]);
        
        if(!fFillOnlySimpleSSHisto)
        {
          fhMCLambda0vsClusterMaxCellDiffE0[i]  = new TH2F(Form("hLambda0vsClusterMaxCellDiffE0_MC%s",pnamess[i].Data()),
                                                           Form("cluster from %s : #lambda^{2}_{0} vs fraction of energy carried by max cell, E < 2 GeV",ptypess[i].Data()),
                                                           ssbins,ssmin,ssmax,500,0,1.);
          fhMCLambda0vsClusterMaxCellDiffE0[i]->SetXTitle("#lambda_{0}^{2}");
          fhMCLambda0vsClusterMaxCellDiffE0[i]->SetYTitle("(#it{E}_{cluster} - #it{E}_{cell max})/ #it{E}_{cluster}");
          outputContainer->Add(fhMCLambda0vsClusterMaxCellDiffE0[i]) ;
          
          fhMCLambda0vsClusterMaxCellDiffE2[i]  = new TH2F(Form("hLambda0vsClusterMaxCellDiffE2_MC%s",pnamess[i].Data()),
                                                           Form("cluster from %s : #lambda^{2}_{0} vs fraction of energy carried by max cell, 2< E < 6 GeV",ptypess[i].Data()),
                                                           ssbins,ssmin,ssmax,500,0,1.);
          fhMCLambda0vsClusterMaxCellDiffE2[i]->SetXTitle("#lambda_{0}^{2}");
          fhMCLambda0vsClusterMaxCellDiffE2[i]->SetYTitle("(#it{E}_{cluster} - #it{E}_{cell max})/ #it{E}_{cluster}");
          outputContainer->Add(fhMCLambda0vsClusterMaxCellDiffE2[i]) ;
          
          fhMCLambda0vsClusterMaxCellDiffE6[i]  = new TH2F(Form("hLambda0vsClusterMaxCellDiffE6_MC%s",pnamess[i].Data()),
                                                           Form("cluster from %s : #lambda^{2}_{0} vs fraction of energy carried by max cell, #it{E} > 6 GeV",ptypess[i].Data()),
                                                           ssbins,ssmin,ssmax,500,0,1.);
          fhMCLambda0vsClusterMaxCellDiffE6[i]->SetXTitle("#lambda_{0}^{2}");
          fhMCLambda0vsClusterMaxCellDiffE6[i]->SetYTitle("(#it{E}_{cluster} - #it{E}_{cell max})/ #it{E}_{cluster}");
          outputContainer->Add(fhMCLambda0vsClusterMaxCellDiffE6[i]) ;
          
          fhMCNCellsvsClusterMaxCellDiffE0[i]  = new TH2F(Form("hNCellsvsClusterMaxCellDiffE0_MC%s",pnamess[i].Data()),
                                                          Form("cluster from %s : N cells in cluster vs fraction of energy carried by max cell, E < 2 GeV",ptypess[i].Data()),
                                                          nbins/5,nmin,nmax/5,500,0,1.);
          fhMCNCellsvsClusterMaxCellDiffE0[i]->SetXTitle("N cells in cluster");
          fhMCNCellsvsClusterMaxCellDiffE0[i]->SetYTitle("(#it{E}_{cluster} - #it{E}_{cell max})/ #it{E}_{cluster}");
          outputContainer->Add(fhMCNCellsvsClusterMaxCellDiffE0[i]) ;
          
          fhMCNCellsvsClusterMaxCellDiffE2[i]  = new TH2F(Form("hNCellsvsClusterMaxCellDiffE2_MC%s",pnamess[i].Data()),
                                                          Form("cluster from %s : N cells in cluster vs fraction of energy carried by max cell, 2< E < 6 GeV",ptypess[i].Data()),
                                                          nbins/5,nmin,nmax/5,500,0,1.);
          fhMCNCellsvsClusterMaxCellDiffE2[i]->SetXTitle("N cells in cluster");
          fhMCNCellsvsClusterMaxCellDiffE2[i]->SetYTitle("(#it{E}_{cluster} - #it{E}_{cell max})/ #it{E}_{cluster}");
          outputContainer->Add(fhMCNCellsvsClusterMaxCellDiffE2[i]) ;
          
          fhMCNCellsvsClusterMaxCellDiffE6[i]  = new TH2F(Form("hNCellsvsClusterMaxCellDiffE6_MC%s",pnamess[i].Data()),
                                                          Form("cluster from %s : N cells in cluster vs fraction of energy carried by max cell, #it{E} > 6 GeV",ptypess[i].Data()),
                                                          nbins/5,nmin,nmax/5,500,0,1.);
          fhMCNCellsvsClusterMaxCellDiffE6[i]->SetXTitle("N cells in cluster");
          fhMCNCellsvsClusterMaxCellDiffE6[i]->SetYTitle("#it{E} (GeV)");
          outputContainer->Add(fhMCNCellsvsClusterMaxCellDiffE6[i]) ;
          
          if(GetCalorimeter()==kEMCAL)
          {
            fhMCEDispEta[i]  = new TH2F (Form("hEDispEtaE_MC%s",pnamess[i].Data()),
                                         Form("cluster from %s : #sigma^{2}_{#eta #eta} = #Sigma w_{i}(#eta_{i} - <#eta>)^{2}/ #Sigma w_{i} vs E",ptypess[i].Data()),
                                         nptbins,ptmin,ptmax, ssbins,ssmin,ssmax);
            fhMCEDispEta[i]->SetXTitle("#it{E} (GeV)");
            fhMCEDispEta[i]->SetYTitle("#sigma^{2}_{#eta #eta}");
            outputContainer->Add(fhMCEDispEta[i]);
            
            fhMCEDispPhi[i]  = new TH2F (Form("hEDispPhiE_MC%s",pnamess[i].Data()),
                                         Form("cluster from %s : #sigma^{2}_{#phi #phi} = #Sigma w_{i}(#phi_{i} - <#phi>)^{2} / #Sigma w_{i} vs E",ptypess[i].Data()),
                                         nptbins,ptmin,ptmax, ssbins,ssmin,ssmax);
            fhMCEDispPhi[i]->SetXTitle("#it{E} (GeV)");
            fhMCEDispPhi[i]->SetYTitle("#sigma^{2}_{#phi #phi}");
            outputContainer->Add(fhMCEDispPhi[i]);
            
            fhMCESumEtaPhi[i]  = new TH2F (Form("hESumEtaPhiE_MC%s",pnamess[i].Data()),
                                           Form("cluster from %s : #delta^{2}_{#eta #phi} = #Sigma w_{i}(#phi_{i} #eta_{i} ) / #Sigma w_{i} - <#phi><#eta> vs E",ptypess[i].Data()),
                                           nptbins,ptmin,ptmax, 2*ssbins,-ssmax,ssmax);
            fhMCESumEtaPhi[i]->SetXTitle("#it{E} (GeV)");
            fhMCESumEtaPhi[i]->SetYTitle("#delta^{2}_{#eta #phi}");
            outputContainer->Add(fhMCESumEtaPhi[i]);
            
            fhMCEDispEtaPhiDiff[i]  = new TH2F (Form("hEDispEtaPhiDiffE_MC%s",pnamess[i].Data()),
                                                Form("cluster from %s : #sigma^{2}_{#phi #phi} - #sigma^{2}_{#eta #eta} vs E",ptypess[i].Data()),
                                                nptbins,ptmin,ptmax,200,-10,10);
            fhMCEDispEtaPhiDiff[i]->SetXTitle("#it{E} (GeV)");
            fhMCEDispEtaPhiDiff[i]->SetYTitle("#sigma^{2}_{#phi #phi}-#sigma^{2}_{#eta #eta}");
            outputContainer->Add(fhMCEDispEtaPhiDiff[i]);
            
            fhMCESphericity[i]  = new TH2F (Form("hESphericity_MC%s",pnamess[i].Data()),
                                            Form("cluster from %s : (#sigma^{2}_{#phi #phi} - #sigma^{2}_{#eta #eta}) / (#sigma^{2}_{#eta #eta} + #sigma^{2}_{#phi #phi}) vs E",ptypess[i].Data()),
                                            nptbins,ptmin,ptmax, 200,-1,1);
            fhMCESphericity[i]->SetXTitle("#it{E} (GeV)");
            fhMCESphericity[i]->SetYTitle("s = (#sigma^{2}_{#phi #phi} - #sigma^{2}_{#eta #eta}) / (#sigma^{2}_{#eta #eta} + #sigma^{2}_{#phi #phi})");
            outputContainer->Add(fhMCESphericity[i]);
            
            for(Int_t ie = 0; ie < 7; ie++)
            {
              fhMCDispEtaDispPhi[ie][i] = new TH2F (Form("hMCDispEtaDispPhi_EBin%d_MC%s",ie,pnamess[i].Data()),
                                                    Form("cluster from %s : #sigma^{2}_{#phi #phi} vs #sigma^{2}_{#eta #eta} for %d < E < %d GeV",pnamess[i].Data(),bin[ie],bin[ie+1]),
                                                    ssbins,ssmin,ssmax , ssbins,ssmin,ssmax);
              fhMCDispEtaDispPhi[ie][i]->SetXTitle("#sigma^{2}_{#eta #eta}");
              fhMCDispEtaDispPhi[ie][i]->SetYTitle("#sigma^{2}_{#phi #phi}");
              outputContainer->Add(fhMCDispEtaDispPhi[ie][i]);
              
              fhMCLambda0DispEta[ie][i] = new TH2F (Form("hMCLambda0DispEta_EBin%d_MC%s",ie,pnamess[i].Data()),
                                                    Form("cluster from %s : #lambda^{2}_{0} vs #sigma^{2}_{#eta #eta} for %d < E < %d GeV",pnamess[i].Data(),bin[ie],bin[ie+1]),
                                                    ssbins,ssmin,ssmax , ssbins,ssmin,ssmax);
              fhMCLambda0DispEta[ie][i]->SetXTitle("#lambda^{2}_{0}");
              fhMCLambda0DispEta[ie][i]->SetYTitle("#sigma^{2}_{#phi #phi}");
              outputContainer->Add(fhMCLambda0DispEta[ie][i]);
              
              fhMCLambda0DispPhi[ie][i] = new TH2F (Form("hMCLambda0DispPhi_EBin%d_MC%s",ie,pnamess[i].Data()),
                                                    Form("cluster from %s :#lambda^{2}_{0} vs #sigma^{2}_{#phi #phi} for %d < E < %d GeV",pnamess[i].Data(),bin[ie],bin[ie+1]),
                                                    ssbins,ssmin,ssmax , ssbins,ssmin,ssmax);
              fhMCLambda0DispPhi[ie][i]->SetXTitle("#lambda^{2}_{0}");
              fhMCLambda0DispPhi[ie][i]->SetYTitle("#sigma^{2}_{#phi #phi}");
              outputContainer->Add(fhMCLambda0DispPhi[ie][i]);
            }
          }
        }
      }// loop
      
      if(!GetReader()->IsEmbeddedClusterSelectionOn())
      {
        fhMCPhotonELambda0NoOverlap  = new TH2F("hELambda0_MCPhoton_NoOverlap",
                                                "cluster from Photon : E vs #lambda_{0}^{2}",
                                                nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
        fhMCPhotonELambda0NoOverlap->SetYTitle("#lambda_{0}^{2}");
        fhMCPhotonELambda0NoOverlap->SetXTitle("#it{E} (GeV)");
        outputContainer->Add(fhMCPhotonELambda0NoOverlap) ;
        
        fhMCPhotonELambda0TwoOverlap  = new TH2F("hELambda0_MCPhoton_TwoOverlap",
                                                 "cluster from Photon : E vs #lambda_{0}^{2}",
                                                 nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
        fhMCPhotonELambda0TwoOverlap->SetYTitle("#lambda_{0}^{2}");
        fhMCPhotonELambda0TwoOverlap->SetXTitle("#it{E} (GeV)");
        outputContainer->Add(fhMCPhotonELambda0TwoOverlap) ;
        
        fhMCPhotonELambda0NOverlap  = new TH2F("hELambda0_MCPhoton_NOverlap",
                                               "cluster from Photon : E vs #lambda_{0}^{2}",
                                               nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
        fhMCPhotonELambda0NOverlap->SetYTitle("#lambda_{0}^{2}");
        fhMCPhotonELambda0NOverlap->SetXTitle("#it{E} (GeV)");
        outputContainer->Add(fhMCPhotonELambda0NOverlap) ;
      } // No embedding
      
      if(GetReader()->IsEmbeddedClusterSelectionOn())
      {
        fhEmbeddedSignalFractionEnergy  = new TH2F("hEmbeddedSignal_FractionEnergy",
                                                   "Energy Fraction of embedded signal versus cluster energy",
                                                   nptbins,ptmin,ptmax,100,0.,1.);
        fhEmbeddedSignalFractionEnergy->SetYTitle("Fraction");
        fhEmbeddedSignalFractionEnergy->SetXTitle("#it{E} (GeV)");
        outputContainer->Add(fhEmbeddedSignalFractionEnergy) ;
        
        fhEmbedPhotonELambda0FullSignal  = new TH2F("hELambda0_EmbedPhoton_FullSignal",
                                                    "cluster from Photon embedded with more than 90% energy in cluster : E vs #lambda_{0}^{2}",
                                                    nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
        fhEmbedPhotonELambda0FullSignal->SetYTitle("#lambda_{0}^{2}");
        fhEmbedPhotonELambda0FullSignal->SetXTitle("#it{E} (GeV)");
        outputContainer->Add(fhEmbedPhotonELambda0FullSignal) ;
        
        fhEmbedPhotonELambda0MostlySignal  = new TH2F("hELambda0_EmbedPhoton_MostlySignal",
                                                      "cluster from Photon embedded with 50% to 90% energy in cluster : E vs #lambda_{0}^{2}",
                                                      nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
        fhEmbedPhotonELambda0MostlySignal->SetYTitle("#lambda_{0}^{2}");
        fhEmbedPhotonELambda0MostlySignal->SetXTitle("#it{E} (GeV)");
        outputContainer->Add(fhEmbedPhotonELambda0MostlySignal) ;
        
        fhEmbedPhotonELambda0MostlyBkg  = new TH2F("hELambda0_EmbedPhoton_MostlyBkg",
                                                   "cluster from Photon embedded with 10% to 50% energy in cluster : E vs #lambda_{0}^{2}",
                                                   nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
        fhEmbedPhotonELambda0MostlyBkg->SetYTitle("#lambda_{0}^{2}");
        fhEmbedPhotonELambda0MostlyBkg->SetXTitle("#it{E} (GeV)");
        outputContainer->Add(fhEmbedPhotonELambda0MostlyBkg) ;
        
        fhEmbedPhotonELambda0FullBkg  = new TH2F("hELambda0_EmbedPhoton_FullBkg",
                                                 "cluster from Photonm embedded with 0% to 10% energy in cluster : E vs #lambda_{0}^{2}",
                                                 nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
        fhEmbedPhotonELambda0FullBkg->SetYTitle("#lambda_{0}^{2}");
        fhEmbedPhotonELambda0FullBkg->SetXTitle("#it{E} (GeV)");
        outputContainer->Add(fhEmbedPhotonELambda0FullBkg) ;
        
        fhEmbedPi0ELambda0FullSignal  = new TH2F("hELambda0_EmbedPi0_FullSignal",
                                                 "cluster from Pi0 embedded with more than 90% energy in cluster : E vs #lambda_{0}^{2}",
                                                 nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
        fhEmbedPi0ELambda0FullSignal->SetYTitle("#lambda_{0}^{2}");
        fhEmbedPi0ELambda0FullSignal->SetXTitle("#it{E} (GeV)");
        outputContainer->Add(fhEmbedPi0ELambda0FullSignal) ;
        
        fhEmbedPi0ELambda0MostlySignal  = new TH2F("hELambda0_EmbedPi0_MostlySignal",
                                                   "cluster from Pi0 embedded with 50% to 90% energy in cluster : E vs #lambda_{0}^{2}",
                                                   nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
        fhEmbedPi0ELambda0MostlySignal->SetYTitle("#lambda_{0}^{2}");
        fhEmbedPi0ELambda0MostlySignal->SetXTitle("#it{E} (GeV)");
        outputContainer->Add(fhEmbedPi0ELambda0MostlySignal) ;
        
        fhEmbedPi0ELambda0MostlyBkg  = new TH2F("hELambda0_EmbedPi0_MostlyBkg",
                                                "cluster from Pi0 embedded with 10% to 50% energy in cluster : E vs #lambda_{0}^{2}",
                                                nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
        fhEmbedPi0ELambda0MostlyBkg->SetYTitle("#lambda_{0}^{2}");
        fhEmbedPi0ELambda0MostlyBkg->SetXTitle("#it{E} (GeV)");
        outputContainer->Add(fhEmbedPi0ELambda0MostlyBkg) ;
        
        fhEmbedPi0ELambda0FullBkg  = new TH2F("hELambda0_EmbedPi0_FullBkg",
                                              "cluster from Pi0 embedded with 0% to 10% energy in cluster : E vs #lambda_{0}^{2}",
                                              nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
        fhEmbedPi0ELambda0FullBkg->SetYTitle("#lambda_{0}^{2}");
        fhEmbedPi0ELambda0FullBkg->SetXTitle("#it{E} (GeV)");
        outputContainer->Add(fhEmbedPi0ELambda0FullBkg) ;
      }// embedded histograms
    }// Fill SS MC histograms
    
    
    if(fFillConversionVertexHisto)
    {
      fhMCConversionVertex = new TH2F("hMCPhotonConversionVertex","cluster from converted photon, #it{p}_{T} vs vertex distance",
                                      nptbins,ptmin,ptmax,500,0,500);
      fhMCConversionVertex->SetYTitle("#it{R} (cm)");
      fhMCConversionVertex->SetXTitle("#it{p}_{T} (GeV/#it{c})");
      outputContainer->Add(fhMCConversionVertex) ;
      
      if(GetCalorimeter() == kEMCAL &&  GetFirstSMCoveredByTRD() >= 0)
      {
        fhMCConversionVertexTRD = new TH2F("hMCPhotonConversionVertexTRD","cluster from converted photon, #it{p}_{T} vs vertex distance, SM covered by TRD",
                                           nptbins,ptmin,ptmax,500,0,500);
        fhMCConversionVertexTRD->SetYTitle("#it{R} (cm)");
        fhMCConversionVertexTRD->SetXTitle("#it{p}_{T} (GeV/#it{c})");
        outputContainer->Add(fhMCConversionVertexTRD) ;
      }
      
      if(fFillSSHistograms)
      {
        TString region[] = {"ITS","TPC","TRD","TOF","Top EMCal","In EMCal"};
        for(Int_t iR = 0; iR < 6; iR++)
        {
          fhMCConversionLambda0Rcut[iR] = new TH2F(Form("hMCPhotonConversionLambda0_R%d",iR),
                                                   Form("cluster from converted photon, #it{p}_{T} vs #lambda_{0}^{2}, conversion in %s",region[iR].Data()),
                                                   nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
          fhMCConversionLambda0Rcut[iR]->SetYTitle("#lambda_{0}^{2}");
          fhMCConversionLambda0Rcut[iR]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
          outputContainer->Add(fhMCConversionLambda0Rcut[iR]) ;
          
          fhMCConversionLambda1Rcut[iR] = new TH2F(Form("hMCPhotonConversionLambda1_R%d",iR),
                                                   Form("cluster from converted photon, #it{p}_{T} vs #lambda_{1}^{2}, conversion in %s",region[iR].Data()),
                                                   nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
          fhMCConversionLambda1Rcut[iR]->SetYTitle("#lambda_{1}^{2}");
          fhMCConversionLambda1Rcut[iR]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
          outputContainer->Add(fhMCConversionLambda1Rcut[iR]) ;
        } // R cut
        
        
        if(GetCalorimeter() == kEMCAL &&  GetFirstSMCoveredByTRD() >= 0)
        {
          for(Int_t iR = 0; iR < 6; iR++)
          {
            fhMCConversionLambda0RcutTRD[iR] = new TH2F(Form("hMCPhotonConversionLambda0TRD_R%d",iR),
                                                        Form("cluster from converted photon, #it{p}_{T} vs #lambda_{0}^{2}, conversion in %s, SM covered by TRD",region[iR].Data()),
                                                        nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
            fhMCConversionLambda0RcutTRD[iR]->SetYTitle("#lambda_{0}^{2}");
            fhMCConversionLambda0RcutTRD[iR]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
            outputContainer->Add(fhMCConversionLambda0RcutTRD[iR]) ;
            
            fhMCConversionLambda1RcutTRD[iR] = new TH2F(Form("hMCPhotonConversionLambda1TRD_R%d",iR),
                                                        Form("cluster from converted photon, #it{p}_{T} vs #lambda_{1}^{2}, conversion in %s, SM covered by TRD",region[iR].Data()),
                                                        nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
            fhMCConversionLambda1RcutTRD[iR]->SetYTitle("#lambda_{1}^{2}");
            fhMCConversionLambda1RcutTRD[iR]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
            outputContainer->Add(fhMCConversionLambda1RcutTRD[iR]) ;
          } // R cut
        }
        
        //      if(GetCalorimeter() == kEMCAL && fFillEMCALRegionSSHistograms)
        //      {
        //        for(Int_t ieta = 0; ieta < 4; ieta++) 
        //        {  
        //          for(Int_t iphi = 0; iphi < 3; iphi++) 
        //          {
        //            for(Int_t iR = 0; iR < 6; iR++) 
        //            {
        //              fhLam0EMCALRegionMCConvRcut[ieta][iphi][iR] = 
        //              new TH2F(Form("hMCPhotonConversionLambda0_R%d_eta%d_phi%d",iR,ieta,iphi),
        //                       Form("cluster from converted photon, #it{p}_{T} vs #lambda_{0}^{2}, conversion in %s, region eta %d, phi %d",region[iR].Data(),ieta,iphi),
        //                       nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
        //              fhLam0EMCALRegionMCConvRcut[ieta][iphi][iR]->SetYTitle("#lambda_{0}^{2}");
        //              fhLam0EMCALRegionMCConvRcut[ieta][iphi][iR]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
        //              outputContainer->Add(fhLam0EMCALRegionMCConvRcut[ieta][iphi][iR]) ;
        //              
        //              if(GetFirstSMCoveredByTRD() >= 0)
        //              {
        //                fhLam0EMCALRegionTRDMCConvRcut[ieta][iphi][iR] = 
        //                new TH2F(Form("hMCPhotonConversionLambda0TRD_R%d_eta%d_phi%d",iR,ieta,iphi),
        //                         Form("cluster from converted photon, #it{p}_{T} vs #lambda_{0}^{2}, conversion in %s, region eta %d, phi %d, SM covered by TRD",region[iR].Data(),ieta,iphi),
        //                         nptbins,ptmin,ptmax,ssbins,ssmin,ssmax);
        //                fhLam0EMCALRegionTRDMCConvRcut[ieta][iphi][iR]->SetYTitle("#lambda_{0}^{2}");
        //                fhLam0EMCALRegionTRDMCConvRcut[ieta][iphi][iR]->SetXTitle("#it{p}_{T} (GeV/#it{c})");
        //                outputContainer->Add(fhLam0EMCALRegionTRDMCConvRcut[ieta][iphi][iR]) ;
        //              } // TRD
        //
        //            } // iR
        //          } // iphi 
        //        } // ieta
        //      } // regions in EMCal
      } // shower shape
    } // conversion vertex
  } // Histos with MC
  
  if(fFillEBinAcceptanceHisto)
  {
    for(Int_t ie=0; ie<fNEBinCuts; ie++)
    {
      fhEBinClusterEtaPhi[ie] = new TH2F
      (Form("hEBin%d_Cluster_EtaPhi",ie),
       Form("#eta vs #phi, cluster, %2.2f<#it{p}_{T}<%2.2f GeV/#it{c}",fEBinCuts[ie],fEBinCuts[ie+1]),
       netabins,etamin,etamax,nphibins,phimin,phimax);
      fhEBinClusterEtaPhi[ie]->SetYTitle("#phi (rad)");
      fhEBinClusterEtaPhi[ie]->SetXTitle("#eta");
      outputContainer->Add(fhEBinClusterEtaPhi[ie]) ;
      
      fhEBinClusterColRow[ie] = new TH2F
      (Form("hEBin%d_Cluster_ColRow",ie),
       Form("column vs row, cluster max E cell, %2.2f<#it{p}_{T}<%2.2f GeV/#it{c}",fEBinCuts[ie],fEBinCuts[ie+1]),
       96+2,-1.5,96+0.5,(8*24+2*8)+2,-1.5,(8*24+2*8)+0.5);
      fhEBinClusterColRow[ie]->SetYTitle("row");
      fhEBinClusterColRow[ie]->SetXTitle("column");
      outputContainer->Add(fhEBinClusterColRow[ie]) ;

      fhEBinClusterEtaPhiPID[ie] = new TH2F
      (Form("hEBin%d_Cluster_EtaPhi_PID",ie),
       Form("#eta vs #phi, cluster, %2.2f<#it{p}_{T}<%2.2f GeV/#it{c}, PID cut",fEBinCuts[ie],fEBinCuts[ie+1]),
       netabins,etamin,etamax,nphibins,phimin,phimax);
      fhEBinClusterEtaPhiPID[ie]->SetYTitle("#phi (rad)");
      fhEBinClusterEtaPhiPID[ie]->SetXTitle("#eta");
      outputContainer->Add(fhEBinClusterEtaPhiPID[ie]) ;
      
      fhEBinClusterColRowPID[ie] = new TH2F
      (Form("hEBin%d_Cluster_ColRow_PID",ie),
       Form("column vs row, cluster max E cell, %2.2f<#it{p}_{T}<%2.2f GeV/#it{c}, PID cut",fEBinCuts[ie],fEBinCuts[ie+1]),
       96+2,-1.5,96+0.5,(8*24+2*8)+2,-1.5,(8*24+2*8)+0.5);
      fhEBinClusterColRowPID[ie]->SetYTitle("row");
      fhEBinClusterColRowPID[ie]->SetXTitle("column");
      outputContainer->Add(fhEBinClusterColRowPID[ie]) ;
    }
  }

  if(fStudyActivityNearCluster)
  {
    fhLocalRegionClusterEtaPhi  = new TH2F
    ("hLocalRegionClusterEtaPhi","cluster,#it{E} > 0.5 GeV, #eta vs #phi",netabins,etamin,etamax,nphibins,phimin,phimax);
    fhLocalRegionClusterEtaPhi->SetYTitle("#phi (rad)");
    fhLocalRegionClusterEtaPhi->SetXTitle("#eta");
    outputContainer->Add(fhLocalRegionClusterEtaPhi) ;
    
    fhLocalRegionClusterEnergySum = new TH2F ("hLocalRegionClusterEnergySum",
                                              "Sum of cluster energy around trigger cluster #it{E} with R=0.2", 
                                              nptbins,ptmin,ptmax, 200,0,100);
    fhLocalRegionClusterEnergySum->SetXTitle("#it{E} (GeV)");
    fhLocalRegionClusterEnergySum->SetYTitle("#Sigma #it{E} (GeV)");
    outputContainer->Add(fhLocalRegionClusterEnergySum);    
    
    fhLocalRegionClusterMultiplicity = new TH2F ("hLocalRegionClusterMultiplicity",
                                                 "Cluster multiplicity around trigger cluster #it{E} with R=0.2", 
                                                 nptbins,ptmin,ptmax, 200,0,200);
    fhLocalRegionClusterMultiplicity->SetXTitle("#it{E} (GeV)");
    fhLocalRegionClusterMultiplicity->SetYTitle("Multiplicity");
    outputContainer->Add(fhLocalRegionClusterMultiplicity);    
    
    if(IsHighMultiplicityAnalysisOn())
    {
      fhLocalRegionClusterEnergySumPerCentrality = new TH2F ("hLocalRegionClusterEnergySumPerCentrality",
                                                             "Sum of cluster energy around trigger cluster vs centrality with R=0.2", 
                                                             100,0,100, 200,0,100);
      fhLocalRegionClusterEnergySumPerCentrality->SetXTitle("Centrality");
      fhLocalRegionClusterEnergySumPerCentrality->SetYTitle("#Sigma #it{E} (GeV)");
      outputContainer->Add(fhLocalRegionClusterEnergySumPerCentrality);    

      fhLocalRegionClusterMultiplicityPerCentrality = new TH2F ("hLocalRegionClusterMultiplicityPerCentrality",
                                                                      "Cluster multiplicity around trigger cluster vs centrality with R=0.2", 
                                                                      100,0,100, 200,0,200);
      fhLocalRegionClusterMultiplicityPerCentrality->SetXTitle("Centrality");
      fhLocalRegionClusterMultiplicityPerCentrality->SetYTitle("Multiplicity");
      outputContainer->Add(fhLocalRegionClusterMultiplicityPerCentrality);        
    }

    if(IsStudyClusterOverlapsPerGeneratorOn() && IsDataMC())
    {
      fhLocalRegionClusterEnergySumHijing = new TH2F ("hLocalRegionClusterEnergySumHijing",
                                                      "Sum of cluster energy (HIJING) around trigger cluster #it{E} with R=0.2", 
                                                      nptbins,ptmin,ptmax, 200,0,100);
      fhLocalRegionClusterEnergySumHijing->SetXTitle("#it{E} (GeV)");
      fhLocalRegionClusterEnergySumHijing->SetYTitle("#Sigma #it{E} (GeV)");
      outputContainer->Add(fhLocalRegionClusterEnergySumHijing);    
      
      fhLocalRegionClusterMultiplicityHijing = new TH2F ("hLocalRegionClusterMultiplicityHijing",
                                                         "Cluster multiplicity (HIJING) around trigger cluster #it{E} with R=0.2", 
                                                         nptbins,ptmin,ptmax, 200,0,200);
      fhLocalRegionClusterMultiplicityHijing->SetXTitle("#it{E} (GeV)");
      fhLocalRegionClusterMultiplicityHijing->SetYTitle("Multiplicity");
      outputContainer->Add(fhLocalRegionClusterMultiplicityHijing);    
      
      if(IsHighMultiplicityAnalysisOn())
      {
        fhLocalRegionClusterEnergySumPerCentralityHijing = new TH2F ("hLocalRegionClusterEnergySumPerCentralityHijing",
                                                                     "Sum of cluster energy (HIJING) around trigger cluster vs centrality with R=0.2", 
                                                                     100,0,100, 200,0,100);
        fhLocalRegionClusterEnergySumPerCentralityHijing->SetXTitle("Centrality");
        fhLocalRegionClusterEnergySumPerCentralityHijing->SetYTitle("#Sigma #it{E} (GeV)");
        outputContainer->Add(fhLocalRegionClusterEnergySumPerCentralityHijing);    
        
        fhLocalRegionClusterMultiplicityPerCentralityHijing = new TH2F ("hLocalRegionClusterMultiplicityPerCentralityHijing",
                                                                        "Cluster multiplicity (HIJING) around trigger cluster vs centrality with R=0.2", 
                                                                        100,0,100, 200,0,200);
        fhLocalRegionClusterMultiplicityPerCentralityHijing->SetXTitle("Centrality");
        fhLocalRegionClusterMultiplicityPerCentralityHijing->SetYTitle("Multiplicity");
        outputContainer->Add(fhLocalRegionClusterMultiplicityPerCentralityHijing);        
      }
    }
  }

  if(IsStudyClusterOverlapsPerGeneratorOn() && IsDataMC())
  {
     
    TString mcGenNames[] = {"","_MC_Pi0Merged","_MC_Pi0Decay","_MC_EtaDecay","_MC_PhotonOther","_MC_Electron","_MC_Other"};
    TString mcGenTitle[] = {"",",MC Pi0-Merged",",MC Pi0-Decay",", MC Eta-Decay",", MC Photon other sources",", MC Electron",", MC other sources"};
    for(Int_t igen = 0; igen < GetNCocktailGenNamesToCheck(); igen++)
    {
      TString add = "_MainGener_";
      if(igen==0) add = "";
      for(Int_t imc = 0; imc < fgkNGenTypes; imc++)
      {
        fhCleanGeneratorCluster[igen][imc] = new TH1F(Form("hCleanGeneratorCluster%s%s%s",add.Data(),GetCocktailGenNameToCheck(igen).Data(), mcGenNames[imc].Data()),
                                                      Form("Number of selected clusters with contribution of %s generator%s, no overlap",
                                                           GetCocktailGenNameToCheck(igen).Data(), mcGenTitle[imc].Data()),
                                                      nptbins,ptmin,ptmax);
        fhCleanGeneratorCluster[igen][imc]->SetYTitle("#it{counts}");
        fhCleanGeneratorCluster[igen][imc]->SetXTitle("#it{E} (GeV)");
        outputContainer->Add(fhCleanGeneratorCluster[igen][imc]) ;
        
        fhCleanGeneratorClusterEPrimRecoRatio[igen][imc] = new TH2F(Form("hCleanGeneratorClusterEPrimRecoRatio%s%s%s",add.Data(),GetCocktailGenNameToCheck(igen).Data(), mcGenNames[imc].Data()),
                                                                    Form("#it{E}_{reco}/#it{E}_{gen} clusters with contribution of %s generator%s, no overlap",
                                                                         GetCocktailGenNameToCheck(igen).Data(), mcGenTitle[imc].Data()),
                                                                    nptbins,ptmin,ptmax,300,0,3);
        fhCleanGeneratorClusterEPrimRecoRatio[igen][imc]->SetYTitle("#it{E}_{reco}/#it{E}_{gen}");
        fhCleanGeneratorClusterEPrimRecoRatio[igen][imc]->SetXTitle("#it{E}_{reco} (GeV)");
        outputContainer->Add(fhCleanGeneratorClusterEPrimRecoRatio[igen][imc]) ;
        
        fhCleanGeneratorClusterEPrimRecoDiff[igen][imc] = new TH2F(Form("hCleanGeneratorClusterEPrimRecoDiff%s%s%s",add.Data(),GetCocktailGenNameToCheck(igen).Data(), mcGenNames[imc].Data()),
                                                                   Form("#it{E}_{reco}-#it{E}_{gen} clusters with contribution of %s generator%s, no overlap",
                                                                        GetCocktailGenNameToCheck(igen).Data(), mcGenTitle[imc].Data()),
                                                                   nptbins,ptmin,ptmax,400,-20,20);
        fhCleanGeneratorClusterEPrimRecoDiff[igen][imc]->SetYTitle("#it{E}_{reco}-#it{E}_{gen} (GeV)");
        fhCleanGeneratorClusterEPrimRecoDiff[igen][imc]->SetXTitle("#it{E}_{reco} (GeV)");
        outputContainer->Add(fhCleanGeneratorClusterEPrimRecoDiff[igen][imc]) ;
     
        //
        
        fhMergeGeneratorCluster[igen][imc] = new TH1F(Form("hMergeGeneratorCluster%s%s%s",add.Data(),GetCocktailGenNameToCheck(igen).Data(), mcGenNames[imc].Data()),
                                                      Form("Number of selected clusters with contribution of >=2 generators, main %s%s",
                                                           GetCocktailGenNameToCheck(igen).Data(), mcGenTitle[imc].Data()),
                                                      nptbins,ptmin,ptmax);
        fhMergeGeneratorCluster[igen][imc]->SetYTitle("#it{counts}");
        fhMergeGeneratorCluster[igen][imc]->SetXTitle("#it{E} (GeV)");
        outputContainer->Add(fhMergeGeneratorCluster[igen][imc]) ;
        
        fhMergeGeneratorClusterEPrimRecoRatio[igen][imc] = new TH2F(Form("hMergeGeneratorClusterEPrimRecoRatio%s%s%s",add.Data(),GetCocktailGenNameToCheck(igen).Data(), mcGenNames[imc].Data()),
                                                                    Form("#it{E}_{reco}/#it{E}_{gen}clusters with contribution of >=2 generators, main %s%s",
                                                                         GetCocktailGenNameToCheck(igen).Data(), mcGenTitle[imc].Data()),
                                                                    nptbins,ptmin,ptmax,300,0,3);
        fhMergeGeneratorClusterEPrimRecoRatio[igen][imc]->SetYTitle("#it{E}_{reco}/#it{E}_{gen}");
        fhMergeGeneratorClusterEPrimRecoRatio[igen][imc]->SetXTitle("#it{E}_{reco} (GeV)");
        outputContainer->Add(fhMergeGeneratorClusterEPrimRecoRatio[igen][imc]) ;
        
        fhMergeGeneratorClusterEPrimRecoDiff[igen][imc] = new TH2F(Form("hMergeGeneratorClusterEPrimRecoDiff%s%s%s",add.Data(),GetCocktailGenNameToCheck(igen).Data(), mcGenNames[imc].Data()),
                                                                   Form("#it{E}_{reco}-#it{E}_{gen}clusters with contribution of >=2 generators, main %s%s",
                                                                        GetCocktailGenNameToCheck(igen).Data(), mcGenTitle[imc].Data()),
                                                                   nptbins,ptmin,ptmax,400,-20,20);
        fhMergeGeneratorClusterEPrimRecoDiff[igen][imc]->SetYTitle("#it{E}_{reco}-#it{E}_{gen} (GeV)");
        fhMergeGeneratorClusterEPrimRecoDiff[igen][imc]->SetXTitle("#it{E}_{reco} (GeV)");
        outputContainer->Add(fhMergeGeneratorClusterEPrimRecoDiff[igen][imc]) ;
        
        if(GetCocktailGenNameToCheck(igen).Contains("ijing")) continue;
        
        //
        
        fhMergeGeneratorClusterNotHijingBkg[igen][imc] = new TH1F(Form("hMergeGeneratorClusterNotHijingBkg%s%s%s",add.Data(),GetCocktailGenNameToCheck(igen).Data(), mcGenNames[imc].Data()),
                                                                  Form("Number of selected clusters with contribution of >=2 generators, , none is HIJING, main %s%s",
                                                                       GetCocktailGenNameToCheck(igen).Data(), mcGenTitle[imc].Data()),
                                                                  nptbins,ptmin,ptmax);
        fhMergeGeneratorClusterNotHijingBkg[igen][imc]->SetYTitle("#it{counts}");
        fhMergeGeneratorClusterNotHijingBkg[igen][imc]->SetXTitle("#it{E} (GeV)");
        outputContainer->Add(fhMergeGeneratorClusterNotHijingBkg[igen][imc]) ;
        
        fhMergeGeneratorClusterNotHijingBkgEPrimRecoRatio[igen][imc] = new TH2F(Form("hMergeGeneratorClusterNotHijingBkgEPrimRecoRatio%s%s%s",add.Data(),GetCocktailGenNameToCheck(igen).Data(), mcGenNames[imc].Data()),
                                                                                Form("#it{E}_{reco}/#it{E}_{gen} clusters with contribution of >=2 generators, , none is HIJING, main %s%s",
                                                                                     GetCocktailGenNameToCheck(igen).Data(), mcGenTitle[imc].Data()),
                                                                                nptbins,ptmin,ptmax,300,0,3);
        fhMergeGeneratorClusterNotHijingBkgEPrimRecoRatio[igen][imc]->SetYTitle("#it{E}_{reco}/#it{E}_{gen}");
        fhMergeGeneratorClusterNotHijingBkgEPrimRecoRatio[igen][imc]->SetXTitle("#it{E}_{reco} (GeV)");
        outputContainer->Add(fhMergeGeneratorClusterNotHijingBkgEPrimRecoRatio[igen][imc]) ;
        
        fhMergeGeneratorClusterNotHijingBkgEPrimRecoDiff[igen][imc] = new TH2F(Form("hMergeGeneratorClusterNotHijingBkgEPrimRecoDiff%s%s%s",add.Data(),GetCocktailGenNameToCheck(igen).Data(), mcGenNames[imc].Data()),
                                                                               Form("#it{E}_{reco}-#it{E}_{gen} clusters with contribution of >=2 generators, , none is HIJING, main %s%s",
                                                                                    GetCocktailGenNameToCheck(igen).Data(), mcGenTitle[imc].Data()),
                                                                               nptbins,ptmin,ptmax,400,-20,20);
        fhMergeGeneratorClusterNotHijingBkgEPrimRecoDiff[igen][imc]->SetYTitle("#it{E}_{reco}-#it{E}_{gen} (GeV)");
        fhMergeGeneratorClusterNotHijingBkgEPrimRecoDiff[igen][imc]->SetXTitle("#it{E}_{reco} (GeV)");
        outputContainer->Add(fhMergeGeneratorClusterNotHijingBkgEPrimRecoDiff[igen][imc]) ;

        //
        
        fhMergeGeneratorClusterHijingAndOtherBkg[igen][imc] = new TH1F(Form("hMergeGeneratorClusterHijingAndOtherBkg%s%s%s",add.Data(),GetCocktailGenNameToCheck(igen).Data(), mcGenNames[imc].Data()),
                                                                       Form("Number of selected clusters with contribution of >=3 generators, none is HIJING, main %s%s",
                                                                            GetCocktailGenNameToCheck(igen).Data(), mcGenTitle[imc].Data()),
                                                                       nptbins,ptmin,ptmax);
        fhMergeGeneratorClusterHijingAndOtherBkg[igen][imc]->SetYTitle("#it{counts}");
        fhMergeGeneratorClusterHijingAndOtherBkg[igen][imc]->SetXTitle("#it{E} (GeV)");
        outputContainer->Add(fhMergeGeneratorClusterHijingAndOtherBkg[igen][imc]) ;
        
        
        fhMergeGeneratorClusterHijingAndOtherBkgEPrimRecoRatio[igen][imc] = new TH2F(Form("hMergeGeneratorClusterHijingAndOtherBkgEPrimRecoRatio%s%s%s",add.Data(),GetCocktailGenNameToCheck(igen).Data(), mcGenNames[imc].Data()),
                                                                                     Form("#it{E}_{reco}/#it{E}_{gen} clusters with contribution of >=3 generators, none is HIJING, main %s%s",
                                                                                          GetCocktailGenNameToCheck(igen).Data(), mcGenTitle[imc].Data()),
                                                                                     nptbins,ptmin,ptmax,300,0,3);
        fhMergeGeneratorClusterHijingAndOtherBkgEPrimRecoRatio[igen][imc]->SetYTitle("#it{E}_{reco}/#it{E}_{gen}");
        fhMergeGeneratorClusterHijingAndOtherBkgEPrimRecoRatio[igen][imc]->SetXTitle("#it{E}_{reco} (GeV)");
        outputContainer->Add(fhMergeGeneratorClusterHijingAndOtherBkgEPrimRecoRatio[igen][imc]) ;
        
        
        fhMergeGeneratorClusterHijingAndOtherBkgEPrimRecoDiff[igen][imc] = new TH2F(Form("hMergeGeneratorClusterHijingAndOtherBkgEPrimRecoDiff%s%s%s",add.Data(),GetCocktailGenNameToCheck(igen).Data(), mcGenNames[imc].Data()),
                                                                                    Form("#it{E}_{reco}-#it{E}_{gen} clusters with contribution of >=3 generators, none is HIJING, main %s%s",
                                                                                         GetCocktailGenNameToCheck(igen).Data(), mcGenTitle[imc].Data()),
                                                                                    nptbins,ptmin,ptmax,400,-20,20);
        fhMergeGeneratorClusterHijingAndOtherBkgEPrimRecoDiff[igen][imc]->SetYTitle("#it{E}_{reco}-#it{E}_{gen} (GeV)");
        fhMergeGeneratorClusterHijingAndOtherBkgEPrimRecoDiff[igen][imc]->SetXTitle("#it{E}_{reco} (GeV)");
        outputContainer->Add(fhMergeGeneratorClusterHijingAndOtherBkgEPrimRecoDiff[igen][imc]) ;

        //
        
        fhMergeGeneratorClusterHijingBkg[igen][imc] = new TH1F(Form("hMergeGeneratorClusterHijingBkg%s%s%s",add.Data(),GetCocktailGenNameToCheck(igen).Data(), mcGenNames[imc].Data()),
                                                               Form("Number of selected clusters with contribution of >=3 generators, none is HIJING, main %s%s",
                                                                    GetCocktailGenNameToCheck(igen).Data(), mcGenTitle[imc].Data()),
                                                               nptbins,ptmin,ptmax);
        fhMergeGeneratorClusterHijingBkg[igen][imc]->SetYTitle("#it{counts}");
        fhMergeGeneratorClusterHijingBkg[igen][imc]->SetXTitle("#it{E} (GeV)");
        outputContainer->Add(fhMergeGeneratorClusterHijingBkg[igen][imc]) ;
        
        fhMergeGeneratorClusterHijingBkgEPrimRecoRatio[igen][imc] = new TH2F(Form("hMergeGeneratorClusterHijingBkgEPrimRecoRatio%s%s%s",add.Data(),GetCocktailGenNameToCheck(igen).Data(), mcGenNames[imc].Data()),
                                                                             Form("#it{E}_{reco}/#it{E}_{gen} clusters with contribution of >=3 generators, none is HIJING, main %s%s",
                                                                                  GetCocktailGenNameToCheck(igen).Data(), mcGenTitle[imc].Data()),
                                                                             nptbins,ptmin,ptmax,300,0,3);
        fhMergeGeneratorClusterHijingBkgEPrimRecoRatio[igen][imc]->SetYTitle("#it{E}_{reco}/#it{E}_{gen}");
        fhMergeGeneratorClusterHijingBkgEPrimRecoRatio[igen][imc]->SetXTitle("#it{E}_{reco} (GeV)");
        outputContainer->Add(fhMergeGeneratorClusterHijingBkgEPrimRecoRatio[igen][imc]) ;
       
        fhMergeGeneratorClusterHijingBkgEPrimRecoDiff[igen][imc] = new TH2F(Form("hMergeGeneratorClusterHijingBkgEPrimRecoDiff%s%s%s",add.Data(),GetCocktailGenNameToCheck(igen).Data(), mcGenNames[imc].Data()),
                                                                             Form("#it{E}_{reco}-#it{E}_{gen} clusters with contribution of >=3 generators, none is HIJING, main %s%s",
                                                                                  GetCocktailGenNameToCheck(igen).Data(), mcGenTitle[imc].Data()),
                                                                             nptbins,ptmin,ptmax,400,-20,20);
        fhMergeGeneratorClusterHijingBkgEPrimRecoDiff[igen][imc]->SetYTitle("#it{E}_{reco}-#it{E}_{gen} (GeV)");
        fhMergeGeneratorClusterHijingBkgEPrimRecoDiff[igen][imc]->SetXTitle("#it{E}_{reco} (GeV)");
        outputContainer->Add(fhMergeGeneratorClusterHijingBkgEPrimRecoDiff[igen][imc]) ;
       }
    }
  }
  
  return outputContainer ;
}

//_______________________
/// Init. Do some checks, abort if the cluster is not the expected PHOS or EMCal
//_______________________
void AliAnaPhoton::Init()
{
  if      ( GetCalorimeter() == kPHOS  && !GetReader()->IsPHOSSwitchedOn()  && NewOutputAOD() )
    AliFatal("!!STOP: You want to use PHOS in analysis but it is not read!! \n!!Check the configuration file!!");
  else  if( GetCalorimeter() == kEMCAL && !GetReader()->IsEMCALSwitchedOn() && NewOutputAOD() )
    AliFatal("!!STOP: You want to use EMCAL in analysis but it is not read!! \n!!Check the configuration file!!");

  // Trick to select primary photon particles in the analysis of pure MC input
  if(GetReader()->GetDataType() == AliCaloTrackReader::kMC) GetCaloPID()->SwitchOnBayesian();
}

//_________________________________
/// Initialize the parameters of the analysis.
//_________________________________
void AliAnaPhoton::InitParameters()
{
  AddToHistogramsName("AnaPhoton_");
  
  fMinDist     = 2.;
  fMinDist2    = 4.;
  fMinDist3    = 5.;
	
  fTimeCutMin  =-1000000;
  fTimeCutMax  = 1000000;
  fNCellsCut   = 0;
	
  fRejectTrackMatch       = kTRUE ;
  
  fNEBinCuts = 14;
  fEBinCuts[0] = 0.;  fEBinCuts[1] = 0.3;  fEBinCuts[2] = 0.5;
  fEBinCuts[3] = 1.;  fEBinCuts[4] = 2. ;  fEBinCuts[5] = 3. ;
  fEBinCuts[6] = 4.;  fEBinCuts[7] = 5. ;  fEBinCuts[8] = 7. ;
  fEBinCuts[9] = 9.;  fEBinCuts[10]= 12.;  fEBinCuts[11]= 15.;
  fEBinCuts[12]= 20.; fEBinCuts[13]= 50.;  fEBinCuts[14]= 100.;
  for(Int_t i = fNEBinCuts; i < 15; i++) fEBinCuts[i] = 1000.;  
}

//_______________________________________
/// Do photon analysis and fill output particle aods with the selected clusters.
/// Clusters are selected in method ClustersSelected, and besides there
/// is a shower shape selection done after.
//_______________________________________
void  AliAnaPhoton::MakeAnalysisFillAOD()
{
  // Get the vertex
  Double_t v[3] = {0,0,0}; //vertex ;
  GetReader()->GetVertex(v);
  
  // Select the Calorimeter of the photon
  TObjArray * pl = 0x0;
  AliVCaloCells* cells    = 0;
  if      (GetCalorimeter() == kPHOS )
  {
    pl    = GetPHOSClusters();
    cells = GetPHOSCells();
  }
  else if (GetCalorimeter() == kEMCAL)
  {
    pl    = GetEMCALClusters();
    cells = GetEMCALCells();
  }
  
  if(!pl)
  {
    AliWarning(Form("TObjArray with %s clusters is NULL!",GetCalorimeterString().Data()));
    return;
  }
  
  // Loop on raw clusters before filtering in the reader and fill control histogram
  if((GetReader()->GetEMCALClusterListName()=="" && GetCalorimeter()==kEMCAL) || GetCalorimeter()==kPHOS)
  {
    for(Int_t iclus = 0; iclus < GetReader()->GetInputEvent()->GetNumberOfCaloClusters(); iclus++ )
    {
      AliVCluster * clus = GetReader()->GetInputEvent()->GetCaloCluster(iclus);
        
      if     (GetCalorimeter() == kPHOS  && clus->IsPHOS()  && clus->E() > GetReader()->GetPHOSPtMin() )
      {
        clus->GetMomentum(fMomentum,GetVertex(0)) ;
          
        fhClusterCutsE [0]->Fill(fMomentum.E() , GetEventWeight());
        fhClusterCutsPt[0]->Fill(fMomentum.Pt(), GetEventWeight());
      }
      else if(GetCalorimeter() == kEMCAL && clus->IsEMCAL() && clus->E() > GetReader()->GetEMCALPtMin())
      {
        clus->GetMomentum(fMomentum,GetVertex(0)) ;
          
        fhClusterCutsE [0]->Fill(fMomentum.E(), GetEventWeight());
        fhClusterCutsPt[0]->Fill(fMomentum.Pt(), GetEventWeight());
      }
    }
  }
  else
  { // reclusterized
    TClonesArray * clusterList = 0;
    
    if(GetReader()->GetInputEvent()->FindListObject(GetReader()->GetEMCALClusterListName()))
      clusterList = dynamic_cast<TClonesArray*> (GetReader()->GetInputEvent()->FindListObject(GetReader()->GetEMCALClusterListName()));
    else if(GetReader()->GetOutputEvent())
      clusterList = dynamic_cast<TClonesArray*> (GetReader()->GetOutputEvent()->FindListObject(GetReader()->GetEMCALClusterListName()));
    
    if(clusterList)
    {
      Int_t nclusters = clusterList->GetEntriesFast();
      for (Int_t iclus =  0; iclus <  nclusters; iclus++)
      {
        AliVCluster * clus = dynamic_cast<AliVCluster*> (clusterList->At(iclus));
          
        if(clus && clus->E() > GetReader()->GetEMCALPtMin())
        {
          clus->GetMomentum(fMomentum,GetVertex(0)) ;

          fhClusterCutsE [0]->Fill(clus->E()     , GetEventWeight());
          fhClusterCutsPt[0]->Fill(fMomentum.Pt(), GetEventWeight());
        }
      }
    }
  }
  
  // Init arrays, variables, get number of clusters
  Int_t nCaloClusters = pl->GetEntriesFast();
  
  AliDebug(1,Form("Input %s cluster entries %d", GetCalorimeterString().Data(), nCaloClusters));
  
  //----------------------------------------------------
  // Fill AOD with PHOS/EMCAL AliAODPWG4Particle objects
  //----------------------------------------------------
  // Loop on clusters
  for(Int_t icalo = 0; icalo < nCaloClusters; icalo++)
  {
    AliVCluster * calo =  (AliVCluster*) (pl->At(icalo));
    //printf("calo %d, %f\n",icalo,calo->E());
    
    //Get the index where the cluster comes, to retrieve the corresponding vertex
    Int_t evtIndex = 0 ;
    if (GetMixedEvent())
    {
      evtIndex=GetMixedEvent()->EventIndexForCaloCluster(calo->GetID()) ;
      //Get the vertex and check it is not too large in z
      if(TMath::Abs(GetVertex(evtIndex)[2])> GetZvertexCut()) continue;
    }
    
    //Cluster selection, not charged, with photon id and in fiducial cut
    if(GetReader()->GetDataType() != AliCaloTrackReader::kMC)
    {
      calo->GetMomentum(fMomentum,GetVertex(evtIndex)) ;
    }//Assume that come from vertex in straight line
    else
    {
      Double_t vertex[]={0,0,0};
      calo->GetMomentum(fMomentum,vertex) ;
    }
    
    //-----------------------------
    // Cluster selection
    //-----------------------------
    Int_t nMaxima = GetCaloUtils()->GetNumberOfLocalMaxima(calo, cells); // NLM
    if(!ClusterSelected(calo,nMaxima)) continue;
    
    //----------------------------
    // Create AOD for analysis
    //----------------------------
    AliAODPWG4Particle aodph = AliAODPWG4Particle(fMomentum);
    
    //...............................................
    // Set the indeces of the original caloclusters (MC, ID), and calorimeter
    Int_t label = calo->GetLabel();
    aodph.SetLabel(label);
    aodph.SetCaloLabel(calo->GetID(),-1);
    aodph.SetDetectorTag(GetCalorimeter());
    //printf("Index %d, Id %d, iaod %d\n",icalo, calo->GetID(),GetOutputAODBranch()->GetEntriesFast());
    
    //...............................................
    // Set bad channel distance bit
    Double_t distBad=calo->GetDistanceToBadChannel() ; //Distance to bad channel
    if     (distBad > fMinDist3) aodph.SetDistToBad(2) ;
    else if(distBad > fMinDist2) aodph.SetDistToBad(1) ;
    else                         aodph.SetDistToBad(0) ;
    //printf("DistBad %f Bit %d\n",distBad, aodph.DistToBad());
    
    //-------------------------------------
    // Play with the MC stack if available
    //-------------------------------------
    
    // Check origin of the candidates
    Int_t tag = -1;
    
    if(IsDataMC())
    {
      tag = GetMCAnalysisUtils()->CheckOrigin(calo->GetLabels(),calo->GetNLabels(),GetReader(),GetCalorimeter());
      aodph.SetTag(tag);
      
      AliDebug(1,Form("Origin of candidate, bit map %d",aodph.GetTag()));
    }//Work with stack also
    
    //--------------------------------------------------------
    // Fill some shower shape histograms before PID is applied
    //--------------------------------------------------------
    
    Float_t maxCellFraction = 0;
    Int_t absIdMax = GetCaloUtils()->GetMaxEnergyCell(cells, calo, maxCellFraction);
    if( absIdMax < 0 ) AliFatal("Wrong absID");
    
    Int_t largeTimeInCellCluster = kFALSE;
    FillShowerShapeHistograms(calo,tag,nMaxima,maxCellFraction,largeTimeInCellCluster);
    aodph.SetFiducialArea(largeTimeInCellCluster); // Temporary use of this container, FIXME
    //if(largeTimeInCellCluster > 1) printf("Set n cells large time %d, pt %2.2f\n",aodph.GetFiducialArea(),aodph.Pt());
    
    aodph.SetM02(calo->GetM02());
    aodph.SetM20(calo->GetM20());
    aodph.SetNLM(nMaxima);
    
    Float_t time = calo->GetTOF()*1e9;
    if(time > 400) time-=fConstantTimeShift; // in case of clusterizer running before (calibrate clusters not cells)
    aodph.SetTime(time);
    
    aodph.SetNCells(calo->GetNCells());
    Int_t nSM = GetModuleNumber(calo);
    aodph.SetSModNumber(nSM);

    Float_t en  = fMomentum.E ();
    Float_t pt  = fMomentum.Pt();
    Float_t eta = fMomentum.Eta();
    Float_t phi = GetPhi(fMomentum.Phi());
    Int_t ebin = -1;
    for(Int_t ie = 0; ie < fNEBinCuts; ie++)
    {
      if( en >= fEBinCuts[ie] && en < fEBinCuts[ie+1] ) ebin = ie;
    }
    
    Int_t icolAbs = -1, irowAbs = -1;
    if(fFillEBinAcceptanceHisto)
    {
      Float_t maxCellFraction = 0;
      Int_t absIdMax = GetCaloUtils()->GetMaxEnergyCell(cells,calo,maxCellFraction);
      
      Int_t icol = -1, irow = -1, iRCU = -1; 
      GetModuleNumberCellIndexesAbsCaloMap(absIdMax,GetCalorimeter(), icol, irow, iRCU, icolAbs, irowAbs);
      
      if(ebin>=0 && ebin < fNEBinCuts) 
      {
        fhEBinClusterEtaPhi[ebin]->Fill(eta,phi,GetEventWeight()) ;
        
        fhEBinClusterColRow[ebin]->Fill(icolAbs,irowAbs,GetEventWeight()) ;
      }
    }

    //-------------------------------------
    // PID selection or bit setting
    //-------------------------------------
    
    //...............................................
    // Data, PID check on
    if(IsCaloPIDOn())
    {
      // Get most probable PID, 2 options check bayesian PID weights or redo PID
      // By default, redo PID
      
      aodph.SetIdentifiedParticleType(GetCaloPID()->GetIdentifiedParticleType(calo));
      
      AliDebug(1,Form("PDG of identified particle %d",aodph.GetIdentifiedParticleType()));
      
      //If cluster does not pass pid, not photon, skip it.
      if(aodph.GetIdentifiedParticleType() != AliCaloPID::kPhoton) continue ;
    }
    
    //...............................................
    // Data, PID check off
    else
    {
      // Set PID bits for later selection (AliAnaPi0 for example)
      // GetIdentifiedParticleType already called in SetPIDBits.
      
      GetCaloPID()->SetPIDBits(calo,&aodph, GetCaloUtils(),GetReader()->GetInputEvent());
      
      AliDebug(1,"PID Bits set");
    }
    
    AliDebug(1,Form("Photon selection cuts passed: pT %3.2f, pdg %d",aodph.Pt(),aodph.GetIdentifiedParticleType()));
    
    fhClusterCutsE [9]->Fill(en, GetEventWeight());
    fhClusterCutsPt[9]->Fill(pt, GetEventWeight());
    
    //
    // Check local cluster activity around the current cluster
    //
    if(fStudyActivityNearCluster && en > 1.5) // 1.5 GeV cut used on Pb-Pb analysis
      ActivityNearCluster(icalo,en,eta,phi,pl);

    //
    // Check if other generators contributed to the cluster
    //
    if( IsDataMC() && IsStudyClusterOverlapsPerGeneratorOn() )
      CocktailGeneratorsClusterOverlaps(calo,tag);

    
    if(fFillEBinAcceptanceHisto)
    {  
      if(ebin>=0 && ebin < fNEBinCuts) 
      {
        fhEBinClusterEtaPhiPID[ebin]->Fill(eta,phi,GetEventWeight()) ;
        
        fhEBinClusterColRowPID[ebin]->Fill(icolAbs,irowAbs,GetEventWeight()) ;
      }
    }
    
    if(nSM < GetCaloUtils()->GetNumberOfSuperModulesUsed() && nSM >=0)
    {
      fhEPhotonSM ->Fill(en, nSM, GetEventWeight());
      fhPtPhotonSM->Fill(pt, nSM, GetEventWeight());
    }
    
    fhNLocMax->Fill(calo->E(),nMaxima);
    
    // Few more control histograms for selected clusters
    fhMaxCellDiffClusterE->Fill(en, maxCellFraction    , GetEventWeight());
    fhNCellsE            ->Fill(en, calo->GetNCells()  , GetEventWeight());
    fhTimePt             ->Fill(pt, time               , GetEventWeight());
    
    if(cells)
    {
      for(Int_t icell = 0; icell <  calo->GetNCells(); icell++)
        fhCellsE->Fill(en, cells->GetCellAmplitude(calo->GetCellsAbsId()[icell]), GetEventWeight());
    }
    
    // Matching after cuts
    if( fFillTMHisto )         FillTrackMatchingResidualHistograms(calo,1);
    
    // Fill histograms to undertand pile-up before other cuts applied
    // Remember to relax time cuts in the reader
    if( IsPileUpAnalysisOn() ) FillPileUpHistograms(calo,cells, absIdMax);
    
    // Add AOD with photon object to aod branch
    AddAODParticle(aodph);
  }// loop
  
  AliDebug(1,Form("End fill AODs, with %d entries",GetOutputAODBranch()->GetEntriesFast()));
}

//______________________________________________
// Fill histograms with selected clusters/output AOD particles.
//______________________________________________
void  AliAnaPhoton::MakeAnalysisFillHistograms()
{
  // In case of simulated data, fill acceptance histograms
  if(IsDataMC()) FillAcceptanceHistograms();
  
  // Get vertex
  Double_t v[3] = {0,0,0}; //vertex ;
  GetReader()->GetVertex(v);
  //fhVertex->Fill(v[0],v[1],v[2]);
  if(TMath::Abs(v[2]) > GetZvertexCut()) return ; // done elsewhere for Single Event analysis, but there for mixed event
  
  //----------------------------------
  // Loop on stored AOD photons
  Int_t naod = GetOutputAODBranch()->GetEntriesFast();
  AliDebug(1,Form("AOD branch entries %d", naod));
  
  Float_t cen = GetEventCentrality();
  // printf("++++++++++ GetEventCentrality() %f\n",cen);
  
  Float_t ep  = GetEventPlaneAngle();

  for(Int_t iaod = 0; iaod < naod ; iaod++)
  {
    AliAODPWG4Particle* ph =  (AliAODPWG4Particle*) (GetOutputAODBranch()->At(iaod));
    Int_t pdg = ph->GetIdentifiedParticleType();
    
    AliDebug(2,Form("PDG %d, MC TAG %d, Calorimeter <%d>",ph->GetIdentifiedParticleType(),ph->GetTag(), ph->GetDetectorTag())) ;
    
    // If PID used, fill histos with photons in Calorimeter GetCalorimeter()
    if(IsCaloPIDOn() && pdg != AliCaloPID::kPhoton) continue;
    
    if(((Int_t) ph->GetDetectorTag()) != GetCalorimeter()) continue;
    
    AliDebug(2,Form("ID Photon: pt %f, phi %f, eta %f", ph->Pt(),ph->Phi(),ph->Eta())) ;
    
    //................................
    //Fill photon histograms
    Float_t ptcluster  = ph->Pt();
    Float_t phicluster = ph->Phi();
    Float_t etacluster = ph->Eta();
    Float_t ecluster   = ph->E();
    
    fhEPhoton   ->Fill(ecluster , GetEventWeight());
    fhPtPhoton  ->Fill(ptcluster, GetEventWeight());
      
    fhPhiPhoton ->Fill(ptcluster, phicluster, GetEventWeight());
    fhEtaPhoton ->Fill(ptcluster, etacluster, GetEventWeight());
      
    if     (ecluster   > 0.5) fhEtaPhiPhoton  ->Fill(etacluster, phicluster, GetEventWeight());
    else if(GetMinPt() < 0.5) fhEtaPhi05Photon->Fill(etacluster, phicluster, GetEventWeight());
    
    if(IsHighMultiplicityAnalysisOn())
    {
      fhPtCentralityPhoton ->Fill(ptcluster,cen, GetEventWeight()) ;
      fhPtEventPlanePhoton ->Fill(ptcluster,ep , GetEventWeight()) ;
    }
  
//    Comment this part, not needed but in case to know how to do it in the future
//    // Get original cluster, to recover some information
//    AliVCaloCells* cells    = 0;
//    TObjArray * clusters    = 0;
//    if(GetCalorimeter() == kEMCAL)
//    {
//      cells    = GetEMCALCells();
//      clusters = GetEMCALClusters();
//    }
//    else
//    {
//      cells    = GetPHOSCells();
//      clusters = GetPHOSClusters();
//    }
//    
//    Int_t iclus = -1;
//    AliVCluster *cluster = FindCluster(clusters,ph->GetCaloLabel(0),iclus);
//    if(cluster)
    
    //.......................................
    // Play with the MC data if available
    if(IsDataMC())
    {
      //....................................................................
      // Access MC information in stack if requested, check that it exists.
      Int_t label = ph->GetLabel();
      
      if(label < 0)
      {
        AliDebug(1,Form("*** bad label ***:  label %d", label));
        continue;
      }
      
      Float_t eprim   = 0;
      Float_t ptprim  = 0;
      Bool_t ok = kFALSE;
      Int_t pdg = 0, status = 0, momLabel = -1;
      
      //fPrimaryMom = GetMCAnalysisUtils()->GetMother(label,GetReader(),ok);
      fPrimaryMom = GetMCAnalysisUtils()->GetMother(label,GetReader(), pdg, status, ok, momLabel);     
      
      if(ok)
      {
        eprim   = fPrimaryMom.Energy();
        ptprim  = fPrimaryMom.Pt();
      }
      
      Int_t tag =ph->GetTag();
      Int_t mcParticleTag = -1;
      if( GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCPhoton) && fhMCE[kmcPhoton])
      {
        fhMCE  [kmcPhoton] ->Fill(ecluster , GetEventWeight());
        fhMCPt [kmcPhoton] ->Fill(ptcluster, GetEventWeight());
          
        fhMCPhi[kmcPhoton] ->Fill(ecluster,phicluster, GetEventWeight());
        fhMCEta[kmcPhoton] ->Fill(ecluster,etacluster, GetEventWeight());
        
        fhMC2E     [kmcPhoton] ->Fill(ecluster , eprim , GetEventWeight());
        fhMC2Pt    [kmcPhoton] ->Fill(ptcluster, ptprim, GetEventWeight());
          
        fhMCDeltaE [kmcPhoton] ->Fill(ecluster , eprim-ecluster  , GetEventWeight());
        fhMCDeltaPt[kmcPhoton] ->Fill(ptcluster, ptprim-ptcluster, GetEventWeight());
        
        if(GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCConversion) &&
           fhMCE[kmcConversion])
        {
          fhMCE  [kmcConversion] ->Fill(ecluster , GetEventWeight());
          fhMCPt [kmcConversion] ->Fill(ptcluster, GetEventWeight());
            
          fhMCPhi[kmcConversion] ->Fill(ecluster, phicluster, GetEventWeight());
          fhMCEta[kmcConversion] ->Fill(ecluster, etacluster, GetEventWeight());
          
          fhMC2E     [kmcConversion] ->Fill(ecluster , eprim , GetEventWeight());
          fhMC2Pt    [kmcConversion] ->Fill(ptcluster, ptprim, GetEventWeight());
            
          fhMCDeltaE [kmcConversion] ->Fill(ecluster , eprim-ecluster  , GetEventWeight());
          fhMCDeltaPt[kmcConversion] ->Fill(ptcluster, ptprim-ptcluster, GetEventWeight());
                    
          Int_t pdgD = 0, statusD = 0, daugLabel = -1;
          Bool_t okD = kFALSE;
          
          //fMomentum = 
          GetMCAnalysisUtils()->GetDaughter(0,momLabel,GetReader(),pdgD, statusD, okD, daugLabel, fProdVertex);
          
          if(okD)
          {
            Float_t prodR = TMath::Sqrt(fProdVertex.X()*fProdVertex.X()+fProdVertex.Y()*fProdVertex.Y());

            //printf("Conversion: mom pdg %d (stat %d), 1st daugher %d (stat %d), mom label %d, org label %d, daugh label %d, prodR %f\n",pdg,status, pdgD, statusD, 
            //       momLabel, label,daugLabel,prodR);

            if(fFillConversionVertexHisto)
            {
              
              fhMCConversionVertex->Fill(ptcluster,prodR,GetEventWeight());
              
              if(GetCalorimeter() == kEMCAL && GetFirstSMCoveredByTRD() >= 0 && ph->GetSModNumber() >= GetFirstSMCoveredByTRD() )
                fhMCConversionVertexTRD->Fill(ptcluster,prodR,GetEventWeight());
            }
            
            if ( fFillSSHistograms )
            {
              Float_t m02 = ph->GetM02();
              Float_t m20 = ph->GetM20();
              
              // conversion vertex vs shower shape
              if(fFillConversionVertexHisto)
              {
                Int_t convR = -1;
                if      ( prodR < 75.  ) convR = 0;
                else if ( prodR < 275. ) convR = 1;
                else if ( prodR < 375. ) convR = 2;
                else if ( prodR < 400. ) convR = 3;
                else if ( prodR < 430. ) convR = 4;
                else                     convR = 5;
                
                if ( convR >= 0 )
                {
                  fhMCConversionLambda0Rcut[convR]->Fill(ptcluster,m02,GetEventWeight());
                  fhMCConversionLambda1Rcut[convR]->Fill(ptcluster,m20,GetEventWeight());
                  
                  if ( GetCalorimeter() == kEMCAL && GetFirstSMCoveredByTRD() >= 0 && ph->GetSModNumber() >= GetFirstSMCoveredByTRD() )
                  {
                    fhMCConversionLambda0RcutTRD[convR]->Fill(ptcluster,m02,GetEventWeight());
                    fhMCConversionLambda1RcutTRD[convR]->Fill(ptcluster,m20,GetEventWeight());
                  }
                  //                //
                  //                // EMCAL SM regions
                  //                //
                  //                if ( GetCalorimeter() == kEMCAL && fFillEMCALRegionSSHistograms )
                  //                {
                  //                  // Get original cluster, needed to feed the subregion selection method
                  //                  
                  //                  Int_t iclus = -1;
                  //                  AliVCluster *cluster = FindCluster(GetEMCALClusters(),ph->GetCaloLabel(0),iclus);
                  //
                  //                  Int_t etaRegion = -1, phiRegion = -1;
                  //                  
                  //                  if ( cluster ) GetCaloUtils()->GetEMCALSubregion(cluster,GetReader()->GetEMCALCells(),etaRegion,phiRegion);
                  //                  
                  //                  if( etaRegion >= 0 && etaRegion < 4 && phiRegion >=0 && phiRegion < 3 ) 
                  //                  {
                  //                    fhLam0EMCALRegionMCConvRcut[etaRegion][phiRegion][convR]->Fill(ptcluster,m02, GetEventWeight());
                  //                    
                  //                    if ( GetFirstSMCoveredByTRD() >= 0 && ph->GetSModNumber() >= GetFirstSMCoveredByTRD()  )
                  //                      fhLam0EMCALRegionTRDMCConvRcut[etaRegion][phiRegion][convR]->Fill(ptcluster, m02, GetEventWeight());
                  //                    
                  //                  } // region found
                  //                } // check region
                } // conv region
              } // check conv region
              
            } // fill Sh Sh histograms
          } // okD
        } // conversion
        
        if     ( GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCPrompt) )
        {
          mcParticleTag = kmcPrompt;
        }
        else if( GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCFragmentation) )
        {
          mcParticleTag = kmcFragmentation;
        }
        else if( GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCISR) )
        {
          mcParticleTag = kmcISR;
        }
        else if( GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCPi0) )
        {
          mcParticleTag = kmcPi0;
        }
        else if( GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCEta) )
        {
          mcParticleTag = kmcEta;
        }
        else if( GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCPi0Decay) )
        {
          mcParticleTag = kmcPi0Decay;
        }
        else if( GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCEtaDecay) )
        {
          mcParticleTag = kmcEtaDecay;
        }
        else
        {
          mcParticleTag = kmcOtherDecay;
        }
      }
      else if( GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCAntiNeutron) )
      {
        mcParticleTag = kmcAntiNeutron;
      }
      else if( GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCAntiProton) )
      {
        mcParticleTag = kmcAntiProton;
      }
      else if( GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCNeutron) )
      {
        mcParticleTag = kmcNeutron;
      }
      else if( GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCProton) )
      {
        mcParticleTag = kmcProton;
      }
      else if( GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCPion) )
      {
        mcParticleTag = kmcChPion;
      }
      else if( GetMCAnalysisUtils()->CheckTagBit(tag,AliMCAnalysisUtils::kMCElectron) )
      {
        mcParticleTag = kmcElectron;
      }
      else if( fhMCE[kmcOther] )
      {
        mcParticleTag = kmcOther;
        
        //		 printf(" AliAnaPhoton::MakeAnalysisFillHistograms() - Label %d, pT %2.3f Unknown, bits set: ",
        //					ph->GetLabel(),ph->Pt());
        //		  for(Int_t i = 0; i < 20; i++) {
        //			  if(GetMCAnalysisUtils()->CheckTagBit(tag,i)) printf(" %d, ",i);
        //		  }
        //		  printf("\n");
      }
      
      if(mcParticleTag >= 0 && fhMCE[mcParticleTag])
      {
        fhMCE      [mcParticleTag]->Fill(ecluster , GetEventWeight());
        fhMCPt     [mcParticleTag]->Fill(ptcluster, GetEventWeight());
          
        fhMCPhi    [mcParticleTag]->Fill(ecluster,  phicluster, GetEventWeight());
        fhMCEta    [mcParticleTag]->Fill(ecluster,  etacluster, GetEventWeight());
        
        fhMC2E     [mcParticleTag]->Fill(ecluster , eprim , GetEventWeight());
        fhMC2Pt    [mcParticleTag]->Fill(ptcluster, ptprim, GetEventWeight());
          
        fhMCDeltaE [mcParticleTag]->Fill(ecluster , eprim-ecluster  , GetEventWeight());
        fhMCDeltaPt[mcParticleTag]->Fill(ptcluster, ptprim-ptcluster, GetEventWeight());
      }
    }// Histograms with MC
  }// aod loop
}

//__________________________________________________
/// Print some relevant parameters set for the analysis.
//__________________________________________________
void AliAnaPhoton::Print(const Option_t * opt) const
{
  if(! opt)
    return;
  
  printf("**** Print %s %s ****\n", GetName(), GetTitle() ) ;
  AliAnaCaloTrackCorrBaseClass::Print(" ");
  
  printf("Calorimeter            =     %s\n", GetCalorimeterString().Data()) ;
  printf("Min Distance to Bad Channel   = %2.1f\n",fMinDist);
  printf("Min Distance to Bad Channel 2 = %2.1f\n",fMinDist2);
  printf("Min Distance to Bad Channel 3 = %2.1f\n",fMinDist3);
  printf("Reject clusters with a track matched = %d\n",fRejectTrackMatch);
  printf("Time Cut: %3.1f < TOF  < %3.1f\n", fTimeCutMin, fTimeCutMax);
  printf("Number of cells in cluster is        > %d \n", fNCellsCut);
  printf("    \n") ;
}
