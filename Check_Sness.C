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
const int NClass       = 7; //number of particle classes to count    
const int NEventClass  = 2; //number of special events to select
const int NKaonClass   = 5;
const int NLambdaClass = 3;
const int NSigmaClass  = 7;
const int NXiClass     = 5;
const int NOmegaClass  = 3;
const int NNucleonClass= 4;

// for cendef
TString energy = "14";
const int cen_select = 9;
const int num_omega_cut = 1;

int FindType(int PID);

void Check_Sness(const Char_t *inFile = "placeholder.list", const TString JobID = "1234", const int mode = 1)
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
    TH1D* hstrangeness = new TH1D("sness", "total strangeness", 50, -24.5, 25.5);
    TH1D* hOmegaRatio  = new TH1D("hOmegaRatio", "Omega vs. Anti-Omega", 2 , -0.5 , 1.5 );
    TH1D* hMult_each   = new TH1D("hMult_each", "Mult for each choice of events", NEventClass+1, -0.5, -0.5+NEventClass+1);
    
    // new QA stuff
    // for adding STAR acceptance kaon antib count
    TProfile* hkaonct_woallx_for_wx_eta  = new TProfile("hkaonct_woallx_for_wx_eta", "", 8, -0.5, 7.5, 0, 100);
    TProfile* hkaonct_woallx_for_wxb_eta = new TProfile("hkaonct_woallx_for_wxb_eta", "", 8, -0.5, 7.5, 0, 100);
    TProfile* hbaryonct_woallx_eta = new TProfile("hbaryonct_woallx_eta", "", 6, -0.5, 5.5, -200, 600);
    TProfile* hbaryonct_wx_eta     = new TProfile("hbaryonct_wx_eta", "", 6, -0.5, 5.5, -200, 600);
    TProfile* hbaryonct_wxb_eta    = new TProfile("hbaryonct_wxb_eta", "", 6, -0.5, 5.5, -200, 600);
    TProfile* hbaryonctbar_woallx_eta = new TProfile("hbaryonctbar_woallx_eta", "", 6, -0.5, 5.5, -200, 600);
    TProfile* hbaryonctbar_wx_eta     = new TProfile("hbaryonctbar_wx_eta", "", 6, -0.5, 5.5, -200, 600);
    TProfile* hbaryonctbar_wxb_eta    = new TProfile("hbaryonctbar_wxb_eta", "", 6, -0.5, 5.5, -200, 600);

    // for counting pairs
    TH1D* hQualifiedEvt = new TH1D("hQualifiedEvt", "Number of qualified events", 9, 0.5, 9.5); //all 0-5%
    TH1D* hPairKO = new TH1D("hPairKO", "Number of kaon-Omega pairs",   4, -0.5, 3.5); //0: K+O-, 1: K+O+, 3: K-O-, 4: K-O+
    TH1D* hPairXO = new TH1D("hPairXO", "Number of Xi-Omega pairs",     4, -0.5, 3.5);
    TH1D* hPairLO = new TH1D("hPairLO", "Number of Lambda-Omega pairs", 4, -0.5, 3.5);
    TH1D* hPairpO = new TH1D("hPairpO", "Number of p-Omega pairs",      4, -0.5, 3.5);

    // for spectra
    TH1D* hOmegaPtSpectrum    = new TH1D("hOmegaPtSpectrum", "",   3000, 0., 6.); //0-10%
    TH1D* hOmegabarPtSpectrum = new TH1D("hOmegabarPtSpectrum","", 3000, 0., 6.); //0-10%
    TH1D* hXiPtSpectrum    = new TH1D("hXiPtSpectrum",      "",    3000, 0., 6.); //0-5%
    TH1D* hXibarPtSpectrum = new TH1D("hXibarPtSpectrum",      "", 3000, 0., 6.); //0-5%
    TH1D* hLambdaPtSpecctrum    = new TH1D("hLambdaPtSpectrum"," ",    3000, 0., 6.); // 0-5%
    TH1D* hLambdabarPtSpecctrum = new TH1D("hLambdabarPtSpectrum"," ", 3000, 0., 6.); // 0-5%

    // dN/dy vs Npart
    TH1D* hOmegadNdy    = new TH1D("hOmegadNdy", "Omega dN/dy at mid rapidity", 9, 0.5, 9.5);
    TH1D* hOmegabardNdy = new TH1D("hOmegabardNdy", "Omegabar dN/dy at mid rapidity", 9, 0.5, 9.5);
    TH1D* hXidNdy    = new TH1D("hXidNdy", "Xi dN/dy at mid rapidity", 9, 0.5, 9.5);
    TH1D* hXibardNdy = new TH1D("hXibardNdy", "Xibar dN/dy at mid rapidity", 9, 0.5, 9.5);
    TH1D* hLambdadNdy    = new TH1D("hLambdadNdy", "Lambda dN/dy at mid rapidity", 9, 0.5, 9.5);
    TH1D* hLambdabardNdy = new TH1D("hLambdabardNdy", "Lambdabar dN/dy at mid rapidity", 9, 0.5, 9.5);

    TH1D* hBaryon_yield = new TH1D("hBaryon_yield", "Baryon yield for BES comparison", 6, -0.5, 5.5); //0-1 lambda, 2-3 xi, 4-5 omega
    TH2D* hBaryon_yield_np = new TH2D("hBaryon_yield_np", "Baryon yield based on npart distribution", 6, -0.5, 5.5, 1000, -0.5, 999.5);

    TH1D* hnp_cen[9];
    char hname[200];
    for (int i = 0; i < 9; i++) 
    {   
        sprintf(hname, "hnp_cen_%d", i+1);
        hnp_cen[i] = new TH1D(hname, hname, 1000, -0.5, 999.5);
    }
    TH2D* hnp_vs_ssbar = new TH2D("hnp_vs_ssbar", "Npart vs SSbar pair", 1000, -0.5, 999.5, 1000, -0.5, 999.5);
    TH1D* hnp         = new TH1D("hnp", "Participant number for all qualified events", 1000, -0.5, 999.5);
    TH1D* hnp_owx     = new TH1D("hnp_owx", "Participant number for criteria owx", 1000, -0.5, 999.5);
    TH1D* hnp_owox    = new TH1D("hnp_owox", "Participant number for criteria owox", 1000, -0.5, 999.5);
    TH1D* hnp_owxb    = new TH1D("hnp_owxb", "Participant number for criteria owxb", 1000, -0.5, 999.5);
    TH1D* hnp_owoxb   = new TH1D("hnp_owoxb", "Participant number for criteria owoxb", 1000, -0.5, 999.5);
    TH1D* hnp_obwx    = new TH1D("hnp_obwx", "Participant number for criteria obwx", 1000, -0.5, 999.5);
    TH1D* hnp_obwox   = new TH1D("hnp_obwox", "Participant number for criteria obwox", 1000, -0.5, 999.5);
    TH1D* hnp_obwxb   = new TH1D("hnp_obwxb", "Participant number for criteria obwxb", 1000, -0.5, 999.5);
    TH1D* hnp_obwoxb  = new TH1D("hnp_obwoxb", "Participant number for criteria obwoxb", 1000, -0.5, 999.5);
    TH1D* hnp_wx      = new TH1D("hnp_wx", "Participant number for criteria wx", 1000, -0.5, 999.5);
    TH1D* hnp_wox     = new TH1D("hnp_wox", "Participant number for criteria wox", 1000, -0.5, 999.5);
    TH1D* hnp_wxb     = new TH1D("hnp_wxb", "Participant number for criteria wxb", 1000, -0.5, 999.5);
    TH1D* hnp_woxb    = new TH1D("hnp_woxb", "Participant number for criteria woxb", 1000, -0.5, 999.5);
    TH1D* hnp_wo      = new TH1D("hnp_wo", "Participant number for criteria wo", 1000, -0.5, 999.5);
    TH1D* hnp_woo     = new TH1D("hnp_woo", "Participant number for criteria woo", 1000, -0.5, 999.5);
    TH1D* hnp_wob     = new TH1D("hnp_wob", "Participant number for criteria wob", 1000, -0.5, 999.5);
    TH1D* hnp_woob    = new TH1D("hnp_woob", "Participant number for criteria woob", 1000, -0.5, 999.5);
    TH1D* hnp_woallx_for_wx  = new TH1D("hnp_woallx_for_wx", "Participant number for criteria woallx_for_wx", 1000, -0.5, 999.5);
    TH1D* hnp_woallx_for_wxb = new TH1D("hnp_woallx_for_wxb", "Participant number for criteria woallx_for_wxb", 1000, -0.5, 999.5);
    TH1D* hnp_woallo_for_wo  = new TH1D("hnp_woallo_for_wo", "Participant number for criteria woallo_for_wo", 1000, -0.5, 999.5);
    TH1D* hnp_woallo_for_wob = new TH1D("hnp_woallo_for_wob", "Participant number for criteria woallo_for_wob", 1000, -0.5, 999.5);

    TH1D* hbaryon            = new TH1D("hbaryon", "total baryon number", 800, -199.5, 600.5);
    TProfile* hbndist_wo     = new TProfile("hbndist_wo", "Baryon number distribution for with omega events", 6, -0.5, 5.5, -200, 600);
    TProfile* hbndist_woo    = new TProfile("hbndist_woo", "Baryon number distribution for without omega events", 6, -0.5, 5.5, -200, 600);
    TProfile* hbndist_wob    = new TProfile("hbndist_wob", "Baryon number distribution for with omegabar events", 6, -0.5, 5.5, -200, 600);
    TProfile* hbndist_woob   = new TProfile("hbndist_woob", "Baryon number distribution for without omegabar events", 6, -0.5, 5.5, -200, 600);
    TProfile* hbndist_wx     = new TProfile("hbndist_wx", "Baryon number distribution for with xi events", 6, -0.5, 5.5, -200, 600);
    TProfile* hbndist_wox    = new TProfile("hbndist_wox", "Baryon number distribution for without xi events", 6, -0.5, 5.5, -200, 600);
    TProfile* hbndist_wxb    = new TProfile("hbndist_wxb", "Baryon number distribution for with xibar events", 6, -0.5, 5.5, -200, 600);
    TProfile* hbndist_woxb   = new TProfile("hbndist_woxb", "Baryon number distribution for without xibar events", 6, -0.5, 5.5, -200, 600);
    TProfile* hbndist_bar_wx     = new TProfile("hbndist_bar_wx", "Baryon number distribution for with xi events", 6, -0.5, 5.5, -200, 600);
    TProfile* hbndist_bar_wxb    = new TProfile("hbndist_bar_wxb", "Baryon number distribution for with xibar events", 6, -0.5, 5.5, -200, 600);
    TProfile* hbndist_owxb   = new TProfile("hbndist_owxb", "Baryon number distribution for omega events with xibar", 6, -0.5, 5.5, -200, 600);
    TProfile* hbndist_owoxb  = new TProfile("hbndist_owoxb", "Baryon number distribution for omega events without xibar", 6, -0.5, 5.5, -200, 600);
    TProfile* hbndist_owx    = new TProfile("hbndist_owx", "Baryon number distribution for omega events with xi", 6, -0.5, 5.5, -200, 600);
    TProfile* hbndist_owox   = new TProfile("hbndist_owox", "Baryon number distribution for omega events without xi", 6, -0.5, 5.5, -200, 600);
    TProfile* hbndist_obwx   = new TProfile("hbndist_obwx", "Baryon number distribution for omegabar events with xi", 6, -0.5, 5.5, -200, 600);
    TProfile* hbndist_obwox  = new TProfile("hbndist_obwox", "Baryon number distribution for omegabar events without xi", 6, -0.5, 5.5, -200, 600);
    TProfile* hbndist_obwxb  = new TProfile("hbndist_obwxb", "Baryon number distribution for omegabar events with xibar", 6, -0.5, 5.5, -200, 600);
    TProfile* hbndist_obwoxb = new TProfile("hbndist_obwoxb", "Baryon number distribution for omegabar events without xibar", 6, -0.5, 5.5, -200, 600);
    TProfile* hbndist_woallo_for_wo  = new TProfile("hbndist_woallo_for_wo" , "Baryon number distribution for events without all omega for omega", 6, -0.5, 5.5, -200, 600);
    TProfile* hbndist_woallo_for_wob = new TProfile("hbndist_woallo_for_wob", "Baryon number distribution for events without all omega for omegabar", 6, -0.5, 5.5, -200, 600);
    TProfile* hbndist_woallx_for_wx  = new TProfile("hbndist_woallx_for_wx" , "Baryon number distribution for events without all xi for xi", 6, -0.5, 5.5, -200, 600);
    TProfile* hbndist_woallx_for_wxb = new TProfile("hbndist_woallx_for_wxb", "Baryon number distribution for events without all xi for xibar", 6, -0.5, 5.5, -200, 600);
    TProfile* hbndist_bar_woallx_for_wx  = new TProfile("hbndist_bar_woallx_for_wx" , "Baryon number distribution for events without all xi for xi", 6, -0.5, 5.5, -200, 600);
    TProfile* hbndist_bar_woallx_for_wxb = new TProfile("hbndist_bar_woallx_for_wxb", "Baryon number distribution for events without all xi for xibar", 6, -0.5, 5.5, -200, 600);
    
    TProfile* hkaonct_owx   = new TProfile("hkaonct_owx"  , "Kaon count for omega event with xi and without",8, -0.5, 7.5, 0, 100); //0-1: k+, 2-3: k-, 4-5: k0, 6-7: k0bar
    TProfile* hkaonct_owxb  = new TProfile("hkaonct_owxb" , "Kaon count for omega event with xibar and without",8, -0.5, 7.5, 0, 100);
    TProfile* hkaonct_obwx  = new TProfile("hkaonct_obwx" , "Kaon count for omegabar event with xi and without",8, -0.5, 7.5, 0, 100);
    TProfile* hkaonct_obwxb = new TProfile("hkaonct_obwxb", "Kaon count for omegabar event with xibar and without",8, -0.5, 7.5, 0, 100);
    TProfile* hkaonct_x     = new TProfile("hkaonct_x"  , "Kaon count for events with xi and without",8, -0.5, 7.5, 0, 100);
    TProfile* hkaonct_xb    = new TProfile("hkaonct_xb" , "Kaon count for events with xibar and without",8, -0.5, 7.5, 0, 100);
    TProfile* hkaonct_o     = new TProfile("hkaonct_o"  , "Kaon count for events with omega and without",8, -0.5, 7.5, 0, 100);
    TProfile* hkaonct_ob    = new TProfile("hkaonct_ob" , "Kaon count for events with omegabar and without",8, -0.5, 7.5, 0, 100);
    TProfile* hkaonct_woallo_for_wo  = new TProfile("hkaonct_woallo_for_wo" , "Kaon count for events without all omega and with omega",8, -0.5, 7.5, 0, 100);
    TProfile* hkaonct_woallo_for_wob = new TProfile("hkaonct_woallo_for_wob", "Kaon count for events without all omega and with omegabar",8, -0.5, 7.5, 0, 100);
    TProfile* hkaonct_woallx_for_wx  = new TProfile("hkaonct_woallx_for_wx" , "Kaon count for events without all xi and with xi",8, -0.5, 7.5, 0, 100);
    TProfile* hkaonct_woallx_for_wxb = new TProfile("hkaonct_woallx_for_wxb", "Kaon count for events without all xi and with xibar",8, -0.5, 7.5, 0, 100);

    // 0 - total, 1 - Anti-O, 2 - O, 3 - Anti-X, 4 - X, 5 - Anti-X0, 6 - X0， 7 - Nucleons
    TH1D *hsdist[NEventClass+1], *hpdist[NEventClass+1], *hKaon [NEventClass+1], *hLambda[NEventClass+1], 
         *hSigma[NEventClass+1], *hXi   [NEventClass+1], *hOmega[NEventClass+1], *hNucleon[NEventClass+1];
    char fname[8][200]; 
    for (int i = 0; i < NEventClass+1; i++)
    {
        sprintf(fname[0], "hsdist_%d" , i);
        sprintf(fname[1], "hpdist_%d" , i);
        sprintf(fname[2], "hKaon_%d"     , i);
        sprintf(fname[3], "hLambda_%d", i);
        sprintf(fname[4], "hSigma_%d" , i);
        sprintf(fname[5], "hXi_%d"    , i);
        sprintf(fname[6], "hOmega_%d" , i);
        sprintf(fname[7], "hNucleon_%d_count", i);
        hsdist[i]  = new TH1D(fname[0], fname[0], NClass       , 0.5, 0.5 + NClass       );
        hpdist[i]  = new TH1D(fname[1], fname[1], NClass       , 0.5, 0.5 + NClass       );
        hKaon[i]   = new TH1D(fname[2], fname[2], NKaonClass   , 0.5, 0.5 + NKaonClass   );
        hLambda[i] = new TH1D(fname[3], fname[3], NLambdaClass , 0.5, 0.5 + NLambdaClass );
        hSigma[i]  = new TH1D(fname[4], fname[4], NSigmaClass  , 0.5, 0.5 + NSigmaClass  );
        hXi[i]     = new TH1D(fname[5], fname[5], NXiClass     , 0.5, 0.5 + NXiClass     );
        hOmega[i]  = new TH1D(fname[6], fname[6], NOmegaClass  , 0.5, 0.5 + NOmegaClass  );
        hNucleon[i]= new TH1D(fname[7], fname[7], NNucleonClass, 0.5, 0.5 + NNucleonClass);
    
    }

    // setting PID and momentum branches
    TBranch* bpid = nullptr;
    int npp = 0, npt = 0;
    int refmult;
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
    chain->SetBranchAddress("pid", &pid_vec, &bpid);
    chain->SetBranchAddress("px",  &px_vec);
    chain->SetBranchAddress("py",  &py_vec);
    chain->SetBranchAddress("pz",  &pz_vec);

    // loops
    //std::set<int> s_pid_set;
    int nentries = chain->GetEntries();
    bool hasParticle[NEventClass+1]; 
    bool hasXi, hasAntiXi;
    int M[NEventClass+1] = {0}; //number of events that contain anti_o, o, anti_x, x, anti_x0, x0;

    // evt count
    int nevt_spec[9] = {0};

    // event loop
    for (int i = 0; i < nentries; ++i)
    {
        if((i+1)%1000==0) cout<<"Processing entry == "<< i+1 <<" == out of "<<nentries<<".\n";
        chain->GetEntry(i);
        int np = 0;
        if (mode != 2) np = npp + npt;
    
        // centrality
        CenMaker cenmaker;
        int cen = cenmaker.cent9(refmult, energy, mode);
        for (int i = 1; i <= 9; i++) {if (cen == i) nevt_spec[i-1]++;}

        // counting particle for BES comparison and count omega
        int NKp=0, NKm=0, NK0=0, NK0bar=0, NOmega=0, NOmegabar=0, NXi=0, NXibar=0, NLambda=0, NLambdabar=0, Np=0, Npbar=0;
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

            // count particles
            if (fabs(pid) ==  3334) num_omega_total++;
        
            // count "np" for UrQMD
            if (mode == 2)
            {
                if (fabs(eta) < 1 && p_info->Charge() != 0) np++;
            }

            // STAR acceptance
            if (fabs(eta) < 1)
            {
                if (pid ==  3334) NOmega++;
                if (pid == -3334) NOmegabar++;
                if (pid ==  3312) NXi++;
                if (pid == -3312) NXibar++;
                if (pid ==  3122) NLambda++;
                if (pid == -3122) NLambdabar++;
                if (pid ==  321) NKp++;
                if (pid == -321) NKm++;
                if (pid ==  311) NK0++;
                if (pid == -311) NK0bar++;
                if (pid ==  2212) Np++;
                if (pid == -2212) Npbar++;
            }

            // for baryon/anti-baryon ratios
            if (fabs(y) > 0.5) continue; // mid-rapidity
            if (pid ==  3334) NO_y++;
            if (pid == -3334) NOb_y++;
            if (pid ==  3312) NX_y++;
            if (pid == -3312) NXb_y++;
            if (pid ==  3122) NL_y++;
            if (pid == -3122) NLb_y++;
            

            if (pid == -3122 && cen == 9)                {hBaryon_yield->Fill(0.); hBaryon_yield_np->Fill(0., np*1.0);}
            if (pid ==  3122 && cen == 9)                {hBaryon_yield->Fill(1.); hBaryon_yield_np->Fill(1., np*1.0);}
            if (pid == -3312 && cen == 9)                {hBaryon_yield->Fill(2.); hBaryon_yield_np->Fill(2., np*1.0);}
            if (pid ==  3312 && cen == 9)                {hBaryon_yield->Fill(3.); hBaryon_yield_np->Fill(3., np*1.0);}
            if (pid == -3334 && (cen == 9 || cen == 8)) {hBaryon_yield->Fill(4.); hBaryon_yield_np->Fill(4., np*1.0);}
            if (pid ==  3334 && (cen == 9 || cen == 8)) {hBaryon_yield->Fill(5.); hBaryon_yield_np->Fill(5., np*1.0);}
        }

        hOmegadNdy ->Fill(cen*1.0, NO_y*1.0); hOmegabardNdy ->Fill(cen*1.0, NOb_y*1.0); 
        hXidNdy    ->Fill(cen*1.0, NX_y*1.0); hXibardNdy    ->Fill(cen*1.0, NXb_y*1.0); 
        hLambdadNdy->Fill(cen*1.0, NL_y*1.0); hLambdabardNdy->Fill(cen*1.0, NLb_y*1.0); 

        hPairKO->Fill(0., NKp    *NOmega*1.0); hPairKO->Fill(1., NKp    *NOmegabar*1.0); hPairKO->Fill(2., NKm       *NOmega*1.0); hPairKO->Fill(3., NKm       *NOmegabar*1.0); 
        hPairLO->Fill(0., NLambda*NOmega*1.0); hPairLO->Fill(1., NLambda*NOmegabar*1.0); hPairLO->Fill(2., NLambdabar*NOmega*1.0); hPairLO->Fill(3., NLambdabar*NOmegabar*1.0); 
        hPairXO->Fill(0., NXi    *NOmega*1.0); hPairXO->Fill(1., NXi    *NOmegabar*1.0); hPairXO->Fill(2., NXibar    *NOmega*1.0); hPairXO->Fill(3., NXibar    *NOmegabar*1.0); 
        hPairpO->Fill(0., Np     *NOmega*1.0); hPairpO->Fill(1., Np     *NOmegabar*1.0); hPairpO->Fill(2., Npbar     *NOmega*1.0); hPairpO->Fill(3., Npbar     *NOmegabar*1.0); 

        // event cut for sness analysis
        if (cen != cen_select) continue;
        if (num_omega_total > num_omega_cut) continue;

        // for npart normalization
        NpartNormalizer normalizer(mode);
        normalizer.unit_weight(); // IF DONE CALCULATING, COMMENT OUT

        int total_s = 0, total_bn = 0, itrack = 0;
        float total_ssbar_pair = 0;

        // check if event has certain particles
        for (int j = 0; j < NEventClass+1; j++) hasParticle[j] = false;
        hasXi = false; hasAntiXi = false;

        // fill examples and check anti-omega
        for (std::vector<int>::iterator it = pid_vec->begin(); it != pid_vec->end(); ++it)
        {
            int s = sness_map[(int)(*it)];

            // check for particle
            if (*it == -3312) hasParticle[1] = true; if (*it == 3312) hasParticle[2] = true; // can switch 3312 and 3334 to switch between owx and xwo
            if (*it == -3334) hasAntiXi      = true; if (*it == 3334) hasXi          = true;

            // omega ratio
            if (*it ==  3334) hOmegaRatio->Fill(0);
            if (*it == -3334) hOmegaRatio->Fill(1);
            
            // total strangeness and baryon number
            total_s += s;
            total_ssbar_pair += fabs(s)/2.0;
            if (*it < 10000 && *it > 999) total_bn += 1;
            if (*it >-10000 && *it <-999) total_bn -= 1;
        }
    
        // count events
        hasParticle[0] = true; //total count
        for (int j = 0; j < NEventClass+1; j++) {if (hasParticle[j]) M[j]++;}

        // loop thru events
        int ntrack = pid_vec->size();
        assert(ntrack == px_vec->size() && ntrack == py_vec->size() && ntrack == pz_vec->size() && "Ntrack size mismatch!");
        int kaonct[4] = {0}; int kaonct_y[4] = {0};
        int bct[6] = {0};    int bct_y[6] = {0};
        int bctbar[6] = {0}; int bctbar_y[6] = {0};
        for (int i = 0; i < ntrack; ++i)
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
            
            // fill all and selected events
            if (type == -1) continue;
            for (int j = 0; j < NEventClass+1; j++)
            {
                if (!hasParticle[j]) continue;
                
                hpdist[j]->Fill(type);
                hsdist[j]->Fill(type, s);
                if (type == 1) //kaons
                {
                    if      (pid ==  311) hKaon[j]->Fill(1, s); //K^0
                    else if (pid == -311) hKaon[j]->Fill(2, s); //anti K^0
                    else if (pid ==  321) hKaon[j]->Fill(3, s); //K^+
                    else if (pid == -321) hKaon[j]->Fill(4, s); //K^-;
                    else                  hKaon[j]->Fill(5, s); //others
                }

                if (type == 3) //lambdas
                {
                    if      (pid ==  3122) hLambda[j]->Fill(1, s); //Lambda
                    else if (pid == -3122) hLambda[j]->Fill(2, s); //anti Lambda
                    else                   hLambda[j]->Fill(3, s); //others
                }

                if (type == 4) //Sigmas
                {
                    if      (pid ==  3222) hSigma[j]->Fill(1, s); //sigma^+
                    else if (pid == -3222) hSigma[j]->Fill(2, s); //anti sigma^-
                    else if (pid ==  3212) hSigma[j]->Fill(3, s); //sigma^0
                    else if (pid == -3212) hSigma[j]->Fill(4, s); //anti sigma^0
                    else if (pid ==  3112) hSigma[j]->Fill(5, s); //sigma^-
                    else if (pid == -3112) hSigma[j]->Fill(6, s); //anti sigma^0
                    else                   hSigma[j]->Fill(7, s); //others
                }

                if (type == 5) //Xi
                {
                    if      (pid ==  3312) hXi[j]->Fill(1, s); //Xi^-
                    else if (pid == -3312) hXi[j]->Fill(2, s); //anti Xi^+
                    else if (pid ==  3322) hXi[j]->Fill(3, s); //Xi^0
                    else if (pid == -3322) hXi[j]->Fill(4, s); //anti X^0;
                    else                   hXi[j]->Fill(5, s); //others
                }

                if (type == 6) //Omegas
                {
                    if      (pid ==  3334) hOmega[j]->Fill(1, s); //Omega^-
                    else if (pid == -3334) hOmega[j]->Fill(2, s); //anti Omega^+
                    else                   hOmega[j]->Fill(3, s); //others
                }

                if (type == 7) //Nucleons
                {
                    if      (pid ==  2212) hNucleon[j]->Fill(1); //proton
                    else if (pid == -2212) hNucleon[j]->Fill(2); //anti-proton
                    else if (pid ==  2112) hNucleon[j]->Fill(3); //neutron
                    else if (pid == -2112) hNucleon[j]->Fill(4); //anti-neutron
                }
            } 

            // count kaons and baryon number
            if (pid ==  321) {kaonct[0]++; if (fabs(eta) < 1) kaonct_y[0]++;}//k+
            if (pid == -321) {kaonct[1]++; if (fabs(eta) < 1) kaonct_y[1]++;}//k-
            if (pid ==  311) {kaonct[2]++; if (fabs(eta) < 1) kaonct_y[2]++;}//k0
            if (pid == -311) {kaonct[3]++; if (fabs(eta) < 1) kaonct_y[3]++;}//k0bar
            if (fabs(pid) >= 10000 || fabs(pid) <= 999) continue;
            // total
            if (pid > 0) {bct   [0]++; if (fabs(eta) < 1) bct_y   [0]++;}
            if (pid < 0) {bctbar[0]++; if (fabs(eta) < 1) bctbar_y[0]++;}
            // lambda
            if (type == 3)
            {
                if (pid > 0) {bct   [1]++; if (fabs(eta) < 1) bct_y   [1]++;}
                if (pid < 0) {bctbar[1]++; if (fabs(eta) < 1) bctbar_y[1]++;}
            }
            // sigma
            if (type == 4)
            {
                if (pid > 0) {bct   [2]++; if (fabs(eta) < 1) bct_y   [2]++;}
                if (pid < 0) {bctbar[2]++; if (fabs(eta) < 1) bctbar_y[2]++;}
            }
            // cascade
            if (type == 5)
            {
                if (pid > 0) {bct   [3]++; if (fabs(eta) < 1) bct_y   [3]++;}
                if (pid < 0) {bctbar[3]++; if (fabs(eta) < 1) bctbar_y[3]++;}
            }
            // omega
            if (type == 6)
            {
                if (pid > 0) {bct   [4]++; if (fabs(eta) < 1) bct_y   [4]++;}
                if (pid < 0) {bctbar[4]++; if (fabs(eta) < 1) bctbar_y[4]++;}
            }
            // nucleon
            if (type == 7)
            {
                if (pid > 0) {bct   [5]++; if (fabs(eta) < 1) bct_y   [5]++;}
                if (pid < 0) {bctbar[5]++; if (fabs(eta) < 1) bctbar_y[5]++;}
            }

        }

        hstrangeness->Fill(total_s);  
        hbaryon->Fill(total_bn);  
        //np = static_cast<int>(total_ssbar_pair); //change to ssbar pair counts, comment to switch back to npart
        int ssbar_pair = static_cast<int>(total_ssbar_pair);
        // baryon number subplots
        for (int k = 0; k < 6; k++)
        {
            if (hasParticle[2]) // if has omega
            {
                hbndist_wo->Fill(k, bct[k]);
                if (hasXi)     hbndist_owx  ->Fill(k, bct[k]);
                else           hbndist_owox ->Fill(k, bct[k], normalizer.weight(np, "ox"));
                if (hasAntiXi) hbndist_owxb ->Fill(k, bct[k]);
                else           hbndist_owoxb->Fill(k, bct[k], normalizer.weight(np, "oxb"));
            }
            else hbndist_woo->Fill(k, bct[k], normalizer.weight(np, "o"));

            if (hasParticle[1]) // if has anti-omega
            {
                hbndist_wob->Fill(k, bct[k]);
                if (hasXi)     hbndist_obwx  ->Fill(k, bct[k]);
                else           hbndist_obwox ->Fill(k, bct[k], normalizer.weight(np, "obx"));
                if (hasAntiXi) hbndist_obwxb ->Fill(k, bct[k]);
                else           hbndist_obwoxb->Fill(k, bct[k], normalizer.weight(np, "obxb"));
            }
            else hbndist_woob->Fill(k, bct[k], normalizer.weight(np, "ob")); 

            if (hasXi)     
            {
                hbndist_wx         ->Fill(k, bct[k]); 
                hbndist_bar_wx     ->Fill(k, bctbar[k]);
                hbaryonct_wx_eta   ->Fill(k, bct_y[k]);
                hbaryonctbar_wx_eta->Fill(k, bctbar_y[k]);
            }
            else hbndist_wox ->Fill(k, bct[k], normalizer.weight(np, "x")); 
            if (hasAntiXi) 
            {
                hbndist_wxb         ->Fill(k, bct[k]); 
                hbndist_bar_wxb     ->Fill(k, bctbar[k]);
                hbaryonct_wxb_eta   ->Fill(k, bct_y[k]);
                hbaryonctbar_wxb_eta->Fill(k, bctbar_y[k]);
            }
            else           hbndist_woxb->Fill(k, bct[k], normalizer.weight(np, "xb")); 

            if ((!hasParticle[2]) && (!hasParticle[1]))
            {
                hbndist_woallo_for_wo ->Fill(k, bct[k], normalizer.weight(np, "woallo_for_wo"));
                hbndist_woallo_for_wob->Fill(k, bct[k], normalizer.weight(np, "woallo_for_wob"));
            }
            if ((!hasXi) && (!hasAntiXi))
            {
                //hbndist_woallx_for_wx ->Fill(k, bct[k], normalizer.weight(np, "woallx_for_wx"));
                //hbndist_woallx_for_wxb->Fill(k, bct[k], normalizer.weight(np, "woallx_for_wxb"));
                hbndist_woallx_for_wx ->Fill(k, bct[k], normalizer.weight(ssbar_pair, "ss_woallx_for_wx"));
                hbndist_woallx_for_wxb->Fill(k, bct[k], normalizer.weight(ssbar_pair, "ss_woallx_for_wxb"));
                hbndist_bar_woallx_for_wx ->Fill(k, bctbar[k], normalizer.weight(ssbar_pair, "ss_woallx_for_wx"));
                hbndist_bar_woallx_for_wxb->Fill(k, bctbar[k], normalizer.weight(ssbar_pair, "ss_woallx_for_wxb"));
                hbaryonct_woallx_eta   ->Fill(k, bct_y[k]);
                hbaryonctbar_woallx_eta->Fill(k, bctbar_y[k]);
            }
        }

        // kaon ct subplots
        for (int i=0;i<4;i++)
        {
            if (hasParticle[2])
            { 
                if (hasXi)     hkaonct_owx  ->Fill(2.*i  , kaonct[i]*1.0); 
                else           hkaonct_owx  ->Fill(2.*i+1, kaonct[i]*1.0, normalizer.weight(np, "ox")); 
                if (hasAntiXi) hkaonct_owxb ->Fill(2.*i  , kaonct[i]*1.0); 
                else           hkaonct_owxb ->Fill(2.*i+1, kaonct[i]*1.0, normalizer.weight(np, "oxb"));
            }
            if (hasParticle[1])
            { 
                if (hasXi)     hkaonct_obwx ->Fill(2.*i  , kaonct[i]*1.0); 
                else           hkaonct_obwx ->Fill(2.*i+1, kaonct[i]*1.0, normalizer.weight(np, "obx")); 
                if (hasAntiXi) hkaonct_obwxb->Fill(2.*i  , kaonct[i]*1.0); 
                else           hkaonct_obwxb->Fill(2.*i+1, kaonct[i]*1.0, normalizer.weight(np, "obxb")); 
            }
            if (hasXi)     hkaonct_x ->Fill(2.*i  , kaonct[i]*1.0); 
            else           hkaonct_x ->Fill(2.*i+1, kaonct[i]*1.0, normalizer.weight(np, "x")); 
            if (hasAntiXi) hkaonct_xb->Fill(2.*i  , kaonct[i]*1.0); 
            else           hkaonct_xb->Fill(2.*i+1, kaonct[i]*1.0, normalizer.weight(np, "xb"));
            if (hasParticle[2]) hkaonct_o ->Fill(2.*i  , kaonct[i]*1.0); 
            else                hkaonct_o ->Fill(2.*i+1, kaonct[i]*1.0, normalizer.weight(np, "o")); 
            if (hasParticle[1]) hkaonct_ob->Fill(2.*i  , kaonct[i]*1.0); 
            else                hkaonct_ob->Fill(2.*i+1, kaonct[i]*1.0, normalizer.weight(np, "ob"));

            if (hasParticle[2])                         hkaonct_woallo_for_wob->Fill(2.*i  , kaonct[i]*1.0);
            if ((!hasParticle[2]) && (!hasParticle[1])) hkaonct_woallo_for_wob->Fill(2.*i+1, kaonct[i]*1.0, normalizer.weight(np, "woallo_for_wob"));
            if (hasParticle[1])                         hkaonct_woallo_for_wo ->Fill(2.*i  , kaonct[i]*1.0);
            if ((!hasParticle[2]) && (!hasParticle[1])) hkaonct_woallo_for_wo ->Fill(2.*i+1, kaonct[i]*1.0, normalizer.weight(np, "woallo_for_wo"));

            if (hasAntiXi)                
            {
                hkaonct_woallx_for_wxb->Fill(2.*i  , kaonct[i]*1.0);
                hkaonct_woallx_for_wxb_eta->Fill(2.*i  , kaonct_y[i]*1.0);
            }
            //if ((!hasXi) && (!hasAntiXi)) hkaonct_woallx_for_wxb->Fill(2.*i+1, kaonct[i]*1.0, normalizer.weight(np, "woallx_for_wxb"));
            if ((!hasXi) && (!hasAntiXi)) 
            {
                hkaonct_woallx_for_wxb->Fill(2.*i+1, kaonct[i]*1.0, normalizer.weight(ssbar_pair, "ss_woallx_for_wxb"));
                hkaonct_woallx_for_wxb_eta->Fill(2.*i+1  , kaonct_y[i]*1.0);
            }
            if (hasXi)                    
            {
                hkaonct_woallx_for_wx ->Fill(2.*i  , kaonct[i]*1.0);
                hkaonct_woallx_for_wx_eta->Fill(2.*i  , kaonct_y[i]*1.0);
            }
            //if ((!hasXi) && (!hasAntiXi)) hkaonct_woallx_for_wx ->Fill(2.*i+1, kaonct[i]*1.0, normalizer.weight(np, "woallx_for_wx"));
            if ((!hasXi) && (!hasAntiXi)) 
            {
                hkaonct_woallx_for_wx ->Fill(2.*i+1, kaonct[i]*1.0, normalizer.weight(ssbar_pair, "ss_woallx_for_wx"));
                hkaonct_woallx_for_wx_eta->Fill(2.*i+1  , kaonct_y[i]*1.0);
            }

        }

        // np distributions
        hnp->Fill(np);
        hnp_vs_ssbar->Fill(np, ssbar_pair);
        if (hasParticle[2])
        {   
            hnp_wo->Fill(np);
            if (hasXi)     hnp_owx   ->Fill(np); 
            else           hnp_owox  ->Fill(np, normalizer.weight(np, "ox")); 
            if (hasAntiXi) hnp_owxb  ->Fill(np); 
            else           hnp_owoxb ->Fill(np, normalizer.weight(np, "oxb"));
        }
        else hnp_woo->Fill(np, normalizer.weight(np, "o"));
        if (hasParticle[1])
        { 
            hnp_wob->Fill(np);
            if (hasXi)     hnp_obwx  ->Fill(np); 
            else           hnp_obwox ->Fill(np, normalizer.weight(np, "obx")); 
            if (hasAntiXi) hnp_obwxb ->Fill(np); 
            else           hnp_obwoxb->Fill(np, normalizer.weight(np, "obxb")); 
        }
        else hnp_woob->Fill(np, normalizer.weight(np, "ob"));
        if (hasXi)     hnp_wx  ->Fill(np); 
        else           hnp_wox ->Fill(np, normalizer.weight(np, "x")); 
        if (hasAntiXi) hnp_wxb ->Fill(np); 
        else           hnp_woxb->Fill(np, normalizer.weight(np, "xb")); 
        if ((!hasParticle[2]) && (!hasParticle[1])) // test case without both omega and anti-omega (xi and anti-xi)
        {
            hnp_woallo_for_wo ->Fill(np, normalizer.weight(np, "woallo_for_wo"));
            hnp_woallo_for_wob->Fill(np, normalizer.weight(np, "woallo_for_wob"));
        }
        if ((!hasXi) && (!hasAntiXi))
        {
            //hnp_woallx_for_wx ->Fill(np, normalizer.weight(np, "woallx_for_wx"));
            //hnp_woallx_for_wxb->Fill(np, normalizer.weight(np, "woallx_for_wxb"));
            hnp_woallx_for_wx ->Fill(ssbar_pair, normalizer.weight(ssbar_pair, "ss_woallx_for_wx"));
            hnp_woallx_for_wxb->Fill(ssbar_pair, normalizer.weight(ssbar_pair, "ss_woallx_for_wxb"));
        }
    }

    for (int i = 1; i <= 9; i++) hQualifiedEvt->Fill(i, nevt_spec[i - 1]);
    for (int i = 0; i < NEventClass+1; i++) hMult_each->Fill(i, M[i]); //i = 0: total event count

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