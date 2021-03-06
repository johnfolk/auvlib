/* Copyright 2018 Nils Bore (nbore@kth.se)
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <iostream>
#include <cereal/archives/json.hpp>
#include <data_tools/xtf_data.h>
#include <data_tools/dtools.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cstdlib>
using namespace std;
using namespace std_data;
using namespace xtf_data;

/** call as:
./create_xtf_waterfall ./pathtoXTFfolder maxIntensity rowdownsample 
**/

extern int verbose_level;

int main(int argc, char** argv)
{
  dtools::SidescanSide port("Port");
  dtools::SidescanSide stbd("S");
  if (1){
    boost::filesystem::path folder(argv[1]);
    port.xtf_parse(folder);
    cout<<"Port parsed\n";
    stbd.xtf_parse(folder);
    cout<<"stbd parsed\n";
    //xtf_sss_ping::PingsT pings = parse_folder_ordered<xtf_sss_ping>(folder);
    //port=pings;
    //  stbd=pings;
    std::cout<<folder<<" is parsed\n";
  }

  long maxIntensity=65535;
  long rowdownsample=1;
  if (argc>2) maxIntensity=atol(argv[2]);
  if (argc>3) rowdownsample=atol(argv[3]);
  long p_nadir[port.h];
  long s_nadir[stbd.h];
  long artif[port.h];
  cout<<port.h<<" "<<port.alloc<<" "<<port.w<<" "<<port.next;
  port.slant_range(0);

  //  cout<<port[port.alloc+10][0];
  // generally the minimum intensity param must be adjusted for differe bottoms and SSL or other factors
  double minintensity=10000;
  port.removeLineArtifact(artif,minintensity,8,100,1);
  port.findNadir(p_nadir, minintensity, 8, 80);
  stbd.findNadir(s_nadir, minintensity, 8, 80);
  //plot the nadir in a bright wide line

  for (int i=0; i<port.h; i++){
      for (int k=0; k<40; k++){
	if (p_nadir[i]-k>0)
	  port[i][p_nadir[i]-k]=maxIntensity;
	if (s_nadir[i]-k>0)
	stbd[i][s_nadir[i]-k]=maxIntensity;
      }
  }
  port.show_waterfall(1280,512,maxIntensity, 0, -1,
  		      "Port Before Normalization");
  stbd.show_waterfall(1280, 512,maxIntensity, 0, -1,
  		      "Starboard Before Normalization");
 
  std::cout<<" Normalizing Now\n";
  double roll[port.h];
  double nadir_angle=(22.0*M_PI/180.0);
  memset(roll, 0, sizeof(roll));
  port.normalize(p_nadir,s_nadir, 0, nadir_angle);
    stbd.normalize(p_nadir,s_nadir, 0, nadir_angle);
    port.show_waterfall(1280, 512,maxIntensity, 0, -1,
			"Port After Normalization");
  stbd.show_waterfall(1280, 512,maxIntensity, 0, -1,
			"Starboard After Normalization");
    port.regularize(roll,p_nadir,s_nadir,0.05, .85, nadir_angle);
    stbd.regularize(roll,p_nadir,s_nadir,0.05, .85, nadir_angle);
    port.show_waterfall(1280, 512,maxIntensity, 0, -1,
			"Port After Regularization");
  stbd.show_waterfall(1280, 512,maxIntensity, 0, -1,
			"Starboard After Regularization");
    /*  pimg=port.make_waterfall_image(1024, maxIntensity, rowdownsample, 0, -1, 0, -1);
    cv::imshow("After regualrixe port image", pimg);
    simg=stbd.make_waterfall_image(1024, maxIntensity, rowdownsample, 0, -1,  0, -1);
    cv::imshow("After regularize starboard image", simg);
    cv::waitKey();
    */

    /*
      for (auto pos = pings.begin(); pos != pings.end(); ) {
      auto next = std::find_if(pos, pings.end(), [&](const xtf_sss_ping& ping) {
            return ping.first_in_file_ && (&ping != &(*pos));
        });
        xtf_sss_ping::PingsT track_pings(pos, next);
	int r = track_pings.size()/rowdownsample;
	cv::Mat waterfall_img = dtools::make_waterfall_image(track_pings, 1024, r, maxIntensity);
	std::cerr<<n<<"th file\n";
	n++;
	cv::imwrite("regularized.png",waterfall_img);
	cv::imshow("My regularzed image", waterfall_img);
	cv::waitKey();
        pos = next;
    }
    */
    return 0;
}

