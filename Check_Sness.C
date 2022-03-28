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
#include "TDatabasePDG.h"
#include "TParticlePDG.h"
#include <string>

using namespace std;

// constants
const float eta_cut    = 1.;
const int NClass       = 6; //number of particle classes to count    
const int NEventClass  = 6; //number of special events to select
const int NKaonClass   = 5;
const int NLambdaClass = 3;
const int NSigmaClass  = 7;
const int NXiClass     = 5;
const int NOmegaClass  = 3;

int FindStrangeType(int PID);

void Check_Sness(const Char_t *inFile = "placeholder.list", const TString JobID = "1234")
{   
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

    // 0 - total, 1 - Anti-O, 2 - O, 3 - Anti-X, 4 - X, 5 - Anti-X0, 6 - X0
    TH1D *hsdist [NEventClass+1], *hpdist[NEventClass+1], *hKaon[NEventClass+1], 
         *hLambda[NEventClass+1], *hSigma[NEventClass+1], *hXi  [NEventClass+1], *hOmega[NEventClass+1];
    char fname[7][200]; 
    for (int i = 0; i < NEventClass+1; i++)
    {
        sprintf(fname[0], "hsdist_%d" , i);
        sprintf(fname[1], "hpdist_%d" , i);
        sprintf(fname[2], "hKaon_%d"     , i);
        sprintf(fname[3], "hLambda_%d", i);
        sprintf(fname[4], "hSigma_%d" , i);
        sprintf(fname[5], "hXi_%d"    , i);
        sprintf(fname[6], "hOmega_%d" , i);
        hsdist[i]  = new TH1D(fname[0], fname[0], NClass      , 0.5, 0.5 + NClass      );
        hpdist[i]  = new TH1D(fname[1], fname[1], NClass      , 0.5, 0.5 + NClass      );
        hKaon[i]   = new TH1D(fname[2], fname[2], NKaonClass  , 0.5, 0.5 + NKaonClass  );
        hLambda[i] = new TH1D(fname[3], fname[3], NLambdaClass, 0.5, 0.5 + NLambdaClass);
        hSigma[i]  = new TH1D(fname[4], fname[4], NSigmaClass , 0.5, 0.5 + NSigmaClass );
        hXi[i]     = new TH1D(fname[5], fname[5], NXiClass    , 0.5, 0.5 + NXiClass    );
        hOmega[i]  = new TH1D(fname[6], fname[6], NOmegaClass , 0.5, 0.5 + NOmegaClass );
    
    }

    // setting PID and momentum branches
    TBranch* bpid = nullptr;
    std::vector<int>   *pid_vec = nullptr;
    std::vector<float> *px_vec  = nullptr;
    std::vector<float> *py_vec  = nullptr;
    std::vector<float> *pz_vec  = nullptr;
    chain->SetBranchAddress("pid", &pid_vec, &bpid);
    chain->SetBranchAddress("px",  &px_vec);
    chain->SetBranchAddress("py",  &py_vec);
    chain->SetBranchAddress("pz",  &pz_vec);

    // loops
    //std::set<int> s_pid_set;
    int nentries = chain->GetEntries();
    bool hasParticle[NEventClass+1]; 
    int M[NEventClass+1] = {0}; //number of events that contain anti_o, o, anti_x, x, anti_x0, x0;

    // event loop
    for (int i = 0; i < nentries; ++i)
    {
        if((i+1)%1000==0) cout<<"Processing entry == "<< i+1 <<" == out of "<<nentries<<".\n";
        chain->GetEntry(i);

        int total_s = 0, itrack = 0;

        // check if event has certain particles
        for (int j = 0; j < NEventClass+1; j++) hasParticle[j] = false;

        // fill examples and check anti-omega
        for (std::vector<int>::iterator it = pid_vec->begin(); it != pid_vec->end(); ++it)
        {
            int s = sness_map[(int)(*it)];

            // check for particle
            if (*it == -3334) hasParticle[1] = true; if (*it == 3334) hasParticle[2] = true; 
            if (*it == -3312) hasParticle[3] = true; if (*it == 3312) hasParticle[4] = true; 
            if (*it == -3322) hasParticle[5] = true; if (*it == 3322) hasParticle[6] = true; 

            // omega ratio
            if (*it ==  3334) hOmegaRatio->Fill(0);
            if (*it == -3334) hOmegaRatio->Fill(1);
            
            // total strangeness
            total_s += s;
        }

        // count events
        hasParticle[0] = true; //total count
        for (int j = 0; j < NEventClass+1; j++) {if (hasParticle[j]) M[j]++;}

        // loop thru events
        int ntrack = pid_vec->size();
        assert(ntrack == px_vec->size() && ntrack == py_vec->size() && ntrack == pz_vec->size() && "Ntrack size mismatch!");
        for (int i = 0; i < ntrack; ++i)
        {
            int   pid   = pid_vec->at(i);
            int   s     = sness_map[pid];
            int   type  = FindStrangeType(pid);
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
            }                   
        }

        hstrangeness->Fill(total_s);    

    }

    for (int i = 0; i < NEventClass+1; i++) hMult_each->Fill(i, M[i]); //i = 0: total event count

    fout.Write();
}

int FindStrangeType(int PID)
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
    
    return -1; //not strange
}