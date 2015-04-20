#include "MyString.h"
#include <fstream>
#include <cassert>
#include <iostream>
using namespace std;

#define NUMDATA 28
#define NUMPROJ 3
#define NUMBETA 6
int main(int argc, char* argv[]){
	// data
	MyString dataRoot("C:\\Users\\GuohaoZhang\\Desktop\\Keqin_data\\");
	MyString complexities[2] = { MyString("normal_allfb\\"), MyString("normal_whole\\") };
	MyString task("region_for_task1\\");
	MyString resolution("s2\\");
	MyString bundles[4] = { MyString("cc\\"), MyString("cst\\"),
		MyString("ifo\\"), MyString("ilf\\") };
	MyString positions[4] = { MyString(""), MyString("pos1\\"),
		MyString("pos2\\"), MyString("pos3\\") };
	MyString tracts("region_s2.data");
	MyString boxes[2] = { MyString("tumorbox_0_region_s2.data"), MyString("tumorbox_1_region_s2.data") };

	// projection
	MyString projections[NUMPROJ] = { MyString("PERSPECTIVE"), MyString("ORTHOGONAL"), MyString("SCREEBSPACE") };

	// scale
	float betas[NUMBETA] = { -1, 0.4, 1, 1.4, 2, 3 };

	// output dir
	int configIdx = 0;
	MyString outputDir("configs\\");
	for (int idata = 0; idata < NUMDATA; idata++){
		MyString dataDir = dataRoot;
		if (idata < NUMDATA / 2){
			dataDir += complexities[0];
		}
		else{
			dataDir += complexities[1];
		}
		dataDir += task;
		dataDir += resolution;
		if (idata % (NUMDATA / 2) < 4){
			dataDir += bundles[0];
			dataDir += positions[idata % (NUMDATA / 2)];
		}
		else if (idata % (NUMDATA / 2) < 8){
			dataDir += bundles[1];
			dataDir += positions[idata % (NUMDATA / 2)-4];
		}
		else if (idata % (NUMDATA / 2) < 11){
			dataDir += bundles[2];
			dataDir += positions[idata % (NUMDATA / 2) - 8];
		}
		else{
			assert(idata % (NUMDATA / 2) < 14);
			dataDir += bundles[3];
			dataDir += positions[idata % (NUMDATA / 2) - 11];
		}
		MyString tractName = dataDir + tracts;
		MyString boxName0 = dataDir + boxes[0];
		MyString boxName1 = dataDir + boxes[1];

		for (int iproj = 0; iproj < NUMPROJ; iproj++){
			for (int ibeta = 0; ibeta < NUMBETA; ibeta++){
				MyString outfileName(outputDir + "config_" + MyString(configIdx++) + ".txt");
				ofstream outfile(outfileName);
				if (outfile.is_open()){
					outfile << tractName << endl;
					outfile << boxName0 << endl;
					outfile << boxName1 << endl;
					outfile << betas[ibeta] << endl;
					outfile << projections[iproj] << endl;
				}
				else{
					cout << "Error: cannot open file to write: " << outfileName << endl;
					return -1;
				}
				outfile.close();
			}
		}
	}
}