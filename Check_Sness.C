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
const int NClass       = 7; //number of particle classes to count    
const int NEventClass  = 2; //number of special events to select
const int NKaonClass   = 5;
const int NLambdaClass = 3;
const int NSigmaClass  = 7;
const int NXiClass     = 5;
const int NOmegaClass  = 3;
const int NNucleonClass= 4;

int FindType(int PID);

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

    TH1D* hbaryon            = new TH1D("hbaryon", "total baryon number", 400, -199.5, 600.5);
    TProfile* hbndist_wo     = new TProfile("hbndist_wo", "Baryon number distribution for with omega events", 6, -0.5, 5.5, -200, 600);
    TProfile* hbndist_woo    = new TProfile("hbndist_woo", "Baryon number distribution for without omega events", 6, -0.5, 5.5, -200, 600);
    TProfile* hbndist_wob    = new TProfile("hbndist_wo", "Baryon number distribution for with omegabar events", 6, -0.5, 5.5, -200, 600);
    TProfile* hbndist_woob   = new TProfile("hbndist_woo", "Baryon number distribution for without omegabar events", 6, -0.5, 5.5, -200, 600);
    TProfile* hbndist_owxb   = new TProfile("hbndist_owxb", "Baryon number distribution for omega events with xibar", 6, -0.5, 5.5, -200, 600);
    TProfile* hbndist_owoxb  = new TProfile("hbndist_owoxb", "Baryon number distribution for omega events without xibar", 6, -0.5, 5.5, -200, 600);
    TProfile* hbndist_owx    = new TProfile("hbndist_owx", "Baryon number distribution for omega events with xi", 6, -0.5, 5.5, -200, 600);
    TProfile* hbndist_owox   = new TProfile("hbndist_owox", "Baryon number distribution for omega events without xi", 6, -0.5, 5.5, -200, 600);
    TProfile* hbndist_obwx   = new TProfile("hbndist_obwx", "Baryon number distribution for omegabar events with xi", 6, -0.5, 5.5, -200, 600);
    TProfile* hbndist_obwox  = new TProfile("hbndist_obwox", "Baryon number distribution for omegabar events without xi", 6, -0.5, 5.5, -200, 600);
    TProfile* hbndist_obwxb  = new TProfile("hbndist_obwxb", "Baryon number distribution for omegabar events with xibar", 6, -0.5, 5.5, -200, 600);
    TProfile* hbndist_obwoxb = new TProfile("hbndist_obwoxb", "Baryon number distribution for omegabar events without xibar", 6, -0.5, 5.5, -200, 600);
    
    TProfile* hkaonct_owx   = new TProfile("hkaonct_owx"  , "Kaon count for omega event with xi and without",2, -0.5, 1.5, 0, 100);
    TProfile* hkaonct_owxb  = new TProfile("hkaonct_owxb" , "Kaon count for omega event with xibar and without",2, -0.5, 1.5, 0, 100);
    TProfile* hkaonct_obwx  = new TProfile("hkaonct_obwx" , "Kaon count for omegabar event with xi and without",2, -0.5, 1.5, 0, 100);
    TProfile* hkaonct_obwxb = new TProfile("hkaonct_obwxb", "Kaon count for omegabar event with xibar and without",2, -0.5, 1.5, 0, 100);
    

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
    bool hasXi, hasAntiXi;
    int M[NEventClass+1] = {0}; //number of events that contain anti_o, o, anti_x, x, anti_x0, x0;

    // event loop
    for (int i = 0; i < nentries; ++i)
    {
        if((i+1)%1000==0) cout<<"Processing entry == "<< i+1 <<" == out of "<<nentries<<".\n";
        chain->GetEntry(i);

        int total_s = 0, total_bn = 0, itrack = 0;

        // check if event has certain particles
        for (int j = 0; j < NEventClass+1; j++) hasParticle[j] = false;
        hasXi = false; hasAntiXi = false;

        // fill examples and check anti-omega
        for (std::vector<int>::iterator it = pid_vec->begin(); it != pid_vec->end(); ++it)
        {
            int s = sness_map[(int)(*it)];

            // check for particle
            if (*it == -3334) hasParticle[1] = true; if (*it == 3334) hasParticle[2] = true; 
            if (*it == -3312) hasAntiXi      = true; if (*it == 3312) hasXi          = true;

            // omega ratio
            if (*it ==  3334) hOmegaRatio->Fill(0);
            if (*it == -3334) hOmegaRatio->Fill(1);
            
            // total strangeness and baryon number
            total_s += s;
            if (*it < 10000 && *it > 999) total_bn += 1;
            if (*it >-10000 && *it <-999) total_bn -= 1;
        }

        // count events
        hasParticle[0] = true; //total count
        for (int j = 0; j < NEventClass+1; j++) {if (hasParticle[j]) M[j]++;}

        // loop thru events
        int ntrack = pid_vec->size();
        assert(ntrack == px_vec->size() && ntrack == py_vec->size() && ntrack == pz_vec->size() && "Ntrack size mismatch!");
        int kaonct = 0; int bct[6] = {0};
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
            if (pid == 321) kaonct++;
            if (fabs(pid) >= 10000 || fabs(pid) <= 999) continue;
            // total
            if (pid > 0) bct[0] += 1;
            if (pid < 0) bct[0] -= 1;
            // lambda
            if (type == 3)
            {
                if (pid > 0) bct[1] += 1;
                if (pid < 0) bct[1] -= 1;
            }
            // sigma
            if (type == 4)
            {
                if (pid > 0) bct[2] += 1;
                if (pid < 0) bct[2] -= 1;
            }
            // cascade
            if (type == 5)
            {
                if (pid > 0) bct[3] += 1;
                if (pid < 0) bct[3] -= 1;
            }
            // omega
            if (type == 6)
            {
                if (pid > 0) bct[4] += 1;
                if (pid < 0) bct[4] -= 1;
            }
            // nucleon
            if (type == 7)
            {
                if (pid > 0) bct[5] += 1;
                if (pid < 0) bct[5] -= 1;
            }

        }

        hstrangeness->Fill(total_s);  
        hbaryon->Fill(total_bn);  
        // baryon number subplots
        for (int k = 0; k < 6; k++)
        {
            if (hasParticle[2]) // if has omega
            {
                hbndist_wo->Fill(k, bct[k]);
                if (hasXi)     hbndist_owx  ->Fill(k, bct[k]);
                else           hbndist_owox ->Fill(k, bct[k]);
                if (hasAntiXi) hbndist_owxb ->Fill(k, bct[k]);
                else           hbndist_owoxb->Fill(k, bct[k]);
            }
            else hbndist_woo->Fill(k, bct[k]);

            if (hasParticle[1]) // if has anti-omega
            {
                hbndist_wob->Fill(k, bct[k]);
                if (hasXi)     hbndist_obwx  ->Fill(k, bct[k]);
                else           hbndist_obwox ->Fill(k, bct[k]);
                if (hasAntiXi) hbndist_obwxb ->Fill(k, bct[k]);
                else           hbndist_obwoxb->Fill(k, bct[k]);
            }
            else hbndist_woob->Fill(k, bct[k]); 
        }

        // kaon ct subplots
        if (hasParticle[2])
        { 
            if (hasXi)     hkaonct_owx ->Fill(0., kaonct*1.0);
            else           hkaonct_owx ->Fill(1., kaonct*1.0);
            if (hasAntiXi) hkaonct_owxb->Fill(0., kaonct*1.0);
            else           hkaonct_owxb->Fill(1., kaonct*1.0);
        }

        if (hasParticle[1])
        { 
            if (hasXi)     hkaonct_obwx ->Fill(0., kaonct*1.0);
            else           hkaonct_obwx ->Fill(1., kaonct*1.0);
            if (hasAntiXi) hkaonct_obwxb->Fill(0., kaonct*1.0);
            else           hkaonct_obwxb->Fill(1., kaonct*1.0);
        }

    }

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