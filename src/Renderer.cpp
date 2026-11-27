#include "Renderer.h"
#include <iostream>

namespace Tetris {

/**
 * [組員 B 任務 1] 建構子：載入字型檔
 * 實作要求：呼叫 m_font.loadFromFile(fontPath)，記錄成功與否於 m_fontLoaded
 */
Renderer::Renderer(const std::string& fontPath) {
    // 1. 建構子載入字型：將 loadFromFile 改為 openFromFile
    m_fontLoaded = m_font.openFromFile(fontPath);

    if (!m_fontLoaded) {
        std::cerr << "[Warning] Font not loaded: " << fontPath << "\n";
    }
}

/**
 * [組員 B 任務 2] 主繪製流程控制
 * 實作要求：
 * - MainMenu: 僅呼叫 drawMainMenu(window)
 * - Playing:  依序呼叫 drawBoard -> drawTetromino(ghost, true) -> drawTetromino(current, false) -> drawHUD
 * - Paused:   先畫出遊戲畫面，再疊加 drawPauseOverlay(window)
 * - GameOver: 先畫出遊戲畫面，再疊加 drawGameOverOverlay(window, session.getScore())
 */
void Renderer::render(sf::RenderWindow& window, const GameSession& session, GameState state) {
    if (state == GameState::MainMenu) {
        drawMainMenu(window);
        return;
    }

    // 繪製遊戲主體
    drawBoard(window, session.getBoard());
    drawTetromino(window, session.getGhostPiece(), true);
    drawTetromino(window, session.getCurrentPiece(), false);
    drawHUD(window, session);

    // 依狀態疊加半透明遮罩介面
    if (state == GameState::Paused) {
        drawPauseOverlay(window);
    } else if (state == GameState::GameOver) {
        drawGameOverOverlay(window, session.getScore());
    }
}

/**
 * [組員 B 任務 3] 繪製棋盤底圖、格線與所有已固定方塊
 * 座標換算重要提醒：
 * - 棋盤陣列 y = 0 ~ 3 是隱藏緩衝區，**不應繪製**！
 * - 請從 y = BUFFER_HEIGHT (4) 走訪到 TOTAL_HEIGHT - 1 (23)
 * - 顯示列號 visibleY = y - BUFFER_HEIGHT (範圍 0 ~ 19)
 * - 像素座標：
 *     pixelX = BOARD_OFFSET_X + x * CELL_SIZE
 *     pixelY = BOARD_OFFSET_Y + visibleY * CELL_SIZE
 */
void Renderer::drawBoard(sf::RenderWindow& window, const Board& board) {
    // 1. 畫棋盤背景框（10 * CELL_SIZE 寬，20 * CELL_SIZE 高）
    sf::RectangleShape bg(sf::Vector2f(BOARD_WIDTH * CELL_SIZE, VISIBLE_HEIGHT * CELL_SIZE));
    bg.setPosition({BOARD_OFFSET_X, BOARD_OFFSET_Y});
    bg.setFillColor(sf::Color(15, 15, 22));
    bg.setOutlineThickness(2.0f);
    bg.setOutlineColor(sf::Color(100, 100, 120));
    window.draw(bg);

    // TODO:
    // 2. 繪製淺色背景格線（增加對齊手感）
    // 3. 走訪 y = BUFFER_HEIGHT ~ TOTAL_HEIGHT-1, x = 0 ~ BOARD_WIDTH-1
    //    若 board.getCell(x, y).isOccupied()，呼叫 drawCell() 繪製對應顏色的方塊
    (void)board;
}

/**
 * [組員 B 任務 4] 繪製目前正在下落的方塊（或落地陰影）
 * @param piece   目標方塊
 * @param isGhost true 代表畫落地預測陰影（建議用半透明或空心框），false 代表實體方塊
 *
 * 注意：若方塊的某格座標仍在隱藏區 (p.y < BUFFER_HEIGHT)，該格請跳過不要繪製
 */
void Renderer::drawTetromino(sf::RenderWindow& window, const Tetromino& piece, bool isGhost) {
    if (piece.getType() == TetrominoType::None) return;

    // TODO: 取得 piece.getWorldBlocks()，過濾掉 p.y < BUFFER_HEIGHT 的格子並呼叫 drawCell()
    (void)window; (void)isGhost;
}

/**
 * [組員 B 任務 5] 繪製右側資訊欄 (HUD)
 * 顯示內容建議（放置於棋盤右側，例如 x = BOARD_OFFSET_X + BOARD_WIDTH * CELL_SIZE + 30）：
 * 1. "NEXT" 預覽框（呼叫 drawNextPreview）
 * 2. SCORE（分數）、LEVEL（等級）、LINES（消除行數）
 * 3. 簡易按鍵操作提示（方向鍵移動、空白鍵硬降、P 暫停）
 * 注意：繪製文字前需先檢查 if (!m_fontLoaded) return;
 */
void Renderer::drawHUD(sf::RenderWindow& window, const GameSession& session) {
    drawNextPreview(window, session.getNextType());
    if (!m_fontLoaded) return;

    // TODO: 使用 sf::Text 繪製分數、等級、消除行數與操作說明
    (void)window;
}

/**
 * [組員 B 任務 6] 繪製 Next 方塊預覽框
 * 提示：呼叫 Tetromino::getShapeOffsets(nextType, 0) 即可拿到 4 個相對座標，
 *       無需存取整個棋盤，直接畫在預覽框內即可。
 */
void Renderer::drawNextPreview(sf::RenderWindow& window, TetrominoType nextType) {
    // TODO: 畫出預覽外框與下一顆方塊的縮圖
    (void)window; (void)nextType;
}

/**
 * [組員 B 任務 7] 繪製主選單畫面
 * 顯示遊戲標題 "TETRIS" 與 "Press ENTER to Start" 提示文字
 */
void Renderer::drawMainMenu(sf::RenderWindow& window) {
    if (!m_fontLoaded) return;
    // TODO: 設計置中的主選單標題與開始提示
    (void)window;
}

/**
 * [組員 B 任務 8] 繪製暫停半透明遮罩與 "PAUSED" 文字
 */
void Renderer::drawPauseOverlay(sf::RenderWindow& window) {
    // TODO: 畫一層全螢幕半透明黑色 sf::Color(0, 0, 0, 150) 與文字
    (void)window;
}

/**
 * [組員 B 任務 9] 繪製結算遮罩、最終分數與 "Press R to Restart" 提示
 */
void Renderer::drawGameOverOverlay(sf::RenderWindow& window, int finalScore) {
    // TODO: 畫出半透明遮罩、GAME OVER 標題、finalScore 與重開提示
    (void)window; (void)finalScore;
}

/**
 * [組員 B 任務 10] 繪製單一格方塊的視覺樣式
 * 技巧：將大小設為 (CELL_SIZE - 2, CELL_SIZE - 2) 並位移 +1px，即可自然呈現格線間距
 */
void Renderer::drawCell(sf::RenderWindow& window, float pixelX, float pixelY, sf::Color color, bool isGhost) {
    sf::RectangleShape rect(sf::Vector2f(CELL_SIZE - 2.0f, CELL_SIZE - 2.0f));
    rect.setPosition({pixelX + 1.0f, pixelY + 1.0f});

    if (isGhost) {
        rect.setFillColor(sf::Color(color.r, color.g, color.b, 50)); // 半透明填色
        rect.setOutlineThickness(1.5f);
        rect.setOutlineColor(sf::Color(color.r, color.g, color.b, 180));
    } else {
        rect.setFillColor(color);
    }
    window.draw(rect);
}

/**
 * 標準俄羅斯方塊配色表（組員 B 可自由美化配色）
 */
sf::Color Renderer::getColorByType(TetrominoType type) {
    switch (type) {
        case TetrominoType::I:       return sf::Color(0, 240, 240);   // 青色
        case TetrominoType::O:       return sf::Color(240, 240, 0);   // 黃色
        case TetrominoType::T:       return sf::Color(160, 0, 240);   // 紫色
        case TetrominoType::S:       return sf::Color(0, 240, 0);     // 綠色
        case TetrominoType::Z:       return sf::Color(240, 0, 0);     // 紅色
        case TetrominoType::J:       return sf::Color(0, 0, 240);     // 藍色
        case TetrominoType::L:       return sf::Color(240, 160, 0);   // 橘色
        case TetrominoType::Garbage: return sf::Color(120, 120, 120); // 灰色
        default:                     return sf::Color::Transparent;
    }
}

} // namespace Tetris