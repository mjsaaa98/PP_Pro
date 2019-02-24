#include "find_armour.h"
/**
 * @brief find_armour::find_armour  构造函数 完成一些从yaml读取数据的操作
 * @param f  写入yaml文件的类
 */
find_armour::find_armour()
{
    fs["x2h_rate_min1"] >> _x2h_rate_min1;
    fs["x2h_rate_min2"] >> _x2h_rate_min2;
    fs["x2h_rate_max1"] >> _x2h_rate_max1;
    fs["x2h_rate_max2"] >> _x2h_rate_max2;
    fs["angle_of_Rotated"] >> _angle_of_Rotated;
    fs["k"] >> _k;
    fs["area_rate_min"] >> _area_rate_min;
    fs["area_rate_max"] >> _area_rate_max;
//    cout<<"sAdaaaaaaaaa"<<endl;
//    cout<<_x2h_rate_min1<<endl;
//    cout<<_x2h_rate_min2<<endl;
//    cout<<_x2h_rate_max1<<endl;
//    cout<<_x2h_rate_max2<<endl;
//    cout<<_angle_of_Rotated<<endl;
//    cout<<_area_rate_min<<endl;
//    cout<<_area_rate_max<<endl;
//    cout<<_k<<endl;
    isROIflag = 0;
    Clear();
    clear_data();
}

/**
 * @brief find_armour::Clear  清空所有数据
 */
void find_armour::Clear()
{
    diameters.clear();
    armour_center.clear();
    big_diameters.clear();
    big_armour_center.clear();
    fir_armor.clear();
    result_armor.clear();
    Armorlists.clear();
    diameters.clear();
    armour_center.clear();
    Rotate_Point.clear();
    Rotate_Points.clear();
    big_Rotate_Points.clear();
    contours_para.clear();
    CellMaxs.clear();
    isfind = 0;
    ismiddle =0;
}
/**
 * @brief find_armour::clear_data  切换指令时清空所有数据
 */
void find_armour::clear_data()
{
    last_angle = 0;
    last_d = 0;
    last_angle = 0;
    last_size = Size(0,0);
    isROIflag = 0;
    last_center = Point2f(0,0);
    x1 = 0;
    x2 = 0;
    y1 = 0;
    y2 = 0;
}


/**
 * @brief find_armour::roi   截图
 * @param img
 * @param center
 * @param d
 * @param cols
 * @param rows
 * @return
 */
Mat find_armour::roi(Mat img,Point center,float d/*,float k*/)
{
    Mat roi;
    cout<<"center:"<<center<<" ";
    x1 = center.x-d*1.5;
    x2 = center.x+d*1.5;
//    if(k>0)
//    {
//        x1 = center.x-d*1.5;
//        x2 = center.x+d*1.3;
//    }
//    else
//    {
//        x1 = center.x-d*1.3;
//        x2 = center.x+d*1.5;
//    }
    y1 = center.y-d*1.50;
    y2 = center.y+d*1.50;
    cout<<"x:"<<x1<<" "<<x2<<" ";
    cout<<"y:"<<y1<<" "<<y2<<endl;
    if(x1<=0) x1 = 1;
    if(x2>=cols) x2 = cols-1;
    if(y1<=0) y1 = 1;
    if(y2>=rows) y2 = rows-1;
    roi = img(Range(y1,y2),Range(x1,x2));
    return roi;
}


/**
 * @brief find_armour::image_preprocess  图像预处理
 * @param mode  识别模式
 * @param src  input  输入图像
 * @param dst  output 输出图像
 */
