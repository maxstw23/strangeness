#include <TROOT.h>
#include <TSystem.h>
#include <TString.h>

void check(const Char_t *inFile = "placeholder.list", const TString JobID = "1234", const int mode = 1)
{
    gSystem->Load("./lib/my_particle_cpp.so");
    gSystem->Load("./lib/my_event_cpp.so");
    gSystem->Load("./lib/MixedBuffer_cpp.so");
    gSystem->Load("./lib/CenMaker_cpp.so");

    TString str;
    str = ".x Check_Cf.C+(";
    str += "\"";
    str += inFile;
    str += "\", \"";
    str += JobID;
    str += "\", ";
    str += mode;
    str += ")";
    gROOT->ProcessLine( str.Data() );
    gROOT->ProcessLine(".!rm -f check_C* ");
    gROOT->ProcessLine(".!rm -f Check_Cf_C* ");
}