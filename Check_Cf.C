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
#include "TLorentzVector.h"
#include <algorithm>
#include <stdlib.h>
#include "TComplex.h"
#include "TChain.h"
#include "TVector2.h"
#include "TDatabasePDG.h"
#include "TParticlePDG.h"
#include <string>
// my classes
#include "./lib/my_particle.h"
#include "./lib/my_event.h"
#include "./lib/MixedBuffer.h"
#include "./lib/CenMaker.h"

using namespace std;

// constants
const float PI = TMath::Pi();
const float eta_cut = 1.;
const float y_cut   = 1.;
const int buffer_size = 10;
const bool  CutEta  = true;
const bool  Cuty    = false; //only select either CutEta or Cuty or none, not both!
int cen_select = 9;
TString energy = "14";

const float mKaon = 0.493677;
const float mOmega = 1.67245;
const float mProton = 0.938272;
const float mLambda = 1.115683;
const float mXim = 1.32171;
const int   KaonPID = 321;
const int   OmegaPID = 3334;
const int   ProtonPID = 2212;
const int   LambdaPID = 3122;
const int   Xi0PID = 3322;
const int   XimPID = 3312;


void Check_Cf(const Char_t *inFile = "placeholder.list", const TString JobID = "1234", const int mode = 1)
//  mode 0: AMPT default
//  mode 1: AMPT string-melting or UrQMD (will add UrQMD cendef)
{   
    // PDG Database
	TDatabasePDG *db = new TDatabasePDG();
	TParticlePDG *p_info = new TParticlePDG();

    // initialize map
    std::map<int, int> sness_map;
    sness_map.insert(std::pair<int, int>( 321,  1)); //K^+
    sness_map.insert(std::pair<int, int>( 3334,-3)); //omega^-
    sness_map.insert(std::pair<int, int>(-3334, 3)); //anti_omega^+

    /*
    std::map<int, float> mass_map;
    mass_map.insert(std::pair<int, float>(ProtonPID, mProton));
    mass_map.insert(std::pair<int, float>(LambdaPID, mLambda));
    mass_map.insert(std::pair<int, float>(OmegaPID, mOmega));
    mass_map.insert(std::pair<int, float>(KaonPID, mKaon));*/

    // mixed event buffer

    // input
    if ((CutEta && Cuty) == true) {cout << "Cannot have both eta and y cuts. Exiting..." << endl; return;}
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

    // QA hist 
    TH1D* hMult     = new TH1D("hMult"     , "Total Multiplicity"               , 1000, -0.5, 999.5);
    TH1D* hRefMult  = new TH1D("hRefMult"  , "RefMult"                          , 1000, -0.5, 999.5);
    TH1D* hImpPar   = new TH1D("hImpPar"   , "Impact Parameter"                 , 200,    0., 20.  );
    TH1D* hP        = new TH1D("hP"        , "Total Momentum"                   , 500 ,   0., 15.  );
    TH1D* hPt       = new TH1D("hPt"       , "Tranverse Momentum"               , 500 ,   0., 15.  );
    TH1D* hEta      = new TH1D("hEta"      , "Pseudorapidity"                   , 500 , -10., 10.  );
    TH1D* hy        = new TH1D("hy"        , "Rapidity"                         , 500 , -10., 10.  );
    TH2D* hPtEta    = new TH2D("hPtEta"    , "Pt-eta map"                       , 500 ,   0., 15., 500, -10., 10.);
    TH1D* hEtaO     = new TH1D("hEtaO"     , "#Omega^{-} Pseudorapidity"        , 500 , -10., 10.  );
    TH1D* hEtaObar  = new TH1D("hEtaObar"  , "#bar{#Omega^{+}} Pseudorapidity"  , 500 , -10., 10.  );
    TH1D* hyO       = new TH1D("hyO"       , "#Omega^{-} Rapidity"              , 500 , -10., 10.  );
    TH1D* hyObar    = new TH1D("hyObar"    , "#bar{#Omega^{+}} Rapidity"        , 500 , -10., 10.  );
    TH1D* hPtO      = new TH1D("hPtO"      , "#Omega^{-} p_T"                   , 500 ,   0., 15.  );
    TH1D* hPtObar   = new TH1D("hPtObar"   , "#bar{#Omega^{+}} p_T"             , 500 ,   0., 15.  );
    TH1D* hEtaKplus = new TH1D("hEtaKplus" , "K^{+} Pseudorapidity"             , 500 , -10., 10.  );
    TH1D* hEtaKminus= new TH1D("hEtaKminus", "K^{-} Pseudorapidity"             , 500 , -10., 10.  );
    TH1D* hPtKplus  = new TH1D("hPtKplus"  , "K^{+} p_T"                        , 500 ,   0., 15.  );
    TH1D* hPtKminus = new TH1D("hPtKminus" , "K^{-} p_T"                        , 500 ,   0., 15.  );
    TH1D* hEtaPplus = new TH1D("hEtaPplus" , "p Pseudorapidity"                 , 500 , -10., 10.  );
    TH1D* hEtaPminus= new TH1D("hEtaPminus", "#bar{p} Pseudorapidity"           , 500 , -10., 10.  );
    TH1D* hyPplus   = new TH1D("hyPplus"   , "p Rapidity"                       , 500 , -10., 10.  );
    TH1D* hyPminus  = new TH1D("hyPminus"  , "#bar{p} Rapidity"                 , 500 , -10., 10.  );
    TH1D* hEtaLambda   = new TH1D("hEtaLambda"   , "#Lambda Pseudorapidity"       , 500 , -10., 10.  );
    TH1D* hEtaLambdabar= new TH1D("hEtaLambdabar", "#bar{#Lambda} Pseudorapidity" , 500 , -10., 10.  );
    TH1D* hyLambda     = new TH1D("hyLambda"   , "#Lambda Rapidity"               , 500 , -10., 10.  );
    TH1D* hyLambdabar  = new TH1D("hyLambdabar", "#bar{#Lambda} Rapidity"         , 500 , -10., 10.  );
    TH1D* hCOM = new TH1D("hCOM", "Momentum Sum in COM (should be 0)", 100, -5., 5.);
    TH1D* hCen = new TH1D("hCen", "Centrality", 11, -1.5, 9.5);
    TProfile* hKaonCt = new TProfile("hKaonCt", "Kaon Count for #Omega^{-} and #bar{#Omega^{+}} events", 2, -0.5, 1.5); // to check average kaon+ counts per events. Omega events should have more
    TH1D* hSingleEvtCt = new TH1D("hSingleEvtCt", "Count Single Hyperon Event (0 is total hyperon event)", 2, -0.5, 1.5);
    TH1D* hOmegaUsed = new TH1D("hOmegaUsed", "Actual #Omega^{-}/#bar{#Omega^{+}} used (1-2 orig 3-4 mixed)", 4, 0.5, 4.5); // for mixed event normalization
    TH1D* hOmegaXi0Used = new TH1D("hOmegaXi0Used", 
                                   "1-#Omega^{-}#bar{#Xi^{0}},2-#Omega^{-},3-#bar{#Omega^{+}}#Xi^{0},4-#bar{#Omega^{+}}", 4, 0.5, 4.5); 
                                   // for Xi0 coincident correlation norm
    TH1D* hKplusEtaDist_O     = new TH1D("hKplusEtaDist_O"    , "Kaon+ Eta Dist for Qualified Omega"     , 500, -10., 10.);
    TH1D* hKplusEtaDist_Obar  = new TH1D("hKplusEtaDist_Obar" , "Kaon+ Eta Dist for Qualified Anti-Omega", 500, -10., 10.);
    TH1D* hKminusEtaDist_O    = new TH1D("hKminusEtaDist_O"   , "Kaon- Eta Dist for Qualified Omega"     , 500, -10., 10.);
    TH1D* hKminusEtaDist_Obar = new TH1D("hKminusEtaDist_Obar", "Kaon- Eta Dist for Qualified Anti-Omega", 500, -10., 10.);

    // Result hist
    TH1D* hEvtCt = new TH1D("hEvtCt", "Omega/Anti-Omega event ct", 2, -0.5, 1.5); // for normalization
    /* K* */
    TH1D *hCorrKplusO[2], *hCorrKplusObar[2], *hCorrKminusO[2], *hCorrKminusObar[2];
    hCorrKplusO[0]     = new TH1D("hCorrKplusO"    , "K^{+}-#Omega^{-} Correlation"      , 5000, 0.0, 50.0);
    hCorrKplusObar[0]  = new TH1D("hCorrKplusObar" , "K^{+}-#bar{#Omega^{+}} Correlation", 5000, 0.0, 50.0);
    hCorrKminusO[0]    = new TH1D("hCorrKminusO"   , "K^{-}-#Omega^{-} Correlation"      , 5000, 0.0, 50.0);
    hCorrKminusObar[0] = new TH1D("hCorrKminusObar", "K^{-}-#bar{#Omega^{+}} Correlation", 5000, 0.0, 50.0);
    hCorrKplusO[1]     = new TH1D("hCorrKplusO_mix"    , "K^{+}-#Omega^{-} Correlation"      , 5000, 0.0, 50.0);
    hCorrKplusObar[1]  = new TH1D("hCorrKplusObar_mix" , "K^{+}-#bar{#Omega^{+}} Correlation", 5000, 0.0, 50.0);
    hCorrKminusO[1]    = new TH1D("hCorrKminusO_mix"   , "K^{-}-#Omega^{-} Correlation"      , 5000, 0.0, 50.0);
    hCorrKminusObar[1] = new TH1D("hCorrKminusObar_mix", "K^{-}-#bar{#Omega^{+}} Correlation", 5000, 0.0, 50.0);
    /* pT */
    TH1D *hPtCorrKplusO[2], *hPtCorrKplusObar[2], *hPtCorrKminusO[2], *hPtCorrKminusObar[2];
    hPtCorrKplusO[0]     = new TH1D("hPtCorrKplusO"    , "K^{+}-#Omega^{-} p_T Correlation"      , 5000, 0.0, 50.0);
    hPtCorrKplusObar[0]  = new TH1D("hPtCorrKplusObar" , "K^{+}-#bar{#Omega^{+}} p_T Correlation", 5000, 0.0, 50.0);
    hPtCorrKminusO[0]    = new TH1D("hPtCorrKminusO"   , "K^{-}-#Omega^{-} p_T Correlation"      , 5000, 0.0, 50.0);
    hPtCorrKminusObar[0] = new TH1D("hPtCorrKminusObar", "K^{-}-#bar{#Omega^{+}} p_T Correlation", 5000, 0.0, 50.0);
    hPtCorrKplusO[1]     = new TH1D("hPtCorrKplusO_mix"    , "K^{+}-#Omega^{-} p_T Correlation"      , 5000, 0.0, 50.0);
    hPtCorrKplusObar[1]  = new TH1D("hPtCorrKplusObar_mix" , "K^{+}-#bar{#Omega^{+}} p_T Correlation", 5000, 0.0, 50.0);
    hPtCorrKminusO[1]    = new TH1D("hPtCorrKminusO_mix"   , "K^{-}-#Omega^{-} p_T Correlation"      , 5000, 0.0, 50.0);
    hPtCorrKminusObar[1] = new TH1D("hPtCorrKminusObar_mix", "K^{-}-#bar{#Omega^{+}} p_T Correlation", 5000, 0.0, 50.0);
    /* y */
    TH1D *hyCorrKplusO[2], *hyCorrKplusObar[2], *hyCorrKminusO[2], *hyCorrKminusObar[2];
    hyCorrKplusO[0]     = new TH1D("hyCorrKplusO"    , "K^{+}-#Omega^{-} y Correlation"      , 2000, -10.0, 10.0);
    hyCorrKplusObar[0]  = new TH1D("hyCorrKplusObar" , "K^{+}-#bar{#Omega^{+}} y Correlation", 2000, -10.0, 10.0);
    hyCorrKminusO[0]    = new TH1D("hyCorrKminusO"   , "K^{-}-#Omega^{-} y Correlation"      , 2000, -10.0, 10.0);
    hyCorrKminusObar[0] = new TH1D("hyCorrKminusObar", "K^{-}-#bar{#Omega^{+}} y Correlation", 2000, -10.0, 10.0);
    hyCorrKplusO[1]     = new TH1D("hyCorrKplusO_mix"    , "K^{+}-#Omega^{-} y Correlation"      , 2000, -10.0, 10.0);
    hyCorrKplusObar[1]  = new TH1D("hyCorrKplusObar_mix" , "K^{+}-#bar{#Omega^{+}} y Correlation", 2000, -10.0, 10.0);
    hyCorrKminusO[1]    = new TH1D("hyCorrKminusO_mix"   , "K^{-}-#Omega^{-} y Correlation"      , 2000, -10.0, 10.0);
    hyCorrKminusObar[1] = new TH1D("hyCorrKminusObar_mix", "K^{-}-#bar{#Omega^{+}} y Correlation", 2000, -10.0, 10.0);
    /* phi */
    TH1D *hphiCorrKplusO[2], *hphiCorrKplusObar[2], *hphiCorrKminusO[2], *hphiCorrKminusObar[2];
    hphiCorrKplusO[0]     = new TH1D("hphiCorrKplusO"    , "K^{+}-#Omega^{-} phi Correlation"      , 500, 0.0, PI);
    hphiCorrKplusObar[0]  = new TH1D("hphiCorrKplusObar" , "K^{+}-#bar{#Omega^{+}} phi Correlation", 500, 0.0, PI);
    hphiCorrKminusO[0]    = new TH1D("hphiCorrKminusO"   , "K^{-}-#Omega^{-} phi Correlation"      , 500, 0.0, PI);
    hphiCorrKminusObar[0] = new TH1D("hphiCorrKminusObar", "K^{-}-#bar{#Omega^{+}} phi Correlation", 500, 0.0, PI);
    hphiCorrKplusO[1]     = new TH1D("hphiCorrKplusO_mix"    , "K^{+}-#Omega^{-} phi Correlation"      , 500, 0.0, PI);
    hphiCorrKplusObar[1]  = new TH1D("hphiCorrKplusObar_mix" , "K^{+}-#bar{#Omega^{+}} phi Correlation", 500, 0.0, PI);
    hphiCorrKminusO[1]    = new TH1D("hphiCorrKminusO_mix"   , "K^{-}-#Omega^{-} phi Correlation"      , 500, 0.0, PI);
    hphiCorrKminusObar[1] = new TH1D("hphiCorrKminusObar_mix", "K^{-}-#bar{#Omega^{+}} phi Correlation", 500, 0.0, PI);
    /* theta */
    TH1D* hthetaCorrKplusO     = new TH1D("hthetaCorrKplusO"    , "K^{+}-#Omega^{-} theta Correlation"      , 500, 0.0, PI);
    TH1D* hthetaCorrKplusObar  = new TH1D("hthetaCorrKplusObar" , "K^{+}-#bar{#Omega^{+}} theta Correlation", 500, 0.0, PI);
    TH1D* hthetaCorrKminusO    = new TH1D("hthetaCorrKminusO"   , "K^{-}-#Omega^{-} theta Correlation"      , 500, 0.0, PI);
    TH1D* hthetaCorrKminusObar = new TH1D("hthetaCorrKminusObar", "K^{-}-#bar{#Omega^{+}} theta Correlation", 500, 0.0, PI);
    /* xi0 coincident test */
    TH1D* hKplusO_wXi0      = new TH1D("hKplusO_wXi0"  , "K^{+}-#Omega^{-} k* Correlation with Xi0"   , 5000, 0.0, 50.0);
    TH1D* hKplusO_woXi0     = new TH1D("hKplusO_woXi0" , "K^{+}-#Omega^{-} k* Correlation without Xi0", 5000, 0.0, 50.0);
    TH1D* hKminusObar_wXi0  = new TH1D("hKminusObar_wXi0" , "K^{-}-#bar{#Omega^{+}} k* Correlation with Xi0"   , 5000, 0.0, 50.0);
    TH1D* hKminusObar_woXi0 = new TH1D("hKminusObar_woXi0", "K^{-}-#bar{#Omega^{+}} k* Correlation without Xi0", 5000, 0.0, 50.0);
    TH1D* hPtKplusO_wXi0      = new TH1D("hPtKplusO_wXi0"  , "K^{+}-#Omega^{-} p_T Correlation with Xi0"   , 5000, 0.0, 50.0);
    TH1D* hPtKplusO_woXi0     = new TH1D("hPtKplusO_woXi0" , "K^{+}-#Omega^{-} p_T Correlation without Xi0", 5000, 0.0, 50.0);
    TH1D* hPtKminusObar_wXi0  = new TH1D("hPtKminusObar_wXi0" , "K^{-}-#bar{#Omega^{+}} p_T Correlation with Xi0"   , 5000, 0.0, 50.0);
    TH1D* hPtKminusObar_woXi0 = new TH1D("hPtKminusObar_woXi0", "K^{-}-#bar{#Omega^{+}} p_T Correlation without Xi0", 5000, 0.0, 50.0);
    TH1D* hyKplusO_wXi0      = new TH1D("hyKplusO_wXi0"  , "K^{+}-#Omega^{-} y Correlation with Xi0"   , 2000, -10.0, 10.0);
    TH1D* hyKplusO_woXi0     = new TH1D("hyKplusO_woXi0" , "K^{+}-#Omega^{-} y Correlation without Xi0", 2000, -10.0, 10.0);
    TH1D* hyKminusObar_wXi0  = new TH1D("hyKminusObar_wXi0" , "K^{-}-#bar{#Omega^{+}} y Correlation with Xi0"   , 2000, -10.0, 10.0);
    TH1D* hyKminusObar_woXi0 = new TH1D("hyKminusObar_woXi0", "K^{-}-#bar{#Omega^{+}} y Correlation without Xi0", 2000, -10.0, 10.0);
    TH1D* hphiKplusO_wXi0      = new TH1D("hphiKplusO_wXi0"  , "K^{+}-#Omega^{-} phi Correlation with Xi0"   , 500, 0.0, PI);
    TH1D* hphiKplusO_woXi0     = new TH1D("hphiKplusO_woXi0" , "K^{+}-#Omega^{-} phi Correlation without Xi0", 500, 0.0, PI);
    TH1D* hphiKminusObar_wXi0  = new TH1D("hphiKminusObar_wXi0" , "K^{-}-#bar{#Omega^{+}} phi Correlation with Xi0"   , 500, 0.0, PI);
    TH1D* hphiKminusObar_woXi0 = new TH1D("hphiKminusObar_woXi0", "K^{-}-#bar{#Omega^{+}} phi Correlation without Xi0", 500, 0.0, PI);

    // setting PID and momentum branches
    float imp;
    int refmult;
    std::vector<int>   *pid_vec = nullptr;
    std::vector<float> *px_vec  = nullptr;
    std::vector<float> *py_vec  = nullptr;
    std::vector<float> *pz_vec  = nullptr;
    chain->SetBranchAddress("imp", &imp);
    chain->SetBranchAddress("refmult", &refmult);
    chain->SetBranchAddress("pid", &pid_vec);
    chain->SetBranchAddress("px",  &px_vec);
    chain->SetBranchAddress("py",  &py_vec);
    chain->SetBranchAddress("pz",  &pz_vec);

    // variables
    float px, py, pz, p, pt, theta, eta, y;
    int pid, mult;
    TLorentzVector lv;

    // mixed-event buffer
    MixedBuffer buffer(buffer_size);
    buffer.Init();

    // loop
    int nentries = chain->GetEntries();
    int P1PID = XimPID, P2PID = OmegaPID, P3PID = Xi0PID;
    float P1Mass = mXim, P2Mass = mOmega;
    bool hasP2 = false, hasAntiP2 = false;
    bool hasP3 = false, hasAntiP3 = false;
    int P2evt_ct = 0, AntiP2evt_ct = 0;
    int P3_used[2] = {0}, AntiP3_used[2] = {0};
    int evt_ct_all = 0, evt_ct_single = 0;
    int omega_ct[2] = {0}; int omegab_ct[2] = {0};

    // event loop
    for (int i = 0; i < nentries; ++i)
    {
        if((i+1)%1000==0) cout<<"Processing entry == "<< i+1 <<" == out of "<<nentries<<".\n";
        chain->GetEntry(i);

        // initialize
        mult = 0;
        hasP2 = false; hasAntiP2 = false;
        hasP3 = false; hasAntiP3 = false;
        my_event current_evt;
        CenMaker cenmaker;
        int cen = cenmaker.cent9(refmult, energy, mode);   

        // fill track vectors and QA plots
        int ntrack = pid_vec->size();
        assert(ntrack == px_vec->size() && ntrack == py_vec->size() && ntrack == pz_vec->size() && "Ntrack size mismatch!");
        int kaonct = 0;
        vector<float> px1_vec, py1_vec, pz1_vec, px2_vec, py2_vec, pz2_vec;
        vector<int> pid1_vec, pid2_vec; 
        for (int i = 0; i < ntrack; ++i)
        {
            pid   = pid_vec->at(i);
            px    = px_vec->at(i);
            py    = py_vec->at(i);
            pz    = pz_vec->at(i);
            p     = sqrt(px*px + py*py + pz*pz);
            pt    = sqrt(px*px + py*py);
            theta = atan2(pt,pz);
            eta   = -log(tan(theta/2.));
            y     = -999;
            p_info= db->GetParticle((int)pid);
            if (!p_info) continue;
            lv.SetXYZM(px, py, pz, p_info->Mass());
            y = lv.Rapidity();
                        
            // fill relevant hists
            hP->Fill(p);
            hPt->Fill(pt);
            hEta->Fill(eta);
            if (y > -999) hy->Fill(y);
            hPtEta->Fill(pt,eta);
            if (pid ==  OmegaPID) {hEtaO   ->Fill(eta); if (y > -999) hyO   ->Fill(y); hPtO   ->Fill(pt);}
            if (pid == -OmegaPID) {hEtaObar->Fill(eta); if (y > -999) hyObar->Fill(y); hPtObar->Fill(pt);}
            if (pid ==  KaonPID)  {hEtaKplus ->Fill(eta); hPtKplus ->Fill(pt);}
            if (pid == -KaonPID)  {hEtaKminus->Fill(eta); hPtKminus->Fill(pt);}
            if (pid ==  ProtonPID){hEtaPplus ->Fill(eta); if (y > -999)hyPplus ->Fill(y);}
            if (pid == -ProtonPID){hEtaPminus->Fill(eta); if (y > -999)hyPminus->Fill(y);}
            if (pid ==  LambdaPID){hEtaLambda   ->Fill(eta); if (y > -999)hyLambda   ->Fill(y);}
            if (pid == -LambdaPID){hEtaLambdabar->Fill(eta); if (y > -999)hyLambdabar->Fill(y);}
            mult++;
            if (pid == KaonPID) kaonct++;

            // track cut
            if (CutEta) {if (fabs(eta) > eta_cut) continue;}
            if (Cuty)   {if (fabs(y)   > y_cut  ) continue;}

            // find particles
            if (!hasP2    ) {if (pid ==  P2PID) hasP2     = true;}
            if (!hasAntiP2) {if (pid == -P2PID) hasAntiP2 = true;}
            if (!hasP3    ) {if (pid ==  P3PID) hasP3     = true;}
            if (!hasAntiP3) {if (pid == -P3PID) hasAntiP3 = true;}

            // fill the vectors
            if (fabs(pid) == P1PID) 
            {
                px1_vec.push_back(px);
                py1_vec.push_back(py);
                pz1_vec.push_back(pz);
                pid1_vec.push_back(pid);
            }
            if (fabs(pid) == P2PID)
            {
                px2_vec.push_back(px);
                py2_vec.push_back(py);
                pz2_vec.push_back(pz);
                pid2_vec.push_back(pid);
            }
            
            // fill particle for mixed-event
            if (fabs(pid) == P2PID) 
            {
                my_particle current_h(px, py, pz, pid);
                current_evt.push_back(current_h);
            }
        }

        cout << "Got here" << endl;
        // select events with only one hyperon
        if (hasP2)               P2evt_ct++;
        if (hasAntiP2)           AntiP2evt_ct++;
        if (px2_vec.size() == 1) evt_ct_single++;
        if (px2_vec.size() >  0) evt_ct_all++;

        // filling event info
        hMult->Fill(mult);
        hRefMult->Fill(refmult); 
        hImpPar->Fill(imp);
        hCen->Fill(cen*1.0);
        if (hasP2)     hKaonCt->Fill(0., kaonct*1.0);
        if (hasAntiP2) hKaonCt->Fill(1., kaonct*1.0);

        // event cut
        if (px2_vec.size() != 1) continue; // get rid of this if not 14.6 GeV
        if (cen != cen_select) continue;

        // double loop for cf
        /****** normal cf ******/
        for (int j = 0; j < px2_vec.size(); ++j) // this can be viewed as an event loop since only one omega per event
        {
            int pid2 = pid2_vec[j];
            float px2 = px2_vec[j];
            float py2 = py2_vec[j];
            float pz2 = pz2_vec[j];
            float pt2 = sqrt(px2*px2 + py2*py2);
            float theta2 = atan2(pt2, pz2);
            float phi2 = atan2(py2, px2);
            float eta2 = -log(tan(theta2/2.));
            lv.SetXYZM(px2, py2, pz2, P2Mass);
            float y2 = lv.Rapidity();
            
            // for normalization
            if (pid2 ==  P2PID) 
            {
                omega_ct[0]++;
                if (hasAntiP3) AntiP3_used[0]++;
                else AntiP3_used[1]++;
            }
            if (pid2 == -P2PID) 
            {
                omegab_ct[0]++;
                if (hasP3) P3_used[0]++;
                else P3_used[1]++;
            }

            for (int i = 0; i < px1_vec.size(); ++i)
            {   
                int  pid1 = pid1_vec[i];
                float px1 = px1_vec[i];
                float py1 = py1_vec[i];
                float pz1 = pz1_vec[i];
                float pt1 = sqrt(px1*px1 + py1*py1);
                float theta1 = atan2(pt1, pz1);
                float phi1 = atan2(py1, px1);
                float eta1 = -log(tan(theta1/2.));
                lv.SetXYZM(px1, py1, pz1, P1Mass);
                float y1 = lv.Rapidity();

                // test eta dist
                if (pid2 ==  P2PID && pid1 ==  P1PID && fabs(eta2) < eta_cut) hKplusEtaDist_O    ->Fill(eta1);
                if (pid2 == -P2PID && pid1 ==  P1PID && fabs(eta2) < eta_cut) hKplusEtaDist_Obar ->Fill(eta1);
                if (pid2 ==  P2PID && pid1 == -P1PID && fabs(eta2) < eta_cut) hKminusEtaDist_O   ->Fill(eta1);
                if (pid2 == -P2PID && pid1 == -P1PID && fabs(eta2) < eta_cut) hKminusEtaDist_Obar->Fill(eta1);
                
                // boost to COM frame
                TLorentzVector p1; p1.SetXYZM(px1, py1, pz1, P1Mass);
                TLorentzVector p2; p2.SetXYZM(px2, py2, pz2, P2Mass);
                TLorentzVector P = p1 + p2;
                TVector3 beta = P.BoostVector();
                p1.Boost((-1)*beta);
                p2.Boost((-1)*beta);
                hCOM->Fill((p1+p2).Vect().Mag());

                // fill quantity
                float phi_diff = fabs(phi1-phi2);
                if (phi_diff   > PI) phi_diff   = 2*PI - phi_diff;
                float kstar = 0.5*(p1-p2).Vect().Mag();

                // fill cf
                if (pid1 ==  P1PID && pid2 == -P2PID) 
                {
                    hCorrKplusObar[0]   ->Fill(kstar);
                    hPtCorrKplusObar[0] ->Fill(fabs(pt1-pt2));
                    hyCorrKplusObar[0]  ->Fill(y1-y2);
                    hphiCorrKplusObar[0]->Fill(phi_diff);
                    hthetaCorrKplusObar ->Fill(fabs(theta1-theta2));
                }
                if (pid1 ==  P1PID && pid2 ==  P2PID) 
                {
                    hCorrKplusO[0]   ->Fill(kstar);
                    if (hasAntiP3) 
                    {
                        hKplusO_wXi0->Fill(kstar);
                        hPtKplusO_wXi0->Fill(fabs(pt1-pt2));
                        hyKplusO_wXi0->Fill(y1-y2);
                        hphiKplusO_wXi0->Fill(phi_diff);
                    }
                    else 
                    {
                        hKplusO_woXi0->Fill(kstar);
                        hPtKplusO_woXi0->Fill(fabs(pt1-pt2));
                        hyKplusO_woXi0->Fill(y1-y2);
                        hphiKplusO_woXi0->Fill(phi_diff);
                    }
                    hPtCorrKplusO[0] ->Fill(fabs(pt1-pt2));
                    hyCorrKplusO[0]  ->Fill(y1-y2);
                    hphiCorrKplusO[0]->Fill(phi_diff);
                    hthetaCorrKplusO ->Fill(fabs(theta1-theta2));
                }
                if (pid1 == -P1PID && pid2 == -P2PID) 
                {
                    hCorrKminusObar[0]   ->Fill(kstar);
                    if (hasP3) 
                    {
                        hKminusObar_wXi0->Fill(kstar);
                        hPtKminusObar_wXi0->Fill(fabs(pt1-pt2));
                        hyKminusObar_wXi0->Fill(y1-y2);
                        hphiKminusObar_wXi0->Fill(phi_diff);
                    }
                    else 
                    {
                        hKminusObar_woXi0->Fill(kstar);
                        hPtKminusObar_woXi0->Fill(fabs(pt1-pt2));
                        hyKminusObar_woXi0->Fill(y1-y2);
                        hphiKminusObar_woXi0->Fill(phi_diff);
                    }
                    hPtCorrKminusObar[0] ->Fill(fabs(pt1-pt2));
                    hyCorrKminusObar[0]  ->Fill(y1-y2);
                    hphiCorrKminusObar[0]->Fill(phi_diff);
                    hthetaCorrKminusObar ->Fill(fabs(theta1-theta2));
                }
                if (pid1 == -P1PID && pid2 ==  P2PID) 
                {
                    hCorrKminusO[0]   ->Fill(kstar);
                    hPtCorrKminusO[0] ->Fill(fabs(pt1-pt2));
                    hyCorrKminusO[0]  ->Fill(y1-y2);
                    hphiCorrKminusO[0]->Fill(phi_diff);
                    hthetaCorrKminusO ->Fill(fabs(theta1-theta2));
                }
            }
        }

        /****** mixed cf ******/
        if (!buffer.IsEmpty(cen))
        {   
            std::vector<my_event> mixed_buffer = buffer.Sample_All(cen);
            for (int evt = 0; evt < mixed_buffer.size(); ++evt)
            {
                std::vector<my_particle> mixed_vec = mixed_buffer[evt].GetParticles();
                for (int j = 0; j < mixed_vec.size(); ++j)
                {
                    int pid2 = mixed_vec[j].GetPID();
                    TVector3 mom2 = mixed_vec[j].GetMomentum();
                    float px2 = mom2.X();
                    float py2 = mom2.Y();
                    float pz2 = mom2.Z();
                    float pt2 = sqrt(px2*px2 + py2*py2);
                    float theta2 = atan2(pt2, pz2);
                    float phi2 = atan2(py2, px2);
                    float eta2 = -log(tan(theta2/2.));
                    lv.SetXYZM(px2, py2, pz2, P2Mass);
                    float y2 = lv.Rapidity();

                    // for normalization
                    if (pid2 ==  P2PID) omega_ct[1]++;
                    if (pid2 == -P2PID) omegab_ct[1]++;

                    for (int i = 0; i < px1_vec.size(); ++i)
                    {   
                        int  pid1 = pid1_vec[i];
                        float px1 = px1_vec[i];
                        float py1 = py1_vec[i];
                        float pz1 = pz1_vec[i];
                        float pt1 = sqrt(px1*px1 + py1*py1);
                        float theta1 = atan2(pt1, pz1);
                        float phi1 = atan2(py1, px1);
                        float eta1 = -log(tan(theta1/2.));
                        lv.SetXYZM(px1, py1, pz1, P1Mass);
                        float y1 = lv.Rapidity();

                        // boost to COM frame
                        TLorentzVector p1; p1.SetXYZM(px1, py1, pz1, P1Mass);
                        TLorentzVector p2; p2.SetXYZM(px2, py2, pz2, P2Mass);
                        TLorentzVector P = p1 + p2;
                        TVector3 beta = P.BoostVector();
                        p1.Boost((-1)*beta);
                        p2.Boost((-1)*beta);
                        hCOM->Fill((p1+p2).Vect().Mag());

                        // angle folding
                        float phi_diff = fabs(phi1-phi2);
                        if (phi_diff   > PI) phi_diff   = 2*PI - phi_diff;

                        // fill cf
                        if (pid1 ==  P1PID && pid2 == -P2PID)
                        {
                            hCorrKplusObar[1]   ->Fill(0.5*(p1-p2).Vect().Mag());
                            hPtCorrKplusObar[1] ->Fill(fabs(pt1-pt2));
                            hyCorrKplusObar[1]  ->Fill(y1-y2);
                            hphiCorrKplusObar[1]->Fill(phi_diff);
                        }
                        if (pid1 ==  P1PID && pid2 ==  P2PID) 
                        {
                            hCorrKplusO[1]   ->Fill(0.5*(p1-p2).Vect().Mag());
                            hPtCorrKplusO[1] ->Fill(fabs(pt1-pt2));
                            hyCorrKplusO[1]  ->Fill(y1-y2);
                            hphiCorrKplusO[1]->Fill(phi_diff);
                        }
                        if (pid1 == -P1PID && pid2 == -P2PID)
                        {
                            hCorrKminusObar[1]   ->Fill(0.5*(p1-p2).Vect().Mag());
                            hPtCorrKminusObar[1] ->Fill(fabs(pt1-pt2));
                            hyCorrKminusObar[1]  ->Fill(y1-y2);
                            hphiCorrKminusObar[1]->Fill(phi_diff); 
                        } 
                        if (pid1 == -P1PID && pid2 ==  P2PID) 
                        {
                            hCorrKminusO[1]   ->Fill(0.5*(p1-p2).Vect().Mag());
                            hPtCorrKminusO[1] ->Fill(fabs(pt1-pt2));
                            hyCorrKminusO[1]  ->Fill(y1-y2);
                            hphiCorrKminusO[1]->Fill(phi_diff);
                        }
                    }
                }
            }
        }

        // fill mixed-event buffer
        buffer.Add_FIFO(current_evt, cen);
    }

    hEvtCt->Fill(0.,P2evt_ct*1.0); // Note these did not go thru the event cut!
    hEvtCt->Fill(1.,AntiP2evt_ct*1.0); // Note these did not go thru the event cut!
    hSingleEvtCt->Fill(0., evt_ct_all*1.0);
    hSingleEvtCt->Fill(1., evt_ct_single*1.0);

    // omega used for cf and mixed-event
    hOmegaUsed->Fill(1,omega_ct[0]);
    hOmegaUsed->Fill(2,omegab_ct[0]);
    hOmegaUsed->Fill(3,omega_ct[1]);
    hOmegaUsed->Fill(4,omegab_ct[1]);

    // xi0 event
    hOmegaXi0Used->Fill(1,AntiP3_used[0]);
    hOmegaXi0Used->Fill(2,AntiP3_used[1]);
    hOmegaXi0Used->Fill(3,P3_used[0]);
    hOmegaXi0Used->Fill(4,P3_used[1]);
  
    fout.Write();
}