void find_armour::image_preprocess(int mode,Mat src,Mat &dst)
{
//    Mat gray;
//    vector <Mat> planne;
//    Mat k = getStructuringElement(MORPH_RECT,Size(11,11));
//    Mat k1 = getStructuringElement(MORPH_RECT,Size(7,7));
    if(mode==2)
    {
        //    蓝色通道减去红色通道
        Mat gray;
        vector <Mat> planne;
//        Mat k = getStructuringElement(MORPH_RECT,Size(7,7));
//        Mat k1 = getStructuringElement(MORPH_RECT,Size(5,5));
        Mat k = getStructuringElement(MORPH_RECT,Size(11,11));
        Mat k1 = getStructuringElement(MORPH_RECT,Size(7,7));
        split(src,planne);
//        cvtColor(src,gray,CV_BGR2GRAY);
        threshold(planne[1],gray,100,255,THRESH_BINARY);
//        dilate(gray,gray,k);

        subtract(planne[0],planne[2],dst);

        threshold(dst,dst,100,255,THRESH_BINARY);
        dilate(dst,dst,k);
        dst = dst&gray;
        dilate(dst,dst,k1);
    }
    else if(mode==1)
    {

    }
    //        Mat gray;
    ////        Mat k = getStructuringElement(MORPH_RECT,Size(3,3));
    //        Mat k1 = getStructuringElement(MORPH_RECT,Size(7,7));
    //        cvtColor(src,gray,CV_BGR2GRAY);
    //        threshold(gray,gray,110,255,THRESH_BINARY);
    //        Mat img_blue;
    //        int iLowH, iHighH, iLowS, iHighS, iLowV, iHighV;

    //        fs["iLowH"]>>iLowH;
    //        fs["iHighH"]>>iHighH;
    //        fs["iLowS"]>>iLowS;
    //        fs["iHighS"]>>iHighS;
    //        fs["iLowV"]>>iLowV;
    //        fs["iHighV"]>>iHighV;

    //        cvtColor(src, img_blue, COLOR_BGR2HSV);
    //    //    vector<Mat> planes;
    //    //    split(img_blue,planes);
    //    //    planes[2]-=30;
    //    //    merge(planes,img_blue);
    //        inRange(img_blue, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), dst);

    //        //open and close operation.
    ////        Mat kernel = getStructuringElement(MORPH_RECT, Size(5, 5));
    ////        Mat kernel1 = getStructuringElement(MORPH_RECT, Size(3, 3));
    //    //    Mat kernel2 = getStructuringElement(MORPH_RECT, Size(11,11));

    //    //    morphologyEx(dst, dst, MORPH_CLOSE, kernel);

    //    //    morphologyEx(dst, dst, MORPH_OPEN, kernel1);
    //    //    morphologyEx(dst, dst, CV_MOP_OPEN, kernel1);
    //        dilate(dst,dst,k1);
    //    //    morphologyEx(dst, dst, CV_MOP_CLOSE, kernel1);
    //    //        erode(dst,dst,kernel);
    //        dst = dst&gray;
    //        dilate(dst,dst,k1);



    //    Mat gray;
    //    Mat k = getStructuringElement(MORPH_RECT,Size(3,3));
    //    Mat k1 = getStructuringElement(MORPH_RECT,Size(7,7));
    //    cvtColor(src,gray,CV_BGR2GRAY);
    //    imshow("gray",gray);
    //    threshold(gray,gray,110,255,THRESH_BINARY);
}


/**
 * @brief find_armour::find_blue4
 * @param img  input  输入相机原始图像
 * @param dst  output 输出图像
 * @param RRect output 输出旋转矩形，用于解算出角度
 * @param mode  input 输入指令
 * @return Mat
 */
