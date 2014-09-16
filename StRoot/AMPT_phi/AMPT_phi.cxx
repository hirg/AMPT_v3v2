#include "AMPT_phi.h"
#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TBranch.h"
#include "TProfile.h"
#include "TVector2.h"
#include "TVector3.h"
#include "TMath.h"
#include "TLorentzVector.h"

ClassImp(AMPT_phi)

Int_t AMPT_phi::mInput_flag = 1;
TString AMPT_phi::mBeamEnergy[7] = {"7GeV","11GeV","19GeV","27GeV","39GeV","62GeV","200GeV"};
TString AMPT_phi::mMode_AMPT[2] = {"Default","StringMelting"};
Int_t AMPT_phi::mRefMult[2][7][10] = {
			 	        { // Default
					  {9,16,26,42,63,91,129,182,219,327}, //  7GeV
					  {10,18,31,49,74,106,148,207,246,352}, // 11GeV
					  {12,21,35,56,83,119,166,229,270,385}, // 19GeV
					  {13,23,39,60,90,128,178,246,289,422}, // 27GeV
			 	          {15,26,43,68,100,143,198,273,321,457}, // 39GeV
					  {16,28,48,75,112,161,225,312,368,521}, // 62GeV
					  {23,42,72,116,176,257,364,513,610,890}  // 200GeV
			 	        },
			 	        { // String Melting
					  {9,16,27,41,61,86,119,163,193,287}, //  7GeV
					  {11,19,32,51,75,106,147,201,237,345}, // 11GeV
					  {12,22,38,59,88,126,175,240,281,414}, // 19GeV
			 	          {13,24,40,64,95,137,190,261,307,443}, // 27GeV
					  {15,27,45,71,106,152,212,292,342,517}, // 39GeV
					  {16,29,49,77,117,169,238,331,390,628},  // 62GeV
					  {23,42,73,119,184,273,392,557,666,1020}  // 200GeV
			 	        }
			 	      }; // 80%,70%,60%,50%,40%,30%,20%,10%,5%,0%
// Centrality bin
Int_t AMPT_phi::cent_low[4] = {0,7,4,0}; // 0 = 0-80%, 1 = 0-10%, 2 = 10-40%, 3 = 40-80%
Int_t AMPT_phi::cent_up[4]  = {8,8,6,3}; // 0 = 0-80%, 1 = 0-10%, 2 = 10-40%, 3 = 40-80%
Int_t AMPT_phi::mList_start[15] = {  1,101,201,301,401,501,601,701,801, 901,1001,1101,1201,1301,1401};
Int_t AMPT_phi::mList_stop[15]  = {100,200,300,400,500,600,700,800,900,1000,1100,1200,1300,1400,1500};
Int_t AMPT_phi::Centrality_start = 0;
Int_t AMPT_phi::Centrality_stop  = 4;
Float_t AMPT_phi::mMassKaon = 0.49368;

// pt bin
//                                       0 , 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 ,10 ,11 ,12 ,13 ,14 ,15 ,16 ,17 ,18 ,19 ,10 ,21 ,22
Float_t AMPT_phi::pt_low_phi[23] = {0.2,0.4,0.6,0.8,1.0,1.2,1.4,1.6,1.8,2.0,2.2,2.4,2.6,2.8,3.0,3.4,3.8,4.2,4.6,5.0,5.4,5.8,6.2};
Float_t AMPT_phi::pt_up_phi[23]  = {0.4,0.6,0.8,1.0,1.2,1.4,1.6,1.8,2.0,2.2,2.4,2.6,2.8,3.0,3.4,3.8,4.2,4.6,5.0,5.4,5.8,6.2,6.6};

