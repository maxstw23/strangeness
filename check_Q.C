#include <TROOT.h>
#include <TSystem.h>
#include <TString.h>

void check_Q(const Char_t *inFile = "placeholder.list", const TString JobID = "1234", const int mode = 1)
{
    gSystem->Load("./lib/CenMaker_cpp.so");
    gSystem->Load("./lib/NpartNormalizer_cpp.so");

    TString str;
    str = ".x Check_QA.C+(";
    str += "\"";
    str += inFile;
    str += "\", \"";
    str += JobID;
    str += "\", ";
    str += mode;
    str += ")";
    gROOT->ProcessLine( str.Data() );
    gROOT->ProcessLine(".!rm -f check_Q_C* ");
    gROOT->ProcessLine(".!rm -f Check_QA_C* ");
}