void find_armour::get_armor(Mat& img,Mat& dst,int mode)
{
    Clear();
//    //判断是否切换命令
//    if(last_mode!=mode)
//    {//切换命令，清空所有数据
//        clear_data();
//        last_mode = mode;
//    }
    if(1)
    {
        cout<<"here"<<endl;
        clear_data();
        image_preprocess(mode,img,dst);  //图片预处理
        search_armour(img,dst);
        if(big_armour_center.size()!=0)
        {
            if(big_armour_center.size() == 1)
            {
                last_center = big_armour_center[0];
                last_d = big_diameters[0];
                Rotate_Point = big_Rotate_Points[0];
            }
            else
            {
                int n = 0;
                vector<Point2f> temp_center = big_armour_center;
                sort(temp_center.begin(),temp_center.end(),SortArmorCenterY);  //降序
                if(temp_center[0].x>temp_center[1].x&&(temp_center[0].x-temp_center[1].x)<0.5*(temp_center[0].y-temp_center[1].y))
                {
                    temp_center[0] = temp_center[1];
                }
                for (int i = 1;i<big_armour_center.size();i++)
                {
                    if(temp_center[0]==big_armour_center[i])
                    {
                        n = i;
                        break;
                    }
                }
                last_center = big_armour_center[n];
                last_d = big_diameters[n];
                Rotate_Point = big_Rotate_Points[n];
            }
            circle(img,last_center,40,Scalar(255,0,0),5);
            isROIflag = 1;
            isfind = 1;
        }
        else if(armour_center.size()==0)
        {
            isROIflag = 0;
            isfind = 0;
        }
        else if(armour_center.size()==1)
        {
            last_center = armour_center[0];
            last_d = diameters[0];
            circle(img,last_center,40,Scalar(255,0,0),5);
            Rotate_Point = Rotate_Points[0];
            isROIflag = 1;
            isfind = 1;
        }
        else
        {
            int n = 0;
            vector<Point2f> temp_center = armour_center;
            sort(temp_center.begin(),temp_center.end(),SortArmorCenterY);  //降序
            if(temp_center[0].x>temp_center[1].x&&(temp_center[0].x-temp_center[1].x)<0.5*(temp_center[0].y-temp_center[1].y))
            {
                temp_center[0] = temp_center[1];
            }
            for (int i = 1;i<armour_center.size();i++)
            {
                if(temp_center[0]==armour_center[i])
                {
                    n = i;
                    break;
                }
            }
            last_center = armour_center[n];
            last_d = diameters[n];
            Rotate_Point = Rotate_Points[n];
            circle(img,last_center,40,Scalar(255,0,0),5);
            isROIflag = 1;
            isfind = 1;
        }
    }
    else
    {
        //截取本帧图片，只对截图操作
        img_ROI = roi(img,last_center,last_d);
        image_preprocess(mode,img_ROI,dst);  //图片预处理
        search_armour(img,dst);
//        //当前截图区域没找到，以半径扩展
//        if(Armordatas.size()==0)
//        {

//            LastArmor.diameter = LastArmor.diameter*1.8;
//            if(x1==1||x2==img.cols-1||y1==1||y2==img.rows-1)
//            {
//                isROIflag = 0;
//            }
//        }


        if(big_armour_center.size()!=0)
        {
            if(big_armour_center.size() == 1)
            {
                last_center = big_armour_center[0];
                last_d = big_diameters[0];
                Rotate_Point = big_Rotate_Points[0];
            }
            else
            {
                int n = 0;
                vector<Point2f> temp_center = big_armour_center;
                sort(temp_center.begin(),temp_center.end(),SortArmorCenterY);  //降序
                if(temp_center[0].x>temp_center[1].x&&(temp_center[0].x-temp_center[1].x)<0.5*(temp_center[0].y-temp_center[1].y))
                {
                    temp_center[0] = temp_center[1];
                }
                for (int i = 1;i<big_armour_center.size();i++)
                {
                    if(temp_center[0]==big_armour_center[i])
                    {
                        n = i;
                        break;
                    }
                }
                last_center = big_armour_center[n];
                last_d = big_diameters[n];
                Rotate_Point = big_Rotate_Points[n];
            }
            circle(img,last_center,last_d/2,Scalar(255,0,0),5);
            isROIflag = 1;
            isfind = 1;
        }
        else if(armour_center.size()==0)
        {
            isROIflag = 0;
            isfind = 0;
        }
        else if(armour_center.size()==1)
        {
            last_center = armour_center[0];
            last_d = diameters[0];
            Rotate_Point = Rotate_Points[0];
            circle(img,last_center,40,Scalar(255,0,0),5);
            isROIflag = 1;
            isfind = 1;
        }
        else
        {
            int n = 0;
            vector<Point2f> temp_center = armour_center;
            sort(temp_center.begin(),temp_center.end(),SortArmorCenterY);  //降序
            if(temp_center[0].x>temp_center[1].x&&(temp_center[0].x-temp_center[1].x)<0.5*(temp_center[0].y-temp_center[1].y))
            {
                temp_center[0] = temp_center[1];
            }
            for (int i = 1;i<armour_center.size();i++)
            {
                if(temp_center[0]==armour_center[i])
                {
                    n = i;
                    break;
                }
            }
            last_center = armour_center[n];
            last_d = diameters[n];
            Rotate_Point = Rotate_Points[n];
            circle(img,last_center,40,Scalar(255,0,0));
            isROIflag = 1;
            isfind = 1;
        }
    }
#ifdef SHOW_DEBUG
    for (int i=0 ;i<armour_center.size();i++)
    {
        circle(img,armour_center[i],30,Scalar(0,0,255),5);
    }
    for (int i=0;i<big_armour_center.size();i++)
    {
        circle(img,big_armour_center[i],40,Scalar(0,255,0),5);
    }
#endif


//    cout<<"L:"<<last_center<<" "<<Rotate_Point.size()<<endl;
}

//    imshow("find",img);
//    imshow("dst",dst);


/**
 * @brief find_armour::get_Light   剔出领近的灯柱
 */
