#include "video.h"
#include <serialport.h>
float GetSpeed(float angle,float last_angle,float t,float&v)
{
    v = (angle - last_angle)/(t)*1000;
    v = v>30?30:v;
    v = v<-30?-30:v;
#ifdef SHOW_DEBUG
    cout<<"速度:"<<v<<endl;
#endif
}
video::video(string c)
{
    filename = c;
//    flag = f;
#ifdef CAMERA_DEBUG
    mode = 3;
#else
    mode = 0;
#endif
//    ismiddle = 0;
//    isfind = 0;
//    XY = Point2f(0,0);
}

video::video(int num,string c)
{
    n = num;
#ifdef CAMERA_DEBUG
    mode = 2;
#else
    mode = 0;
#endif
    filename = c;
//    flag = f;
}

#ifndef VIDEO_DEBUG
void video::camera_read_write()
{

    //solvePnP
    double camera_canshu[9] = {527.3444,0,337.5232,0,531.2206,254.4946,0,0,1};
    double dist_coeff[5] = {-0.4259,0.2928,-0.0106,-0.0031,0};
    Mat camera_matrix(3,3,CV_64FC1,camera_canshu);
    Mat dist_matrix(1,5,CV_64FC1,dist_coeff);
    AngleSolve ans(camera_matrix,dist_matrix,13.5,12.5,0,20,1000,1);
    double rot_c[] = {1,0,0,0,1,0,0,0,1};
    double tran_c[] = {0,0,0};
    Mat rot_martrix(3,3,CV_64FC1,rot_c);
    Mat tran_matrix(3,1,CV_64FC1,tran_c);
    ans.Relation_Camera_PTZ(rot_martrix,tran_matrix,0);

#ifdef KALMAN_OPEN
    const int stateNum = 4;
    const int measureNum = 4;
    KalmanFilter KF(stateNum,measureNum);   //初始化

    KF.transitionMatrix = (Mat_<float>(4,4)<<1,0,0.01,0,0,1,0,0.01,0,0,1,0,0,0,0,1);
    setIdentity(KF.measurementMatrix);   //H
//    setIdentity(KF.measurementNoiseCov,Scalar::all(1));  //R
    setIdentity(KF.processNoiseCov,Scalar(1));   //Q
    //R
    KF.measurementNoiseCov = (Mat_<float>(4,4)<<2000,0,0,0,0,2000,0,0,0,0,5000,0,0,0,0,5000);
    //P
    setIdentity(KF.errorCovPost,Scalar(1));
    //x(k-1)
    KF.statePost = (Mat_<float>(4,1)<<1,1,1,1);
    //z
    Mat measurement = Mat::zeros(measureNum,1,CV_32F);
#endif
#ifdef KALMAN_2
    const int stateNum2 = 2;
    const int measureNum2 = 1;
    KalmanFilter KF2(stateNum2,measureNum2);   //初始化

    KF2.transitionMatrix = (Mat_<float>(2,2)<<1,1,0,1);
    setIdentity(KF2.measurementMatrix);
    setIdentity(KF2.measurementNoiseCov,Scalar::all(1));  //R
    setIdentity(KF2.processNoiseCov,Scalar(10e-3));   //Q
    setIdentity(KF2.errorCovPost,Scalar(1));
    KF2.statePost = (Mat_<float>(2,1)<<1,1);
    Mat measurement2 = Mat::zeros(measureNum2,1,CV_32F);
#endif
    //各个类的初始化
#ifdef OPEN_SERIAL
    SerialPort sp;
    sp.initSerialPort();
#endif
    VisionData data = {0,0,0,0,0};
    VisionData receive_data = {0,0,0,0,0};
    RotatedRect RRect;
    find_armour f_armour(fs);
    VideoCapture camera0(0);
#ifdef F640
    //设置摄像头分辨率为1280x720
    camera0.set(CV_CAP_PROP_FRAME_WIDTH,640);
    camera0.set(CV_CAP_PROP_FRAME_HEIGHT,480);
#else
    camera0.set(CV_CAP_PROP_FRAME_WIDTH,1280);
    camera0.set(CV_CAP_PROP_FRAME_HEIGHT,720);
#endif
//    VideoWriter writer(filename, CV_FOURCC('M', 'J', 'P', 'G'), 10, Size(640, 480),true);
    if (!camera0.isOpened())
    {
        cout << "Failed!"<<endl;
    }

    float t=0;
    float last_time = 0;
    int numofpic = 0;
    Mat camera_location;
    float vx=0,vy=0;
    KFparam kp = {0,0,0};
    float firstfind_t = 0,nowfind_t = 0,firnotfind_t = 0,nownotfind_t = 0;
    int isfirstfind = 1;
    int isfirnotfind = 0;
    int isreceiveflag = 0;   //是否接收到数据
    vector<float> history_yaw_offset(2);
#ifdef SHOW_DEBUG
    cout<<"history"<<history_yaw_offset[0]<<","<<history_yaw_offset[1]<<endl;
#endif
    last_time = getTickCount();
    int num = 0;
    float sum_yaw_offset = 0;   //当前云台的总偏移量（相对第一次识别到的偏移）
    while (1)
    {
        t = getTickCount();
        float delta_t = (t-last_time)/getTickFrequency()*1000;   //获取两帧之间的时间差
        last_time = t;
#ifdef SHOW_DEBUG
        cout<<delta_t<<"ms"<<endl;
#endif
        Mat frame;
        camera0 >> frame;
        if (frame.empty())
        {
            cout<<"No frame!"<<endl;
            break;
        }
//        writer<<frame;     //写入视频文件
        imshow("src",frame);

        Mat dst = Mat::zeros(frame.size(), CV_8UC1);
#ifdef OPEN_SERIAL
        sp.get_Mode(mode,receive_data,isreceiveflag);
#endif

        data.isfind = f_armour.isfind;
        if(mode == 0)
        {
            data.isfind = 0;
            data.pitch_angle.f = 0;
            data.yaw_angle.f = 0;
            isfirstfind = 1;
            isfirnotfind = 0;
#ifdef KALMAN_OPEN
            history_yaw_offset.clear();
            history_yaw_offset.push_back(0);
            history_yaw_offset.push_back(0);
            num = 0;
#endif
        }
        else dst = f_armour.find_blue4(frame,dst,RRect,mode);

        double xAngle=0,yAngle=0,dis=0;
        if( data.isfind == 1)
        {
            if (ans.Rotated_SolveAngle(RRect,xAngle,yAngle,dis,camera_location,20,0,Point2f(0,0)))  //结算角度
            {
                //第一次接收数据标志
                if (isreceiveflag == 1)
                {
                    isreceiveflag = 0;
                }
                //第一次找到目标标志
                if (isfirstfind == 1)
                {
                    firstfind_t = getTickCount();
                    isfirstfind = 0;
                    isfirnotfind = 1;
                }
                nowfind_t = getTickCount();
#ifdef KALMAN_OPEN
                //1-2
                Mat prediction = KF.predict();
                float pre_xAngle = prediction.at<float>(0);
                float pre_yAngle = prediction.at<float>(1);

                //get speed
                GetSpeed(pre_xAngle,kp.last_xAngle,delta_t,vx);
                kp.last_xAngle = pre_xAngle;
                GetSpeed(pre_yAngle,kp.last_yAngle,delta_t,vy);
                kp.last_yAngle = pre_yAngle;


                //3-4
                measurement.at<float>(0) = (float)xAngle;
                measurement.at<float>(1) = (float)yAngle+sum_yaw_offset;
                measurement.at<float>(2) = 0;
                measurement.at<float>(3) = vy;
                //5
                KF.correct(measurement);
                if((nowfind_t-firstfind_t)/getTickFrequency()*1000>=1000)   //1000ms之后再发送预测值
                {
                    data.pitch_angle.f = xAngle;//-KF.statePost.at<float>(0))*10+KF.statePost.at<float>(0);
                    data.yaw_angle.f = KF.statePost.at<float>(1)-sum_yaw_offset+KF.statePost.at<float>(3)*delta_t/1000;
                    data.dis.f = yAngle;
                }
                else
                {
#ifdef SHOW_DEBUG
                    cout<<"发送当前值的帧数:"<<num++<<endl;
#endif
                    data.pitch_angle.f = xAngle;//-KF.statePost.at<float>(0))*10+KF.statePost.at<float>(0);
                    data.yaw_angle.f = yAngle;
                    data.dis.f = yAngle;
                }
                history_yaw_offset.erase(history_yaw_offset.begin());   //清除已经被使用的第一个数据
                history_yaw_offset.push_back(data.yaw_angle.f);    //加入新传输的数据
#ifdef SHOW_DEBUG
                cout<<"当前历史数据中的第一个值:"<<history_yaw_offset[0]<<endl;
#endif
#else
                data.pitch_angle.f = xAngle;
                data.yaw_angle.f =  yAngle;
                data.dis.f = dis;
#endif
#ifdef KALMAN_2
                //1-2
                Mat prediction2 = KF2.predict();
                //3-4
                measurement2.at<float>(0) = data.yaw_angle.f;
                //5
                KF2.correct(measurement2);
                data.yaw_angle.f = KF2.statePost.at<float>(0);
#endif
            }
            else
            {
                data.pitch_angle.f = 0;
                data.yaw_angle.f =  0;
                data.dis.f = 0;
                history_yaw_offset.erase(history_yaw_offset.begin());   //清除已经被使用的第一个数据
                history_yaw_offset.push_back(data.yaw_angle.f);    //加入新传输的数据
            }
        }
        else if(data.isfind==0)
        {
            if(isfirnotfind == 1)
            {
                firnotfind_t = getTickCount();
                isfirnotfind = 0;
                nowfind_t = 0;
            }
            nownotfind_t = getTickCount();
            numofpic = 0;
            isfirstfind = 1;
            sum_yaw_offset = 0;  //清零
            history_yaw_offset.clear(); //清空
            history_yaw_offset.push_back(0);
            history_yaw_offset.push_back(0);
            num = 0;
            data.pitch_angle.f = 0;
            data.yaw_angle.f = 0;
            data.dis.f = 0;
        }
        else
        {
            data.pitch_angle.f = 0;
            data.yaw_angle.f = 0;\
            data.dis.f = 0;
        }

#ifdef OPEN_SERIAL
        sp.TransformData(data);
#endif
        int i = waitKey(1);
        if( i=='q') break;
    }
    sp.Close();
    camera0.release();
}
#else

