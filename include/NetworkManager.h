#pragma once
#include <SFML/Network.hpp>
#include <array>
#include <cstdint>
#include <string>
#include <vector>
#include "Types.h"

namespace Tetris {

// 網路封包識別碼
enum class PacketType : std::uint8_t {
    MatchStart = 1,    // 主機通知雙方同步開局
    BoardSync = 2,     // 定期同步棋盤狀態快照
    AttackGarbage = 3, // 消行攻擊：送出垃圾行數
    ItemAttack = 4,    // 道具攻擊：送出干擾道具 ID
    PlayerGameOver = 5 // 通知對方自己已落敗
};

// 對手的精簡狀態快照（供本地右側副視窗繪製對手畫面）
struct RemotePlayerState {
    // 只傳可視區 20x10 格的方塊顏色/類型 ID（200 bytes，極低頻寬消耗）
    std::array<std::uint8_t, 200> visibleGrid{};
    int score = 0;
    bool isAlive = true;
};

class NetworkManager {
public:
    NetworkManager();
    ~NetworkManager();

    // 1. 連線控制（無公開伺服器：一方開房，一方輸入 IP 加入）
    bool startHost(unsigned short port = 54000);
    bool connectToHost(const std::string& ipAddress, unsigned short port = 54000);
    void disconnect();

    // 2. 每幀呼叫：處理等待連線（Host）與接收非阻塞封包
    void poll();

    // 3. 狀態查詢
    bool isConnected() const { return m_connected; }
    NetRole getRole() const { return m_role; }
    std::string getStatusMessage() const { return m_statusMessage; }

    // 4. 傳送資料給對手
    void sendMatchStart(GameModeType mode);
    void sendBoardSnapshot(const RemotePlayerState& localSnapshot);
    void sendGarbageAttack(int lines);
    void sendItemAttack(ItemType item);
    void sendGameOver();

    // 5. 讀取從對手收到的事件與最新畫面狀態
    const RemotePlayerState& getRemoteState() const { return m_remoteState; }
    int consumePendingGarbage();             // 取出並清空待處理的垃圾行數
    std::vector<ItemType> consumeEnemyItems(); // 取出並清空待處理的對手干擾道具
    bool hasOpponentLost() const { return m_opponentLost; }

private:
    sf::TcpListener m_listener; // 僅 Host 使用
    sf::TcpSocket m_socket;     // 雙方資料傳輸通道
    NetRole m_role = NetRole::None;
    bool m_connected = false;
    bool m_opponentLost = false;
    std::string m_statusMessage = "Offline";

    RemotePlayerState m_remoteState;
    int m_pendingGarbage = 0;
    std::vector<ItemType> m_pendingEnemyItems;
};

} // namespace Tetris