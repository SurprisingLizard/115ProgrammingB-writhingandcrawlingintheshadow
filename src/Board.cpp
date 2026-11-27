#include "Board.h"

namespace Tetris {

Board::Board() {
    clear();
}

/**
 * [組員 A 任務 7] 清空棋盤
 * 實作要求：將 m_grid 中所有 Cell 的 type 與 item 重設為 None
 */
void Board::clear() {
    for (int y = 0; y < TOTAL_HEIGHT; ++y) {
        for (int x = 0; x < BOARD_WIDTH; ++x) {
            m_grid[y][x] = Cell{};
        }
    }
}

/**
 * [組員 A 任務 8] 碰撞與邊界偵測
 * @param piece 待檢查的方塊（可能是已試探移動或旋轉後的複本）
 * @return 合法回傳 true；若超出邊界或與已有方塊重疊則回傳 false
 *
 * 邊界條件檢查清單：
 * 1. 取得 piece.getWorldBlocks() 的 4 個座標 (x, y)
 * 2. 檢查 x < 0 或 x >= BOARD_WIDTH -> false
 * 3. 檢查 y < 0 或 y >= TOTAL_HEIGHT -> false
 * 4. 檢查 m_grid[y][x].isOccupied() -> 若為 true 代表碰撞，回傳 false
 */
bool Board::isValidPosition(const Tetromino& piece) const {
    // TODO: 實作完整邊界與重疊檢查
    (void)piece;
    return true;
}

/**
 * [組員 A 任務 9] 固定方塊到棋盤上
 * @param piece 已確認落地且合法的方塊
 *
 * 實作要求：
 * 走訪 piece.getWorldBlocks()，在合法範圍內將對應的 m_grid[y][x].type 設為 piece.getType()
 */
void Board::lockPiece(const Tetromino& piece) {
    // TODO: 將方塊 4 格寫入 m_grid
    (void)piece;
}

/**
 * [組員 A 任務 10] 消除滿行並讓上方方塊下移
 * @return 本次消除的總行數 (0 ~ 4)
 *
 * 實作提示（由下往上掃描最不易出錯）：
 * 1. 從 y = TOTAL_HEIGHT - 1 往上檢查到 y = 0
 * 2. 判斷第 y 列是否全部 10 格都 isOccupied()
 * 3. 若滿行：
 *    - 累計 clearedLines++
 *    - 將第 y-1 列到第 0 列全部往下複製一格 (m_grid[row] = m_grid[row - 1])
 *    - 最頂端 m_grid[0] 清空為空白列
 *    - 注意：因為上方列掉下來了，當前這個 y 索引必須再檢查一次（++y）！
 */
int Board::clearFullLines() {
    // TODO: 實作消行與上方下移邏輯
    return 0;
}

/**
 * [組員 A 任務 11] 檢查是否頂部溢出 (Game Over 輔助判定)
 * @return 若緩衝區（y < BUFFER_HEIGHT）中有任何已固定方塊，回傳 true
 */
bool Board::isTopOut() const {
    // TODO: 掃描 y = 0 ~ BUFFER_HEIGHT - 1 檢查是否有格子被佔用
    return false;
}

/**
 * [Phase 3 預留任務] 插入垃圾行（Phase 1 可先留空）
 */
void Board::addGarbageLines(int count) {
    // Phase 3 實作：整張棋盤往上推 count 行，底部填入 Garbage 類型並隨機留一空格
    (void)count;
}

/**
 * 唯讀取得格子資料（含越界防呆）
 */
Cell Board::getCell(int x, int y) const {
    if (x < 0 || x >= BOARD_WIDTH || y < 0 || y >= TOTAL_HEIGHT) {
        return Cell{};
    }
    return m_grid[y][x];
}

} // namespace Tetris