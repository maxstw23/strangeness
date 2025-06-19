#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <map>
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TProfile.h"
#include "TMath.h"
#include "TFile.h"
#include "TObjArray.h"
#include "TList.h"
#include "TString.h"
#include "TRandom.h" 
#include "TRandom3.h" 
#include "TH1F.h"
#include "TH2F.h"
#include "TVector3.h"
#include <algorithm>
#include <stdlib.h>
#include "TComplex.h"
#include "TChain.h"
#include "TVector2.h"
#include "TLorentzVector.h"
#include "TDatabasePDG.h"
#include "TParticlePDG.h"
#include <string>
// my classes
#include "./lib/CenMaker.h"
#include "./lib/NpartNormalizer.h"

using namespace std;

// constants
const float eta_cut    = 1.;
const float PI = TMath::Pi();
const float cen_cut_1 = 8;
const float cen_cut_2 = 9;
const float rho_mass = 0.77;
const float rho_sigma = 0.001;
const float EP_eta_lo = 2.1;
const float EP_eta_hi = 5.1;

// for cendef
TString energy = "14";

int FindType(int PID);

void Check_QA(const Char_t *inFile = "placeholder.list", const TString JobID = "1234", const int mode = 1)
{   
    // PDG Database
	TDatabasePDG *db = new TDatabasePDG();
	TParticlePDG *p_info = new TParticlePDG();

    // initialize map
    std::map<int, int> sness_map;
    sness_map.insert(std::pair<int, int>( 311,  1)); //K^0
    sness_map.insert(std::pair<int, int>(-311, -1)); //anti K^0
    sness_map.insert(std::pair<int, int>( 321,  1)); //K^+
    sness_map.insert(std::pair<int, int>(-321, -1)); //K^-
    sness_map.insert(std::pair<int, int>( 313,  1)); //K*(892)^0
    sness_map.insert(std::pair<int, int>(-313, -1)); //anti-above
    sness_map.insert(std::pair<int, int>( 323,  1)); //K*(892)^+
    sness_map.insert(std::pair<int, int>(-323, -1)); //anti-above
    sness_map.insert(std::pair<int, int>( 431,  1)); //D_s^+
    sness_map.insert(std::pair<int, int>(-431, -1)); //anti-above
    sness_map.insert(std::pair<int, int>( 433,  1)); //D*_s^+
    sness_map.insert(std::pair<int, int>(-433, -1)); //anti-above
    sness_map.insert(std::pair<int, int>( 3122,-1)); //lambda
    sness_map.insert(std::pair<int, int>(-3122, 1)); //anti-lambda
    sness_map.insert(std::pair<int, int>( 3222,-1)); //sigma^+
    sness_map.insert(std::pair<int, int>(-3222, 1)); //anti_sigma^-
    sness_map.insert(std::pair<int, int>( 3212,-1)); //sigma^0
    sness_map.insert(std::pair<int, int>(-3212, 1)); //anti_sigma^0
    sness_map.insert(std::pair<int, int>( 3112,-1)); //sigma^-
    sness_map.insert(std::pair<int, int>(-3112, 1)); //anti_sigma^+
    sness_map.insert(std::pair<int, int>( 3322,-2)); //xi^0
    sness_map.insert(std::pair<int, int>(-3322, 2)); //anti_xi^0
    sness_map.insert(std::pair<int, int>( 3312,-2)); //xi^-
    sness_map.insert(std::pair<int, int>(-3312, 2)); //anti_xi^+
    sness_map.insert(std::pair<int, int>( 3324,-2)); //xi(1530)^0
    sness_map.insert(std::pair<int, int>(-3324, 2)); //anti-above
    sness_map.insert(std::pair<int, int>( 3314,-2)); //xi(1530)^-
    sness_map.insert(std::pair<int, int>(-3314, 2)); //anti-above
    sness_map.insert(std::pair<int, int>( 3334,-3)); //omega^-
    sness_map.insert(std::pair<int, int>(-3334, 3)); //anti_omega^+
    sness_map.insert(std::pair<int, int>( 4232,-1)); //xi_c^+
    sness_map.insert(std::pair<int, int>(-4232, 1)); //anti_xi_c^-
    sness_map.insert(std::pair<int, int>( 4132,-1)); //xi_c^0
    sness_map.insert(std::pair<int, int>(-4132, 1)); //anti-above
    sness_map.insert(std::pair<int, int>( 4332,-2)); //omega_c^0
    sness_map.insert(std::pair<int, int>(-4332, 2)); //anti-above
    sness_map.insert(std::pair<int, int>( 4432,-1)); //omega_cc^0
    sness_map.insert(std::pair<int, int>(-4432, 1)); //anti-above
    sness_map.insert(std::pair<int, int>( 5332,-2)); //omega_b^0
    sness_map.insert(std::pair<int, int>(-5332, 2)); //anti-above

    // input
    TChain* chain = new TChain("tree");
	int nfile = 0;

    std::ifstream fin(inFile);
    std::string line;
    while(getline(fin, line)) //loop wiill run till end of file
    {
        cout << line << endl;
        nfile += chain->Add(line.c_str());                        
    }
    fin.close();

	cout <<"Added "<<nfile<<" files"<<endl;
	cout<<"# entries in chain: "<<chain->GetEntries()<<endl;

    // output
    char fname_out[200];
    sprintf(fname_out,"out_%s.root",JobID.Data());
	TFile fout(fname_out,"RECREATE");

    // hist and profiles
    TH1D* hRefMult     = new TH1D("hRefMult"   , "RefMult"                   , 1000, -0.5, 999.5);
    TH1D* hImpPar      = new TH1D("hImpPar"    , "Impact Parameter"          , 200,    0., 20.  );
    TH1D* hEta         = new TH1D("hEta"       , "Pseudorapidity"            , 500 , -10., 10.  );
    TH1D* hy           = new TH1D("hy"         , "Rapidity"                  , 500 , -10., 10.  );
    TH1D* hyOmega      = new TH1D("hyOmega"    , "#Omega^{-} Rapidity"       , 500 , -10., 10.  );
    TH1D* hyOmegabar   = new TH1D("hyOmegabar" , "#bar{#Omega^{+}} Rapidity" , 500 , -10., 10.  );
    TH1D* hyP          = new TH1D("hyP"        , "p Rapidity"                , 500 , -10., 10.  );
    TH1D* hyPbar       = new TH1D("hyPbar"     , "#bar{p} Rapidity"          , 500 , -10., 10.  );
    TH1D* hyLambda     = new TH1D("hyLambda"   , "#Lambda Rapidity"          , 500 , -10., 10.  );
    TH1D* hyLambdabar  = new TH1D("hyLambdabar", "#bar{#Lambda} Rapidity"    , 500 , -10., 10.  );
    TH1D* hyXi         = new TH1D("hyXi"       , "#Xi Rapidity"          , 500 , -10., 10.  );
    TH1D* hyXibar      = new TH1D("hyXibar"    , "#bar{#Xi} Rapidity"    , 500 , -10., 10.  );
    TH1D* hbaryon = new TH1D("hbaryon", "total baryon number", 800, -199.5, 600.5);
    TH1D* hQualifiedEvt = new TH1D("hQualifiedEvt", "Number of qualified events", 9, 0.5, 9.5); //all 0-5%
    TH1D* hstrangeness = new TH1D("sness", "total strangeness", 50, -24.5, 25.5);

    // for rho decay pion v_2 test
    TH1D *hEP_e = new TH1D("hEP_e", "hEP_e", 100, 0, TMath::Pi());
    TH1D *hEP_w = new TH1D("hEP_w", "hEP_w", 100, 0, TMath::Pi());
    TH1D *hEP_ew_cos = new TH1D("hEP_ew_cos", "hEP_ew_cos", 1, -0.5, 0.5);
    TH1D *hpipiinvmass_SS = new TH1D("hpipiinvmass_SS", "hpipiinvmass_SS", 2000, 0., 2.);
    TH1D *hpipiinvmass_OS = new TH1D("hpipiinvmass_OS", "hpipiinvmass_OS", 2000, 0., 2.);
    TProfile *hpiplus_v2_pt_inc  = new TProfile("hpiplus_v2_pt_inc",  "hpiplus_v2_pt_inc",  200, 0., 10.);
    TProfile *hpiminus_v2_pt_inc = new TProfile("hpiminus_v2_pt_inc", "hpiminus_v2_pt_inc", 200, 0., 10.);
    TProfile *hpiplus_v2_pt_sig  = new TProfile("hpiplus_v2_pt_sig",  "hpiplus_v2_pt_sig",  200, 0., 10.);
    TProfile *hpiminus_v2_pt_sig = new TProfile("hpiminus_v2_pt_sig", "hpiminus_v2_pt_sig", 200, 0., 10.);
    TProfile *hpiplus_v2_pt_sbd  = new TProfile("hpiplus_v2_pt_sbd",  "hpiplus_v2_pt_sbd",  200, 0., 10.);
    TProfile *hpiminus_v2_pt_sbd = new TProfile("hpiminus_v2_pt_sbd", "hpiminus_v2_pt_sbd", 200, 0., 10.);
    TProfile *hrho_v2_pt_sig = new TProfile("hrho_v2_pt_sig", "hrho_v2_pt_sig", 200, 0., 10.);
    TProfile *hrho_v2_pt_sbd = new TProfile("hrho_v2_pt_sbd", "hrho_v2_pt_sbd", 200, 0., 10.);
        
    // transported net proton net neutron flow
    TProfile *hproton_v2_pt = new TProfile("hproton_v2_pt", "hproton_v2_pt", 200, 0., 10.);
    TProfile *hneutron_v2_pt = new TProfile("hneutron_v2_pt", "hneutron_v2_pt", 200, 0., 10.);
    TProfile *hantiproton_v2_pt = new TProfile("hantiproton_v2_pt", "hantiproton_v2_pt", 200, 0., 10.);
    TProfile *hantineutron_v2_pt = new TProfile("hantineutron_v2_pt", "hantineutron_v2_pt", 200, 0., 10.);

    // v1 coalescence test
    TProfile* hproton_v1_y[9];
    TProfile* hantiproton_v1_y[9];
    TProfile* hneutron_v1_y[9];
    TProfile* hantineutron_v1_y[9];
    TProfile* hkplus_v1_y[9];
    TProfile* hkminus_v1_y[9];
    TProfile* hpiplus_v1_y[9];
    TProfile* hpiminus_v1_y[9];
    TProfile* hlambda_v1_y[9];
    TProfile* hantilambda_v1_y[9];
    TProfile* hxi_v1_y[9];
    TProfile* hantixi_v1_y[9];
    for (int i = 0; i < 9; i++)
    {
        hproton_v1_y[i] = new TProfile(Form("hproton_v1_y_%d", i+1), Form("hproton_v1_y_%d", i+1), 20, -1.0, 1.0, -1.0, 1.0);
        hantiproton_v1_y[i] = new TProfile(Form("hantiproton_v1_y_%d", i+1), Form("hantiproton_v1_y_%d", i+1), 20, -1.0, 1.0, -1.0, 1.0);
        hneutron_v1_y[i] = new TProfile(Form("hneutron_v1_y_%d", i+1), Form("hneutron_v1_y_%d", i+1), 20, -1.0, 1.0, -1.0, 1.0);
        hantineutron_v1_y[i] = new TProfile(Form("hantineutron_v1_y_%d", i+1), Form("hantineutron_v1_y_%d", i+1), 20, -1.0, 1.0, -1.0, 1.0);
        hkplus_v1_y[i] = new TProfile(Form("hkplus_v1_y_%d", i+1), Form("hkplus_v1_y_%d", i+1), 20, -1.0, 1.0, -1.0, 1.0);
        hkminus_v1_y[i] = new TProfile(Form("hkminus_v1_y_%d", i+1), Form("hkminus_v1_y_%d", i+1), 20, -1.0, 1.0, -1.0, 1.0);
        hpiplus_v1_y[i] = new TProfile(Form("hpiplus_v1_y_%d", i+1), Form("hpiplus_v1_y_%d", i+1), 20, -1.0, 1.0, -1.0, 1.0);
        hpiminus_v1_y[i] = new TProfile(Form("hpiminus_v1_y_%d", i+1), Form("hpiminus_v1_y_%d", i+1), 20, -1.0, 1.0, -1.0, 1.0);
        hlambda_v1_y[i] = new TProfile(Form("hlambda_v1_y_%d", i+1), Form("hlambda_v1_y_%d", i+1), 20, -1.0, 1.0, -1.0, 1.0);
        hantilambda_v1_y[i] = new TProfile(Form("hantilambda_v1_y_%d", i+1), Form("hantilambda_v1_y_%d", i+1), 20, -1.0, 1.0, -1.0, 1.0);
        hxi_v1_y[i] = new TProfile(Form("hxi_v1_y_%d", i+1), Form("hxi_v1_y_%d", i+1), 20, -1.0, 1.0, -1.0, 1.0);
        hantixi_v1_y[i] = new TProfile(Form("hantixi_v1_y_%d", i+1), Form("hantixi_v1_y_%d", i+1), 20, -1.0, 1.0, -1.0, 1.0);
    }

    // for spectra
    TH1D* hOmegaPtSpectrum[9];          
    TH1D* hOmegabarPtSpectrum[9];   
    TH1D* hXiPtSpectrum[9];      
    TH1D* hXibarPtSpectrum[9];      
    TH1D* hLambdaPtSpectrum[9];    
    TH1D* hLambdabarPtSpectrum[9]; 
    char hname_pt[200];
    for (int i = 0; i < 9; i++)
    {
        sprintf(hname_pt, "hOmegaPtSpectrum_cen_%d", i+1);
        hOmegaPtSpectrum[i] = new TH1D(hname_pt, hname_pt, 600, 0., 6.); 
        sprintf(hname_pt, "hOmegabarPtSpectrum_cen_%d", i+1);
        hOmegabarPtSpectrum[i] = new TH1D(hname_pt, hname_pt, 600, 0., 6.); 
        sprintf(hname_pt, "hXiPtSpectrum_cen_%d", i+1);
        hXiPtSpectrum[i] = new TH1D(hname_pt, hname_pt, 600, 0., 6.); 
        sprintf(hname_pt, "hXibarPtSpectrum_cen_%d", i+1);
        hXibarPtSpectrum[i] = new TH1D(hname_pt, hname_pt, 600, 0., 6.); 
        sprintf(hname_pt, "hLambdaPtSpectrum_cen_%d", i+1);
        hLambdaPtSpectrum[i] = new TH1D(hname_pt, hname_pt, 600, 0., 6.); 
        sprintf(hname_pt, "hLambdabarPtSpectrum_cen_%d", i+1);
        hLambdabarPtSpectrum[i] = new TH1D(hname_pt, hname_pt, 600, 0., 6.); 
    }

    // dN/dy vs Npart
    TH1D* hOmegadNdy    = new TH1D("hOmegadNdy", "Omega dN/dy at mid rapidity", 9, 0.5, 9.5);
    TH1D* hOmegabardNdy = new TH1D("hOmegabardNdy", "Omegabar dN/dy at mid rapidity", 9, 0.5, 9.5);
    TH1D* hXidNdy    = new TH1D("hXidNdy", "Xi dN/dy at mid rapidity", 9, 0.5, 9.5);
    TH1D* hXibardNdy = new TH1D("hXibardNdy", "Xibar dN/dy at mid rapidity", 9, 0.5, 9.5);
    TH1D* hLambdadNdy    = new TH1D("hLambdadNdy", "Lambda dN/dy at mid rapidity", 9, 0.5, 9.5);
    TH1D* hLambdabardNdy = new TH1D("hLambdabardNdy", "Lambdabar dN/dy at mid rapidity", 9, 0.5, 9.5);

    // Bbar/B ratios
    TH1D* hBaryon_yield = new TH1D("hBaryon_yield", "Baryon yield for BES comparison", 6, -0.5, 5.5); //0-1 lambda, 2-3 xi, 4-5 omega
    TH2D* hBaryon_yield_np = new TH2D("hBaryon_yield_np", "Baryon yield based on npart distribution", 6, -0.5, 5.5, 1000, -0.5, 999.5);

    TH1D* hnp_cen[9];
    TH1D* hnssbar_cen[9];
    char hname[200];
    char hname_2[200];
    for (int i = 0; i < 9; i++) 
    {   
        sprintf(hname, "hnp_cen_%d", i+1);
        sprintf(hname_2, "hnssbar_cen_%d", i+1);
        hnp_cen[i] = new TH1D(hname, hname, 1000, -0.5, 999.5);
        hnssbar_cen[i] = new TH1D(hname_2, hname_2, 1000, -0.5, 999.5);
    }
    TH2D* hnp_vs_ssbar = new TH2D("hnp_vs_ssbar", "Npart vs SSbar pair", 1000, -0.5, 999.5, 1000, -0.5, 999.5);
    TH1D* hnp          = new TH1D("hnp", "Participant number for all qualified events", 1000, -0.5, 999.5);
    TH2D* hRefMult_np  = new TH2D("hRefMult_np", "RefMult vs Npart", 1000, -0.5, 999.5, 1000, -0.5, 999.5);
    

    // setting PID and momentum branches
    TBranch* bpid = nullptr;
    int npp = 0, npt = 0;
    int refmult;
    float imp;
    std::vector<int>   *pid_vec = nullptr;
    std::vector<float> *px_vec  = nullptr;
    std::vector<float> *py_vec  = nullptr;
    std::vector<float> *pz_vec  = nullptr;
    if (mode != 2) // only for AMPT
    {
        chain->SetBranchAddress("npp", &npp);
        chain->SetBranchAddress("npt", &npt);
    }
    chain->SetBranchAddress("refmult", &refmult);
    chain->SetBranchAddress("imp", &imp);
    chain->SetBranchAddress("pid", &pid_vec, &bpid);
    chain->SetBranchAddress("px",  &px_vec);
    chain->SetBranchAddress("py",  &py_vec);
    chain->SetBranchAddress("pz",  &pz_vec);

    // loops
    int nentries = chain->GetEntries();

    // evt count
    int nevt_spec[9] = {0};

    // event loop
    for (int i = 0; i < nentries; ++i)
    {
        if((i+1)%1000==0) cout<<"Processing entry == "<< i+1 <<" == out of "<<nentries<<".\n";
        chain->GetEntry(i);
        int np = 0;
        if (mode != 2) np = npp + npt;

        // some QA
        hRefMult->Fill(refmult);
        hImpPar->Fill(imp);

        // centrality
        CenMaker cenmaker;
        int cen = cenmaker.cent9(refmult, energy, mode);
        if (cen < 1 || cen > 9) continue;
        // if ((cen_cut_1 || cen_cut_2) && (cen != cen_cut_1 && cen != cen_cut_2)) continue;
        for (int i = 1; i <= 9; i++) {if (cen == i) nevt_spec[i-1]++;}
        hnp_cen[cen-1]->Fill(np);
        hnp->Fill(np);
        hRefMult_np->Fill(np, refmult);
    
        // counting particle for BES comparison and count omega
        int NO_y=0, NOb_y=0, NX_y=0, NXb_y=0, NL_y=0, NLb_y=0;
        int num_omega_total = 0;
        float mQxe = 0, mQye = 0, mQxw = 0, mQyw = 0;
        for (int i = 0; i < pid_vec->size(); ++i)
        {
            int   pid   = pid_vec->at(i);
            int   s     = sness_map[pid];
            int   type  = FindType(pid);
            float px    = px_vec->at(i);
            float py    = py_vec->at(i);
            float pz    = pz_vec->at(i);
            float pt    = px*px + py*py;
            float theta = atan2(pt,pz);
            float eta   = -log(tan(theta/2.));
            float y     = -999;
            p_info= db->GetParticle((int)pid);
            if (!p_info) continue;
            TLorentzVector lv;
            lv.SetXYZM(px, py, pz, p_info->Mass());
            y = lv.Rapidity();

            // event plane
            if (EP_eta_lo < eta && eta < EP_eta_hi)
            {
                mQxe += pt*cos(2*theta);
                mQye += pt*sin(2*theta);
            }
            if (-EP_eta_hi < eta && eta < -EP_eta_lo)
            {
                mQxw += pt*cos(2*theta);
                mQyw += pt*sin(2*theta);
            }

            // QA
            hEta->Fill(eta);
            hy->Fill(y);
            if (pid ==  3334) hyOmega->Fill(y);
            if (pid == -3334) hyOmegabar->Fill(y);
            if (pid ==  2212) hyP->Fill(y);
            if (pid == -2212) hyPbar->Fill(y);
            if (pid ==  3122) hyLambda->Fill(y);
            if (pid == -3122) hyLambdabar->Fill(y);
            if (pid ==  3312) hyXi->Fill(y);
            if (pid == -3312) hyXibar->Fill(y);

            // count particles
            if (fabs(pid) ==  3334) num_omega_total++;
        
            // count "np" for UrQMD
            if (mode == 2)
            {
                if (fabs(eta) < 1 && p_info->Charge() != 0) np++;
            }

            // for dNdy
            if (fabs(y) > 0.5) continue; // mid-rapidity
            if (pid ==  3334) NO_y++;
            if (pid == -3334) NOb_y++;
            if (pid ==  3312) NX_y++;
            if (pid == -3312) NXb_y++;
            if (pid ==  3122) NL_y++;
            if (pid == -3122) NLb_y++;

            // for Bbar/B ratios
            if (pid == -3122 && cen == 9)                {hBaryon_yield->Fill(0.); hBaryon_yield_np->Fill(0., np*1.0);}
            if (pid ==  3122 && cen == 9)                {hBaryon_yield->Fill(1.); hBaryon_yield_np->Fill(1., np*1.0);}
            if (pid == -3312 && cen == 9)                {hBaryon_yield->Fill(2.); hBaryon_yield_np->Fill(2., np*1.0);}
            if (pid ==  3312 && cen == 9)                {hBaryon_yield->Fill(3.); hBaryon_yield_np->Fill(3., np*1.0);}
            if (pid == -3334 && (cen == 9 || cen == 8))  {hBaryon_yield->Fill(4.); hBaryon_yield_np->Fill(4., np*1.0);}
            if (pid ==  3334 && (cen == 9 || cen == 8))  {hBaryon_yield->Fill(5.); hBaryon_yield_np->Fill(5., np*1.0);}

            // pT spectrum
            if (pid ==  3334) hOmegaPtSpectrum    [cen-1]->Fill(pt);      
            if (pid == -3334) hOmegabarPtSpectrum [cen-1]->Fill(pt);   
            if (pid ==  3312) hXiPtSpectrum       [cen-1]->Fill(pt);       
            if (pid == -3312) hXibarPtSpectrum    [cen-1]->Fill(pt);       
            if (pid ==  3122) hLambdaPtSpectrum   [cen-1]->Fill(pt);      
            if (pid == -3122) hLambdabarPtSpectrum[cen-1]->Fill(pt);  

        }

        if (mQxe == 0 || mQye == 0 || mQxw == 0 || mQyw == 0) continue;
        TVector2 mQe; mQe.Set(mQxe, mQye);
        TVector2 mQw; mQw.Set(mQxw, mQyw);
        float EP_e = mQe.Phi()/2.;
        float EP_w = mQw.Phi()/2.;
        hEP_e->Fill(EP_e);
        hEP_w->Fill(EP_w);
        hEP_ew_cos->Fill(cos(2*EP_e-2*EP_w));

        // rho/pion v2 test
        // std::vector<int> used_tracks;
        // used_tracks.resize(0);
        // for (int i = 0; i < pid_vec->size(); ++i)
        // {
        //     int   pid1   = pid_vec->at(i);
        //     if (abs(pid1) != 211) continue;
        //     float px1    = px_vec->at(i);
        //     float py1    = py_vec->at(i);
        //     float pz1    = pz_vec->at(i);
        //     float pt1    = px1*px1 + py1*py1;
        //     float phi1   = atan2(py1,px1);
        //     float theta1 = atan2(pt1,pz1);
        //     float eta1   = -log(tan(theta1/2.));
        //     if (fabs(eta1) > 1) continue;
        //     float y1     = -999;
        //     p_info= db->GetParticle((int)pid1);
        //     if (!p_info) continue;
        //     TLorentzVector lv1;
        //     lv1.SetXYZM(px1, py1, pz1, p_info->Mass());
        //     y1 = lv1.Rapidity();

        //     // inclusive v2
        //     if (pid1 ==  211) hpiplus_v2_pt_inc ->Fill(pt1, cos(2*phi1));
        //     if (pid1 == -211) hpiminus_v2_pt_inc->Fill(pt1, cos(2*phi1));
            
        //     for (int j = i+1; j < pid_vec->size(); ++j)
        //     {
        //         int   pid2   = pid_vec->at(j);
        //         if (abs(pid2) != 211) continue;
        //         float px2    = px_vec->at(j);
        //         float py2    = py_vec->at(j);
        //         float pz2    = pz_vec->at(j);
        //         float pt2    = px2*px2 + py2*py2;
        //         float phi2   = atan2(py2,px2);
        //         float theta2 = atan2(pt2,pz2);
        //         float eta2   = -log(tan(theta2/2.));
        //         if (fabs(eta2) > 1) continue;
        //         float y2     = -999;
        //         p_info= db->GetParticle((int)pid2);
        //         if (!p_info) continue;
        //         TLorentzVector lv2;
        //         lv2.SetXYZM(px2, py2, pz2, p_info->Mass());
        //         y2 = lv2.Rapidity();
                
        //         // rho inv mass
        //         float invmass = (lv1+lv2).M();
        //         float rho_phi = (lv1+lv2).Phi();
        //         float rho_pt  = (lv1+lv2).Pt();
        //         if (pid1 * pid2 < 0) hpipiinvmass_OS->Fill(invmass);
        //         else                 hpipiinvmass_SS->Fill(invmass);

        //         // pion v2
        //         if (pid1 * pid2 < 0) continue;
        //         // sideband first
        //         if ((rho_mass - 6 * rho_sigma < invmass && invmass < rho_mass - 4 * rho_sigma) || (rho_mass + 4 * rho_sigma < invmass && invmass < rho_mass + 6 * rho_sigma))
        //         {
        //             if (pid1 > 0) { hpiplus_v2_pt_sbd->Fill(pt1, cos(2*(phi1))); hpiminus_v2_pt_sbd->Fill(pt2, cos(2*(phi2))); }
        //             else          { hpiplus_v2_pt_sbd->Fill(pt2, cos(2*(phi2))); hpiminus_v2_pt_sbd->Fill(pt1, cos(2*(phi1))); }
        //             hrho_v2_pt_sbd->Fill(rho_pt, cos(2*(rho_phi)));
                    
        //         }
        //         // signal
        //         if (rho_mass - 3 * rho_sigma < invmass && invmass < rho_mass + 3 * rho_sigma)
        //         {
        //             // if track not used
        //             if (std::find(used_tracks.begin(), used_tracks.end(), i) != used_tracks.end()) break;
        //             if (std::find(used_tracks.begin(), used_tracks.end(), j) != used_tracks.end()) continue;
        //             used_tracks.push_back(i); used_tracks.push_back(j);
        //             if (pid1 > 0) { hpiplus_v2_pt_sig->Fill(pt1, cos(2*(phi1))); hpiminus_v2_pt_sig->Fill(pt2, cos(2*(phi2))); }
        //             else          { hpiplus_v2_pt_sig->Fill(pt2, cos(2*(phi2))); hpiminus_v2_pt_sig->Fill(pt1, cos(2*(phi1))); }
        //             hrho_v2_pt_sig->Fill(rho_pt, cos(2*(rho_phi)));
        //         }  
        //     }
        // }
        
        // proton neutron v2 test
        for (int i = 0; i < pid_vec->size(); ++i)
        {
            int   pid   = pid_vec->at(i);
            int   s     = sness_map[pid];
            int   type  = FindType(pid);
            float px    = px_vec->at(i);
            float py    = py_vec->at(i);
            float phi   = atan2(py,px);
            float pz    = pz_vec->at(i);
            float pt    = px*px + py*py;
            float theta = atan2(pt,pz);
            float eta   = -log(tan(theta/2.));
            float y     = -999;
            p_info= db->GetParticle((int)pid);
            if (!p_info) continue;
            TLorentzVector lv;
            lv.SetXYZM(px, py, pz, p_info->Mass());
            y = lv.Rapidity();
            if (fabs(y) > 1) continue; // mid-rapidity

            // proton v2
            if (pid ==  2212) hproton_v2_pt->Fill(pt, cos(2*phi));
            if (pid == -2212) hantiproton_v2_pt->Fill(pt, cos(2*phi));
            // neutron v2
            if (pid ==  2112) hneutron_v2_pt->Fill(pt, cos(2*phi));
            if (pid == -2112) hantineutron_v2_pt->Fill(pt, cos(2*phi));
        }
        
        // v1 coalescence
        for (int i = 0; i < pid_vec->size(); ++i)
        {
            int   pid   = pid_vec->at(i);
            int   s     = sness_map[pid];
            int   type  = FindType(pid);
            float px    = px_vec->at(i);
            float py    = py_vec->at(i);
            float phi   = atan2(py,px);
            float pz    = pz_vec->at(i);
            float pt    = px*px + py*py;
            float theta = atan2(pt,pz);
            float eta   = -log(tan(theta/2.));
            float y     = -999;
            p_info= db->GetParticle((int)pid);
            if (!p_info) continue;
            TLorentzVector lv;
            lv.SetXYZM(px, py, pz, p_info->Mass());
            y = lv.Rapidity();
            if (fabs(y) > 1) continue; // mid-rapidity

            if (pid ==  2212) hproton_v1_y[cen-1]->Fill(y, cos(phi));
            if (pid == -2212) hantiproton_v1_y[cen-1]->Fill(y, cos(phi));
            if (pid ==  2112) hneutron_v1_y[cen-1]->Fill(y, cos(phi));
            if (pid == -2112) hantineutron_v1_y[cen-1]->Fill(y, cos(phi));
            if (pid ==   211) hpiplus_v1_y[cen-1]->Fill(y, cos(phi));
            if (pid ==  -211) hpiminus_v1_y[cen-1]->Fill(y, cos(phi));
            if (pid ==   321) hkplus_v1_y[cen-1]->Fill(y, cos(phi));
            if (pid ==  -321) hkminus_v1_y[cen-1]->Fill(y, cos(phi));
            if (pid ==  3122) hlambda_v1_y[cen-1]->Fill(y, cos(phi));
            if (pid == -3122) hantilambda_v1_y[cen-1]->Fill(y, cos(phi));
            if (pid ==  3312) hxi_v1_y[cen-1]->Fill(y, cos(phi));
            if (pid == -3312) hantixi_v1_y[cen-1]->Fill(y, cos(phi));
        }
        
        hOmegadNdy ->Fill(cen*1.0, NO_y*1.0); hOmegabardNdy ->Fill(cen*1.0, NOb_y*1.0); 
        hXidNdy    ->Fill(cen*1.0, NX_y*1.0); hXibardNdy    ->Fill(cen*1.0, NXb_y*1.0); 
        hLambdadNdy->Fill(cen*1.0, NL_y*1.0); hLambdabardNdy->Fill(cen*1.0, NLb_y*1.0); 

        int total_s = 0, total_bn = 0, itrack = 0;
        float total_ssbar_pair = 0;

        // fill examples and check anti-omega
        for (std::vector<int>::iterator it = pid_vec->begin(); it != pid_vec->end(); ++it)
        {
            int s = sness_map[(int)(*it)];
            // total strangeness and baryon number
            total_s += s;
            total_ssbar_pair += fabs(s)/2.0;
            if (*it < 10000 && *it > 999) total_bn += 1;
            if (*it >-10000 && *it <-999) total_bn -= 1;
        }

        hstrangeness->Fill(total_s);  
        hbaryon->Fill(total_bn);  
        int ssbar_pair = static_cast<int>(total_ssbar_pair);
        hnp_vs_ssbar->Fill(np, ssbar_pair);
        if (cen >= 1 && cen <= 9) hnssbar_cen[cen-1]->Fill(ssbar_pair);
        
    }

    for (int i = 1; i <= 9; i++) hQualifiedEvt->Fill(i, nevt_spec[i - 1]);

    fout.Write();
}

int FindType(int PID)
{
    // kaons
     if (fabs(PID) == 311 || fabs(PID) == 321 || fabs(PID) == 313 || fabs(PID) == 323)  
        return 1; 

    // strange d-mesons
    if (fabs(PID) == 431 || fabs(PID) == 433)
        return 2;

    // lambda
    if (fabs(PID) == 3122)
        return 3;

    // sigma
    if (fabs(PID) == 3222 || fabs(PID) == 3212 || fabs(PID) == 3112)    
        return 4;
    
    // xi
    if (fabs(PID) == 3322 || fabs(PID) == 3312 || fabs(PID) == 3324 || fabs(PID) == 3314 || fabs(PID) == 4232 || fabs(PID) == 4132)
        return 5;

    // omega
    if (fabs(PID) == 3334 || fabs(PID) == 4332 || fabs(PID) == 4432 || fabs(PID) == 5332)
        return 6;

    // nucleon
    if (fabs(PID) == 2212 || fabs(PID) == 2112)
        return 7;
    
    return -1; //not strange
}