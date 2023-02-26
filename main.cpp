// Third-party dependencies
#include <opencv2/opencv.hpp>
// Command-line user interface
#define OPENPOSE_FLAGS_DISABLE_POSE
#include <openpose/flags.hpp>
// OpenPose dependencies
#include <openpose/headers.hpp>
#include "2Dalgorithm.hpp"
#include "Object.hpp"
#include "PrintUtil.hpp"
#include "tags.hpp"
#include "utils.hpp"
// Custom OpenPose flags

// Producer
DEFINE_string(
    image_path,
    "E:/VScode-projects/OpenPose/examples/media/COCO_val2014_000000000192.jpg",
    "Process an image. Read all standard formats (jpg, png, bmp, etc.).");
// Display
DEFINE_bool(no_display, false, "Enable to disable the visual display.");
const cv::Point ZERO(0, 0);
class Processer {
 private:
  op::Wrapper* opWrapper;
  std::map<std::string, cv::Point> bestPerson(
      const std::shared_ptr<std::vector<std::shared_ptr<op::Datum>>>&
          datumsPtr) {
    int pos = 0;
    double maxgrade = 0;
    std::map<std::string, cv::Point> ret;
    try {
      if (datumsPtr != nullptr && !datumsPtr->empty()) {
        const auto& poseKeypoints = datumsPtr->at(0)->poseKeypoints;
        for (auto person = 0; person < poseKeypoints.getSize(0); person++) {
          double tmp = 0;
          for (auto bodyPart = 0; bodyPart < poseKeypoints.getSize(1);
               bodyPart++) {
            tmp += poseKeypoints[{person, bodyPart, 2}];
          }
          if (tmp > maxgrade) {
            maxgrade = tmp;
            pos = person;
          }
        }
        std::string* tags;
        if (poseKeypoints.getSize(1) == 18) {
          tags = tag18;
        } else {
          tags = tag25;
        }
        for (auto bodyPart = 0; bodyPart < poseKeypoints.getSize(1);
             bodyPart++) {
          ret[tags[bodyPart]] = {(int)poseKeypoints[{pos, bodyPart, 0}],
                                 (int)poseKeypoints[{pos, bodyPart, 1}]};
        }
      } else
        op::opLog("Nullptr or empty datumsPtr found.", op::Priority::High);
    } catch (const std::exception& e) {
      op::error(e.what(), __LINE__, __FUNCTION__, __FILE__);
    }
    return ret;
  }

 public:
  Processer() {
    // Configuring OpenPose
    op::opLog("Configuring OpenPose...", op::Priority::High);
    opWrapper = new op::Wrapper(op::ThreadManagerMode::Asynchronous);
    // Set to single-thread (for sequential processing and/or debugging and/or
    // reducing latency)
    if (FLAGS_disable_multi_thread)
      opWrapper->disableMultiThreading();

    // Starting OpenPose
    op::opLog("Starting thread(s)...", op::Priority::High);
    opWrapper->start();
  }
  void personPoseImage(const cv::Mat& cvImageToProcess) {
    const op::Matrix imageToProcess = OP_CV2OPCONSTMAT(cvImageToProcess);
    auto datumProcessed = opWrapper->emplaceAndPop(imageToProcess);
    if (datumProcessed != nullptr) {
      // printKeypoints(datumProcessed);
      std::map<std::string, cv::Point> person = bestPerson(datumProcessed);
      if (person.empty())
        std::cerr << "Person Undetected." << std::endl;
      cv::Point neck = person["Neck"];
      cv::Point hip = person.count("MidHip")
                          ? person["MidHip"]
                          : center(person["LHip"], person["RHip"]);
      // std::cout << neck << " " << hip << std::endl;
      if (neck == ZERO || hip == ZERO) {
        std::cerr << "The person not intact." << std::endl;
        display(cvImageToProcess);
        return;
      }
      std::cout << "angle:" << angle(Line(neck, hip)) << std::endl;
      if (!FLAGS_no_display)
        display(cvImageToProcess, {hip, neck}, {Line(hip, neck)});
    } else
      op::opLog("Image could not be processed.", op::Priority::High);
  }
  ~Processer() { delete opWrapper; }
};

int main(int argc, char* argv[]) {
  // Parsing command line flags
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  try {
    op::opLog("Starting OpenPose demo...", op::Priority::High);
    // const auto opTimer = op::getTimerInit();

    Processer processer;

    cv::VideoCapture cap;  // 声明相机捕获对象
    cap.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);  // 图像的宽，需要相机支持此宽
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);  // 图像的高，需要相机支持此高
    if (argc > 1)
      cap.open(std::string(
          argv[1]));  //"E:/VScode-projects/SchoolProject/demo1/medias/running.mp4"
    else
      cap.open(0);
    if (!cap.isOpened())  // 判断相机是否打开
    {
      std::cerr << "ERROR!!Unable to open camera\n";
      return -2;
    }
    // Process and display image
    cv::Mat cvImageToProcess;
    while (true) {
      cap >> cvImageToProcess;
      if (cvImageToProcess.empty()) {
        break;
      }

      processer.personPoseImage(cvImageToProcess);
      // Measuring total time
      // op::printTime(opTimer,
      //            "OpenPose demo successfully finished. Total time: ",
      //            " seconds.", op::Priority::High);
      int key = cv::waitKey(10);  // 等待10ms
      if (key == int('q')) {
        break;  // 按下q退出
      }
    }
    // system("pause");
    cap.release();            // 释放相机捕获对象
    cv::destroyAllWindows();  // 关闭所有窗口

    return 0;
  } catch (const std::exception&) {
    return -1;
  }
}