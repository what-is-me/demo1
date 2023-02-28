// Third-party dependencies
#include <opencv2/opencv.hpp>
// Command-line user interface
#define OPENPOSE_FLAGS_DISABLE_POSE
#include <openpose/flags.hpp>
// OpenPose dependencies
#include <openpose/headers.hpp>

#include "2Dalgorithm.hpp"
#include "Configure.hpp"
#include "Object.hpp"
#include "PrintUtil.hpp"
#include "exception.hpp"
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
    try {
      const op::Matrix imageToProcess = OP_CV2OPCONSTMAT(cvImageToProcess);
      auto datumProcessed = opWrapper->emplaceAndPop(imageToProcess);
      if (datumProcessed != nullptr) {
        // printKeypoints(datumProcessed);
        std::map<std::string, cv::Point> person = bestPerson(datumProcessed);
        /* process the person */
        if (person.empty()) {
          throw Exception("Person Undetected.");
        }
        /* points we need */
        cv::Point neck = person["Neck"];
        cv::Point hip = person.count("MidHip")
                            ? person["MidHip"]
                            : center(person["LHip"], person["RHip"]);
        cv::Point Lknee = person["LKnee"];
        cv::Point Rknee = person["RKnee"];

        if (hip == ZERO || neck == ZERO || Lknee == ZERO || Rknee == ZERO) {
          throw Exception("The person not intact.");
        }

        float angle_body = fabs(angle(Line(neck, hip)));
        float angle_left_leg = fabs(angle(Lknee, hip, neck));
        float angle_right_leg = fabs(angle(Rknee, hip, neck));
        float angle_legs = fabs(angle(Lknee, hip, Rknee));
        float angle_front_leg = std::min(angle_left_leg, angle_right_leg);

        op::opLog("body angle: " + std::to_string(angle_body),
                  op::Priority::High);
        op::opLog("angle between body and front leg: " +
                      std::to_string(angle_front_leg),
                  op::Priority::High);
        op::opLog("angle between legs: " + std::to_string(angle_legs),
                  op::Priority::High);

        /* display the person */
        if (!FLAGS_no_display)
          display(cvImageToProcess, {hip, neck, Lknee, Rknee},
                  {Line(hip, neck), Line(hip, Lknee), Line(hip, Rknee)});
      } else
        op::opLog("Image could not be processed.", op::Priority::High);
    } catch (const Warning& e) {
    } catch (const std::exception& e) {
      display(cvImageToProcess);
      op::opLog(e.what(), op::Priority::High);
    }
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

    cv::VideoCapture cap;
    cap.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
    /* if (argc > 1)
      cap.open(std::string(
          argv[1]));
    //"E:/VScode-projects/SchoolProject/demo1/medias/running.mp4" else
      cap.open(0);*/
    cap.open("medias/running.mp4");
    if (!cap.isOpened())  // �ж�����Ƿ��
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
      int key = cv::waitKey(0);  // wait 10 ms
      if (key == int('q')) {
        break;  // pause q and exit
      }
    }
    // system("pause");
    /* release the source */
    cap.release();
    cv::destroyAllWindows();

    return 0;
  } catch (const std::exception&) {
    return -1;
  }
}