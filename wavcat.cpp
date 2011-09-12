/** 
  Similar to unix cat, except it is used for wav files. Last file is the output.
  Files must have same sample rate, format and number of channels.
  Uses libsndfile.
 */
#include <iostream>
#include <vector>
#include <string>
#include <sndfile.h>
using namespace std;

int main(int argc, char *argv[])
{
  bool error = false;
  // read all arguments
  if(argc < 3)
    {
      cerr << "invalid usage" << endl;
      cout << "usage: " << argv[0] << " file1.wav file2.wav ... fileN.wav output.wav" << endl;
      return 1;
    }
  vector<string> inputfiles;
  for(int i = 1; i < argc-1; i++)
    inputfiles.push_back(string(argv[i]));
  // open all referenced files
  vector<SF_INFO> infos;
  vector<SNDFILE*> sndfiles;
  for(vector<string>::iterator it = inputfiles.begin(); it != inputfiles.end(); it++)
    {
      SF_INFO sfinfo;
      SNDFILE* sndfile = sf_open((*it).c_str(), SFM_READ, &sfinfo);
      if(sndfile == NULL)
        {
          error = true;
          cerr << "an error occurred on file " << (*it) << endl;
          break;
        }
      infos.push_back(sfinfo);
      sndfiles.push_back(sndfile);
    }
  // check that files can be concatenated
  if(infos.size() > 1 && !error)
    {
      SF_INFO ref = infos[0];
      for(vector<SF_INFO>::iterator it = infos.begin(); it != infos.end(); it++)
        if(ref.format != (*it).format || ref.channels != (*it).channels || ref.samplerate != (*it).samplerate)
          {
            error = true;
            cerr << "one or more files have different characteristics and cannot be concatenated" << endl;
            break;
          }
    }
  // everything ok, proceed
  if(!error)
    {
      SF_INFO outinfo;
      outinfo.format = infos[0].format;
      outinfo.channels = infos[0].channels;
      outinfo.samplerate = infos[0].samplerate;
      SNDFILE* out = sf_open(argv[argc-1], SFM_WRITE, &infos[0]);
      const int BUFFERSIZE = 4096 * outinfo.channels;
      float *buffer = new float[BUFFERSIZE];
      for(vector<SNDFILE*>::iterator it = sndfiles.begin(); it != sndfiles.end(); it++)
        {
          SNDFILE* sndfile = *it;
          while(true)
            {
              int read = sf_read_float(sndfile, buffer, BUFFERSIZE);
              sf_write_float(out, buffer, BUFFERSIZE);
              if(read < BUFFERSIZE)
                break;
            }
        }      
      delete[] buffer;
      sf_close(out);
    }
  // cleanup
  for(vector<SNDFILE*>::iterator it = sndfiles.begin(); it != sndfiles.end(); it++)
    sf_close(*it);  
  if(error)
    return 1;
  return 0;
}
