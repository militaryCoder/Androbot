#include "pch.hpp"

std::ofstream runtimeLogFile("runtime_log.log");

RenderState *renderState = new RenderState();
Graphics *graphics = new Graphics();

bool running = true;

const unsigned int FRAME_WIDTH = 640;
const unsigned int FRAME_HEIGHT = 480;

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

void writeDistanceDataToArray(const rs2::depth_frame &depthFrame)
{
    for (unsigned int y = 0; y < FRAME_HEIGHT; y++)
    {
        for (unsigned int x = 0; x < FRAME_WIDTH; x++)
        {
            depthValues[x + y * FRAME_WIDTH] = depthFrame.get_distance(x, y);
        }
    }
}

int findNearestPointShift()
{
    int nearestPointX = 0;
    float nearestPointValue = depthValues[nearestPointX];

    for (unsigned int y = 0; y < FRAME_HEIGHT; y++)
    {
        for (unsigned int x = 1; x < FRAME_WIDTH; x++)
        {
            nearestPointX = (depthValues[x + y * FRAME_WIDTH] < nearestPointValue) ? x : nearestPointX;
        }
        
    }

    return nearestPointX - FRAME_WIDTH / 2;
}

void copyDistanceDataToImage(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
{
    x = utils::clamp((unsigned int)0, x, renderState->width);
    y = utils::clamp((unsigned int)0, y, renderState->height);

    unsigned int right = utils::clamp(x + width, (unsigned int)0, renderState->width);
    unsigned int bottom = utils::clamp(y + height, (unsigned int)0, renderState->height);

    for (unsigned int y_c = y; y_c < bottom; y_c++)
    {
        unsigned int* currentPixel = (unsigned int*)renderState->memory + x + y_c * width;

        for (unsigned int x_c = x; x_c < right; x_c++)
        {
            float distanceValue = depthValues[x_c + y_c * FRAME_WIDTH];

            *currentPixel++ = distanceValue * 200;
        }
    }
}

LRESULT CALLBACK windowCallback(_In_ HWND hwnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) try
{
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
    runtimeLogFile << "Device connected.\n";
    pipe.start();

    rs2::frameset frames = pipe.wait_for_frames();
    rs2::depth_frame depthFrame = frames.get_depth_frame();

    RECT windowRect = getClientRect(hWnd);

    unsigned int windowWidth = windowRect.right - windowRect.left;
    unsigned int windowHeight = windowRect.bottom - windowRect.top;

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

        frames = pipe.wait_for_frames(200);
        depthFrame = frames.get_depth_frame();

        writeDistanceDataToArray(depthFrame);
        copyDistanceDataToImage(0, 0, FRAME_WIDTH, FRAME_HEIGHT);
        int nearestPoint = findNearestPointShift();

        runtimeLogFile << nearestPoint << "\n";

        StretchDIBits(hdc, 0, 0, windowWidth, windowHeight, 0, 0, FRAME_WIDTH, FRAME_HEIGHT, renderState->memory, &renderState->bitmapInfo, DIB_RGB_COLORS, SRCCOPY);
    }

    delete graphics;
    delete renderState;
    delete[] depthValues;
}
catch (const std::runtime_error &err)
{
    runtimeLogFile << err.what() << "\n";
}

LRESULT CALLBACK windowCallback(_In_ HWND hwnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
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
            unsigned int bufferSize = renderState->width * renderState->height * sizeof(unsigned int);
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
            result = DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }

    return result;
}