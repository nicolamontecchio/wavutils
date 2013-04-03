/** Cut a wave file. Call usage:
    wavcut inputfile.wav outputfile.wav from to
    with from and to values in seconds */

#include <iostream>
#include <sndfile.h>
using namespace std;

void cut(char *in, char*out, float from, float to)
{
  // read whole file
  SF_INFO sfinfoIn;
  SNDFILE*   sndfileIn = sf_open(in, SFM_READ, &sfinfoIn);
  if (sndfileIn == 0)
    {
      cerr << "could not open audio file " << in << endl;
      return;
    }

  // cout << "input" << endl;
  // cout << " frames: " << sfinfo.frames << endl;
  // cout << " samplerate: " << sfinfo.samplerate << endl;
  // cout << " channels: " << sfinfo.channels << endl;
  // cout << " format: " << hex << sfinfo.format << dec << endl;

  const int BUFFERSIZE = sfinfoIn.channels * 1024*8;
  int nSamples = (to-from) * sfinfoIn.samplerate * sfinfoIn.channels;

  SF_INFO sfinfoOut = sfinfoIn;
  SNDFILE*   sndfileOut = sf_open(out, SFM_WRITE, &sfinfoOut);
  sf_seek(sndfileIn, from * sfinfoIn.samplerate, SEEK_SET);

  float *buffer = new float[BUFFERSIZE];
  for (int f = 0; f < nSamples/BUFFERSIZE; f++)
    {
      sf_read_float(sndfileIn, buffer, BUFFERSIZE);
      sf_write_float(sndfileOut, buffer, BUFFERSIZE);
    }
  int n_remaining = nSamples % BUFFERSIZE;
  sf_read_float(sndfileIn, buffer, n_remaining);
  sf_write_float(sndfileOut, buffer, n_remaining);

  sf_close(sndfileIn);
  sf_close(sndfileOut);
  delete[] buffer;
}

int main(int argc, char** argv)
{
  if (argc != 5)
    {
      cout << "usage: " << argv[0] << " input.wav output.wav from to" << endl;
      return 1;
    }
  cut(argv[1],argv[2],atof(argv[3]),atof(argv[4]));
  return 0;
}
