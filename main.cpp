#include "pch.hpp"

std::ofstream runtimeLogFile("runtime_log.log");

RenderState *renderState = new RenderState();
Graphics *graphics = new Graphics();

bool running = true;

const uint FRAME_WIDTH = 640;
const uint FRAME_HEIGHT = 480;

float *depthValues = new float[FRAME_WIDTH * FRAME_HEIGHT];

RECT getClientRect(HWND hWnd)
{
    RECT rect;
    GetClientRect(hWnd, &rect);

    return rect;
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

template<typename T>
void writeDistanceDataToArray(const rs2::depth_frame &depthFrame, T array)
{
    for (uint y = 0; y < FRAME_HEIGHT; y++)
    {
        for (uint x = 0; x < FRAME_WIDTH; x++)
        {
            array[x + y * FRAME_WIDTH] = depthFrame.get_distance(x, y);
        }
    }
}

template<typename T, typename T1, typename T2>
std::pair<T1, T2> getNearestPointCoordinates(T *source)
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

    runtimeLogFile << nearestPointValue << "\n";
    return coords;
}

template<typename T>
void copyDistanceDataToImage(uint x, uint y, uint width, uint height, T *source)
{
    x = utils::clamp((uint)0, x, renderState->width);
    y = utils::clamp((uint)0, y, renderState->height);

    uint right = utils::clamp(x + width, (uint)0, renderState->width);
    uint bottom = utils::clamp(y + height, (uint)0, renderState->height);

    for (uint y_c = y; y_c < bottom; y_c++)
    {
        uint* currentPixel = (uint*)renderState->memory + x + y_c * width;

        for (uint x_c = x; x_c < right; x_c++)
        {
            float distanceValue = source[x_c + y_c * FRAME_WIDTH];

            *currentPixel++ = distanceValue * 200;
        }
    }
}

LRESULT CALLBACK windowCallback(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) try
{
    Clock clock;
    clock.start();

    WNDCLASS windowClass{};
    windowClass.style = CS_HREDRAW | CS_VREDRAW;
    windowClass.lpszClassName = L"RenderWindow";
    windowClass.lpfnWndProc = windowCallback;

    RegisterClass(&windowClass);

    DWORD dwStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE;

    HWND hWnd = CreateWindow(windowClass.lpszClassName, L"RenderWindow", dwStyle, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, 0, 0, hInstance, 0);
    HDC hdc = GetDC(hWnd);



    rs2::context context;
    rs2::pipeline pipe;
    
    rs2::device connectedDevice = getConnectedRealSenseDevice(context);


    std::time_t timeElapsed = clock.getElapsedTime();
    
    runtimeLogFile << "[" << timeElapsed << "] : " << "Device connected.\n";
    pipe.start();
    runtimeLogFile << "[" << clock.getElapsedTime() << "] : " << "Pipeline opened.\n";

    rs2::frameset frames = pipe.wait_for_frames();
    rs2::depth_frame depthFrame = frames.get_depth_frame();

    RECT windowRect = getClientRect(hWnd);

    uint windowWidth = windowRect.right - windowRect.left;
    uint windowHeight = windowRect.bottom - windowRect.top;

    renderState->width = windowWidth;
    renderState->width = windowHeight;

    graphics->setRenderState(renderState);

    while (running)
    {
        MSG message;
        while (PeekMessage(&message, hWnd, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&message);
            DispatchMessage(&message);
        }

        frames = pipe.wait_for_frames(100);
        depthFrame = frames.get_depth_frame();

        writeDistanceDataToArray(depthFrame, depthValues);
        copyDistanceDataToImage(0, 0, FRAME_WIDTH, FRAME_HEIGHT, depthValues);
        std::pair<int, int> nearestCoords = getNearestPointCoordinates<float, int, int>(depthValues);

        runtimeLogFile << "x: " << nearestCoords.first << "y: " << nearestCoords.second << "\n\n";

        StretchDIBits(hdc, 0, 0, windowWidth, windowHeight, 0, 0, FRAME_WIDTH, FRAME_HEIGHT, renderState->memory, &renderState->bitmapInfo, DIB_RGB_COLORS, SRCCOPY);
    }

    pipe.stop();
    
    runtimeLogFile << "[" << clock.getElapsedTime() << "] : " << "Pipeline closed.\n";

    delete graphics;
    delete renderState;
    delete[] depthValues;
}
catch (const std::runtime_error &err)
{
    runtimeLogFile << err.what() << "\n";
}

LRESULT CALLBACK windowCallback(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
    LRESULT result = NULL;

    switch (uMsg)
    {
        case WM_CLOSE:
        case WM_DESTROY:
        {
            running = false;
        } break;

        case WM_SIZE:
        {
            uint bufferSize = renderState->width * renderState->height * sizeof(uint);
            if (renderState->memory)
            {
                VirtualFree(renderState->memory, 0, MEM_RELEASE);
            }
            renderState->memory = VirtualAlloc(renderState->memory, bufferSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

            renderState->bitmapInfo.bmiHeader.biSize = sizeof(renderState->bitmapInfo.bmiHeader);
            renderState->bitmapInfo.bmiHeader.biWidth = FRAME_WIDTH;
            renderState->bitmapInfo.bmiHeader.biHeight = FRAME_HEIGHT;
            renderState->bitmapInfo.bmiHeader.biPlanes = 1;
            renderState->bitmapInfo.bmiHeader.biBitCount = 32;
            renderState->bitmapInfo.bmiHeader.biCompression = BI_RGB;
        } break;


        default:
        {
            result = DefWindowProc(hWnd, uMsg, wParam, lParam);
        }
    }

    return result;
}