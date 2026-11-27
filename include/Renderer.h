#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include "Types.h"
#include "GameSession.h"

namespace Tetris {

class Renderer {
public:
    explicit Renderer(const std::string& fontPath);

    // 主繪製入口：由 main.cpp 每幀呼叫，根據 GameState 決定繪製內容
    void render(sf::RenderWindow& window, const GameSession& session, GameState state);

private:
    // 繪製拆分輔助函式（建議組員 B 依序實作）
    void drawBoard(sf::RenderWindow& window, const Board& board);
    void drawTetromino(sf::RenderWindow& window, const Tetromino& piece, bool isGhost = false);
    void drawHUD(sf::RenderWindow& window, const GameSession& session);
    void drawNextPreview(sf::RenderWindow& window, TetrominoType nextType);
    void drawMainMenu(sf::RenderWindow& window);
    void drawPauseOverlay(sf::RenderWindow& window);
    void drawGameOverOverlay(sf::RenderWindow& window, int finalScore);

    // 繪製單一格方塊（含邊框與內縮效果）
    void drawCell(sf::RenderWindow& window, float pixelX, float pixelY, sf::Color color, bool isGhost = false);

    // 依照方塊類型回傳對應顏色
    static sf::Color getColorByType(TetrominoType type);

    // 版面排版常數（棋盤左上角在視窗中的像素起點）
    static constexpr float BOARD_OFFSET_X = 240.0f;
    static constexpr float BOARD_OFFSET_Y = 40.0f;

    sf::Font m_font;
    bool m_fontLoaded = false; // 若字型載入失敗仍可正常畫出方塊，不會崩潰
};

} // namespace Tetris