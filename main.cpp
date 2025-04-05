#include <windows.h>
#include <ctime>
#include <string>
#include <sstream>

const int BLOCK_SIZE = 30;
const int WIDTH = 10;
const int HEIGHT = 20;
const int PREVIEW_X = WIDTH * BLOCK_SIZE + 60;

int field[HEIGHT][WIDTH] = {0};
COLORREF colorField[HEIGHT][WIDTH] = {0};

enum BlockType { T, L, O, I, J };
BlockType currentType, nextType;
int rotation = 0;
int score = 0;
UINT timerSpeed = 500;
bool gameOver = false;
int timeRemaining = 60;
UINT countdownTimer = 2;

struct Point { int x, y; };
Point current[4];

COLORREF blockColors[5] = {
    RGB(255, 0, 0),     // T: red
    RGB(0, 0, 255),     // L: blue
    RGB(255, 165, 0),   // O: orange
    RGB(0, 255, 255),   // I: cyan
    RGB(128, 0, 255)    // J: purple
};

Point blockShapes[5][4] = {
    {{0,0}, {-1,1}, {0,1}, {1,1}},     // T
    {{0,0}, {0,1}, {0,2}, {1,2}},     // L
    {{0,0}, {1,0}, {0,1}, {1,1}},     // O
    {{-1,0}, {0,0}, {1,0}, {2,0}},    // I
    {{1,0}, {1,1}, {1,2}, {0,2}}      // J (逆L)
};

bool checkValidPosition() {
    for (int i = 0; i < 4; i++) {
        int x = current[i].x;
        int y = current[i].y;
        if (x < 0 || x >= WIDTH || y >= HEIGHT || (y >= 0 && field[y][x]))
            return false;
    }
    return true;
}

void spawnBlock() {
    rotation = 0;
    currentType = nextType;
    nextType = static_cast<BlockType>(rand() % 5);
    for (int i = 0; i < 4; i++) {
        current[i].x = blockShapes[currentType][i].x + WIDTH / 2;
        current[i].y = blockShapes[currentType][i].y;
    }
}

void move(int dx) {
    Point backup[4];
    for (int i = 0; i < 4; i++) {
        backup[i] = current[i];
        current[i].x += dx;
    }
    if (!checkValidPosition()) {
        for (int i = 0; i < 4; i++) current[i] = backup[i];
    }
}

void rotateBlock() {
    if (currentType == O) return;
    Point center = current[0];
    Point backup[4];
    for (int i = 0; i < 4; i++) backup[i] = current[i];
    for (int i = 0; i < 4; i++) {
        int x = current[i].y - center.y;
        int y = current[i].x - center.x;
        current[i].x = center.x - x;
        current[i].y = center.y + y;
    }
    if (!checkValidPosition()) {
        for (int i = 0; i < 4; i++) current[i] = backup[i];
    }
}

void fixBlock() {
    for (int i = 0; i < 4; i++) {
        int x = current[i].x;
        int y = current[i].y;
        if (y >= 0 && y < HEIGHT && x >= 0 && x < WIDTH) {
            field[y][x] = 1;
            colorField[y][x] = blockColors[currentType];
        }
    }
}

void clearLines() {
    for (int y = HEIGHT - 1; y >= 0; y--) {
        bool full = true;
        for (int x = 0; x < WIDTH; x++) {
            if (!field[y][x]) {
                full = false;
                break;
            }
        }
        if (full) {
            score += 100;
            for (int ty = y; ty > 0; ty--) {
                for (int x = 0; x < WIDTH; x++) {
                    field[ty][x] = field[ty - 1][x];
                    colorField[ty][x] = colorField[ty - 1][x];
                }
            }
            for (int x = 0; x < WIDTH; x++) {
                field[0][x] = 0;
                colorField[0][x] = RGB(255, 255, 255);
            }
            y++;
        }
    }
}

bool isGameOver() {
    for (int i = 0; i < 4; i++) {
        int x = current[i].x;
        int y = current[i].y;
        if (y >= 0 && field[y][x]) return true;
    }
    return false;
}

