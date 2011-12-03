/** print duration of audio file(s), in seconds, on stdout */
#include <iostream>
#include <vector>
#include <string>
#include <sndfile.h>
using namespace std;

int main(int argc, char *argv[])
{
  // read all arguments
  if(argc < 2)
    {
      cerr << "invalid usage" << endl;
      cout << "usage: " << argv[0] << " file1.wav file2.wav ... fileN.wav" << endl;
      return 1;
    }
  vector<string> inputfiles;
  for(int i = 1; i < argc; i++)
    inputfiles.push_back(string(argv[i]));
  // open all referenced files, and print length
  int error = 0;
  for(vector<string>::iterator it = inputfiles.begin(); it != inputfiles.end(); it++)
    {
      SF_INFO sfinfo;
      SNDFILE* sndfile = sf_open((*it).c_str(), SFM_READ, &sfinfo);
      if(sndfile == NULL)
        {
          error = true;
          cerr << "an error occurred on file " << (*it) << endl;
        }
      else 
        {
          cout << (float) sfinfo.frames / sfinfo.samplerate << endl;
          sf_close(sndfile);
        }
    }
  return error;
}
