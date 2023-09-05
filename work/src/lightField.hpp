
//  lightField.hpp
//  cgra352
//
//  Created by Graylin Chawanji on 21/05/23.
//

#ifndef lightField_hpp
#define lightField_hpp

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

using namespace cv;
using namespace std;

#endif /* lightField_hpp */
cv::Vec3b getLightFieldPoint(Mat lightField[17][17], int row, int col, int t, int s);
