#include "peopledetector.h"

#include <iostream>
#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

namespace {


static void nms(std::vector<cv::Rect> & boxes, double const nmsTreshold)
{
    // if there any box inside
    if (boxes.size() > 0)
    {
        // initialize the list of picked indexes
//    std::vector<cv::Rect> pick{};

//    std::copy(boxes.begin(), boxes.end(), pick.begin());

    std::sort(boxes.begin(), boxes.end(), [](const cv::Rect & first, const cv::Rect & second){
        return (first.br().y < second.br().y);
    });

    for(auto p : boxes)
    {
        std::cerr << "[" << p.tl().x << " " << p.tl().y << " " << p.br().x << " " << p.br().y << "]\n";
    }




    //// grab the coordinates of the bounding boxes
    //x1 = boxes[:,0]
    //y1 = boxes[:,1]
    //x2 = boxes[:,2]
    //y2 = boxes[:,3]

    //// compute the area of the bounding boxes and sort the bounding
    //// boxes by the bottom-right y-coordinate of the bounding box
    //area = (x2 - x1 + 1) * (y2 - y1 + 1)
    //idxs = np.argsort(y2)

    //// keep looping while some indexes still remain in the indexes
    //// list
    //while len(idxs) > 0:
    //    // grab the last index in the indexes list, add the index
    //    // value to the list of picked indexes, then initialize
    //    // the suppression list (i.e. indexes that will be deleted)
    //    // using the last index
    //    last = len(idxs) - 1
    //    i = idxs[last]
    //    pick.append(i)
    //    suppress = [last]

    //    // loop over all indexes in the indexes list
    //    for pos in xrange(0, last):
    //        // grab the current index
    //        j = idxs[pos]

    //                // find the largest (x, y) coordinates for the start of
    //        // the bounding box and the smallest (x, y) coordinates
    //        // for the end of the bounding box
    //        xx1 = max(x1[i], x1[j])
    //        yy1 = max(y1[i], y1[j])
    //        xx2 = min(x2[i], x2[j])
    //        yy2 = min(y2[i], y2[j])

    //        // compute the width and height of the bounding box
    //        w = max(0, xx2 - xx1 + 1)
    //        h = max(0, yy2 - yy1 + 1)

    //        // compute the ratio of overlap between the computed
    //        // bounding box and the bounding box in the area list
    //        overlap = float(w * h) / area[j]

    //        // if there is sufficient overlap, suppress the
    //        // current bounding box
    //        if overlap > overlapThresh:
    //            suppress.append(pos)

    //    // delete all indexes from the index list that are in the
    //    // suppression list
    //    idxs = np.delete(idxs, suppress)

    }
    else
    {
        std::cerr << "Boxes vector empty...sorry, not much work here xD\n";
    }
}

static void detectAndDraw(const cv::HOGDescriptor &hog, cv::Mat &img)
{
    std::vector<cv::Rect> found, found_filtered;
    double t = (double) cv::getTickCount();
    // Run the detector with default parameters. to get a higher hit-rate
    // (and more false alarms, respectively), decrease the hitThreshold and
    // groupThreshold (set groupThreshold to 0 to turn off the grouping completely).
    hog.detectMultiScale(img, found, 0, cv::Size(8,8), cv::Size(32,32), 1.05, 2);
    t = (double) cv::getTickCount() - t;
    std::cout << "detection time = " << (t*1000./cv::getTickFrequency()) << " ms\n";




    //non maxima supression
    nms(found, 0.3);


    //old function
    {
        for(size_t i = 0; i < found.size(); i++ )
        {
            cv::Rect r = found[i];

            size_t j;
            // Do not add small detections inside a bigger detection.
            for ( j = 0; j < found.size(); j++ )
                if ( j != i && (r & found[j]) == r )
                    break;

            if ( j == found.size() )
                found_filtered.push_back(r);
        }

        for (size_t i = 0; i < found_filtered.size(); i++)
        {
            cv::Rect r = found_filtered[i];

            // The HOG detector returns slightly larger rectangles than the real objects,
            // so we slightly shrink the rectangles to get a nicer output.
            r.x += cvRound(r.width*0.1);
            r.width = cvRound(r.width*0.8);
            r.y += cvRound(r.height*0.07);
            r.height = cvRound(r.height*0.8);
            cv::rectangle(img, r.tl(), r.br(), cv::Scalar(0,255,0), 3);
        }
    }
}

} //empty namespace


PeopleDetector::PeopleDetector(const std::vector<cv::String> & filenames)
 : mFilenames(filenames)
{

}

void PeopleDetector::run()
{
    std::cerr << "PeopleDetector::run\n";
    cv::HOGDescriptor hog;
    hog.setSVMDetector(cv::HOGDescriptor::getDefaultPeopleDetector());
    cv::namedWindow("people detector", 1);

    //Read from input image files
    cv::Mat frame;

    std::cerr << "mFilenames " << mFilenames.size() << "\n";
    for(auto & file : mFilenames)
    {
        //No more valid images
        if (checkImage(file, frame))
        {
            detectAndDraw(hog, frame);

            imshow("people detector", frame);
            int c = cv::waitKey( 0 ) & 255;
            (void)c;
        }
        else
        {
            frame.release();
        }
    }
}

bool PeopleDetector::checkImage(const cv::String & filename, cv::Mat & frame)
{
    // Read current image
    frame = cv::imread(filename);

    if (!frame.empty())
    {
        return true;
    }

    return false;
}
