#include "../../include/cross_platform/pch.hpp"

const uint FRAME_WIDTH = 640;
const uint FRAME_HEIGHT = 480;

std::string PORT_NAME = "\\\\.\\COM20";
std::string WINDOW_NAME = "Render viewport";

float *depthValues;

rs2::device getConnectedRealSenseDevice(rs2::context &ctx);

template<typename ValueType>
void writeDistanceDataToArray(const rs2::depth_frame &depthFrame, ValueType array);

template<typename CoordinatesType, typename ValueType>
std::pair<CoordinatesType, CoordinatesType> getNearestPointCoordinates(ValueType *source);

template<typename ValueType>
ValueType getShortestDistance(ValueType *source);

template<typename ValueType>
cimg_library::CImg<ValueType> copyDistanceDataToImage(ValueType *src);


int main(int argc, char **argv)
{
    depthValues = new float[FRAME_WIDTH * FRAME_HEIGHT];

    rs2::context context;
    rs2::pipeline pipe;
    
    rs2::device connectedDevice = getConnectedRealSenseDevice(context);

    pipe.start();

    rs2::frameset frames = pipe.wait_for_frames();
    rs2::depth_frame depthFrame{ 0 };

    cimg_library::CImg<float> renderedViewport(FRAME_WIDTH, FRAME_HEIGHT);

    cimg_library::CImgDisplay viewport(renderedViewport, WINDOW_NAME.c_str());

    while (!viewport.is_closed())
    {
        writeDistanceDataToArray(depthFrame, depthValues);
        renderedViewport = copyDistanceDataToImage(depthValues);



        std::pair<int, int> nearestPointCoords = getNearestPointCoordinates<int, float>(depthValues);

        std::string xCoordSent = std::to_string(nearestPointCoords.first);
        std::string yCoordSent = std::to_string(nearestPointCoords.second);

        // TODO: Make SerialPort access independent from OS
    }

    pipe.stop();

    delete[] depthValues;

    return 0;
}

template<typename ValueType>
void writeDistanceDataToArray(const rs2::depth_frame &depthFrame, ValueType array)
{
    for (uint y = 0; y < FRAME_HEIGHT; y++)
    {
        for (uint x = 0; x < FRAME_WIDTH; x++)
        {
            array[x + y * FRAME_WIDTH] = depthFrame.get_distance(x, y);
        }
    }
}

template<typename CoordinatesType, typename ValueType>
std::pair<CoordinatesType, CoordinatesType> getNearestPointCoordinates(ValueType *source)
{
    std::pair<CoordinatesType, CoordinatesType> coords;
    float nearestPointValue = 1.0f;

    for (uint y = 0; y < FRAME_HEIGHT; y++)
    {
        for (uint x = 1; x < FRAME_WIDTH; x++)
        {
            float currentCheckedPoint = source[x + y * FRAME_WIDTH];

            if (currentCheckedPoint < nearestPointValue && currentCheckedPoint != 0)
            {
                nearestPointValue = currentCheckedPoint;
                coords = std::pair<int, int>{ x, y };
            }
        }
    }

    return coords;
}

template<typename ValueType>
ValueType getShortestDistance(ValueType *source)
{
    ValueType shortestDist = 1.0f;

    for (uint y = 0; y < FRAME_HEIGHT; y++)
    {
        for (uint x = 0; x < FRAME_WIDTH; x++)
        {
            ValueType currentCheckedPoint = source[x + y * FRAME_WIDTH];

            if (currentCheckedPoint < shortestDist && currentCheckedPoint != 0)
            {
                shortestDist = currentCheckedPoint;
            }
        }
    }

    return shortestDist;
}

template<typename ValueType>
cimg_library::CImg<ValueType> copyDistanceDataToImage(ValueType *src)
{
    cimg_library::CImg<ValueType> image(src, FRAME_WIDTH, FRAME_HEIGHT, 1);

    return image;
}

rs2::device getConnectedRealSenseDevice(rs2::context &ctx)
{
    rs2::device_list devList = ctx.query_devices();

    if (0 == devList.size())
    {
        throw std::runtime_error("No devices connected.");
    }

    return devList.front();
}