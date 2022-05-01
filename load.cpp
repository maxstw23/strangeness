void load() 
{
  gROOT->ProcessLine(".L ./lib/my_particle.cpp++");
  gROOT->ProcessLine(".L ./lib/my_event.cpp++");
  gROOT->ProcessLine(".L ./lib/MixedBuffer.cpp++");
  gROOT->ProcessLine(".L ./lib/CenMaker.cpp++");
  gROOT->ProcessLine(".L ./lib/NpartNormalizer.cpp++");
}
