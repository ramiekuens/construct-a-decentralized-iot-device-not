#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <bitset>

using namespace std;

// Device structure
struct Device {
    string id;
    string type;
    string location;
    bitset<8> status; // 8-bit status register
};

// IoT Hub class
class IoT_Hub {
private:
    map<string, Device> devices;
    map<string, vector<string>> subscriptions;

public:
    void addDevice(Device device) {
        devices[device.id] = device;
    }

    void removeDevice(string deviceId) {
        devices.erase(deviceId);
    }

    void subscribe(string deviceId, string userId) {
        subscriptions[userId].push_back(deviceId);
    }

    void unsubscribe(string userId, string deviceId) {
        auto it = subscriptions.find(userId);
        if (it != subscriptions.end()) {
            it->second.erase(remove(it->second.begin(), it->second.end(), deviceId), it->second.end());
        }
    }

    void notify(string deviceId, bitset<8> status) {
        auto it = devices.find(deviceId);
        if (it != devices.end()) {
            it->second.status = status;
            for (auto userId : subscriptions) {
                if (find(userId.second.begin(), userId.second.end(), deviceId) != userId.second.end()) {
                    cout << "Notifying user " << userId.first << " about device " << deviceId << " status change to " << status << endl;
                }
            }
        }
    }
};

int main() {
    IoT_Hub hub;

    Device device1 = {"device1", "temperature sensor", "living room", bitset<8>(0b00000001)};
    Device device2 = {"device2", "humidity sensor", "kitchen", bitset<8>(0b00000010)};

    hub.addDevice(device1);
    hub.addDevice(device2);

    hub.subscribe("user1", "device1");
    hub.subscribe("user1", "device2");
    hub.subscribe("user2", "device1");

    hub.notify("device1", bitset<8>(0b00000011));
    hub.notify("device2", bitset<8>(0b00000100));

    return 0;
}