#include "video.h"
#include "v4l2_set.h"
#include <thread>
#include <mutex>
#include <ImgFactory.h>

FileStorage fs("/home/mjs/RP/PP_Pro/canshu.yaml",FileStorage::READ);

/**
 * @brief set_camera_para 设置摄像头的参数
 * @param fd1  文件名的序号
 */
void set_camera_para(int fd1)
{
    v4l2_set vs(fd1);
//    vs.set_saturation(saturaion);      //饱和度
//    vs.set_exposure(exposure);     //曝光
//    vs.set_contrast(contrast);    //对比度
//    vs.set_gain(gain);         //增益
//    vs.set_brightness(brightness);   //亮度
//    vs.set_white_balance(white);    //白平衡
    cout<<"摄像头设置成功！"<<endl;
}

int main()
{
    int fd1 = open("/dev/video1",O_RDWR);
    set_camera_para(fd1);

    ImgFactory imgfactory;

    //多线程读图和处理图
    std::thread t1(&ImgFactory::Img_read,&imgfactory);
    std::thread t2(&ImgFactory::Img_handle,&imgfactory);

    t1.join();
    t2.join();

    return 0;
}
