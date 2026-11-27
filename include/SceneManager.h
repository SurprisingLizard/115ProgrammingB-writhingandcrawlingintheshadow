#pragma once
#include <string>
#include "Types.h"

namespace Tetris {

class SceneManager {
public:
    void setScene(SceneType nextScene);
    SceneType getCurrentScene() const { return m_currentScene; }

    // 選單游標控制（上下鍵移動、Enter 確認）
    void moveSelectionUp(int maxOptions);
    void moveSelectionDown(int maxOptions);
    int getSelectedIndex() const { return m_selectedIndex; }

    // 記錄玩家在 ModeSelect / Lobby 選定的參數
    void setSelectedMode(GameModeType mode) { m_selectedMode = mode; }
    GameModeType getSelectedMode() const { return m_selectedMode; }

    void setNetRole(NetRole role) { m_netRole = role; }
    NetRole getNetRole() const { return m_netRole; }

    // 連線大廳輸入目標 IP 用（搭配 SFML 3 sf::Event::TextEntered）
    void appendIpChar(char ch);
    void backspaceIpChar();
    const std::string& getTargetIp() const { return m_targetIp; }

private:
    SceneType m_currentScene = SceneType::MainMenu;
    GameModeType m_selectedMode = GameModeType::Marathon;
    NetRole m_netRole = NetRole::None;
    int m_selectedIndex = 0;
    std::string m_targetIp = "127.0.0.1"; // 預設本機測試位址
};

} // namespace Tetris