#include "BanubaVideoFilter.h"
#include <sstream>
#include <utility>

namespace agora::extension {

        BanubaVideoFilter::BanubaVideoFilter(agora_refptr<BanubaVideoProcessor> videoProcessor) {
            m_video_processor = std::move(videoProcessor);
        }

        BanubaVideoFilter::~BanubaVideoFilter() = default;

        bool BanubaVideoFilter::adaptVideoFrame(const agora::media::base::VideoFrame &capturedFrame,
                                                agora::media::base::VideoFrame &adaptedFrame) {
            m_video_processor->process_frame(capturedFrame);
            adaptedFrame = capturedFrame;
            return true;
        }

        size_t BanubaVideoFilter::setProperty(const char *key, const void *buf,
                                              size_t buf_size) {
            std::string stringParameter((char *) buf);
            m_video_processor->set_parameter(key, stringParameter);
            return 0;
        }

        size_t BanubaVideoFilter::getProperty(const char *key, void *buf, size_t buf_size) {
            return 0;
        }

        void BanubaVideoFilter::setEnabled(bool enable) {
        }

        bool BanubaVideoFilter::isEnabled() {
            return true;
        }
    }
