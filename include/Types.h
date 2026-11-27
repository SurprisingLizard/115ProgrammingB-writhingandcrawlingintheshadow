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

// 1. 遊玩模式類型
enum class GameModeType {
    Marathon,   // 經典無盡（隨消行升級加速）
    Sprint,     // 40 行競速（消除 40 行獲勝，計時）
    Ultra,      // 限時高分（限時 120 秒）
    ItemBattle  // 道具對戰（支援道具與垃圾行）
};

// 2. 道具類型（對應 Cell 內的 itemId 或道具欄）
enum class ItemType : int {
    None = 0,
    ClearBottom2, // 立即消除自己最底層 2 行
    SlowGravity,  // 自身下落速度暫時減半（持續 10 秒）
    SendGarbage,  // 立即送給對手 2 行垃圾行
    FogMask       // 遮蔽對手棋盤下半部視野（持續 6 秒）
};

// 3. 遊戲場景狀態
enum class SceneType {
    MainMenu,   // 主選單
    ModeSelect, // 模式選擇
    Lobby,      // 連線大廳（輸入 IP / 開房等待）
    Playing,    // 遊玩中
    Paused,     // 暫停
    GameOver    // 結算畫面
};

// 4. 聯機角色與封包類型
enum class NetRole {
    None,
    Host,   // 開房主機（監聽 Port）
    Client  // 加入端（輸入主機 IP 連線）
};

// 單局統計數據（供 GameMode 判定勝負與 Renderer 顯示 HUD）
struct MatchStats {
    int score = 0;
    int linesCleared = 0;
    int level = 1;
    float elapsedTime = 0.0f; // 經過秒數
};
} // namespace Tetris