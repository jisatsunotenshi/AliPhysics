void checkThr(const char *filename, const char *what="")
{
  TFile *f = TFile::Open(filename);
  TList *list = (TList*)f->Get("coutput");

  TH1F *h3 = (TH1F*)list->FindObject("fV0Percent");
  Double_t nTotalEvts = (Double_t)h3->Integral(h3->FindBin(0.0),h3->FindBin(91.0))/0.9;

  TH1F *hCent;
  TH1F *hCentAll;
  TH1F *hSemiCent;
  TH1F *hSemiCentAll;
  Double_t nentCent;
  Double_t nentCentall;
  Double_t nentSemiCent;
  Double_t nentSemiCentall;
  TF1 *ffCent;
  TF1 *ffSemiCent;

  hCent = (TH1F*)list->FindObject(Form("fV0Cent%s",what));
  hCentAll = (TH1F*)list->FindObject(Form("fV0Cent%sAll",what));
  hSemiCent = (TH1F*)list->FindObject(Form("fV0SemiCent%s",what));
  hSemiCentAll = (TH1F*)list->FindObject(Form("fV0SemiCent%sAll",what));

  nentCent = hCent->GetEntries();
  Double_t nCentEvts = (Double_t)hCent->Integral(hCent->FindBin(0.0),hCent->FindBin(91.0));
  nentCentall = hCentAll->GetEntries();
  nentSemiCent = hSemiCent->GetEntries();
  Double_t nSemiCentEvts = (Double_t)hSemiCent->Integral(hSemiCent->FindBin(0.0),hSemiCent->FindBin(91.0));
  nentSemiCentall = hSemiCentAll->GetEntries();

  hCent->Sumw2();
  hCent->Divide(hCent,h3,1,1,"B");
  hSemiCent->Sumw2();
  hSemiCent->Divide(hSemiCent,h3,1,1,"B");
  // for(Int_t ibin = 1; ibin <= hSemiCent->GetXaxis()->GetNbins(); ++ibin) {
  //   printf("%f ",hSemiCent->GetBinError(ibin));
  //   if (hSemiCent->GetBinError(ibin) < 1e-6) hSemiCent->SetBinError(ibin,0.01);
  //   printf("%f ",hSemiCent->GetBinError(ibin));
  // }

  ffCent = new TF1("ffCent","(1.-1./(1.+TMath::Exp(-(x-[0])/[1])))*[2]",0,100);
  ffCent->SetLineColor(kBlue);
  Double_t par0 = hCent->GetBinCenter(hCent->FindLastBinAbove((strcmp(what,"Tr") == 0) ? 0.6 : 0.95));
  ffCent->SetParameter(0,par0);
  ffCent->SetParameter(1,0.5);
  if (strcmp(what,"Tr") == 0)
    ffCent->SetParameter(2,0.6);
  else
    ffCent->SetParLimits(2,0.9999,1.0001);
  hCent->Fit(ffCent,"R+");
  hCent->Fit(ffCent,"R+");
  hCent->Fit(ffCent,"R+");

  ffSemiCent = new TF1("ffSemiCent","(1.-1./(1.+TMath::Exp(-(x-[0])/[1])))*[2]",0,100);
  ffSemiCent->SetLineColor(kRed);
  par0 = hSemiCent->GetBinCenter(hSemiCent->FindLastBinAbove((strcmp(what,"Tr") == 0) ? 0.01 : 0.95));
  ffSemiCent->SetParameter(0,par0);
  ffSemiCent->SetParameter(1,0.3);
  printf("%d\n",par0);
  if (strcmp(what,"Tr") == 0)
    ffSemiCent->SetParameter(2,0.1);
  else
    ffSemiCent->SetParLimits(2,0.9999,1.0001);
  hSemiCent->Fit(ffSemiCent,"R+");
  hSemiCent->Fit(ffSemiCent,"R+");
  hSemiCent->Fit(ffSemiCent,"R+");

  //  new TCanvas;
  hSemiCent->SetLineColor(kBlue);
  hSemiCent->SetMarkerColor(kBlue);
  hCent->Draw("e");
  hSemiCent->SetLineColor(kRed);
  hSemiCent->SetMarkerColor(kRed);
  hSemiCent->Draw("e same");

  Float_t rawRatio = (strcmp(what,"Tr") == 0) ? (nentSemiCentall/ffSemiCent->GetParameter(2))/(nentCentall/ffCent->GetParameter(2)) : nentSemiCentall/nentCentall;
  Float_t rawRatioErr = rawRatio*TMath::Sqrt(1./nentCentall-1./nentSemiCentall);
  Float_t psRatio = (strcmp(what,"Tr") == 0) ? (nentSemiCent/ffSemiCent->GetParameter(2))/(nentCent/ffCent->GetParameter(2)) : nentSemiCent/nentCent;
  Float_t psRatioErr = psRatio*TMath::Sqrt(1./nentCent-1./nentSemiCent);
  Float_t psScRatio = (strcmp(what,"Tr") == 0) ? (nSemiCentEvts/ffSemiCent->GetParameter(2))/(nCentEvts/ffCent->GetParameter(2)) : nSemiCentEvts/nCentEvts;
  Float_t psScRatioErr = psScRatio*TMath::Sqrt(1./nCentEvts-1./nSemiCentEvts);

  Float_t centrInt = 100.*nCentEvts/ffCent->GetParameter(2)/nTotalEvts;
  Float_t centrIntErr = 100.*TMath::Sqrt(nCentEvts/ffCent->GetParameter(2)/nTotalEvts*(1.-nCentEvts/ffCent->GetParameter(2)/nTotalEvts)/nTotalEvts);
  Float_t centrIntFit = ffCent->GetX(0.5*ffCent->GetParameter(2));
  Float_t centrIntFitErr = ffCent->GetParError(0);

  printf("\n\n\n");

  printf("Purity: Centr      %.3f %.3f   Cuts ( 0.0-%.1f)%%   rate=%.1f Hz\n",
	 nCentEvts/nCentEvts,
	 nentCent/nentCentall,
	 ffCent->GetX(0.99*ffCent->GetParameter(2)),
	 30.*10./(nentCent/nentCentall));
  printf("Purity: Semi-Centr %.3f %.3f   Cuts ( 0.0-%.1f)%%   rate=%.1f Hz\n",
	 ffSemiCent->Integral(0,50)/ffSemiCent->Integral(0,100),
	 ffSemiCent->Integral(0,50)/ffSemiCent->Integral(0,100)*nentSemiCent/nentSemiCentall,
	 ffSemiCent->GetX(0.99*ffSemiCent->GetParameter(2)),
	 30.*50./(ffSemiCent->Integral(0,50)/ffSemiCent->Integral(0,100)*nentSemiCent/nentSemiCentall));
  printf("Ratio of raw rates: %.2f +- %.2f\n",
	 rawRatio,
	 rawRatioErr);
  printf("Ratio of rates after PS: %.2f +- %.2f\n",
	 psRatio,
	 psRatioErr);
  printf("Ratio of rates after PS and SC: %.2f +- %.2f\n",
	 psScRatio,
	 psScRatioErr);
  printf("Integral of central trigger: %.2f %% +- %.2f %%\n",
	 centrInt,
	 centrIntErr);
  printf("Integral of central trigger (fit): %.2f %% +- %.2f %%\n",
	 centrIntFit,
	 centrIntFitErr);

  printf("\n\n");
  FILE *fout=fopen(Form("./check.%s.txt",filename),"w");
  if (!fout) {
    printf("Failed to open local result file\n");
    return;
  }
  printf("%.2f %.2f   %.2f %.2f   %.2f %.2f   %.2f %.2f   %.2f %.2f\n\n",
	 rawRatio,rawRatioErr,
	 psRatio,psRatioErr,
	 psScRatio,psScRatioErr,
	 centrInt,centrIntErr,
	 centrIntFit,centrIntFitErr);
  fprintf(fout,"%.2f %.2f   %.2f %.2f   %.2f %.2f   %.2f %.2f   %.2f %.2f\n\n",
	  rawRatio,rawRatioErr,
	  psRatio,psRatioErr,
	  psScRatio,psScRatioErr,
	  centrInt,centrIntErr,
	  centrIntFit,centrIntFitErr);
  fclose(fout);

  new TCanvas;
  h3->Rebin(4);
  TFitResultPtr fitRes = h3->Fit("pol0","S","",0,89);
  printf("Centrality flatness, Chi2=%.3f/%d\n",fitRes->Chi2(),fitRes->Ndf());

  // TH1F *hand = (TH1F*)list->FindObject("fV0Percent");
  // TH1F *handall = (TH1F*)list->FindObject("fV0PercentAll");
  // if (handall) {
  //   TH1F *hmb63 = (TH1F*)list->FindObject("fV0Percent63");
  //   TH1F *hmb63all = (TH1F*)list->FindObject("fV0Percent63All");
  //   printf("V0AND: purity=%f %%\n",hand->Integral(0,hand->FindBin(90.))/(Float_t)handall->GetEntries()*100.);
  //   printf(" MB63: purity=%f %% eff=%f %%\n",hmb63->Integral(0,hmb63->FindBin(90.))/hmb63all->GetEntries()*100.,hmb63->Integral(0,hmb63->FindBin(90.))/hand->Integral(0,hand->FindBin(90.))*100.);
  //   hmb63->Sumw2();
  //   hmb63->Divide(hmb63,hand,1,1,"B");
  //   new TCanvas;
  //   hmb63->SetStats(0);
  //   hmb63->SetTitle("MultA+MultC>=63 efficiency as a function of centrality percentile");
  //   hmb63->Draw();
  // }
}
