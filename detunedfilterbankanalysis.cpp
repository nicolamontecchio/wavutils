#include <iostream>
#include <sndfile.h>
#include <vector>
#include <math.h>
#include "BiQuadFilter.h"
using namespace std;



int main(int argc, char *argv[])
{
  if(argc != 5)
    {
      cerr << "invalid arguments" << endl;
      cout << "usage: " << argv[0] << " winlen filterwidth seconds input.wav" << endl;
      exit(1);
    }
  vector<float> center_frequencies(10);
  for (int i = 0; i < 10; i++)
    center_frequencies[i] = 440. * pow(2, 1./24. * (-0.9 + 0.2*i));

  float filter_width, seconds;
  filter_width = atof(argv[2]);
  seconds = atof(argv[3]);
  int winlen = atoi(argv[1]);
  
  // open audio file, read only central part
  SF_INFO sfinfo;
  SNDFILE* sndfile = sf_open(argv[argc-1], SFM_READ, &sfinfo);
  if(sndfile == NULL)
    {
      cerr << "could not open " << argv[argc-1] << endl;
      exit(1);
    }
  if(sfinfo.channels != 1) 
    {
      cerr << "file is not MONO, quitting" << endl;
      sf_close(sndfile);
      exit(1);
    }
  int sample_from = sfinfo.frames / 2 - sfinfo.samplerate * seconds/2;
  int sample_to   = sfinfo.frames / 2 + sfinfo.samplerate * seconds/2;
  sample_from = sample_from < 0 ? 0 : sample_from;
  sample_to   = sample_to > sfinfo.frames ? sfinfo.frames : sample_to;
  
  // construct filters
  vector<BiQuadFilter> filters(center_frequencies.size() * 88);
  for(int j = 0; j < center_frequencies.size(); j++)
    {
      float A0 = center_frequencies[j] / 16;
      for(int i = 0; i < 88; i++)
        {
          float f   = A0 * pow(2., 1./12. * i);
          float fbw = f * (pow(2.,1./12 * filter_width/2) - pow(2, -1./12. * filter_width/2));
          float r   = exp(-3.141592654 * fbw / sfinfo.samplerate);
          float phi = 2*3.141592654*f/sfinfo.samplerate;
          float a1  = -2 * r * cos(phi);
          float a2  = r*r;
          float b0  = (1-r)*sqrt(1-2*r*cos(2*phi) + r*r);
          float b1  = 0;
          float b2  = 0;
          filters[j*88 + i] = BiQuadFilter(b0, b1, b2, a1, a2);
        }
    }

  // do all the filtering
  float *read_buffer   = new float[winlen];
  float *filter_buffer = new float[winlen];
  int toread = sample_to - sample_from;
  sf_seek(sndfile, sample_from, SEEK_SET);
  while(toread > 0)
    {
      int read = sf_read_float(sndfile, read_buffer, winlen);
      toread -= read;
      for(int i = 0; i < filters.size(); i++)
        {
          filters[i].tick(read_buffer, filter_buffer, read);
          float energy = 0;
          for(int k = 0; k < winlen; k++)
            energy += filter_buffer[k] * filter_buffer[k];
          cout << energy << " ";
        }
      cout << endl;
    }

  // close up
  sf_close(sndfile);
  delete[] read_buffer;
  delete[] filter_buffer;

  return 0;
}
