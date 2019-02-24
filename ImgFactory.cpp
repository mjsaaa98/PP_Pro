#include "ImgFactory.h"


ImgFactory::ImgFactory(){
    filename = "/home/mjs/视频/飞机素材/2.avi";
    stop_pro = false;
    handle_flag = false;
#ifdef CAMERA_DEBUG
    mode = 2;
#else
    mode = 0;
#endif
    cout<<filename<<endl;
}

/**
 * @brief ImgFactory::Img_read  读取图像的一个线程
 */
void ImgFactory::Img_read(){
#ifdef VIDEO
    VideoCapture camera0;
    camera0.open(filename);
#else
    VideoCapture camera0(1);
    camera0.set(CV_CAP_PROP_FRAME_WIDTH,1280);
    camera0.set(CV_CAP_PROP_FRAME_HEIGHT,720);
#endif
    if (!camera0.isOpened())
    {
        cout << "Failed!"<<endl;
    }
    else cout<<"摄像头设置打开成功！"<<endl;

    while (1)
    {
//        double t1=0,t3 = 0;
//        t1 = getTickCount();
        camera0 >> frame;
//        t3 = getTickCount();
//        int fps_read = (t3-t1)/getTickFrequency()*1000;
//        cout<<"time_read:"<<fps_read<<"ms"<<endl;

        while(handle_flag==true);  //可以处理标志位还为真，说明图片还没被传进去处理，一直等待。
        //加锁,在列表插入值，同时禁止读取
        Lock.lock();
        list_of_frame.push_back(frame.clone());
        handle_flag = true;
        Lock.unlock();
    }
}

/**
 * @brief ImgFactory::Img_handle  处理图像线程
 */
void ImgFactory::Img_handle(){
    Mat camera_matrix;
    Mat dist_matrix;
    fs["camera_matrix"]>>camera_matrix;
    fs["dist_matrix"]>>dist_matrix;
    AngleSolve ans(camera_matrix,dist_matrix,13.5,12.5,0,20,1000,1);
    double rot_c[] = {1,0,0,0,1,0,0,0,1};
    double tran_c[] = {0,0,0};
    Mat rot_martrix(3,3,CV_64FC1,rot_c);
    Mat tran_matrix(3,1,CV_64FC1,tran_c);
    ans.Relation_Camera_PTZ(rot_martrix,tran_matrix,0);
#ifdef OPEN_SERIAL
    SerialPort sp;
    sp.initSerialPort();
#endif
    VisionData data = {0,0,0,0,0};
    find_armour f_armour;
//    float t=0;
    int numofpic = 0;

    Mat camera_location;
    while(1)
    {
        double t1=0,t3 = 0;
        t1 = getTickCount();
#ifdef OPEN_SERIAL
        sp.get_Mode(mode);
#endif
        Mat src;
        while(handle_flag==false);  //可以处理标志位还为假，说明图片还没读取完成，一直等待。
        Lock.lock();
        src = list_of_frame.back();
        list_of_frame.clear();
        handle_flag = false;
        Lock.unlock();

        Mat dst = Mat::zeros(src.size(), CV_8UC1);
        //mode为0,清空发送数据
        if(mode == 0)
        {
            data.dis.f = 0;
            data.isfind = 0;
            data.pitch_angle.f = 0;
            data.yaw_angle.f = 0;
        }
        else f_armour.get_armor(src,dst,mode);

        double xAngle=0,yAngle=0,dis=0;
        data.isfind = f_armour.isfind;
        if( data.isfind == 1)
        {
            if (ans.Rotated_SolveAngle(f_armour.Rotate_Point,xAngle,yAngle,dis,camera_location,20,0,Point2f(0,0)))  //结算角度
            {
                data.pitch_angle.f = xAngle;
                data.yaw_angle.f =  yAngle;
                data.dis.f = dis;
            }
            else
            {
                data.pitch_angle.f = 0;
                data.yaw_angle.f =  0;
                data.dis.f = 0;
            }
        }
        else if(data.isfind==0)
        {
//            if(isfirnotfind == 1)
//            {
//                firnotfind_t = getTickCount();
//                isfirnotfind = 0;
//                nowfind_t = 0;
//            }
//            nownotfind_t = getTickCount();
            numofpic = 0;
//            isfirstfind = 1;
            data.pitch_angle.f = 0;
            data.yaw_angle.f = 0;
            data.dis.f = 0;
        }
        else
        {
            data.pitch_angle.f = 0;
            data.yaw_angle.f = 0;
            data.dis.f = 0;
        }
#ifdef OPEN_SERIAL
        sp.TransformData(data);
#endif

        t3 = getTickCount();
        int fps_read = (t3-t1)/getTickFrequency()*1000;
        cout<<"time_pro:"<<fps_read<<"ms"<<endl;
        imshow("frame",src);
        imshow("dst",dst);
        int i = waitKey(0);
        if( i=='q') break;
    }
#ifdef OPEN_SERIAL
    sp.Close();
#endif
}
