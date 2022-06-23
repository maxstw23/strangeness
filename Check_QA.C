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
        for (int i = 1; i <= 9; i++) {if (cen == i) nevt_spec[i-1]++;}
        hnp_cen[cen-1]->Fill(np);
        hnp->Fill(np);
    
        // counting particle for BES comparison and count omega
        int NO_y=0, NOb_y=0, NX_y=0, NXb_y=0, NL_y=0, NLb_y=0;
        int num_omega_total = 0;
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