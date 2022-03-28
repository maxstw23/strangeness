#include <TROOT.h>
#include <TSystem.h>
#include <TString.h>

void check_default(const Char_t *inFile = "placeholder.list", const TString JobID = "1234")
{
    gSystem->Load("./lib/my_particle_cpp.so");
    gSystem->Load("./lib/my_event_cpp.so");
    gSystem->Load("./lib/MixedBuffer_cpp.so");
    gSystem->Load("./lib/CenMaker_cpp.so");

    TString str;
    str = ".x Check_Cf_default.C+(";
    str += "\"";
    str += inFile;
    str += "\", \"";
    str += JobID;
    str += "\")";
    gROOT->ProcessLine( str.Data() );
    gROOT->ProcessLine(".!rm -f check_default_C* ");
    gROOT->ProcessLine(".!rm -f Check_Cf_default_C* ");
}
