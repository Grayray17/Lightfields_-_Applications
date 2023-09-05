
//  lightField.cpp
//  cgra352
//
//  Created by Graylin Chawanji on 21/05/23.
//

#include "lightField.hpp"

cv::Vec3b getLightFieldPoint(Mat lightField[17][17], int row, int col, int t, int s)
{
    return lightField[row][col].at<Vec3b>(t, s);
}
