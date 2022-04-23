#ifndef RECOGNITION_HPP
#define RECOGNITION_HPP

#include <algorithm>
#include <iostream>
#include <regex>
#include <vector>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <boost/locale.hpp>
#include <leptonica/allheaders.h>
#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>

namespace ocr
{
    class receipt
    {

    public:
        enum class shop
        {
            edeka,
            rewe,
            aldi,
            unknown
        };

        enum class iterator
        {
            block,
            line,
            word
        };

        struct detection
        {
            int id;
            int x;
            int y;
            int w;
            int h;
            int conf;
            std::string text;
        };

        struct article
        {
            std::string name;
            float price;
        };

        receipt(const std::string &path);
        ~receipt();

        void init();
        void preprocess();
        std::vector<detection> extract(iterator level = iterator::line);
        void overlay(std::vector<detection> detections);
        std::vector<article> process(std::vector<detection> detections);

        friend std::ostream &operator<<(std::ostream &os, const shop &s)
        {
            std::map<shop, std::string> shops{
                {shop::edeka, "edeka"},
                {shop::rewe, "rewe"},
                {shop::aldi, "aldi"},
                {shop::unknown, "unknown"},
            };
            return os << shops[s];
        }

        friend std::ostream &operator<<(std::ostream &os, const detection &d)
        {
            return os << boost::format("Box[%d]: x=%d, y=%d, w=%d, h=%d, conf: %d, text: %s") % d.id % d.x % d.y % d.w % d.h % d.conf % d.text << std::endl;
        }

        friend std::ostream &operator<<(std::ostream &os, const article &a)
        {
            return os << boost::format("Article=%s, Price=%.2f") % a.name % a.price << std::endl;
        }

        shop m_shop;

    private:
        std::string m_path;
        Pix *m_img_pix;
        cv::Mat m_img_cv;
        tesseract::TessBaseAPI *m_api;
    };

} // namespace ocr

#endif // RECOGNITION_HPP
