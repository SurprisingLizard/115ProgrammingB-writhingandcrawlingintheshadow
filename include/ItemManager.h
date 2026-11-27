#pragma once
#include <deque>
#include "Types.h"

namespace Tetris {

// 使用道具後產生的效果指令包
struct ItemEffectPayload {
    bool triggered = false;
    int clearSelfBottomLines = 0; // 消除自身底層行數
    int sendGarbageToEnemy = 0;   // 傳送給對手的垃圾行數
    bool applyFogToEnemy = false; // 對對手施加視野遮蔽
};

class ItemManager {
public:
    static constexpr size_t MAX_SLOTS = 3; // 最多持有 3 個道具

    void reset();

    // 每幀更新持續性狀態計時器（減速、迷霧等）
    void update(float deltaTime);

    // 消行獲得道具時加入道具欄（滿了則忽略）
    bool addItem(ItemType item);

    // 玩家按下使用道具鍵（對應 InputAction::UseItem），取出最前方道具執行
    ItemEffectPayload useNextItem();

    // 接收來自對手的干擾道具效果
    void receiveEnemyEffect(ItemType enemyItem);

    // 供 GameSession / Renderer 查詢目前狀態
    float getGravityMultiplier() const; // 減速中回傳 0.5f，否則回傳 1.0f
    bool isFogActive() const { return m_fogTimer > 0.0f; }
    const std::deque<ItemType>& getInventory() const { return m_inventory; }

private:
    std::deque<ItemType> m_inventory;
    float m_slowTimer = 0.0f; // 自身減速剩餘秒數
    float m_fogTimer = 0.0f;  // 被遮蔽視野剩餘秒數
};

} // namespace Tetris