// phi-Psi bin
Float_t AMPT_phi::phi_Psi2_low[7] = {0.0,TMath::Pi()/14.0,2.0*TMath::Pi()/14.0,3.0*TMath::Pi()/14.0,4.0*TMath::Pi()/14.0,5.0*TMath::Pi()/14.0,6.0*TMath::Pi()/14.0};
Float_t AMPT_phi::phi_Psi2_up[7]  = {TMath::Pi()/14.0,2.0*TMath::Pi()/14.0,3.0*TMath::Pi()/14.0,4.0*TMath::Pi()/14.0,5.0*TMath::Pi()/14.0,6.0*TMath::Pi()/14.0,7.0*TMath::Pi()/14.0};
Float_t AMPT_phi::phi_Psi3_low[7] = {0.0,TMath::Pi()/21.0,2.0*TMath::Pi()/21.0,3.0*TMath::Pi()/21.0,4.0*TMath::Pi()/21.0,5.0*TMath::Pi()/21.0,6.0*TMath::Pi()/21.0};
Float_t AMPT_phi::phi_Psi3_up[7]  = {TMath::Pi()/21.0,2.0*TMath::Pi()/21.0,3.0*TMath::Pi()/21.0,4.0*TMath::Pi()/21.0,5.0*TMath::Pi()/21.0,6.0*TMath::Pi()/21.0,7.0*TMath::Pi()/21.0};

Float_t AMPT_phi::Psi2_low[3] = {-3.0*TMath::Pi()/2.0,-1.0*TMath::Pi()/2.0,1.0*TMath::Pi()/2.0};
Float_t AMPT_phi::Psi2_up[3]  = {-1.0*TMath::Pi()/2.0, 1.0*TMath::Pi()/2.0,3.0*TMath::Pi()/2.0};
Float_t AMPT_phi::Psi3_low[5] = {-4.0*TMath::Pi()/3.0,-3.0*TMath::Pi()/3.0,-1.0*TMath::Pi()/3.0,1.0*TMath::Pi()/3.0,3.0*TMath::Pi()/3.0};
Float_t AMPT_phi::Psi3_up[5]  = {-3.0*TMath::Pi()/3.0,-1.0*TMath::Pi()/3.0, 1.0*TMath::Pi()/3.0,3.0*TMath::Pi()/3.0,4.0*TMath::Pi()/3.0};

Int_t AMPT_phi::pt_total_phi = 23;
Int_t AMPT_phi::Phi_Psi_total = 7;
//------------------------------------------------------------
AMPT_phi::AMPT_phi(Int_t Energy, Int_t Mode, Int_t List, Long64_t StartEvent, Long64_t StopEvent)
{
  mEnergy = Energy;
  mMode = Mode;
  TString InPutList = Form("/project/projectdirs/star/xusun/OutPut/AMPT_%s/List/%s_List/Split_%s_%d_%d.list",mMode_AMPT[Mode].Data(),mBeamEnergy[mEnergy].Data(),mBeamEnergy[mEnergy].Data(),mList_start[List],mList_stop[List]);
  SetInPutList(InPutList); // set input list

  SetStartEvent(StartEvent); // set start event
  SetStopEvent(StopEvent); // set stop event

  TString InPutRes = Form("/project/projectdirs/star/xusun/OutPut/AMPT_%s/Resolution/%s_Resolution/Resolution_%s.root",mMode_AMPT[Mode].Data(),mBeamEnergy[Energy].Data(),mBeamEnergy[Energy].Data());
  SetInPutRes(InPutRes); // set input resolution

  TString OutPutFile = Form("/project/projectdirs/star/xusun/OutPut/AMPT_%s/Flow/%s_%s/Phi/Flow_%s_%d_%d.root",mMode_AMPT[Mode].Data(),mBeamEnergy[mEnergy].Data(),mMode_AMPT[Mode].Data(),mBeamEnergy[mEnergy].Data(),mList_start[List],mList_stop[List]);
  SetOutPutFile(OutPutFile); // set output file
}