void video::file_read()
{
    FileStorage fs("canshu.yaml",FileStorage::READ);

    find_armour f_armour;
    VideoCapture camera0;

    VisionData data = {0,0,0,0,0};

    camera0.open(filename);
    if (!camera0.isOpened())
    {
        cout << "Failed!"<<endl;
    }


    while (1)
    {
        RotatedRect RRect;

        QTime time;
        time.start();
        Mat frame;
        double t1=0,t2=0;
        t1 = getTickCount();
        camera0 >> frame;
        if (frame.empty()) break;

//        imshow("src",frame);

//        int ismiddle,isfind;
//        Point XY;
        Mat dst = Mat::zeros(frame.size(), CV_8UC1);
        Mat img = Mat::zeros(frame.size(), frame.type());
//        if(flag==1) dst = f_armour.find_blue1(frame,dst.clone());
        if(mode == 0)
        {
            ;
        }
//        else if (mode==3){
//            img = f_armour.camshift_findarmor(frame,dst);
//            imshow("img",img);
//        }
        t2 = getTickCount();
//        double fps = (t2-t1)/getTickFrequency()*1000;
//        cout<<"time:"<<fps<<"ms"<<endl;
//        qDebug()<<"time:"<<time.elapsed()<<"ms";//输出计时

        int i = waitKey(100);
        if( i=='q') break;
    }
}
#endif //VIDEO_DEBUG
