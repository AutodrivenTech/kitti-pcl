#include <boost/program_options.hpp>
#include <pcl/point_types.h>
#include <pcl/io/pcd_io.h>
#include <pcl/common/point_operators.h>
#include <pcl/common/io.h>
#include <pcl/search/organized.h>
#include <pcl/search/octree.h>
#include <pcl/search/kdtree.h>
#include <pcl/features/normal_3d_omp.h>
#include <pcl/filters/conditional_removal.h>
#include <pcl/segmentation/sac_segmentation.h>
#include <pcl/segmentation/extract_clusters.h>
#include <pcl/surface/gp3.h>
#include <pcl/io/vtk_io.h>
#include <pcl/filters/voxel_grid.h>

#include <iostream>
#include <fstream>
#include <dirent.h>
#include <cstring>

using namespace pcl;
using namespace std;

//namespace po = boost::program_options;


void kitti2pcd(string infile, string outfile){
    fstream input(infile.c_str(), ios::in | ios::binary);
    if(!input.good()){
        cerr << "Could not read file: " << infile << endl;
        exit(EXIT_FAILURE);
    }
    input.seekg(0, ios::beg);

    pcl::PointCloud<PointXYZI>::Ptr points (new pcl::PointCloud<PointXYZI>);

    int i;
    for (i=0; input.good() && !input.eof(); i++) {
        PointXYZI point;
        input.read((char *) &point.x, 3*sizeof(float));
        input.read((char *) &point.intensity, sizeof(float));
        points->push_back(point);
    }
    input.close();

    cout << "Read KTTI point cloud with " << i << " points, writing to " << outfile << endl;

    pcl::PCDWriter writer;

    // Save DoN features
    writer.write<PointXYZI> (outfile.c_str(), *points, false);
}


int main(int argc, char **argv){
	if(argc != 5){
		cout << "There are two type of usage:" << endl;
		cout << "\t" << "./kitti2pcd --infile <infile path> --outfile <outfile path>"  << endl;
		cout << "\t\t" <<"--infile" << "\t" << "the file to read a point cloud from" << endl;
		cout << "\t\t"  << "--outfile" << "\t" << "the file to write the DoN point cloud & normals to" << endl;
        cout << "\t" << "./kitti2pcd --indir <indir path> --outfile <outdir path>"  << endl;
        cout << "\t\t" <<"--indir" << "\t" << "the dir to read a point cloud from" << endl;
        cout << "\t\t"  << "--outdir" << "\t" << "the dir to write the DoN point cloud & normals to" << endl;
		return 0;
	}

	if(strcmp(argv[1], "--infile") == 0 && strcmp(argv[3], "--outfile") == 0){
	    string infile(argv[2]);
	    string outfile(argv[4]);
	    kitti2pcd(infile, outfile);
	    return 0;
	}
	if(strcmp(argv[1], "--indir") == 0 && strcmp(argv[3], "--outdir") == 0){
//	    cout << "Test" << endl;
        DIR *dir;
        struct dirent *ent;
        if ((dir = opendir (argv[2])) != NULL) {
            string indir(argv[2]);
            string outdir(argv[4]);
            /* print all the files and directories within directory */
            while ((ent = readdir (dir)) != NULL ) {
                if(ent->d_type != DT_DIR){
                    string name(ent->d_name);
                    string postfix = name.substr(name.length()-4);
                    string prefix = name.substr(0, name.length() -4);
//                    cout << name  << ":" << postfix << ":" << prefix << "\n";
                    if(postfix == ".bin"){
                        string outfile = outdir + "/" + prefix + ".pcd";
                        string infile = indir  + "/" + name;
//                        cout << infile << ":\n\t" << outfile << "\n";
                        kitti2pcd(infile.c_str(), outfile.c_str());
                    }
                }

            }
            closedir (dir);
            return 0;
        } else {
            /* could not open directory */
            perror ("can't open directory");
            return 1;
        }
	}

    cout << "There are two type of usage:" << endl;
    cout << "\t" << "./kitti2pcd --infile <infile path> --outfile <outfile path>"  << endl;
    cout << "\t\t" <<"--infile" << "\t" << "the file to read a point cloud from" << endl;
    cout << "\t\t"  << "--outfile" << "\t" << "the file to write the DoN point cloud & normals to" << endl;
    cout << "\t" << "./kitti2pcd --indir <indir path> --outfile <outdir path>"  << endl;
    cout << "\t\t" <<"--indir" << "\t" << "the dir to read a point cloud from" << endl;
    cout << "\t\t"  << "--outdir" << "\t" << "the dir to write the DoN point cloud & normals to" << endl;
    return 0;

}

