#include <iostream>

#include <librealsense2/rs.hpp>

#include "utils.hpp"

typedef unsigned int uint;

static bool running = true;

const uint FRAME_WIDTH = 640;
const uint FRAME_HEIGHT = 480;

const char PORT_NAME[] = "\\\\.\\COM20";

template<typename T, typename T1, typename T2>
std::pair<T1, T2> getNearestPointCoordinates(T* source);

int main()
{
    float *depthValues = new float[FRAME_WIDTH * FRAME_HEIGHT];
    rs2::context context;
    rs2::pipeline pipe;
    
    try
    {
        rs2::device_list devList = context.query_devices();

        if (0 == devList.size())
        {
            throw std::runtime_error("No devices connected.");
        }

        rs2::device dev = devList.front();
    }
    catch(const std::runtime_error& e)
    {
        std::cerr << e.what() << '\n';
    }

    pipe.start();
 
    // Initial frameset as warmup
    rs2::frameset frames = pipe.wait_for_frames();
    rs2::depth_frame depthFrame{ 0 };

    while (!viewport.is_closed())
    {
        frames = pipe.wait_for_frames(100);
        depthFrame = frames.get_depth_frame();
        const std::pair<int, int> proximatePoint = getNearestPointCoordinates<float, int, int>(depthValues);

        std::cout << proximatePoint.first << ' ' << proximatePoint.second << '\n';
    }

    pipe.stop();
}

template<typename T, typename T1, typename T2>
std::pair<T1, T2> getNearestPointCoordinates(T* source)
{
    std::pair<T1, T2> coords;
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