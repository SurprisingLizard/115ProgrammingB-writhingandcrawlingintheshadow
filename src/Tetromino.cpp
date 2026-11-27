#include "Tetromino.h"

namespace Tetris {

Tetromino::Tetromino() : m_type(TetrominoType::None) {}

/**
 * [組員 A 任務 1] 建構子：初始化方塊類型與初始形狀
 * @param type 方塊類型 (I, O, T, S, Z, J, L)
 * 實作要求：
 * 1. 設定 m_type = type, m_rotation = 0
 * 2. 設定初始生成位置 m_position = { 3, 2 }（或依形狀微調，確保在緩衝區內且置中）
 * 3. 呼叫 getShapeOffsets(type, 0) 填入 m_localBlocks
 */
Tetromino::Tetromino(TetrominoType type)
    : m_type(type), m_position{ 3, 2 }, m_rotation(0) {
    // TODO: 確認初始位置並初始化 m_localBlocks
    m_localBlocks = getShapeOffsets(type, 0);
}

/**
 * [組員 A 任務 2] 移動方塊
 * @param dx X 軸位移（左為 -1，右為 +1）
 * @param dy Y 軸位移（下為 +1，上為 -1）
 */
void Tetromino::move(int dx, int dy) {
    // TODO: 更新 m_position.x 與 m_position.y
    m_position.x += dx;
    m_position.y += dy;
}

/**
 * [組員 A 任務 3] 順時針旋轉 90 度
 * 實作要求：
 * 1. O 型方塊旋轉後形狀不變，可直接 return
 * 2. 更新 m_rotation = (m_rotation + 1) % 4
 * 3. 更新 m_localBlocks = getShapeOffsets(m_type, m_rotation)
 *    （或使用 2D 旋轉矩陣公式計算相對座標）
 */
void Tetromino::rotateCW() {
    // TODO: 實作順時針旋轉
}

/**
 * [組員 A 任務 4] 逆時針旋轉 90 度
 * 實作要求：
 * 1. 更新 m_rotation = (m_rotation + 3) % 4（避免負數取餘問題）
 * 2. 更新 m_localBlocks
 */
void Tetromino::rotateCCW() {
    // TODO: 實作逆時針旋轉
}

/**
 * [組員 A 任務 5] 取得 4 個小方塊在棋盤上的「絕對座標」
 * @return 包含 4 個 Point 的 std::array
 * 公式：world[i].x = m_position.x + m_localBlocks[i].x
 *       world[i].y = m_position.y + m_localBlocks[i].y
 */
std::array<Point, 4> Tetromino::getWorldBlocks() const {
    std::array<Point, 4> world{};
    // TODO: 將 m_position 與 m_localBlocks 相加
    for (int i = 0; i < 4; ++i) {
        world[i] = { m_position.x + m_localBlocks[i].x, m_position.y + m_localBlocks[i].y };
    }
    return world;
}

/**
 * [組員 A 任務 6] 定義 7 種標準方塊在 4 個旋轉狀態 (0~3) 下的 4x4 局部座標
 * @param type     方塊類型
 * @param rotation 旋轉狀態 (0, 1, 2, 3)
 * @return 4 個相對座標 Point (建議範圍 x: 0~3, y: 0~3)
 *
 * 提示：可建立 static const 查表陣列，或先寫死 rotation=0 的形狀，
 *       再對座標 (x, y) 套用順時針旋轉公式：
 *       - 3x3 方塊 (T, S, Z, J, L): newX = 2 - y, newY = x
 *       - 4x4 方塊 (I):             newX = 3 - y, newY = x
 *       - O 型方塊:                 保持不動
 */
std::array<Point, 4> Tetromino::getShapeOffsets(TetrominoType type, int rotation) {
    // TODO: 實作所有 7 種方塊在 0~3 角度的相對座標
    // 以下僅為暫時佔位值（讓專案可直接編譯測試）
    (void)rotation;
    switch (type) {
        case TetrominoType::I: return {{ {0,1}, {1,1}, {2,1}, {3,1} }};
        case TetrominoType::O: return {{ {1,0}, {2,0}, {1,1}, {2,1} }};
        case TetrominoType::T: return {{ {1,0}, {0,1}, {1,1}, {2,1} }};
        case TetrominoType::S: return {{ {1,0}, {2,0}, {0,1}, {1,1} }};
        case TetrominoType::Z: return {{ {0,0}, {1,0}, {1,1}, {2,1} }};
        case TetrominoType::J: return {{ {0,0}, {0,1}, {1,1}, {2,1} }};
        case TetrominoType::L: return {{ {2,0}, {0,1}, {1,1}, {2,1} }};
        default:               return {{ {0,0}, {0,0}, {0,0}, {0,0} }};
    }
}

} // namespace Tetris