void find_armour::get_Light()
{
    size_t size = fir_armor.size();
    vector<RotatedRect> Groups;
    int cellmaxsize;
    if(fir_armor.size()<1) return;
    Groups.push_back(fir_armor[0]);
    cellmaxsize = fir_armor[0].size.height * fir_armor[0].size.width;
    if(cellmaxsize > 3500) cellmaxsize = 0;
    int maxsize;
    for(int i=1;i<size;i++){
        if(fir_armor[i].center.x - fir_armor[i-1].center.x <10){
            maxsize = fir_armor[i].size.height * fir_armor[i].size.width;
            if(maxsize > 3500) continue;
            if(maxsize > cellmaxsize) cellmaxsize = maxsize;
            Groups.push_back(fir_armor[i]);
        }else{
            Armorlists.push_back(Groups);\
            CellMaxs.push_back(cellmaxsize);
            cellmaxsize = 0;
            maxsize = 0;
            Groups.clear();
            //if(fir_armor[i].size.height * fir_armor[i].size.width > 2500) continue;
            Groups.push_back(fir_armor[i]);
            cellmaxsize = fir_armor[i].size.height * fir_armor[i].size.width;
        }
        //std::cout<<"max:"<<cellmaxsize<<std::endl;
        //sizescale = (float)fir_armor[i].size.height/(float)fir_armor[i].size.width;
        //std::cout<<"scale:"<<sizescale<<" width:"<<fir_armor[i].size.width<<std::endl;
    }
    Armorlists.push_back(Groups);\
    CellMaxs.push_back(cellmaxsize);
    size = Armorlists.size();
    for(int i=0;i<size;i++){
        int Gsize = Armorlists[i].size();
        int GroupMax = CellMaxs[i];
        if(GroupMax > 5){
            for(int j=0;j<Gsize;j++){
                maxsize = Armorlists[i][j].size.height * Armorlists[i][j].size.width;
                if(maxsize == GroupMax){
                    result_armor.push_back(Armorlists[i][j]);
                    RotatedRect r = Armorlists[i][j];
                    double real_h = r.size.height < r.size.width ? r.size.width : r.size.height;
                    Vec4f contour_para(0,0,0,0);
                    contour_para[0] = real_h;
                    contour_para[1] = r.center.x;
                    contour_para[2] = r.center.y;
                    contour_para[3] = r.angle;
                    contours_para.push_back(contour_para);
                }
            }
        }
    }
}

/**
 * @brief find_armour::src_get_armor  在原图（截图）中找装甲板
 */