AMPT_phi::~AMPT_phi()
{
}
//------------------------------------------------------------
void AMPT_phi::SetInPutList(const TString inputlist)
{
  mInPutList= inputlist.Copy();
  cout << "Input list was set to: " << mInPutList.Data() << endl;
}

void AMPT_phi::SetOutPutFile(const TString outputfile)
{
  mOutPutFile = outputfile.Copy();
  cout << "Output file was set to: " << mOutPutFile.Data() << endl;
}

void AMPT_phi::SetInPutRes(const TString inputres)
{
  mInPutRes = inputres.Copy();
  cout << "Input resolution file was set to: " << mInPutRes.Data() << endl;
}

void AMPT_phi::SetStartEvent(const Long64_t StartEvent)
{
  mStartEvent = StartEvent;
  cout << "nStartEvent = " << mStartEvent << endl;
}

void AMPT_phi::SetStopEvent(const Long64_t StopEvent)
{
  mStopEvent = StopEvent;
  cout << "nStopEvent = " << mStopEvent << endl;
}
//------------------------------------------------------------
Float_t AMPT_phi::getResolution(Int_t order, Int_t i_cent) // 0 for 2nd, 1 for 3rd
{
  Float_t res = -999.9;
  if(p_mRes[order]->GetBinContent(p_mRes[order]->FindBin(i_cent)) > 0.0)
  {
    res = TMath::Sqrt(p_mRes[order]->GetBinContent(p_mRes[order]->FindBin(i_cent)));
  }
  return res;
}

Int_t AMPT_phi::getCentrality(Int_t refMult)
{
  Int_t cent9 = -1;

  // Centrality defination
  for(Int_t i_cent = 0; i_cent < 10; i_cent++)
  {
    if(refMult >= mRefMult[mMode][mEnergy][i_cent] && refMult < mRefMult[mMode][mEnergy][i_cent+1])
    {
      cent9 = i_cent;
    }
  }

  return cent9;
}

void AMPT_phi::FillHist2nd(Float_t pt_track, Int_t cent9, Float_t phi_psi, Float_t res, Float_t InvMass) // fill Histogram for flow calculation
{
  for(Int_t i_pt = 0; i_pt < AMPT_phi::pt_total_phi; i_pt++) // pt_bin
  {
    if(pt_track > AMPT_phi::pt_low_phi[i_pt] && pt_track < AMPT_phi::pt_up_phi[i_pt])
    {
      for(Int_t i_cent = AMPT_phi::Centrality_start; i_cent < AMPT_phi::Centrality_stop; i_cent++) // centrality bin
      {
	if(cent9 >= AMPT_phi::cent_low[i_cent] && cent9 <= AMPT_phi::cent_up[i_cent])
	{
	  for(Int_t i_psi = 0; i_psi < 3; i_psi++)
	  {
	    if(phi_psi >= AMPT_phi::Psi2_low[i_psi] && phi_psi < AMPT_phi::Psi2_up[i_psi])
	    {
	      Float_t phi_psi_final = phi_psi - (i_psi-1)*2.0*TMath::Pi()/2.0;
	      for(Int_t i_phi_psi = 0; i_phi_psi < AMPT_phi::Phi_Psi_total; i_phi_psi++) // phi-psi2 bin
	      {
		if(TMath::Abs(phi_psi_final) >= AMPT_phi::phi_Psi2_low[i_phi_psi] && TMath::Abs(phi_psi_final) < AMPT_phi::phi_Psi2_up[i_phi_psi])
		{
		  // flow
		  h_mFlow_phi[0][i_cent][i_pt][i_phi_psi]->Fill(InvMass,(1.0/res));
		  // raw pt spectra
		  h_mPt_phi[0][i_cent][i_pt]->Fill(InvMass,1.0);
		}
	      }
	    }
	  }
	}
      }
    }
  }
}

