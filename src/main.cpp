#include <iostream>

#include <librealsense2/rs.hpp>
#include<SFML/Graphics.hpp>

#include "color.hpp"

typedef unsigned int uint;

static bool running = true;

const uint FRAME_WIDTH = 640;
const uint FRAME_HEIGHT = 480;

const char PORT_NAME[] = "\\\\.\\COM3";

struct Coordinate2d
{
    uint x;
    uint y;
};

Coordinate2d getProximatePointCoordinates(const rs2::depth_frame &df);

int main()
{
    rs2::context context;
    rs2::pipeline pipe;

    sf::RenderWindow viewport(sf::VideoMode(FRAME_WIDTH * 2, FRAME_HEIGHT * 2), "Viewport");

    try
    {
        rs2::device_list devList = context.query_devices();
        if (0 == devList.size())
        {
            throw std::runtime_error("No devices connected.");
        }

        rs2::device dev = devList.front();
    }
    catch(const std::runtime_error &e)
    {
        std::cerr << e.what() << '\n';
        return 0;
    }

    pipe.start();
 
    // Initial frameset as warmup
    rs2::frameset frames = pipe.wait_for_frames();
    rs2::depth_frame depthFrame { 0 };

    sf::Image depthMap;
    depthMap.create(FRAME_WIDTH * 2, FRAME_HEIGHT * 2);

    Timer<std::chrono::system_clock, int> timer;
    timer.start();

    while (viewport.isOpen())
    {
        frames = pipe.wait_for_frames(1000);
        depthFrame = frames.get_depth_frame();
        const Coordinate2d proximatePoint = getProximatePointCoordinates(depthFrame);

        if (std::chrono::duration_cast<std::chrono::milliseconds>(timer.getElapsedTime()).count() >= DEPTH_FRAME_RECEIVEMENT_TIMEOUT)
        {
            timer.reset();
            proximatePoint = getProximatePoint(depthFrame);
        }

        for (uint x = 0; x < FRAME_WIDTH; x++)
        {
            for (uint y = 0; y < FRAME_HEIGHT; y++)
            {
                const sf::Color col(static_cast<uint>(color::WHITE * depthFrame.get_distance(x, y)));
                
                for (uint dx = 0; dx < 2; dx++)
                {
                    for (uint dy = 0; dy < 2; dy++)
                    {
                        depthMap.setPixel(x*2 + dx, y*2 + dy, col);
                    }
                }
            }
        }

        for (uint x = proximatePoint.x - 5; x < proximatePoint.x + 5; ++x)
        {
            for (uint y = proximatePoint.y - 5; y < proximatePoint.y + 5; ++y)
            {
                depthMap.setPixel(x, y, sf::Color::Red);
            }
        }

        sf::Event e;
        while (viewport.pollEvent(e))
        {
            if (sf::Event::Closed == e.type)
            {
                viewport.close();
            }
        }

        sf::Texture txr;
        txr.loadFromImage(depthMap);
        sf::Sprite sprite(txr);

        viewport.clear();
        viewport.draw(sprite);

        viewport.display();

        std::cout << proximatePoint.x << ' ' << proximatePoint.y << '\n';
    }

    pipe.stop();

    return 0;
}

Coordinate2d getProximatePointCoordinates(const rs2::depth_frame &df)
{
    Coordinate2d c { 0, 0 };
    float nearestPointValue = 1.0f;

    for (uint y = 0; y < FRAME_HEIGHT; y++)
    {
        for (uint x = 1; x < FRAME_WIDTH; x++)
        {
            const float currentCheckedPointDepth = df.get_distance(x, y);

            if (currentCheckedPointDepth < nearestPointValue && currentCheckedPointDepth != 0)
            {
                nearestPointValue = currentCheckedPointDepth;
                c = Coordinate2d { x, y };
            }
        }
    }

    return c;
}