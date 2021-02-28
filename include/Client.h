#pragma once

#include <chrono>
#include <memory>
#include <queue>
#include <string>
#include <unordered_set>

#include "Common.h"
#include "Compat.h"
#include "VehicleData.h"

class TServer;

class TClient final {
public:
    using TSetOfVehicleData = std::unordered_set<TVehicleData>;

    struct TVehicleDataLockPair {
        TSetOfVehicleData& VehicleData;
        std::unique_lock<std::mutex> Lock;
    };

    explicit TClient(TServer& Server);

    void AddNewCar(int Ident, const std::string& Data);
    void SetCarData(int Ident, const std::string& Data);
    TVehicleDataLockPair GetAllCars();
    void SetName(const std::string& Name) { mName = Name; }
    void SetRoles(const std::string& Role) { mRole = Role; }
    std::string GetCarData(int Ident);
    void SetUDPAddr(sockaddr_in Addr) { mUDPAddress = Addr; }
    void SetDownSock(SOCKET CSock) { mSocket[1] = CSock; }
    void SetTCPSock(SOCKET CSock) { mSocket[0] = CSock; }
    void SetStatus(int Status) { mStatus = Status; }
    void DeleteCar(int Ident);
    sockaddr_in GetUDPAddr() const { return mUDPAddress; }
    std::string GetRoles() const { return mRole; }
    std::string GetName() const { return mName; }
    SOCKET GetDownSock() const { return mSocket[1]; }
    SOCKET GetTCPSock() const { return mSocket[0]; }
    void SetID(int ID) { mID = ID; }
    int GetOpenCarID() const;
    int GetCarCount() const;
    void ClearCars();
    int GetStatus() const { return mStatus; }
    int GetID() const { return mID; }
    bool IsConnected() const { return mIsConnected; }
    bool IsSynced() const { return mIsSynced; }
    bool IsSyncing() const { return mIsSyncing; }
    bool IsGuest() const { return mIsGuest; }
    void SetIsGuest(bool NewIsGuest) { mIsGuest = NewIsGuest; }
    void SetIsSynced(bool NewIsSynced) { mIsSynced = NewIsSynced; }
    void SetIsSyncing(bool NewIsSyncing) { mIsSyncing = NewIsSyncing; }
    void EnqueueMissedPacketDuringSyncing(const std::string& Packet) { mMissedPacketsDuringSyncing.push(Packet); }
    size_t MissedPacketQueueSize() const { return mMissedPacketsDuringSyncing.size(); }
    void SetIsConnected(bool NewIsConnected) { mIsConnected = NewIsConnected; }
    TServer& Server() const;
    void UpdatePingTime();
    int SecondsSinceLastPing();

private:
    TServer& mServer;
    bool mIsConnected = false;
    bool mIsSynced = false;
    bool mIsSyncing = false;
    std::queue<std::string> mMissedPacketsDuringSyncing;
    bool mIsGuest = false;
    std::mutex mVehicleDataMutex;
    TSetOfVehicleData mVehicleData;
    std::string mName = "Unknown Client";
    SOCKET mSocket[2] { SOCKET(-1) };
    sockaddr_in mUDPAddress {}; // is this initialization OK?
    std::string mRole;
    std::string mDID;
    int mStatus = 0;
    int mID = -1;
    std::chrono::time_point<std::chrono::high_resolution_clock> mLastPingTime;
};
