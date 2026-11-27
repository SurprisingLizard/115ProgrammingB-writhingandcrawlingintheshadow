#pragma once
#include <array>
#include <cstdint>

namespace Tetris {

// ==========================================
// 1. 遊戲棋盤與視窗常數
// ==========================================
constexpr int BOARD_WIDTH = 10;       // 棋盤寬度（格）
constexpr int VISIBLE_HEIGHT = 20;    // 玩家可見高度（格）
constexpr int BUFFER_HEIGHT = 4;      // 頂部隱藏緩衝區（用於方塊生成，不繪製於主框內）
constexpr int TOTAL_HEIGHT = VISIBLE_HEIGHT + BUFFER_HEIGHT; // 總陣列列數 = 24

constexpr int CELL_SIZE = 32;         // 每格方塊像素大小 (px)
constexpr int WINDOW_WIDTH = 800;     // 視窗寬度 (px)
constexpr int WINDOW_HEIGHT = 720;    // 視窗高度 (px)

// ==========================================
// 2. 方塊種類（使用 uint8_t 方便未來網路封包傳輸）
// ==========================================
enum class TetrominoType : std::uint8_t {
    None = 0,     // 空格
    I,            // 青色長條
    O,            // 黃色方塊
    T,            // 紫色 T 型
    S,            // 綠色 S 型
    Z,            // 紅色 Z 型
    J,            // 藍色 J 型
    L,            // 橘色 L 型
    Garbage       // 灰色垃圾方塊（預留：Phase 3 聯機對戰用）
};

// ==========================================
// 3. 道具種類（預留：Phase 3 道具系統用）
// ==========================================
enum class ItemType : std::uint8_t {
    None = 0,
    ClearBottomRows, // 消除最底層 2 行
    SlowDown,        // 暫時減緩下落速度
    GarbageAttack    // 對對手送出垃圾行
};

// ==========================================
// 4. 玩家輸入動作（解耦鍵盤按鍵與遊戲邏輯）
// ==========================================
enum class InputAction {
    None,
    MoveLeft,     // 左移
    MoveRight,    // 右移
    SoftDrop,     // 緩降（加速下落）
    HardDrop,     // 硬降（瞬間落底並鎖定）
    RotateCW,     // 順時針旋轉
    RotateCCW,    // 逆時針旋轉
    Hold,         // 暫存方塊（Phase 2 實作）
    UseItem       // 使用道具（Phase 3 實作）
};

// ==========================================
// 5. 整體場景狀態機
// ==========================================
enum class GameState {
    MainMenu,     // 主選單
    Playing,      // 遊玩中
    Paused,       // 暫停
    GameOver      // 遊戲結束
};

// ==========================================
// 6. 基礎座標與棋盤格結構
// ==========================================
struct Point {
    int x = 0; // 行索引 (Column): 0 ~ BOARD_WIDTH - 1
    int y = 0; // 列索引 (Row):    0 ~ TOTAL_HEIGHT - 1（由上往下遞增）
};

struct Cell {
    TetrominoType type = TetrominoType::None;
    ItemType item = ItemType::None; // 該格是否附帶道具（Phase 1 保持 None 即可）

    // 快速判斷該格是否已被佔用
    bool isOccupied() const {
        return type != TetrominoType::None;
    }
};

} // namespace Tetris