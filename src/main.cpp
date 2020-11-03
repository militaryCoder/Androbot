#include <iostream>
#include <cmath>

#include <librealsense2/rs.hpp>
#include <SFML/Graphics.hpp>

#include "color.hpp"
#include "Matrix.hpp"
#include "Point.hpp"
#include "Range.hpp"
#include "Timer.hpp"

typedef unsigned int uint;

const uint FRAME_WIDTH = 640u;
const uint FRAME_HEIGHT = 480u;

const uint DEPTH_FRAME_RECEIVEMENT_TIMEOUT = 1000u;

const char PORT_NAME[] = "\\\\.\\COM3";

template <typename T1, typename T2>
struct Pair
{
    T1 fst;
    T2 snd;
};

auto getProximatePoint(const rs2::depth_frame &df) -> Point<float>;
auto getProximateRegion(const rs2::depth_frame &df) -> Pair<Range<uint>, Range<uint>>;

template <typename ResT, typename ArgT>
auto matrixAverage(const Matrix<ArgT> &mat) -> ResT;

int main(int argc, char **argv)
{
    rs2::context context;
    rs2::pipeline pipe;

    sf::RenderWindow viewport(sf::VideoMode(FRAME_WIDTH, FRAME_HEIGHT), "Viewport");

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

    rs2::depth_frame depthFrame = frames.get_depth_frame();

    Pair<Range<uint>, Range<uint>> proximateRegion = getProximateRegion(depthFrame);
    Point<float> proximatePoint = getProximatePoint(depthFrame);

    sf::Image depthMap;
    // depthMap.create(FRAME_WIDTH * 2, FRAME_HEIGHT * 2);

    Timer<std::chrono::system_clock, int> timer;
    timer.start();

    while (viewport.isOpen())
    {
        frames = pipe.wait_for_frames();
        depthFrame = frames.get_depth_frame();

        if (std::chrono::duration_cast<std::chrono::milliseconds>(timer.getElapsedTime()).count() >= DEPTH_FRAME_RECEIVEMENT_TIMEOUT)
        {
            timer.reset();
            proximatePoint = getProximatePoint(depthFrame);
            proximateRegion = getProximateRegion(depthFrame);
        }
        
        for (uint x = 0; x < FRAME_WIDTH; x++)
        {
            for (uint y = 0; y < FRAME_HEIGHT; y++)
            {
                const float dist = depthFrame.get_distance(x, y);
                const sf::Color grayscale = sf::Color(255 * (1 - dist), 255 * (1 - dist), 255 * (1 - dist), 255);
                // const sf::Color color = sf::Color(255 * (1 - dist), 255 * (1 - dist*dist), 255 * dist, 255);
                
                for (uint dx = 0; dx < 2; dx++)
                {
                    for (uint dy = 0; dy < 2; dy++)
                    {
                        depthMap.setPixel(x*2 + dx, y*2 + dy, grayscale);
                    }
                }

                // Debug drawing
                //const float dist = depthFrame.get_distance(x, y);
                //for (uint dx = 0; dx < 2; dx++)
                //{
                //    for (uint dy = 0; dy < 2; dy++)
                //    {
                //        depthMap.setPixel(x*2 + dx, y*2 + dy, sf::Color(255*std::round(dist), 255*std::round(dist), 255*std::round(dist), 255));
                //    }
                //}
            }
        }

        // Drawing proximate region
        for (uint y = proximateRegion.snd.from; y <= proximateRegion.snd.to; y++)
        {
            for (uint x = proximateRegion.fst.from; x <= proximateRegion.fst.to; x++)
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
    }

    pipe.stop();

    return 0;
}

auto matrixFromDepthFrame(const rs2::depth_frame &df) -> Matrix<float>
{
    Matrix<float> res(FRAME_HEIGHT, FRAME_WIDTH);

    for (uint r = 0; r < FRAME_HEIGHT; r++)
    {
        for (uint c = 0; c < FRAME_WIDTH; c++)
        {
            res[r][c] = df.get_distance(c, r);
        }
    }

    return res;
}

auto getProximatePoint(const rs2::depth_frame &df) -> Point<float>
{
    const float EPS = 1e-4f;
    Point<float> p { 10.0f, { 0, 0 } };

    const Matrix<float> depthMatrix = matrixFromDepthFrame(df);

    const uint subMatDimension = 20;
    for (uint r = 0; r < FRAME_HEIGHT; r += subMatDimension)
    {
        for (uint c = 0; c < FRAME_WIDTH; c += subMatDimension)
        {
            const Matrix<float> subMat = depthMatrix.copy(r, c, r + subMatDimension, c + subMatDimension);
            const float averageDepth = matrixAverage<float>(subMat);

            if (averageDepth - p.value <= 0 - EPS)
            {
                p.value = averageDepth;
                p.c.x = c + subMatDimension / 2;
                p.c.y = r + subMatDimension / 2;
            }
        }
    }

    return p;
}

auto getProximateRegion(const rs2::depth_frame &df) -> Pair<Range<uint>, Range<uint>>
{
    const float TRESHOLD = 0.2f;
    const Matrix<float> depthMatrix = matrixFromDepthFrame(df);
    const uint subMatDimension = 40;

    uint startX = 0;
    uint endX = startX + subMatDimension - 1;
    uint startY = 0;
    uint endY = startX + subMatDimension - 1;

    float averageDepth = 10.0f;
    for (uint r = 0; r < FRAME_HEIGHT; r += subMatDimension)
    {
        for (uint c = 0; c < FRAME_WIDTH; c += subMatDimension)
        {
            const Matrix<float> subMat = depthMatrix.copy(r, c, r + subMatDimension, c + subMatDimension);
            const float subMatAverage = matrixAverage<float>(subMat);

            if (subMatAverage >= TRESHOLD && subMatAverage < averageDepth)
            {
                averageDepth = subMatAverage;
                startX = c;
                startY = r;
                endX = c + subMatDimension - 1;
                endY = r + subMatDimension - 1;
            }
        }
    }

    return { { startX, endX }, { startY, endY } };
}

template <typename ResT, typename ArgT>
auto matrixAverage(const Matrix<ArgT> &mat) -> ResT
{
    ResT sum = 0;

    for (uint c = 0; c < mat.columns; c++)
    {
        for (uint r = 0; r < mat.rows; r++)
        {
            sum += mat[r][c];
        }
    }

    return sum / (mat.rows * mat.columns);
}