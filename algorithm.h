#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "qimage.h"
#include "QDebug"

class Algorithm
{
public:
    Algorithm();
    QImage Mat2QImage(const cv::Mat& mat);
    cv::Mat QImage2cvMat(QImage image);
};

#endif // ALGORITHM_H
