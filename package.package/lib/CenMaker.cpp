#include "CenMaker.h"
#include "TString.h"

const int CenMaker::cent9_19GeV_sm[] = {1, 2, 6, 13, 25, 42, 67, 100, 144}; 
const int CenMaker::cent9_14GeV_sm[] = {6, 12, 23, 40, 63, 94, 135, 190, 228}; 
const int CenMaker::cent9_14GeV_default[] = {5, 12, 22, 37, 59, 89, 131, 190, 229};
const int CenMaker::cent9_14GeV_urqmd[] = {3, 5, 11, 24, 45, 79, 128, 204, 257};
const int CenMaker::cent16_19GeV_sm[] = {1, 2, 4, 6, 9, 13, 18, 25, 33, 42, 54, 67, 83, 100, 121, 144};
const int CenMaker::cent16_14GeV_sm[] = {6, 9, 12, 17, 23, 31, 40, 51, 63, 77, 94, 113, 135, 160, 190, 228};
const int CenMaker::cent16_14GeV_default[] = {5, 8, 12, 16, 22, 29, 37, 47, 59, 73, 89, 108, 131, 157, 190, 229};
const int CenMaker::cent16_14GeV_urqmd[] = {3, 4, 5, 7, 11, 16, 24, 33, 45, 60, 79, 101, 128, 162, 204, 257};


int CenMaker::cent9(int refmult, TString energy, int AMPTmode)
{
    int cen = -1;
    if (AMPTmode == 1)
    {
        if (energy == "19")
        {
            for (int i = 0; i < 9; i++)
            {
                if (refmult > cent9_19GeV_sm[i]) cen = i+1;
                else break;
            }
        }
        if (energy == "14")
        {
            for (int i = 0; i < 9; i++)
            {
                if (refmult > cent9_14GeV_sm[i]) cen = i+1;
                else break;
            }
        }
    }
    else if (AMPTmode == 0)
    {
        if (energy == "14")
        {
            for (int i = 0; i < 9; i++)
            {
                if (refmult > cent9_14GeV_default[i]) cen = i+1;
                else break;
            }
        }
    }
    else if (AMPTmode == 2)
    {
        if (energy == "14")
        {
            for (int i = 0; i < 9; i++)
            {
                if (refmult > cent9_14GeV_urqmd[i]) cen = i+1;
                else break;
            }
        }
    }
    return cen;
}

int CenMaker::cent16(int refmult, TString energy, int AMPTmode)
{
    int cen = -1;
    if (AMPTmode == 1)
    {
        if (energy == "19")
        {
            for (int i = 0; i < 16; i++)
            {
                if (refmult > cent16_19GeV_sm[i]) cen = i+1; 
                else break;
            }
        }
        if (energy == "14")
        {
            for (int i = 0; i < 16; i++)
            {
                if (refmult > cent16_14GeV_sm[i]) cen = i+1;
                else break;
            }
        }
    }
    else if (AMPTmode == 0)
    {
        if (energy == "14")
        {
            for (int i = 0; i < 16; i++)
            {
                if (refmult > cent16_14GeV_default[i]) cen = i+1;
                else break;
            }
        }
    }
    else if (AMPTmode == 2)
    {
        if (energy == "14")
        {
            for (int i = 0; i < 16; i++)
            {
                if (refmult > cent16_14GeV_urqmd[i]) cen = i+1;
                else break;
            }
        }
    }
    return cen;
}

