#include <image_transport/image_transport.hpp>
#include "CameraApi.h"
#include <cv_bridge/cv_bridge.h>
#include <rclcpp/node_options.hpp>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <chrono>
#include "std_msgs/msg/string.hpp"
#include <rclcpp/node_options.hpp>
#include "rclcpp/rclcpp.hpp"

using namespace std::chrono_literals;

class MinimalPublisher : public rclcpp::Node
{
public:
  image_transport::Publisher show_image_publisher_;
  MinimalPublisher()
      : Node("minimal_publisher"), count_(0)
  {
    publisher_ = this->create_publisher<std_msgs::msg::String>("topic", 10);
    timer_ = this->create_wall_timer(
        500ms, std::bind(&MinimalPublisher::timer_callback, this));
    show_image_publisher_ = image_transport::create_publisher(this, "show_image");
  }

  std_msgs::msg::Header header;

  void imageProducer()
  {

    unsigned char *g_pRgbBuffer;
    int iCameraCounts = 1;
    int iStatus = -1;
    tSdkCameraDevInfo tCameraEnumList;
    int hCamera;
    tSdkCameraCapbility tCapability; //设备描述信息
    tSdkFrameHead sFrameInfo;
    BYTE *pbyBuffer;

    CameraSdkInit(1);

    //枚举设备，并建立设备列表
    iStatus = CameraEnumerateDevice(&tCameraEnumList, &iCameraCounts);
    if (iStatus != CAMERA_STATUS_SUCCESS)
    {

      exit(1);
    }
    //没有连接设备
    if (iCameraCounts == 0)
    {

      exit(1);
    }

    //相机初始化。初始化成功后，才能调用任何其他相机相关的操作接口
    iStatus = CameraInit(&tCameraEnumList, -1, -1, &hCamera);

    if (iStatus != CAMERA_STATUS_SUCCESS)
    {

      exit(1);
    }

    //获得相机的特性描述结构体。该结构体中包含了相机可设置的各种参数的范围信息。决定了相关函数的参数
    CameraGetCapability(hCamera, &tCapability);

    CameraSetAeState(hCamera, false);
    CameraSetExposureTime(hCamera, 9000);

    g_pRgbBuffer = (unsigned char *)malloc(
        tCapability.sResolutionRange.iHeightMax * tCapability.sResolutionRange.iWidthMax * 3);

    /*让SDK进入工作模式，开始接收来自相机发送的图像
        数据。如果当前相机是触发模式，则需要接收到
        触发帧以后才会更新图像。    */
    CameraPlay(hCamera);

    if (tCapability.sIspCapacity.bMonoSensor)
    {
      CameraSetIspOutFormat(hCamera, CAMERA_MEDIA_TYPE_MONO8);
    }
    else
    {
      CameraSetIspOutFormat(hCamera, CAMERA_MEDIA_TYPE_BGR8);
    }

    for (;;)
    {

      if (CameraGetImageBuffer(hCamera, &sFrameInfo, &pbyBuffer, 1000) == CAMERA_STATUS_SUCCESS)
      {
        uint32_t CameraTimeStampL = 0, CameraTimeStampH = 0;

        if (CameraGetFrameTimeStamp(hCamera, &CameraTimeStampL, &CameraTimeStampH) != CAMERA_STATUS_SUCCESS)
        {
        }

        CameraImageProcess(hCamera, pbyBuffer, g_pRgbBuffer, &sFrameInfo);

        cv::Mat originImage(
            cvSize(sFrameInfo.iWidth, sFrameInfo.iHeight),
            sFrameInfo.uiMediaType == CAMERA_MEDIA_TYPE_MONO8 ? CV_8UC1 : CV_8UC3,
            g_pRgbBuffer);

        // 统一做处理

        auto show_image_msg = cv_bridge::CvImage(header, "rgb8", originImage).toImageMsg();
        show_image_publisher_.publish(show_image_msg);

        // ROS_ERROR_STREAM(TrajectoryCalculation::shoot_velocity);

        //在成功调用CameraGetImageBuffer后，必须调用CameraReleaseImageBuffer来释放获得的buffer。
        //否则再次调用CameraGetImageBuffer时，程序将被挂起一直阻塞，直到其他线程中调用CameraReleaseImageBuffer来释放了buffer
      }
      else
      {
      }
    }
  }

private:
  void timer_callback()
  {
    auto message = std_msgs::msg::String();
    message.data = "Hello, world! " + std::to_string(count_++);
    RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", message.data.c_str());
    publisher_->publish(message);
  }
  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
  size_t count_;
};

int main(int argc, char *argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MinimalPublisher>());
  rclcpp::shutdown();
  return 0;
}
