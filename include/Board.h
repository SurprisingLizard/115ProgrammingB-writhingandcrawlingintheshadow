#pragma once
#include <array>
#include "Types.h"
#include "Tetromino.h"

namespace Tetris {

class Board {
public:
    Board();

    // 清空整個棋盤（重開新局時呼叫）
    void clear();

    // 檢查指定方塊目前所在座標是否合法（未出界且未與已固定方塊重疊）
    bool isValidPosition(const Tetromino& piece) const;

    // 將已落地的方塊寫入棋盤陣列中固定
    void lockPiece(const Tetromino& piece);

    // 檢查並消除所有已填滿的橫列，上方格子自動下移
    // @return 本次消除的總行數 (0 ~ 4)
    int clearFullLines();

    // 檢查頂部緩衝區是否有方塊堆積（用於判定 Game Over）
    bool isTopOut() const;

    // [Phase 3 預留接口] 從棋盤底部插入指定數量的垃圾行（每行留一個隨機空洞）
    void addGarbageLines(int count);

    // 唯讀取得指定座標的格子狀態（若越界應回傳空白 Cell，不可當機）
    Cell getCell(int x, int y) const;

private:
    // 二維陣列：m_grid[y][x]，y 為 0 ~ TOTAL_HEIGHT-1，x 為 0 ~ BOARD_WIDTH-1
    std::array<std::array<Cell, BOARD_WIDTH>, TOTAL_HEIGHT> m_grid{};
};

} // namespace Tetris