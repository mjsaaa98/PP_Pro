#ifndef HEADER_H
#define HEADER_H

#include <QTime>
#include <QDebug>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/ml/ml.hpp>
#include <opencv2/dnn/dnn.hpp>
#include <iostream>
#include <thread>
#include <mutex>
#include <string.h>
#define SHOW_DEBUG
//#define PRINT
//#define OPEN_SERIAL

#define BLUE_DETECT true
#define RED_DETECT false

#define GET_BLUE 0x02
#define GET_RED 0x01
#define GET_NO 0x00

#define LEFT true
#define RIGHT false

using namespace cv;
using namespace std;


typedef enum{
    small_armor,
    big_armor
}armor_type;

struct InitParams{
    uchar armor_thres_whole;
    uchar armor_thres_red;
    uchar armor_thres_blue;

    InitParams(){
        armor_thres_whole = 30;
        armor_thres_red = 40;
        armor_thres_blue = 80;
    }

};

typedef struct{
    float x;
    float y;
    float z;
    int index;
}AbsPosition;

typedef struct{
    Point2f center;
    int index;
}Index_and_Center;

typedef struct{
    Point armor_center = Point(0,0);
    float k;
//    float distance = 0.0;//mm
    Point2f armor_points[4] = {Point(0,0),Point(0,0),Point(0,0),Point(0,0)};
    float diameter;
//    armor_type armor = small_armor;
}Armordata;

#endif // HEADER_H