void drop() {
    Point backup[4];
    for (int i = 0; i < 4; i++) {
        backup[i] = current[i];
        current[i].y++;
    }
    if (!checkValidPosition()) {
        for (int i = 0; i < 4; i++) current[i] = backup[i];
        fixBlock();
        clearLines();
        spawnBlock();
    }
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_TIMER:
            if (wParam == 1 && !gameOver) {
                drop();
                if (isGameOver()) {
                    gameOver = true;
                    KillTimer(hwnd, 1);
                    KillTimer(hwnd, countdownTimer);
                    MessageBox(hwnd, "Game Over", "Block Fall Down", MB_OK);
                    PostQuitMessage(0);
                }
                InvalidateRect(hwnd, NULL, TRUE);
            } else if (wParam == countdownTimer && !gameOver) {
                timeRemaining--;
                if (timeRemaining <= 0) {
                    gameOver = true;
                    KillTimer(hwnd, 1);
                    KillTimer(hwnd, countdownTimer);
                    MessageBox(hwnd, "Time's up!", "Block Fall Down", MB_OK);
                    PostQuitMessage(0);
                }
                InvalidateRect(hwnd, NULL, TRUE);
            }
            return 0;

        case WM_KEYDOWN:
            if (!gameOver) {
                switch (wParam) {
                    case 'A': move(-1); break;
                    case 'D': move(1); break;
                    case 'W': rotateBlock(); break;
                }
                InvalidateRect(hwnd, NULL, TRUE);
            }
            return 0;

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            SetBkMode(hdc, TRANSPARENT);

            for (int y = 0; y < HEIGHT; y++) {
                for (int x = 0; x < WIDTH; x++) {
                    RECT rect = { x * BLOCK_SIZE, y * BLOCK_SIZE, (x + 1) * BLOCK_SIZE, (y + 1) * BLOCK_SIZE };
                    DrawEdge(hdc, &rect, EDGE_ETCHED, BF_RECT);
                    if (field[y][x]) {
                        HBRUSH brush = CreateSolidBrush(colorField[y][x]);
                        FillRect(hdc, &rect, brush);
                        DeleteObject(brush);
                    }
                }
            }

            for (int i = 0; i < 4; i++) {
                HBRUSH brush = CreateSolidBrush(blockColors[currentType]);
                int x = current[i].x;
                int y = current[i].y;
                RECT rect = { x * BLOCK_SIZE, y * BLOCK_SIZE, (x + 1) * BLOCK_SIZE, (y + 1) * BLOCK_SIZE };
                FillRect(hdc, &rect, brush);
                DrawEdge(hdc, &rect, EDGE_ETCHED, BF_RECT);
                DeleteObject(brush);
            }

            RECT previewRect = { PREVIEW_X - 10, 50, PREVIEW_X + BLOCK_SIZE * 5, 50 + BLOCK_SIZE * 4 };
            DrawEdge(hdc, &previewRect, EDGE_ETCHED, BF_RECT);

            for (int i = 0; i < 4; i++) {
                HBRUSH brush = CreateSolidBrush(blockColors[nextType]);
                int x = PREVIEW_X + blockShapes[nextType][i].x * BLOCK_SIZE;
                int y = 60 + blockShapes[nextType][i].y * BLOCK_SIZE;
                RECT rect = { x, y, x + BLOCK_SIZE, y + BLOCK_SIZE };
                DrawEdge(hdc, &rect, EDGE_ETCHED, BF_RECT);
                FillRect(hdc, &rect, brush);
                DrawEdge(hdc, &rect, EDGE_RAISED, BF_RECT);
                DeleteObject(brush);
            }

            TextOutA(hdc, PREVIEW_X - 10, 20, "Score:", 6);
            std::ostringstream oss;
            oss << score;
            std::string scoreText = oss.str();
            TextOutA(hdc, PREVIEW_X + 50, 20, scoreText.c_str(), scoreText.length());

            TextOutA(hdc, PREVIEW_X - 10, 35, "Next", 4);

            std::ostringstream timerText;
            timerText << "Time: " << timeRemaining;
            std::string timeStr = timerText.str();
            TextOutA(hdc, PREVIEW_X - 10, 450, timeStr.c_str(), timeStr.length());

            EndPaint(hwnd, &ps);
            return 0;
        }
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    srand((unsigned int)time(0));
    const char CLASS_NAME[] = "Block Fall Down Window";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0, CLASS_NAME, "Win32 Block Fall Down",
        WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT,
        BLOCK_SIZE * WIDTH + 200, BLOCK_SIZE * HEIGHT + 39,
        nullptr, nullptr, hInstance, nullptr
    );

    if (!hwnd) return 0;

    nextType = static_cast<BlockType>(rand() % 5);
    spawnBlock();
    ShowWindow(hwnd, nCmdShow);
    SetTimer(hwnd, 1, timerSpeed, NULL);
    SetTimer(hwnd, countdownTimer, 1000, NULL);

    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
