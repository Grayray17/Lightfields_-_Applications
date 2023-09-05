
// std
#include <iostream>
#include <stdio.h>
#include <string>

// opencv
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "lightField.hpp"

using namespace cv;
using namespace std;


// main program
// 
int main( int argc, char** argv ) {
    
    // check we have exactly one additional argument
    // eg. res/vgc-logo.png
    if( argc != 2) {
        cerr << "Usage: cgra352 <Image>" << endl;
        abort();
    }
    
    Mat lightField[17][17];
    Mat stArray[100][100];
    Point uv[17][17];
    Mat st(512, 1024, CV_8UC3);
    //parse all images
    std::cout <<"Loading light field..."<< std::endl;
    std::vector<cv::String> lf_imgs;
    cv::glob(argv[1],lf_imgs);
    for(cv::String cv_str : lf_imgs){
        //get the file path
        std::string filepath(cv_str);
        size_t pos = filepath.find_last_of("/\\");
        if(pos!=std::string::npos){
            //replace "_" with " "
            std::string filename = filepath.substr(pos+1);
            pos=0;
            while((pos = filename.find("_", pos)) != std::string::npos){
                filename.replace(pos,1," ");
                pos++;
                
            }
            //parse for values
            std::istringstream ss(filename);
            std::string name;
            int row,col;
            float v,u;
            ss >> name >> row >> col >> v >> u;
            //cout<< row << " "<< col<<endl;
            
            if(ss.good()){
                //TODO something with the image file "filepath"
                //TODO something with the coordinates: row,col,v,u
                //uv array of st images
                lightField[row][col] = imread(filepath, CV_LOAD_IMAGE_COLOR);
                uv[row][col] = Point(u, v);
                continue;
                
            }
            
        }
        //throw error otherwise
        std::cerr << "File path error with:" << filepath << std::endl;
        std::cerr<<"Expected in the form: [prefix]/[name]_[row]_[col]_[v]_[u][suffix]";
        abort();
        
    }
    std::cout<<"Finished loading light field"<<std::endl;
    
    //core1
    cout<<"Core 1"<<getLightFieldPoint(lightField, 7, 10, 384, 768)<<endl;

    //core 2
    int s = 770;
    int t = 205;
    int radius = 40;
    Point uvCentre(533.057190, -776.880371);
    
    for(int i = 0; i< 100; i++){
        for(int j = 0; j< 100; j++){
            Mat uvArray(17, 17, CV_8UC3);
            for(int a = 0; a<17; a++){
                for(int b = 0; b<17; b++){
                    double dist = sqrt(pow((uvCentre.y - uv[a][b].y), 2) + pow((uvCentre.x - uv[a][b].x), 2));
                    if(dist<radius){
                        uvArray.at<Vec3b>(a,b) = getLightFieldPoint(lightField, a, b, t, s);
                    }
                    else{
                        uvArray.at<Vec3b>(a,b) = Vec3b(0,0,0);
                    }
                }
            }
            stArray[i][j] = uvArray;
            t++;
        }
        t=205;
        s++;
    }
    
    Mat output;
    Mat hOut;
    for(int i = 0; i< 100; i++){
        for(int j = 0; j< 100; j++){
            Mat current = stArray[i][j];
            if(j==0){
                hOut = current;
            }
            else{
                vconcat(hOut, current, hOut);
            }
        }
        if(i==0){
            output = hOut;
        }
        else{
            hconcat(output, hOut, output);
        }
    }
    
    //completion/challenge
    //the focal length values for my 5 images are 0.7, 0.75, 0.8, 0.85, 0.9
    double focalLength = 0.7;
    for(int i = 0; i< 512; i++){
        for(int j = 0; j< 1024; j++){
            Vec3i colour;
            int num = 0;
            for(int a = 0; a< 17; a++){
                for(int b = 0; b< 17; b++){
                    double uDist = uvCentre.x - uv[a][b].x;
                    double vDist = uvCentre.y - uv[a][b].y;
                    double tPrime = focalLength * i;
                    double sPrime = focalLength * j;
                    int t = vDist + ((tPrime - vDist) / focalLength);
                    int s = uDist + ((sPrime - uDist) / focalLength);
                    if(t> 0 && s> 0 && t< 512 && s< 1024){
                        colour += getLightFieldPoint(lightField, a, b, t, s);
                        st.at<Vec3b>(i, j) = colour/num;
                        num++;
                    }
                }
            }
        }
    }
    
    //Core 2 75
    // create a window for display and show our image inside it
    string img_display_core1 = "Core 2 Image Display";
    namedWindow(img_display_core1, WINDOW_AUTOSIZE);
    imshow(img_display_core1, output);
    
    //Core 2 40
    // create a window for display and show our image inside it
    string img_display_core2 = "Core 2 Image Display";
    namedWindow(img_display_core2, WINDOW_AUTOSIZE);
    imshow(img_display_core2, output);
    
    if(radius == 75){
        // save image
        imwrite("CGRA352_base/output/core2_75.png", output);
    }
    
    else if (radius == 40){
        // save image
        imwrite("/CGRA352_base/output/core2_40.png", output);
    }
    
    //completion/challenge
    // create a window for display and show our image inside it
    string img_display_com = "Completion 1 Image Display";
    namedWindow(img_display_com, WINDOW_AUTOSIZE);
    imshow(img_display_com, st);

    if(focalLength == 0.70){
        // save image
        imwrite("/CGRA352_base/output/completion1.png", st);
    }
    if(focalLength == 0.75){
        // save image
        imwrite("/CGRA352_base/output/completion2.png", st);
    }
    if(focalLength == 0.80){
        // save image
        imwrite("/CGRA352_base/output/completion3.png", st);
    }
    if(focalLength == 0.85){
        // save image
        imwrite("/CGRA352_base/output/completion4.png", st);
    }
    if(focalLength == 0.90){
        // save image
        imwrite("/CGRA352_base/output/completion5.png", st);
    }
    
    // wait for a keystroke in the window before exiting
    waitKey(0);
    
    
}
