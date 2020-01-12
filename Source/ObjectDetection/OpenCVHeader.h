#pragma once

#pragma push_macro("check")
#undef check

#define int64 OpenCV_int64
#define uint64 OpenCV_uint64

#pragma warning(push)
#pragma warning(disable : 4946)

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"	
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/dnn/dnn.hpp"

#pragma warning(pop)

#undef int64
#undef uint64
#pragma pop_macro("check")