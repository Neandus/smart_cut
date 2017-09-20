#include <iostream>
#include <stdexcept>

#include "peopledetector.h"
#include <opencv2/core.hpp>

const char* keys =
{
    "{ help h      |                     | print help message }"
    "{ image i     |                     | specify input image}"
    "{ directory d |                     | images directory}"
};


int main(int argc, char** argv)
{
    cv::CommandLineParser parser(argc, argv, keys);

    if (parser.has("help"))
    {
        std::cout << "\nThis program demonstrates the use of the HoG descriptor using\n"
            " HOGDescriptor::hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());\n";
        parser.printMessage();
        std::cout << "During execution:\n\tHit q or ESC key to quit.\n"
            "\tUsing OpenCV version " << CV_VERSION << "\n";
        return 0;
    }

    std::string pattern_glob = "";
    if (parser.has("directory"))
    {
        pattern_glob = parser.get<std::string>("directory");
    }
    else if (parser.has("image"))
    {
        pattern_glob = parser.get<std::string>("image");
    }

    std::vector<cv::String> filenames{};

    if (!pattern_glob.empty())
    {
        cv::String folder(pattern_glob);
        cv::glob(folder, filenames);
    }

     PeopleDetector Pd{filenames};
     Pd.run();

    return 0;
}
