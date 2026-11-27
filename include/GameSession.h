#pragma once
#include <vector>
#include "Types.h"
#include "Board.h"
#include "Tetromino.h"

namespace Tetris {

class GameSession {
public:
    GameSession();

    // 重置一局新遊戲（清空棋盤、分數歸零、抽出第一個與下一個方塊）
    void reset();

    // 處理玩家操作指令（左右移、旋轉、緩降、硬降）
    void handleInput(InputAction action);

    // 每個影格更新遊戲狀態（重力自動下落計時）
    // @param deltaTime 距離上一幀經過的秒數
    void update(float deltaTime);

    // [Phase 3 預留] 接收來自對手或道具的垃圾行
    void receiveGarbage(int lines);

    // --- 以下為唯讀 Getter，供組員 B 的 Renderer 繪製畫面使用 ---
    const Board& getBoard() const { return m_board; }
    const Tetromino& getCurrentPiece() const { return m_currentPiece; }
    TetrominoType getNextType() const { return m_nextType; }
    TetrominoType getHoldType() const { return m_holdType; }

    // 計算目前方塊若立刻硬降會停在哪裡（供 Renderer 畫半透明落地陰影）
    Tetromino getGhostPiece() const;

    int getScore() const { return m_score; }
    int getLevel() const { return m_level; }
    int getLinesCleared() const { return m_linesCleared; }
    bool isGameOver() const { return m_gameOver; }

private:
    // 內部輔助函式
    void spawnNextPiece();               // 生成下一個方塊並更新 m_nextType；若生成即碰撞則判定 Game Over
    void lockAndSpawn();                 // 鎖定目前方塊 -> 消行 -> 計分升級 -> 生成新方塊
    bool tryMove(int dx, int dy);        // 嘗試移動，若合法則套用並回傳 true
    bool tryRotate(bool clockwise);      // 嘗試旋轉（含簡易左右位移「牆踢」測試）
    TetrominoType drawFromBag();         // 7-Bag 洗牌隨機抽方塊（確保每 7 顆不重複）
    float getFallInterval() const;       // 依據目前 m_level 回傳下落一格所需秒數

    Board m_board;
    Tetromino m_currentPiece;
    TetrominoType m_nextType = TetrominoType::None;
    TetrominoType m_holdType = TetrominoType::None; // Phase 2 用

    std::vector<TetrominoType> m_bag;    // 7-Bag 抽籤袋
    float m_fallTimer = 0.0f;            // 重力累加計時器（秒）
    int m_score = 0;
    int m_level = 1;
    int m_linesCleared = 0;
    bool m_gameOver = false;
};

} // namespace Tetris