#pragma once
#include <string>
#include <memory>
#include "Types.h"

namespace Tetris {

class GameMode {
public:
    virtual ~GameMode() = default;

    virtual GameModeType getType() const = 0;
    virtual std::string getName() const = 0;

    // 依等級回傳方塊自動下落間隔（秒）
    virtual float getFallInterval(int level) const = 0;

    // 計算本次消行得分並更新等級
    virtual void onLinesCleared(int lines, MatchStats& stats) const = 0;

    // 檢查是否達成模式勝利或時間結束條件
    virtual bool isVictory(const MatchStats& stats) const { return false; }
    virtual bool isTimeUp(const MatchStats& stats) const { return false; }

    // 是否啟用道具生成
    virtual bool isItemEnabled() const { return false; }

    // 工廠函式：依列舉建立對應模式物件
    static std::unique_ptr<GameMode> create(GameModeType type);
};

// --- 各具體模式宣告（實作寫在 src/GameMode.cpp） ---

class MarathonMode : public GameMode {
public:
    GameModeType getType() const override { return GameModeType::Marathon; }
    std::string getName() const override { return "Marathon"; }
    float getFallInterval(int level) const override;
    void onLinesCleared(int lines, MatchStats& stats) const override;
};

class SprintMode : public GameMode {
public:
    explicit SprintMode(int targetLines = 40) : m_targetLines(targetLines) {}
    GameModeType getType() const override { return GameModeType::Sprint; }
    std::string getName() const override { return "40-Line Sprint"; }
    float getFallInterval(int level) const override;
    void onLinesCleared(int lines, MatchStats& stats) const override;
    bool isVictory(const MatchStats& stats) const override;
private:
    int m_targetLines;
};

class UltraMode : public GameMode {
public:
    explicit UltraMode(float timeLimitSec = 120.0f) : m_timeLimit(timeLimitSec) {}
    GameModeType getType() const override { return GameModeType::Ultra; }
    std::string getName() const override { return "Ultra 2-Min"; }
    float getFallInterval(int level) const override;
    void onLinesCleared(int lines, MatchStats& stats) const override;
    bool isTimeUp(const MatchStats& stats) const override;
private:
    float m_timeLimit;
};

class ItemBattleMode : public GameMode {
public:
    GameModeType getType() const override { return GameModeType::ItemBattle; }
    std::string getName() const override { return "Item Battle"; }
    float getFallInterval(int level) const override;
    void onLinesCleared(int lines, MatchStats& stats) const override;
    bool isItemEnabled() const override { return true; }
};

} // namespace Tetris