void find_armour::src_get_armor()
{
    int size = result_armor.size();
    if(size < 2){
        return;
    }
    Point2f center_point1,center_point2;
    Point2f _pt[4],pt[4];
    vector<Point2f> VecPoint;
    float height1,height2;
    float angle1,angle2;
    float area1,area2,area_rate;
    float x1,x2;
    float y1,y2;
    float y_dist,x_dist,min_h,height_d,K,x2h_rate,/*angle_diff,*/max_h;
    float angle_of_Rotated,height_of_Rotated;
    if(size==2){
        cout<<"contours is 2"<<endl;
        height1 = contours_para[0][0];
        x1 = contours_para[0][1];
        y1 = contours_para[0][2];
        angle1 = fabs(contours_para[0][3]);
        center_point1 = result_armor[0].center;
        area1 = result_armor[0].size.height * result_armor[0].size.width;
        result_armor[0].points(_pt);
         /**
          * pt
          * 0 2
          * 1 3
        */
        if(angle1 > 50.0)
        {
            pt[0] = _pt[3];
            pt[1] = _pt[0];
        }
        else
        {
            pt[0] = _pt[2];
            pt[1] = _pt[3];
        }

        height2 = contours_para[1][0];
        x2 = contours_para[1][1];
        y2 = contours_para[1][2];
        angle2 = fabs(contours_para[1][3]);
        center_point2 = result_armor[1].center;
        area2 = result_armor[1].size.height * result_armor[1].size.width;

        float angle_d = fabs(angle2-angle1);
        y_dist = fabs(y2-y1);
        x_dist = x2-x1;
        min_h = min(height1,height2);
        max_h = max(height1,height2);
        height_d = fabs(height2-height1);
        if(center_point1.x != center_point2.x)
        {
            K = (center_point1.y - center_point2.y) / (center_point1.x - center_point2.x);
            //装甲板右边的点
            result_armor[1].points(_pt);
            if(angle2 > 50.0)
            {
                pt[2] = _pt[2];
                pt[3] = _pt[1];
            }else{
                pt[2] = _pt[1];
                pt[3] = _pt[0];
            }
            //面积比
            if(area1 > area2){
                area_rate = area1 / area2;
            }else{
                area_rate = area2 / area1;
            }
            angle_of_Rotated = MAX(Point_Angle(pt[0],pt[2]),Point_Angle(pt[1],pt[3]));//旋转矩形的角度
            height_of_Rotated = MAX(MAX(result_armor[0].size.width,result_armor[1].size.width),
                                    MAX(result_armor[0].size.height,result_armor[1].size.height)); //旋转矩形的
            x2h_rate = x_dist/height_of_Rotated;
            //get circle diameter
            float d=sqrt(pow(contours_para[0][1]-contours_para[1][1],2)
                    +pow(contours_para[0][2]-contours_para[1][2],2));
            float dh_rate = max(d/height1,d/height2);
            cout<<"angle_d:"<<angle_d<<" "<<angle2<<" "<<angle1<<" "<<"x2h_rate:"<<x2h_rate<<" "<<"dh_rate"<<dh_rate<<" "<<"area_rate"<<area_rate<<endl;
               cout<<"y_dist:"<<y_dist<<" "<<0.55*(height1+height2)<<"  "<<max_h<<" "<<"K:"<<fabs(K)<<" "<<"Rotate_angle:"<<angle_of_Rotated<<endl;
            if(y_dist<=max_h
                    &&(angle_d<6||angle_d>84)
                    &&((x2h_rate>=_x2h_rate_min1&&x2h_rate<=_x2h_rate_max1)||(x2h_rate>=_x2h_rate_min2&&x2h_rate<_x2h_rate_max2))
                    &&area_rate<_area_rate_min
                    &&fabs(K)<_k
                    &&angle_of_Rotated<_angle_of_Rotated)
            {
//#ifdef SHOW_DEBUG
                cout<<"FIR_IN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
//#endif
                Point center=Point2f((x1+x2)*0.5,(y1+y2)*0.5);
                VecPoint.push_back(pt[0]);
                VecPoint.push_back(pt[1]);
                VecPoint.push_back(pt[2]);
                VecPoint.push_back(pt[3]);
                if(x2h_rate>3.5)
                {// big_armor
                    big_armour_center.push_back(center);
                    big_diameters.push_back(d);
                    big_Rotate_Points.push_back(VecPoint);
                }
                else
                {//小装甲
                    diameters.push_back(d);
                    armour_center.push_back(center);
                    Rotate_Points.push_back(VecPoint);
                }
            }
            else if(y_dist<max_h
                    &&(angle_d<12||angle_d>=80)
                    &&((x2h_rate>=_x2h_rate_min1&&x2h_rate<=_x2h_rate_max1)||(x2h_rate>=_x2h_rate_min2&&x2h_rate<_x2h_rate_max2))
                    &&fabs(K)<_k
                    &&angle_of_Rotated<_angle_of_Rotated
                    &&area_rate<_area_rate_max
                    &&dh_rate<5)
            {
                cout<<"2--IN!!!!!!!!!!!!!!!!!!!!!!!!!!"<<x2h_rate<<endl;
                Point center=Point2f((x1+x2)*0.5,(y1+y2)*0.5);
                VecPoint.push_back(pt[0]);
                VecPoint.push_back(pt[1]);
                VecPoint.push_back(pt[2]);
                VecPoint.push_back(pt[3]);
                if(x2h_rate>3.5)
                {// big_armor
                    big_armour_center.push_back(center);
                    big_diameters.push_back(d);
                    big_Rotate_Points.push_back(VecPoint);
                }
                else
                {//小装甲
                    diameters.push_back(d);
                    armour_center.push_back(center);
                    Rotate_Points.push_back(VecPoint);
                }

            }
        }
    }
    else if(size>=3){
        cout<<"Size::"<<size<<endl;
        for(int i=0;i<size-1;i++)
        {
            height1 = contours_para[i][0];
            x1 = contours_para[i][1];
            y1 = contours_para[i][2];
            angle1 = fabs(contours_para[i][3]);
            center_point1 = result_armor[i].center;
            area1 = result_armor[i].size.height * result_armor[i].size.width;
            result_armor[i].points(_pt);
             /**
              * pt
              * 0 2
              * 1 3
            */
            if(angle1 > 50.0)
            {
                pt[0] = _pt[3];
                pt[1] = _pt[0];
            }
            else
            {
                pt[0] = _pt[2];
                pt[1] = _pt[3];
            }
            for(int j = i+1;j<size;j++)
            {
                height2 = contours_para[j][0];
                x2 = contours_para[j][1];
                y2 = contours_para[j][2];
                angle2 = fabs(contours_para[j][3]);

                center_point2 = result_armor[j].center;
                area2 = result_armor[j].size.height * result_armor[j].size.width;

                float angle_d = fabs(angle2-angle1);
                y_dist = fabs(y2-y1);
                x_dist = x2-x1;
                min_h = min(height1,height2);
                max_h = max(height1,height2);
                height_d = fabs(height2-height1);
                if(center_point1.x != center_point2.x)
                {
                    K = (center_point1.y - center_point2.y) / (center_point1.x - center_point2.x);

                    //装甲板右边的点
                    result_armor[j].points(_pt);
                    if(angle2 > 50.0)
                    {
                        pt[2] = _pt[2];
                        pt[3] = _pt[1];
                    }else{
                        pt[2] = _pt[1];
                        pt[3] = _pt[0];
                    }
                    //面积比
                    if(area1 > area2){
                        area_rate = area1 / area2;
                    }else{
                        area_rate = area2 / area1;
                    }
                    angle_of_Rotated = MAX(Point_Angle(pt[0],pt[2]),Point_Angle(pt[1],pt[3]));//旋转矩形的角度

                    height_of_Rotated = MAX(MAX(result_armor[i].size.width,result_armor[j].size.width),
                                            MAX(result_armor[i].size.height,result_armor[j].size.height));
                    x2h_rate = x_dist/height_of_Rotated;
                    //get circle diameterArmorPoints.push_back(armor_center);
                    float d=sqrt(pow(contours_para[i][1]-contours_para[j][1],2)
                            +pow(contours_para[i][2]-contours_para[j][2],2));
                    float max_angle = max(angle1,angle2),min_angle = min(angle1,angle2);

                    float dh_rate = max(d/height1,d/height2);

                    cout<<"angle_d:"<<angle_d<<" "<<angle2<<" "<<angle1<<" "<<"x2h_rate:"<<x2h_rate<<" "<<"dh_rate"<<dh_rate<<" "<<"area_rate"<<area_rate<<endl;
                       cout<<"y_dist:"<<y_dist<<" "<<0.55*(height1+height2)<<"  "<<max_h<<" "<<"K:"<<fabs(K)<<" "<<"Rotate_angle:"<<angle_of_Rotated<<endl;
//                    if((60<max_angle&&max_angle<=80&&min_angle>4)||(max_angle>83&&max_angle<88&&min_angle>6)||(max_angle>=80&&max_angle<=83&&min_angle>=9))
//                    {
//                        continue;
//                    }
                       if(angle_d>12&&/*&angle_d<20||angle_d>60&&*/angle_d<78)
                       {
                           cout<<"delect"<<endl;
                           continue;
                       }
                    if(isROIflag==0)
                    {
//                    cout<<"Rate::"<<x2h_rate<<" "<<fabs(K)<<" "<<endl;
//                    cout<<"IN?"<<endl;
//                        cout<<"Rate::"<<x2h_rate<<" "<<fabs(K)<<" "<<endl;
                        if((angle1==90||angle1==0)&&(angle2==90||angle2==0))
                        {
                            cout<<"90***********************************************"<<endl;
                            if(y_dist<=0.4*(height1+height2)/*&&((dh_rate>=0.8&&dh_rate<=3.0)||(dh_rate>=4.5&&dh_rate<6.5))*/
                                    &&((x2h_rate>=_x2h_rate_min1&&x2h_rate<=_x2h_rate_max1)||(x2h_rate>=_x2h_rate_min2&&x2h_rate<_x2h_rate_max2))
                                    &&area_rate<_area_rate_min&&fabs(K)<_k&&angle_of_Rotated<_angle_of_Rotated){
                                cout<<"in"<<endl;
                                Point center=Point2f((x1+x2)*0.5,(y1+y2)*0.5);
                                VecPoint.push_back(pt[0]);
                                VecPoint.push_back(pt[1]);
                                VecPoint.push_back(pt[2]);
                                VecPoint.push_back(pt[3]);
                                if(x2h_rate>3.5)
                                {// big_armor
                                    big_armour_center.push_back(center);
                                    big_diameters.push_back(d);
                                    big_Rotate_Points.push_back(VecPoint);
                                }
                                else
                                {//小装甲
                                    diameters.push_back(d);
                                    armour_center.push_back(center);
                                    Rotate_Points.push_back(VecPoint);
                                }
                            }

                        }
                        else if(y_dist<=0.55*(height1+height2)/*max_h*/&&(angle_d<6||angle_d>84)
                                &&((x2h_rate>=_x2h_rate_min1&&x2h_rate<=_x2h_rate_max1)||(x2h_rate>=_x2h_rate_min2&&x2h_rate<_x2h_rate_max2))
                                &&area_rate<_area_rate_min&&fabs(K)<_k&&angle_of_Rotated<_angle_of_Rotated)
                        {
#ifdef ADJUST_PARAMETERS
                            cout<<"size_FIR_IN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
#endif
                            cout<<"size_FIR_IN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
                            Point center=Point2f((x1+x2)*0.5,(y1+y2)*0.5);
                            VecPoint.push_back(pt[0]);
                            VecPoint.push_back(pt[1]);
                            VecPoint.push_back(pt[2]);
                            VecPoint.push_back(pt[3]);
                            if(x2h_rate>3.5)
                            {// big_armor
                                big_armour_center.push_back(center);
                                big_diameters.push_back(d);
                                big_Rotate_Points.push_back(VecPoint);
                            }
                            else
                            {//小装甲
                                diameters.push_back(d);
                                armour_center.push_back(center);
                                Rotate_Points.push_back(VecPoint);
                            }
                        }
                        else if(y_dist<=max_h&&(angle_d<20||angle_d>60)
                                &&((x2h_rate>=_x2h_rate_min1&&x2h_rate<=_x2h_rate_max1)||(x2h_rate>=_x2h_rate_min2&&x2h_rate<_x2h_rate_max2))
                               &&fabs(K)<_k&&angle_of_Rotated<30&&area_rate<3.0&&dh_rate<5/*&&height_d<0.45*(height1+height2)*/)
                        {
                            cout<<"dst_IN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
                            Point center=Point2f((x1+x2)*0.5,(y1+y2)*0.5);
                            VecPoint.push_back(pt[0]);
                            VecPoint.push_back(pt[1]);
                            VecPoint.push_back(pt[2]);
                            VecPoint.push_back(pt[3]);
                            if(x2h_rate>3.5)
                            {// big_armor
                                big_armour_center.push_back(center);
                                big_diameters.push_back(d);
                                big_Rotate_Points.push_back(VecPoint);
                            }
                            else
                            {//小装甲
                                diameters.push_back(d);
                                armour_center.push_back(center);
                                Rotate_Points.push_back(VecPoint);
                            }
                        }
                    }
                    else
                    {//size>3+截图
                        if((angle1==90||angle1==0)&&(angle2==90||angle2==0))
                        {
                            cout<<"90***********************************************"<<endl;
                            if(y_dist<=0.4*(height1+height2)/*&&((dh_rate>=0.8&&dh_rate<=2.5)||(dh_rate>=4.5&&dh_rate<6.5))*/
                                    &&((x2h_rate>=_x2h_rate_min1&&x2h_rate<=_x2h_rate_max1)||(x2h_rate>=_x2h_rate_min2&&x2h_rate<_x2h_rate_max2))
                                    &&area_rate<_area_rate_min&&fabs(K)<_k&&angle_of_Rotated<_angle_of_Rotated){
                                cout<<"in"<<endl;
                                Point center=Point2f((x1+x2)*0.5,(y1+y2)*0.5);
                                VecPoint.push_back(pt[0]);
                                VecPoint.push_back(pt[1]);
                                VecPoint.push_back(pt[2]);
                                VecPoint.push_back(pt[3]);
                                if(x2h_rate>3.5)
                                {// big_armor
                                    big_armour_center.push_back(center);
                                    big_diameters.push_back(d);
                                    big_Rotate_Points.push_back(VecPoint);
                                }
                                else
                                {//小装甲
                                    diameters.push_back(d);
                                    armour_center.push_back(center);
                                    Rotate_Points.push_back(VecPoint);
                                }
                            }

                        }
                        else if(y_dist<=0.55*(height1+height2)&&(angle_d<6||angle_d>84)
                                &&((x2h_rate>=_x2h_rate_min1&&x2h_rate<=_x2h_rate_max1)||(x2h_rate>=_x2h_rate_min2&&x2h_rate<_x2h_rate_max2))
                                &&area_rate<_area_rate_min
                                &&fabs(K)<_k&&angle_of_Rotated<30)
                        {
#ifdef ADJUST_PARAMETERS
                            cout<<"ROI_FIR_IN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
#endif
                            cout<<"ROI_FIR_IN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
                            Point center=Point2f((x1+x2)*0.5,(y1+y2)*0.5);
                            VecPoint.push_back(pt[0]);
                            VecPoint.push_back(pt[1]);
                            VecPoint.push_back(pt[2]);
                            VecPoint.push_back(pt[3]);
                            if(x2h_rate>3.5)
                            {// big_armor
                                big_armour_center.push_back(center);
                                big_diameters.push_back(d);
                                big_Rotate_Points.push_back(VecPoint);
                            }
                            else
                            {//小装甲
                                diameters.push_back(d);
                                armour_center.push_back(center);
                                Rotate_Points.push_back(VecPoint);
                            }
                        }
                        else if(y_dist<0.55*(height1+height2)
                                &&((x2h_rate>=_x2h_rate_min1&&x2h_rate<=_x2h_rate_max1)||(x2h_rate>=_x2h_rate_min2&&x2h_rate<_x2h_rate_max2))
                               &&fabs(K)<_k&&angle_of_Rotated<_angle_of_Rotated&&area_rate<_area_rate_max&&dh_rate<5&&height_d<0.5*max_h)
                        {
                            cout<<"ROI_IN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<x2h_rate<<endl;
                            Point center=Point2f((x1+x2)*0.5,(y1+y2)*0.5);
                            VecPoint.push_back(pt[0]);
                            VecPoint.push_back(pt[1]);
                            VecPoint.push_back(pt[2]);
                            VecPoint.push_back(pt[3]);
                            if(x2h_rate>3.5)
                            {// big_armor
                                big_armour_center.push_back(center);
                                big_diameters.push_back(d);
                                big_Rotate_Points.push_back(VecPoint);
                            }
                            else
                            {//小装甲
                                diameters.push_back(d);
                                armour_center.push_back(center);
                                Rotate_Points.push_back(VecPoint);
                            }
                        }
                    }
                }
            }
        }
    }
}

