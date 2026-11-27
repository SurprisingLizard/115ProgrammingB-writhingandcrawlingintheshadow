#include <algorithm>
#include <array>
#include <cassert>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <random>
#include <string>
#include <thread>
#include <vector>

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <conio.h>
#include <windows.h>
#else
#include <sys/select.h>
#include <termios.h>
#include <unistd.h>
#endif

namespace {
constexpr int kWidth = 10;
constexpr int kHeight = 20;
using Board = std::array<std::array<int, kWidth>, kHeight>;
using Shape = std::array<std::array<int, 4>, 4>;

const std::array<Shape, 7> kShapes = {{
    {{{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}}},
    {{{1, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}},
    {{{0, 1, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}},
    {{{1, 0, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}},
    {{{0, 0, 1, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}},
    {{{0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}},
    {{{1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}},
}};

Shape rotateClockwise(const Shape& shape) {
    Shape rotated{};
    for (int row = 0; row < 4; ++row)
        for (int col = 0; col < 4; ++col)
            rotated[col][3 - row] = shape[row][col];
    return rotated;
}

class Terminal {
public:
    Terminal() {
#ifdef _WIN32
        HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD mode = 0;
        if (GetConsoleMode(output, &mode)) {
            SetConsoleMode(output, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
        }
#else
        tcgetattr(STDIN_FILENO, &original_);
        termios raw = original_;
        raw.c_lflag &= static_cast<unsigned>(~(ICANON | ECHO));
        raw.c_cc[VMIN] = 0;
        raw.c_cc[VTIME] = 0;
        tcsetattr(STDIN_FILENO, TCSANOW, &raw);
#endif
        std::cout << "\x1b[?25l";
    }

    ~Terminal() {
#ifndef _WIN32
        tcsetattr(STDIN_FILENO, TCSANOW, &original_);
#endif
        std::cout << "\x1b[0m\x1b[?25h\n";
    }

    Terminal(const Terminal&) = delete;

    bool keyAvailable() const {
#ifdef _WIN32
        return _kbhit() != 0;
#else
        timeval timeout{0, 0};
        fd_set input;
        FD_ZERO(&input);
        FD_SET(STDIN_FILENO, &input);
        return select(STDIN_FILENO + 1, &input, nullptr, nullptr, &timeout) > 0;
#endif
    }

    char readKey() const {
#ifdef _WIN32
        return static_cast<char>(_getch());
#else
        char key = 0;
        ::read(STDIN_FILENO, &key, 1);
        return key;
#endif
    }

private:
#ifndef _WIN32
    termios original_{};
#endif
};

struct Piece {
    int type = 0;
    int rotation = 0;
    int row = 0;
    int col = 3;
};

class Game {
public:
    explicit Game(unsigned seed = std::random_device{}()) : random_(seed) {
        reset();
    }

    void reset() {
        board_ = {};
        score_ = 0;
        lines_ = 0;
        level_ = 1;
        paused_ = false;
        gameOver_ = false;
        refillBag();
        current_ = nextPiece();
        next_ = nextPiece();
    }

    void run() {
        Terminal terminal;
        auto lastFall = std::chrono::steady_clock::now();
        draw();
        while (!gameOver_) {
            while (terminal.keyAvailable()) {
                handleKey(terminal.readKey());
                if (gameOver_) break;
            }

            const auto now = std::chrono::steady_clock::now();
            const auto elapsed =
                std::chrono::duration_cast<std::chrono::milliseconds>(now - lastFall).count();
            if (!paused_ && elapsed >= fallDelay()) {
                if (!move(1, 0)) lockPiece();
                lastFall = now;
            }
            draw();
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
        }
        draw();
        std::cout << "\nGame over! Press Enter to exit.\n";
        std::cin.get();
    }

    bool canPlace(const Piece& piece) const {
        const Shape& shape = shapeFor(piece);
        for (int row = 0; row < 4; ++row) {
            for (int col = 0; col < 4; ++col) {
                if (!shape[row][col]) continue;
                const int boardRow = piece.row + row;
                const int boardCol = piece.col + col;
                if (boardCol < 0 || boardCol >= kWidth || boardRow >= kHeight) return false;
                if (boardRow >= 0 && board_[boardRow][boardCol]) return false;
            }
        }
        return true;
    }

    void clearFullLinesForTest() {
        clearFullLines();
    }

    const Board& boardForTest() const {
        return board_;
    }

    void fillRowForTest(int row) {
        board_[row].fill(1);
    }

private:
    Board board_{};
    Piece current_{};
    Piece next_{};
    std::array<int, 7> bag_{};
    int bagIndex_ = 7;
    int score_ = 0;
    int lines_ = 0;
    int level_ = 1;
    bool paused_ = false;
    bool gameOver_ = false;
    std::mt19937 random_;

    const Shape& shapeFor(const Piece& piece) const {
        static Shape rotations[7][4];
        static bool initialized = false;
        if (!initialized) {
            for (int type = 0; type < 7; ++type) {
                rotations[type][0] = kShapes[type];
                for (int rotation = 1; rotation < 4; ++rotation)
                    rotations[type][rotation] = rotateClockwise(rotations[type][rotation - 1]);
            }
            initialized = true;
        }
        return rotations[piece.type][piece.rotation % 4];
    }

    void refillBag() {
        for (int i = 0; i < 7; ++i) bag_[i] = i;
        std::shuffle(bag_.begin(), bag_.end(), random_);
        bagIndex_ = 0;
    }

    Piece nextPiece() {
        if (bagIndex_ == 7) refillBag();
        return Piece{bag_[bagIndex_++], 0, 0, 3};
    }

    long long fallDelay() const {
        return std::max(80, 650 - (level_ - 1) * 55);
    }

    bool move(int rowDelta, int colDelta) {
        Piece candidate = current_;
        candidate.row += rowDelta;
        candidate.col += colDelta;
        if (!canPlace(candidate)) return false;
        current_ = candidate;
        return true;
    }

    void rotate() {
        Piece candidate = current_;
        candidate.rotation = (candidate.rotation + 1) % 4;
        for (int offset : {0, -1, 1, -2, 2}) {
            candidate.col = current_.col + offset;
            if (canPlace(candidate)) {
                current_ = candidate;
                return;
            }
        }
    }

    void hardDrop() {
        int distance = 0;
        while (move(1, 0)) ++distance;
        score_ += distance * 2;
        lockPiece();
    }

    void lockPiece() {
        const Shape& shape = shapeFor(current_);
        for (int row = 0; row < 4; ++row) {
            for (int col = 0; col < 4; ++col) {
                if (shape[row][col] && current_.row + row >= 0)
                    board_[current_.row + row][current_.col + col] = current_.type + 1;
            }
        }
        clearFullLines();
        current_ = next_;
        next_ = nextPiece();
        if (!canPlace(current_)) gameOver_ = true;
    }

    void clearFullLines() {
        int cleared = 0;
        for (int row = kHeight - 1; row >= 0; --row) {
            if (std::all_of(board_[row].begin(), board_[row].end(), [](int cell) { return cell != 0; })) {
                for (int copy = row; copy > 0; --copy) board_[copy] = board_[copy - 1];
                board_[0].fill(0);
                ++cleared;
                ++row;
            }
        }
        if (cleared > 0) {
            lines_ += cleared;
            level_ = lines_ / 10 + 1;
            score_ += std::array<int, 5>{0, 100, 300, 500, 800}[cleared] * level_;
        }
    }

    void handleKey(char key) {
        switch (key) {
            case 'a': case 'A': move(0, -1); break;
            case 'd': case 'D': move(0, 1); break;
            case 's': case 'S': if (move(1, 0)) ++score_; break;
            case 'w': case 'W': rotate(); break;
            case ' ': hardDrop(); break;
            case 'p': case 'P': paused_ = !paused_; break;
            case 'q': case 'Q': gameOver_ = true; break;
            default: break;
        }
    }

    void draw() const {
        std::cout << "\x1b[H\x1b[2J";
        std::cout << "TETRIS  A/D Move  S Soft Drop  W Rotate  Space Hard Drop  P Pause  Q Quit\n";
        std::cout << "Score: " << score_ << "   Lines: " << lines_ << "   Level: " << level_;
        if (paused_) std::cout << "   [PAUSED]";
        std::cout << "\n+";
        for (int col = 0; col < kWidth; ++col) std::cout << "--";
        std::cout << "+\n";

        for (int row = 0; row < kHeight; ++row) {
            std::cout << "|";
            for (int col = 0; col < kWidth; ++col) {
                int value = board_[row][col];
                for (int shapeRow = 0; shapeRow < 4; ++shapeRow) {
                    for (int shapeCol = 0; shapeCol < 4; ++shapeCol) {
                        if (shapeFor(current_)[shapeRow][shapeCol] &&
                            current_.row + shapeRow == row && current_.col + shapeCol == col)
                            value = current_.type + 1;
                    }
                }
                std::cout << (value ? "[]" : "  ");
            }
            std::cout << "|\n";
        }
        std::cout << "+";
        for (int col = 0; col < kWidth; ++col) std::cout << "--";
        std::cout << "+\nNext:\n";
        const Shape& nextShape = shapeFor(next_);
        for (int row = 0; row < 2; ++row) {
            for (int col = 0; col < 4; ++col) std::cout << (nextShape[row][col] ? "[]" : "  ");
            std::cout << "\n";
        }
        std::cout.flush();
    }
};

void runSelfTest() {
    Game game(1234);
    Piece wallPiece{0, 0, 0, -1};
    assert(!game.canPlace(wallPiece));
    game.fillRowForTest(kHeight - 1);
    game.clearFullLinesForTest();
    for (int cell : game.boardForTest()[kHeight - 1]) assert(cell == 0);
    std::cout << "Self-test passed.\n";
}
}  // namespace

int main(int argc, char* argv[]) {
    if (argc > 1 && std::string(argv[1]) == "--self-test") {
        runSelfTest();
        return 0;
    }
    Game game;
    game.run();
    return 0;
}
