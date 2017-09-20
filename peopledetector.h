#ifndef PEOPLEDETECTOR_H
#define PEOPLEDETECTOR_H

#include <vector>

namespace cv {

class String;
class Mat;
}

class PeopleDetector
{
public:
    PeopleDetector(const std::vector<cv::String> & filenames);


    void run();

private:

    bool checkImage(const cv::String &, cv::Mat &);


    const std::vector<cv::String> & mFilenames;
};

#endif // PEOPLEDETECTOR_H