/**
 * @brief find_armour::search_armour  寻找装甲板的方法。分为截图和不截图两种情况
 * @param img input img
 * @param dst input dst
 * @param armour_center  output save center of armor
 * @param diameters      output save diameter of armor
 * @param flags          input  is a flag to get ROI or not
 * @param Rotated_angles    output save RotatedRect's Angle
 * @param Heights        output save Height
 */
void find_armour::search_armour(Mat& img,Mat& dst)
{
    vector<vector<Point> > contours;

    //findcontuors最后一个参数是偏移量，对于截图来说，巨好用
    findContours(dst,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE,Point(x1,y1));

    int num = contours.size();   //contour's amount

    for(int i = 0;i<num;i++)
    {
        RotatedRect r = minAreaRect(contours[i]);
        double real_h = r.size.height < r.size.width ? r.size.width : r.size.height;
        double real_w = r.size.height < r.size.width ? r.size.height : r.size.width;
        if(real_h<1.1*real_w) continue;
        //长宽分明时。筛去平躺的矩形
        if((r.size.height>r.size.width&&r.angle>-30)
            ||(r.size.height<r.size.width&&r.angle<-60))
        {
            Mat vertice;
            boxPoints(r,vertice);
            for(int k = 0;k<4;k++)
            {
                Point p1 = Point(vertice.row(k));
                int n = (k+1)%4;
                Point p2 = Point(vertice.row(n));
                line(img,p1,p2,Scalar(0,255,0),2);
            }
            fir_armor.push_back(r);
        }
    }
    num = fir_armor.size();
    if(num>=4){
        list<float> fir_armor_areas;
        for(int i = 0;i<num;i++)
        {
            fir_armor_areas.push_back(fir_armor[i].size.width*fir_armor[i].size.width);
        }
        fir_armor_areas.sort();
        fir_armor_areas.pop_front();
        while(fir_armor_areas.front()>fir_armor_areas.back()*10)
        {
            cout<<"去掉小矩形"<<endl;
            fir_armor_areas.pop_back();  //去掉小面积
        }
    }
    sort(fir_armor.begin(),fir_armor.end(),Sort_RotatedRect);
    get_Light();
//    cout<<result_armor.size()<<endl;
    sort(result_armor.begin(),result_armor.end(),Sort_RotatedRect);
    src_get_armor();
}