void AMPT_phi::FillHist3rd(Float_t pt_track, Int_t cent9, Float_t phi_psi, Float_t res, Float_t InvMass) // fill Histogram for flow calculation
{
  for(Int_t i_pt = 0; i_pt < AMPT_phi::pt_total_phi; i_pt++) // pt_bin
  {
    if(pt_track > AMPT_phi::pt_low_phi[i_pt] && pt_track < AMPT_phi::pt_up_phi[i_pt])
    {
      for(Int_t i_cent = AMPT_phi::Centrality_start; i_cent < AMPT_phi::Centrality_stop; i_cent++) // centrality bin
      {
	if(cent9 >= AMPT_phi::cent_low[i_cent] && cent9 <= AMPT_phi::cent_up[i_cent])
	{
	  for(Int_t i_psi = 0; i_psi < 5; i_psi++)
	  {
	    if(phi_psi >= AMPT_phi::Psi3_low[i_psi] && phi_psi < AMPT_phi::Psi3_up[i_psi])
	    {
	      Float_t phi_psi_final = phi_psi - (i_psi-2)*2.0*TMath::Pi()/3.0;
	      for(Int_t i_phi_psi = 0; i_phi_psi < AMPT_phi::Phi_Psi_total; i_phi_psi++) // phi-psi3 bin
	      {
		if(TMath::Abs(phi_psi_final) >= AMPT_phi::phi_Psi3_low[i_phi_psi] && TMath::Abs(phi_psi_final) < AMPT_phi::phi_Psi3_up[i_phi_psi])
		{
		  // flow
		  h_mFlow_phi[1][i_cent][i_pt][i_phi_psi]->Fill(InvMass,(1.0/res));
		  // raw pt spectra
		  h_mPt_phi[1][i_cent][i_pt]->Fill(InvMass,1.0);
		}
	      }
	    }
	  }
	}
      }
    }
  }
}
//------------------------------------------------------------
void AMPT_phi::Init()
{
  // initialize the resolution
  mFile_Res = TFile::Open(mInPutRes.Data());
  p_mRes[0] = (TProfile*)mFile_Res->Get("p_mRes2"); // 2nd event plane resolution
  p_mRes[1] = (TProfile*)mFile_Res->Get("p_mRes3"); // 3rd event plane resolution

  mFile_OutPut = new TFile(mOutPutFile.Data(),"RECREATE");

  // flow TProfile
  for(Int_t i_order = 0; i_order < 2; i_order++)
  {
    for(Int_t i_cent = 0; i_cent < 4; i_cent++)
    {
      TString Order[2] = {"2nd","3rd"};
      TString Centrality[4] = {"0080","0010","1040","4080"};
      TString HistName;
      for(Int_t i_pt = 0; i_pt < AMPT_phi::pt_total_phi; i_pt++)
      {
	for(Int_t i_phi_psi = 0; i_phi_psi < AMPT_phi::Phi_Psi_total; i_phi_psi++)
	{
	  // Flow relative to event plane
	  HistName = Form("Flow_phi_%s_%s_pt_%d_phi_Psi_%d",Order[i_order].Data(),Centrality[i_cent].Data(),i_pt,i_phi_psi); // phi
	  h_mFlow_phi[i_order][i_cent][i_pt][i_phi_psi] = new TH1F(HistName.Data(),HistName.Data(),400,0.98,1.05);; // 0 for 2nd, 1 for 3rd | 0 for 0-80%, 1 for 0-10%, 2 for 10-40%, 3 for 40-80% | pt bin | phi-Psi bin
	}
	// Pt spectra
	HistName = Form("Pt_phi_%s_%s_pt_%d",Order[i_order].Data(),Centrality[i_cent].Data(),i_pt);
	h_mPt_phi[i_order][i_cent][i_pt] = new TH1F(HistName.Data(),HistName.Data(),200,0.98,1.05);
      }
    }
  }

  // QA Plot
  h_mPart = new TH1F("h_mPart","h_mPart",2000,0,2000.0);
  h_mMult = new TH1F("h_mMult","h_mMult",10000,0,10000.0);
  h_mRefMult = new TH1F("h_mRefMult","h_mRefMult",10000,0,10000.0);
  h_mEta = new TH1F("h_mEta","h_mEta",800,-10.0,10.0);
  h_mPsi2_East = new TH1F("h_mPsi2_East","h_mPsi2_East",100,-TMath::Pi(),TMath::Pi());
  h_mPsi2_West = new TH1F("h_mPsi2_West","h_mPsi2_West",100,-TMath::Pi(),TMath::Pi());
  h_mPsi3_East = new TH1F("h_mPsi3_East","h_mPsi3_East",100,-TMath::Pi(),TMath::Pi());
  h_mPsi3_West = new TH1F("h_mPsi3_West","h_mPsi3_West",100,-TMath::Pi(),TMath::Pi());
  h_mCentrality = new TH1F("h_mCentrality","h_mCentrality",9,-0.5,8.5);
  h_mPhi = new TH1F("h_mPhi","h_mPhi",500,0.98,1.05);

  // initialize the TChain
  if (!mInPutList.IsNull())   // if input file is ok
  {
    TString COUT = Form("Open AMPT %s file list ",mMode_AMPT[mMode].Data());
    cout << COUT.Data() << mInPutList.Data() << endl;
    ifstream in(mInPutList);  // input stream
    if(in)
    {
      cout << "file list is ok" << endl;
      mChain_Input = new TChain("tr");
      char str[255];       // char array for each file name
      Long64_t entries_save = 0;
      while(in)
      {
	in.getline(str,255);  // take the lines of the file list
	if(str[0] != 0)
	{
	  TString addfile;
	  addfile = str;
	  mChain_Input->AddFile(addfile.Data(),-1,"tr");
	  Long64_t file_entries = mChain_Input->GetEntries();
	  cout << "File added to data chain: " << addfile.Data() << " with " << (file_entries-entries_save) << " entries" << endl;
	  entries_save = file_entries;
	}
      }
    }
    else
    {
      cout << "WARNING: SE file input is problemtic" << endl;
      mInput_flag = 0;
    }
  }

  // set input Tree
  if(mInput_flag == 1 && !mChain_Input->GetBranch("Event"))
  {
    cerr << "ERROR: Could not find branch 'tr' in tree!" << endl;
  }

  if(mInput_flag == 1)
  {
    mChain_Input->SetBranchAddress("Event", &Event, &b_Event);
    mChain_Input->SetBranchAddress("Mult", &Mult, &b_Mult);
    mChain_Input->SetBranchAddress("Npartp", &Npartp, &b_Npartp);
    mChain_Input->SetBranchAddress("Npartt", &Npartt, &b_Npartt);
    mChain_Input->SetBranchAddress("Nesp", &Nesp, &b_Nesp);
    mChain_Input->SetBranchAddress("Ninesp", &Ninesp, &b_Ninesp);
    mChain_Input->SetBranchAddress("Nest", &Nest, &b_Nest);
    mChain_Input->SetBranchAddress("Ninest", &Ninest, &b_Ninest);
    mChain_Input->SetBranchAddress("Imp", &Imp, &b_Imp);
    mChain_Input->SetBranchAddress("Na", &Na, &b_Na);
    mChain_Input->SetBranchAddress("Nb", &Nb, &b_Nb);
    mChain_Input->SetBranchAddress("Nab", &Nab, &b_Nab);
    mChain_Input->SetBranchAddress("Psi", &Psi, &b_Psi);
    mChain_Input->SetBranchAddress("Nx", Nx, &b_Nx);
    mChain_Input->SetBranchAddress("Ny", Ny, &b_Ny);
    mChain_Input->SetBranchAddress("Nz", Nz, &b_Nz);
    mChain_Input->SetBranchAddress("Stat", Stat, &b_Stat);
    mChain_Input->SetBranchAddress("PID", PID, &b_PID);
    mChain_Input->SetBranchAddress("Px", Px, &b_Px);
    mChain_Input->SetBranchAddress("Py", Py, &b_Py);
    mChain_Input->SetBranchAddress("Pz", Pz, &b_Pz);
    mChain_Input->SetBranchAddress("Mass", Mass, &b_Mass);
    mChain_Input->SetBranchAddress("XX", XX, &b_XX);
    mChain_Input->SetBranchAddress("YY", YY, &b_YY);
    mChain_Input->SetBranchAddress("ZZ", ZZ, &b_ZZ);
    mChain_Input->SetBranchAddress("TT", TT, &b_TT);

    Int_t num_events = mChain_Input->GetEntriesFast();
    cout << "Number of events in file(s) = " << num_events << endl;

    if(mStartEvent > num_events) mStartEvent = num_events;
    if(mStopEvent  > num_events) mStopEvent  = num_events;

    cout << "New nStartEvent = " << mStartEvent << ", new nStopEvent = " << mStopEvent << endl;
  }
}
//------------------------------------------------------------
void AMPT_phi::Make()
{
  Long64_t start_event_use = mStartEvent;
  Long64_t stop_event_use = mStopEvent;
  mChain_Input->GetEntry(0);

  for(Long64_t i_event = start_event_use; i_event < stop_event_use; i_event++) // event loop
  {
    if (i_event != 0  &&  i_event % 1000 == 0)
      cout << "." << flush;
    if (i_event != 0  &&  i_event % 10000 == 0)
    {
      if((stop_event_use-start_event_use) > 0)
      {
	Double_t event_percent = 100.0*((Double_t)(i_event-start_event_use))/((Double_t)(stop_event_use-start_event_use));
	cout << " " << i_event-start_event_use << " (" << event_percent << "%) " << "\n" << "==> Processing data (AMPT_phi) " << flush;
      }
    }

    mChain_Input->GetEntry(i_event);

    TVector3 track;
    Float_t Q2x_full = 0.0, Q2y_full = 0.0, Q2x_east = 0.0, Q2y_east = 0.0, Q2x_west = 0.0, Q2y_west = 0.0;
    Float_t Q3x_full = 0.0, Q3y_full = 0.0, Q3x_east = 0.0, Q3y_east = 0.0, Q3x_west = 0.0, Q3y_west = 0.0;
    Int_t refMult = 0;

    h_mPart->Fill(Npartp+Npartt);
    h_mMult->Fill(Mult);

    for(Int_t i_track = 0; i_track < Mult; i_track++) // 1st track loop for event plane reconstruction
    {
      if(Px[i_track] == 0. && Py[i_track] == 0.) continue;
      track.SetXYZ(Px[i_track],Py[i_track],Pz[i_track]);
      Float_t p_track = track.Mag();
      Float_t pt_track = track.Perp();
      Float_t phi_track = track.Phi(); // -pi to pi
      Float_t eta_track = track.Eta();
      h_mEta->Fill(eta_track);
      // track selection
      if(TMath::Abs(PID[i_track]) == 211 || TMath::Abs(PID[i_track]) == 321 || TMath::Abs(PID[i_track]) == 2212) // pi^{+/-}, K^{+/-}, p and pbar
      {
	if(TMath::Abs(eta_track) < 0.5) refMult++; // refMult calculation
	if(pt_track > 0.2 && pt_track < 2.0 && p_track < 10.0) // pt and p cut
	{
	  if(TMath::Abs(eta_track) <= 1.0) // eta cut
	  {
	    // full event
	    Q2x_full += TMath::Cos(2.0*phi_track);
	    Q2y_full += TMath::Sin(2.0*phi_track);
	    Q3x_full += TMath::Cos(3.0*phi_track);
	    Q3y_full += TMath::Sin(3.0*phi_track);

	    if(eta_track < -0.05) // east
	    {
	      Q2x_east += pt_track*TMath::Cos(2.0*phi_track);
	      Q2y_east += pt_track*TMath::Sin(2.0*phi_track);
	      Q3x_east += pt_track*TMath::Cos(3.0*phi_track);
	      Q3y_east += pt_track*TMath::Sin(3.0*phi_track);
	    }
	    if(eta_track > 0.05) // west
	    {
	      Q2x_west += pt_track*TMath::Cos(2.0*phi_track);
	      Q2y_west += pt_track*TMath::Sin(2.0*phi_track);
	      Q3x_west += pt_track*TMath::Cos(3.0*phi_track);
	      Q3y_west += pt_track*TMath::Sin(3.0*phi_track);
	    }
	  }
	}
      }
    }
    h_mRefMult->Fill(refMult); // fill refMult distribution

    // calculate event plane angle 
    Float_t Psi2_East = TMath::ATan2(Q2y_east,Q2x_east)/2.0; // 2nd: -pi/2 to pi/2
    Float_t Psi2_West = TMath::ATan2(Q2y_west,Q2x_west)/2.0;
    Float_t Psi3_East = TMath::ATan2(Q3y_east,Q3x_east)/3.0; // 3rd: -pi/3 to pi/3
    Float_t Psi3_West = TMath::ATan2(Q3y_west,Q3x_west)/3.0;

    Int_t cent9 = getCentrality(refMult);
    Float_t res2 = getResolution(0,cent9);
    Float_t res3 = getResolution(1,cent9);

//    cout << "refMult = " << refMult << ", Centrality = " << cent9 << endl;

    if(cent9 > -1.0)
    {
      for(Int_t i_track = 0; i_track < Mult; i_track++) // 2nd track loop for K+ and K- selection
      {
	if(Px[i_track] == 0. && Py[i_track] == 0.) continue;

	// store Kaons
	if(PID[i_track] == 321) // K_plus
	{
	  TLorentzVector ltrack;
	  ltrack.SetXYZM(Px[i_track],Py[i_track],Pz[i_track],AMPT_phi::mMassKaon);
	  if(ltrack.Pt() > 0.1 && ltrack.Mag() < 10.0) // pt and p cut
	    mKplus.push_back(static_cast<TLorentzVector>(ltrack));
	}
	if(PID[i_track] == -321) // K_minus
	{
	  TLorentzVector ltrack;
	  ltrack.SetXYZM(Px[i_track],Py[i_track],Pz[i_track],AMPT_phi::mMassKaon);
	  if(ltrack.Pt() > 0.1 && ltrack.Mag() < 10.0) // pt and p cut
	    mKminus.push_back(static_cast<TLorentzVector>(ltrack));
	}
      }

      if( // reconstruct phi for 2nd flow
	  !(Q2x_east == 0.0 && Q2y_east == 0.0) 
       && !(Q2x_west == 0.0 && Q2y_west == 0.0)
       && res2 > 0.0
	)
      {
	for(Int_t i_kplus = 0; i_kplus < mKplus.size(); i_kplus++) // Kplus loop
	{
	  TLorentzVector lKplus = mKplus[i_kplus]; // Kplus
	  for(Int_t i_kminus = 0; i_kminus < mKminus.size(); i_kminus++) // Kminus loop
	  {
	    TLorentzVector lKminus = mKminus[i_kminus]; // Kminus

	    TLorentzVector lPhi = lKplus + lKminus; // phi candidate

	    if(lPhi.Px() == 0. && lPhi.Py() == 0.) continue;

	    Float_t InvMass = lPhi.M();
	    Float_t pt_track = lPhi.Perp(); // pt of phi-meson
	    Float_t phi_track = lPhi.Phi(); // -pi to pi
	    Float_t eta_track = lPhi.Eta(); // eta of phi-meson

	    if(TMath::Abs(eta_track) <= 1.0) // eta cut
	    {
	      if(eta_track < 0.0) // east track => west event plane
	      {
		Float_t phi_psi2 = phi_track - Psi2_West;
		FillHist2nd(pt_track,cent9,phi_psi2,res2,InvMass);
	      }
	      if(eta_track > 0.0) // west track => east event plane
	      {
		Float_t phi_psi2 = phi_track - Psi2_East;
		FillHist2nd(pt_track,cent9,phi_psi2,res2,InvMass);
	      }
	    }
	    h_mPhi->Fill(InvMass); // QA for phi-meson peak
	  }
	}

	// QA: 2nd event plane and centrality
	h_mPsi2_East->Fill(Psi2_East);
	h_mPsi2_West->Fill(Psi2_West);
	h_mCentrality->Fill(cent9);
      }

      if( // reconstruct phi for 3rd flow
	  !(Q3x_east == 0.0 && Q3y_east == 0.0) 
       && !(Q3x_west == 0.0 && Q3y_west == 0.0)
       && res3 > 0.0
	)
      {
	for(Int_t i_kplus = 0; i_kplus < mKplus.size(); i_kplus++) // Kplus loop
	{
	  TLorentzVector lKplus = mKplus[i_kplus]; // Kplus
	  for(Int_t i_kminus = 0; i_kminus < mKminus.size(); i_kminus++) // Kminus loop
	  {
	    TLorentzVector lKminus = mKminus[i_kminus]; // Kminus

	    TLorentzVector lPhi = lKplus + lKminus; // phi candidate

	    if(lPhi.Px() == 0. && lPhi.Py() == 0.) continue;

	    Float_t InvMass = lPhi.M();
	    Float_t pt_track = lPhi.Perp(); // pt of phi-meson
	    Float_t phi_track = lPhi.Phi(); // -pi to pi
	    Float_t eta_track = lPhi.Eta(); // eta of phi-meson

	    if(TMath::Abs(eta_track) <= 1.0) // eta cut
	    {
	      if(eta_track < 0.0) // east track => west event plane
	      {
		Float_t phi_psi3 = phi_track - Psi3_West;
		FillHist3rd(pt_track,cent9,phi_psi3,res3,InvMass);
	      }
	      if(eta_track > 0.0) // west track => east event plane
	      {
		Float_t phi_psi3 = phi_track - Psi3_East;
		FillHist3rd(pt_track,cent9,phi_psi3,res3,InvMass);
	      }
	    }
	  }
	}

	// QA: 3rd event plane
	h_mPsi3_East->Fill(Psi3_East);
	h_mPsi3_West->Fill(Psi3_West);
      }
      mKplus.clear();
      mKminus.clear();
    }
  }

  cout << "." << flush;
  cout << " " << stop_event_use-start_event_use << "(" << 100 << "%)";
  cout << endl;
}

void AMPT_phi::Finish()
{
  mFile_Res->Close();
  mFile_OutPut->cd();
  h_mPart->Write();
  h_mMult->Write();
  h_mEta->Write();
  h_mRefMult->Write();
  h_mPsi2_East->Write();
  h_mPsi2_West->Write();
  h_mPsi3_East->Write();
  h_mPsi3_West->Write();
  h_mCentrality->Write();
  h_mPhi->Write();
  for(Int_t i_order = 0; i_order < 2; i_order++)
  {
    for(Int_t i_cent = 0; i_cent < 4; i_cent++)
    {
      for(Int_t i_pt = 0; i_pt < AMPT_phi::pt_total_phi; i_pt++)
      {
	for(Int_t i_phi_psi = 0; i_phi_psi < 7; i_phi_psi++)
	{
	  h_mFlow_phi[i_order][i_cent][i_pt][i_phi_psi]->Write();
	}
	h_mPt_phi[i_order][i_cent][i_pt]->Write();
      }
    }
  }
  mFile_OutPut->Close();
}
