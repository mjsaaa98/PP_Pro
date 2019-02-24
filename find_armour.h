#ifndef FIND_ARMOUR_H
#define FIND_ARMOUR_H

#include "serialport.h"
#include "Header.h"

//#define KALMANXY_OPEN
using namespace cv;
using namespace std;
extern FileStorage fs;

/**
 * @brief Sort_RotatedRect  按旋转矩形的中心点x方向降序排列
 * @param r1  RotatedRect
 * @param r2  RotatedRect
 * @return
 */
static inline bool Sort_RotatedRect(RotatedRect r1,RotatedRect r2)
{
    return r1.center.x<r2.center.x;
}
/**
 * @brief SortArmorCenterX  在X方向升序排列
 * @param p1  Point2f
 * @param p2  Point2f
 * @return
 */
static inline bool SortArmorCenterX(Point2f p1,Point2f p2)
{
    return p1.x<p2.x;
}
/**
 * @brief SortArmorCenterY  在Y方向降序排列
 * @param p1  Point2f
 * @param p2  Point2f
 * @return
 */
static inline bool SortArmorCenterY(Point2f p1,Point2f p2)
{
    return p1.y>p2.y;
}

static inline bool Sort_Areas(float a1,float a2)
{
    return a1<a2;
}

/**
 * @brief The find_armour class  找装甲板的类
 */
class find_armour
{
public:
    find_armour();
    void clear_data();   //切换指令时清空所有数据
    void Clear();   //清空所有数据结构
    Mat roi(Mat img,Point center,float d/*,float k*/);
    float Point_Angle(const Point2f &p1,const Point2f &p2)
    {
        return fabs(atan2(p2.y-p1.y,p2.x-p1.x)*180.0/CV_PI);
    }
    void image_preprocess(int mode,Mat src,Mat &);
    void search_armour(Mat&img,Mat&dst);
    void get_Light();
    void src_get_armor();
    void get_armor(Mat& img,Mat& dst,int);
public:
    //与串口通信有关的数据
    int ismiddle;
    int isfind;     //是否找到目标
    int last_mode;  //上一个指令：判断是否切换指令
    int isROIflag;  //是否截图
//    vector<Armordata> Armordatas;
//    vector<Point2f> ArmorPoints;
//    Armordata LastArmor;
public:
    vector<Point2f> Rotate_Point;
private:
    vector<float> diameters;
    vector<Point2f> armour_center;
    vector<float> big_diameters;
    vector<Point2f> big_armour_center;
    vector<vector<Point2f> > Rotate_Points;
    vector<vector<Point2f> > big_Rotate_Points;
    vector<vector<RotatedRect> > Armorlists;
    vector<RotatedRect> fir_armor,result_armor;
    vector<Vec4f> contours_para;
    vector<int> CellMaxs;
    float x1,x2,y1,y2;
    float _angle_of_Rotated,_k,_area_rate_min,_area_rate_max;
    float _x2h_rate_min1,_x2h_rate_min2,_x2h_rate_max1,_x2h_rate_max2;
private:
    //与截图有关的参数
    Point2f last_center;
    Mat img_ROI;
    float last_d;
    float last_angle;
    Size2f last_size;

    int cols = 1280;
    int rows = 720;
};

#endif // FIND_ARMOUR_H
