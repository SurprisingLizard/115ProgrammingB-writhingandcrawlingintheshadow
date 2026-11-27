#pragma once
#include <array>
#include "Types.h"

namespace Tetris {

class Tetromino {
public:
    Tetromino();
    explicit Tetromino(TetrominoType type);

    // 移動與旋轉操作
    void move(int dx, int dy);
    void rotateCW();   // 順時針旋轉 90 度
    void rotateCCW();  // 逆時針旋轉 90 度

    // 取得目前 4 個小方塊在「棋盤上的絕對座標」
    std::array<Point, 4> getWorldBlocks() const;

    // 取得基礎屬性（供 Board 與 Renderer 讀取）
    TetrominoType getType() const { return m_type; }
    Point getPosition() const { return m_position; }
    int getRotation() const { return m_rotation; }

    // 靜態輔助函式：取得指定類型在預設角度下的 4 個相對座標（供 Renderer 繪製 Next 預覽用）
    static std::array<Point, 4> getShapeOffsets(TetrominoType type, int rotation = 0);

private:
    TetrominoType m_type = TetrominoType::None;
    Point m_position = { 3, 2 }; // 生成起點（位於頂部緩衝區內，x=3 大致置中）
    int m_rotation = 0;          // 旋轉狀態：0, 1, 2, 3
    std::array<Point, 4> m_localBlocks{}; // 相對於 m_position 的 4 格偏移量
};

} // namespace Tetris