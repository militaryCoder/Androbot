#include "../../include/cross_platform/pch.hpp"

const uint FRAME_WIDTH = 640;
const uint FRAME_HEIGHT = 480;

std::string PORT_NAME = "\\\\.\\COM20";

float *depthValues;

rs2::device getConnectedRealSenseDevice(rs2::context &ctx);

template<typename T>
void writeDistanceDataToArray(const rs2::depth_frame & depthFrame, T array);

template<typename CoordinatesType, typename ValueType>
std::pair<CoordinatesType, CoordinatesType> getNearestPointCoordinates(ValueType *source);

int main(int argc, char **argv)
{
    depthValues = new float[FRAME_WIDTH * FRAME_HEIGHT];

    rs2::context context;
    rs2::pipeline pipe;
    
    rs2::device connectedDevice = getConnectedRealSenseDevice(context);

    pipe.start();

    rs2::frameset frames = pipe.wait_for_frames();
    rs2::depth_frame depthFrame{ 0 };

    return 0;
}