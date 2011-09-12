#include <iostream>
#include <sndfile.h>
using namespace std;

#ifdef PYMODULE
#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
#endif

void convert(char *in, char*out)
{
  // read whole file
  SNDFILE* sndfile;
  SF_INFO sfinfo;
  sndfile = sf_open(in, SFM_READ, &sfinfo);
  if (sndfile == 0)
    {
      cerr << "could not open audio file " << in << endl;
      return;
    }
  cout << "input" << endl;
  cout << " frames: " << sfinfo.frames << endl;
  cout << " samplerate: " << sfinfo.samplerate << endl;
  cout << " channels: " << sfinfo.channels << endl;
  cout << " format: " << hex << sfinfo.format << dec << endl;
  float *audioIn = new float[sfinfo.channels * sfinfo.frames];
  sf_read_float(sndfile, audioIn, sfinfo.channels * sfinfo.frames);
  // mixdown
  float *audioOut = new float[sfinfo.frames];
  for(int i = 0; i < sfinfo.frames; i++)
    {
      audioOut[i] = 0;
      for(int j = 0; j < sfinfo.channels; j++)
        audioOut[i] += audioIn[i*sfinfo.channels + j];
      audioOut[i] /= sfinfo.channels;
    }
  sf_close(sndfile);
  // write output
  int frames = sfinfo.frames;    
  sfinfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
  sfinfo.channels = 1;
  sndfile = sf_open(out, SFM_WRITE, &sfinfo);
  sf_write_float(sndfile, audioOut, frames);
  cout << "output" << endl;
  cout << " frames: " << frames << endl;
  cout << " samplerate: " << sfinfo.samplerate << endl;
  cout << " channels: " << sfinfo.channels << endl;
  cout << " format: " << hex << sfinfo.format << dec << endl;
  sf_close(sndfile);
  // free memory
  delete[] audioIn;
  delete[] audioOut;
}

#ifndef PYMODULE

/**
 * Convert an audio file to mono format (input in first arg, output in second)
 */
int main(int argc, char** argv)
{
  if (argc != 3)
    {
      cout << "usage: " << argv[0] << " input.wav output.wav" << endl;
      exit(1);
      return 1;
    }
  convert(argv[1],argv[2]);
  return 0;
}

#else

BOOST_PYTHON_MODULE(tomonowav_ext)
{
  using namespace boost::python;
  def("convert", convert);
}

#endif
