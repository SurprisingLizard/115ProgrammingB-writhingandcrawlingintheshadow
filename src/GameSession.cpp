#include "GameSession.h"
#include <algorithm>
#include <random>

namespace Tetris {

GameSession::GameSession() {
    reset();
}

/**
 * [組員 A 任務 12] 重置整局遊戲狀態
 * 實作要求：
 * 1. m_board.clear()
 * 2. 清空 m_bag，將 m_score = 0, m_level = 1, m_linesCleared = 0, m_fallTimer = 0.0f, m_gameOver = false
 * 3. m_nextType = drawFromBag()
 * 4. 呼叫 spawnNextPiece() 產生第一顆可控方塊
 */
void GameSession::reset() {
    m_board.clear();
    m_bag.clear();
    m_score = 0;
    m_level = 1;
    m_linesCleared = 0;
    m_fallTimer = 0.0f;
    m_gameOver = false;
    m_holdType = TetrominoType::None;

    // TODO: 抽出 m_nextType 並呼叫 spawnNextPiece()
    m_nextType = TetrominoType::I;
    m_currentPiece = Tetromino(TetrominoType::T);
}

/**
 * [組員 A 任務 13] 處理玩家輸入
 * 實作要求（若 m_gameOver 為 true 則直接 return）：
 * - MoveLeft:  tryMove(-1, 0)
 * - MoveRight: tryMove(1, 0)
 * - SoftDrop:  if (tryMove(0, 1)) { m_score += 1; m_fallTimer = 0.0f; }
 * - HardDrop:  用 while (tryMove(0, 1)) { m_score += 2; } 落到底後，立刻呼叫 lockAndSpawn()
 * - RotateCW:  tryRotate(true)
 * - RotateCCW: tryRotate(false)
 */
void GameSession::handleInput(InputAction action) {
    if (m_gameOver) return;
    // TODO: 依 InputAction 呼叫對應的移動、旋轉或硬降邏輯
    (void)action;
}

/**
 * [組員 A 任務 14] 每幀更新重力計時器
 * 實作要求：
 * 1. 若 m_gameOver 為 true 則直接 return
 * 2. m_fallTimer += deltaTime
 * 3. 當 m_fallTimer >= getFallInterval() 時：
 *    - m_fallTimer -= getFallInterval()
 *    - 呼叫 tryMove(0, 1)，若回傳 false（代表已觸底無法再下落），則呼叫 lockAndSpawn()
 */
void GameSession::update(float deltaTime) {
    if (m_gameOver) return;
    // TODO: 實作重力自動下落
    (void)deltaTime;
}

void GameSession::receiveGarbage(int lines) {
    m_board.addGarbageLines(lines);
}

/**
 * [組員 A 任務 15] 計算落地預測陰影 (Ghost Piece)
 * 實作要求：
 * 1. 複製一份 Tetromino ghost = m_currentPiece;
 * 2. 用迴圈嘗試將 ghost 往下移一格，直到下一個位置 !m_board.isValidPosition(test) 為止
 * 3. 回傳最後一個合法位置的 ghost
 */
Tetromino GameSession::getGhostPiece() const {
    Tetromino ghost = m_currentPiece;
    // TODO: 往下探測直到觸底
    return ghost;
}

/**
 * [組員 A 任務 16] 嘗試移動方塊
 * 實作要求：
 * 1. 複製一份 Tetromino candidate = m_currentPiece;
 * 2. candidate.move(dx, dy);
 * 3. 若 m_board.isValidPosition(candidate) 為 true，才將 m_currentPiece = candidate 並回傳 true
 */
bool GameSession::tryMove(int dx, int dy) {
    // TODO: 實作試探性移動
    (void)dx; (void)dy;
    return false;
}

/**
 * [組員 A 任務 17] 嘗試旋轉（含簡易牆踢 Wall Kick）
 * 實作要求：
 * 1. 複製一份 candidate 並執行 rotateCW() 或 rotateCCW()
 * 2. 依序測試 X 偏移量 { 0, -1, 1, -2, 2 }（若貼牆旋轉會出界，自動往左/右推 1~2 格）
 * 3. 找到第一個合法位置即套用並回傳 true；若全不合法則放棄旋轉並回傳 false
 */
bool GameSession::tryRotate(bool clockwise) {
    // TODO: 實作試探性旋轉與簡易牆踢
    (void)clockwise;
    return false;
}

/**
 * [組員 A 任務 18] 鎖定方塊、計分、升級與生成下一顆
 * 建議計分規則（可依標準微調）：
 * - 消 1 行: 100 * m_level
 * - 消 2 行: 300 * m_level
 * - 消 3 行: 500 * m_level
 * - 消 4 行: 800 * m_level
 * 升級規則：m_level = 1 + (m_linesCleared / 10)
 */
void GameSession::lockAndSpawn() {
    // TODO: lockPiece -> clearFullLines -> 更新分數與等級 -> 檢查 isTopOut -> spawnNextPiece
}

/**
 * [組員 A 任務 19] 生成下一顆方塊
 * 實作要求：
 * 1. m_currentPiece = Tetromino(m_nextType)
 * 2. m_nextType = drawFromBag()
 * 3. 若 !m_board.isValidPosition(m_currentPiece) -> 代表出生點已被堵死，設定 m_gameOver = true
 */
void GameSession::spawnNextPiece() {
    // TODO: 生成新方塊並檢查是否立即 Game Over
}

/**
 * [組員 A 任務 20] 7-Bag 隨機抽籤器
 * 實作要求：
 * 1. 若 m_bag 為空，將 I, O, T, S, Z, J, L 七種方塊放入 m_bag
 * 2. 使用 std::shuffle 搭配 std::mt19937 洗牌
 * 3. 取出並移除最後一個元素回傳
 */
TetrominoType GameSession::drawFromBag() {
    // TODO: 實作 7-Bag 演算法
    return TetrominoType::I;
}

/**
 * 依據等級計算下落間隔（秒），等級越高越快，最快不低於 0.1 秒
 */
float GameSession::getFallInterval() const {
    float interval = 0.8f - (m_level - 1) * 0.07f;
    return (interval < 0.1f) ? 0.1f : interval;
}

} // namespace Tetris