#pragma once
#include <Arduino.h>
#include "config.h"

/* =============================================================================
 * SHIFTCTRLHUB — UNIFIED WIRELESS TRANSPORTS (UDP, BLE, SPP)
 * =============================================================================
 * Implements:
 *   - Mode 1: Wi-Fi SoftAP with UDP (Port 3333, IP: 192.168.4.1)
 *   - Mode 2: Wi-Fi STA with WiFiMulti & UDP (Multi-AP failover + UDP Discovery)
 *   - Mode 3: Bluetooth Low Energy (BLE GATT Server UART)
 *   - Mode 4: Bluetooth Classic SPP (BluetoothSerial)
 * ============================================================================= */

#if ACTIVE_TRANSPORT == CONN_STA
  #include <WiFi.h>
  #include <WiFiMulti.h>
  #include <WiFiUdp.h>

  static WiFiMulti wifiMulti;
  static WiFiUDP udp;
  static char incomingBuffer[256];
  static IPAddress remoteClientIP;
  static uint16_t remoteClientPort = 0;
  static unsigned long lastHbTime = 0;

  class TransportManager {
  public:
    void begin() {
      Serial.println("\n[ShiftCtrlHub] Initializing Wi-Fi STA with WiFiMulti...");
      WiFi.mode(WIFI_STA);

      // Register all user-configured AP credentials
      for (size_t i = 0; i < STA_NETWORK_COUNT; i++) {
        wifiMulti.addAP(STA_NETWORKS[i].ssid, STA_NETWORKS[i].password);
        Serial.printf("[WiFiMulti] Registered AP: %s\n", STA_NETWORKS[i].ssid);
      }

      Serial.print("[WiFiMulti] Connecting to best available AP");
      unsigned long connectStart = millis();
      while (wifiMulti.run() != WL_CONNECTED && (millis() - connectStart < 15000UL)) {
        delay(300);
        Serial.print(".");
      }

      if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\n[WiFiMulti] Connected Successfully!");
        Serial.printf("[WiFiMulti] Active SSID: %s\n", WiFi.SSID().c_str());
        Serial.printf("[WiFiMulti] IP Address : %s\n", WiFi.localIP().toString().c_str());
        Serial.printf("[WiFiMulti] Signal RSSI: %d dBm\n", WiFi.RSSI());
      } else {
        Serial.println("\n[WiFiMulti] WARN: No AP connected within 15s (update STA_NETWORKS in config.h). Will retry in background.");
      }

      udp.begin(UDP_PORT);
      Serial.printf("[UDP] Listening for commands on UDP port %d\n", UDP_PORT);
    }

    String checkIncoming() {
      static unsigned long lastReconnectAttempt = 0;
      // 1. Non-blocking periodic auto-reconnect failover
      if (WiFi.status() != WL_CONNECTED) {
        if (millis() - lastReconnectAttempt > 5000UL) {
          lastReconnectAttempt = millis();
          Serial.println("[WiFiMulti] Reconnecting to configured APs...");
          if (wifiMulti.run() == WL_CONNECTED) {
            Serial.printf("[WiFiMulti] Connected! IP: %s\n", WiFi.localIP().toString().c_str());
            udp.begin(UDP_PORT);
          }
        }
        return "";
      }

      // 2. Poll UDP Socket for datagrams
      int packetSize = udp.parsePacket();
      if (packetSize > 0) {
        remoteClientIP = udp.remoteIP();
        remoteClientPort = udp.remotePort();

        int len = udp.read(incomingBuffer, sizeof(incomingBuffer) - 1);
        if (len <= 0) return "";
        incomingBuffer[len] = '\0';
        String msg = String(incomingBuffer);
        msg.trim();

        // Check for UDP Auto-Discovery Broadcast
        if (msg == DISCOVERY_REQ_TOKEN || msg == HANDSHAKE_REQ_TOKEN) {
          Serial.printf("[UDP-DISCOVERY] Request from %s:%d -> Replying with IP\n",
                        remoteClientIP.toString().c_str(), remoteClientPort);
          udp.beginPacket(remoteClientIP, remoteClientPort);
          udp.printf("IP:%s\n", WiFi.localIP().toString().c_str());
          udp.endPacket();
          return ""; // Handled internally
        }

        return msg;
      }

      // 3. Optional periodic heartbeat to client
      if (remoteClientPort > 0 && millis() - lastHbTime > HEARTBEAT_INTERVAL_MS) {
        lastHbTime = millis();
        udp.beginPacket(remoteClientIP, remoteClientPort);
        udp.print("HB\n");
        udp.endPacket();
      }

      return "";
    }
  };

#elif ACTIVE_TRANSPORT == CONN_SOFTAP
  #include <WiFi.h>
  #include <WiFiUdp.h>

  static WiFiUDP udp;
  static char incomingBuffer[256];
  static IPAddress remoteClientIP;
  static uint16_t remoteClientPort = 0;

  class TransportManager {
  public:
    void begin() {
      Serial.println("\n[ShiftCtrlHub] Initializing Wi-Fi SoftAP...");
      WiFi.mode(WIFI_AP);

      IPAddress local_IP(192, 168, 4, 1);
      IPAddress gateway(192, 168, 4, 1);
      IPAddress subnet(255, 255, 255, 0);

      WiFi.softAPConfig(local_IP, gateway, subnet);
      WiFi.softAP(SOFTAP_SSID, SOFTAP_PASS, SOFTAP_CHANNEL, 0, SOFTAP_MAX_CONN);

      Serial.printf("[SoftAP] Hotspot SSID: %s\n", SOFTAP_SSID);
      Serial.printf("[SoftAP] Hotspot Pass: %s\n", SOFTAP_PASS);
      Serial.printf("[SoftAP] Gateway IP  : %s\n", WiFi.softAPIP().toString().c_str());

      udp.begin(UDP_PORT);
      Serial.printf("[UDP] Listening on port %d\n", UDP_PORT);
    }

    String checkIncoming() {
      int packetSize = udp.parsePacket();
      if (packetSize > 0) {
        remoteClientIP = udp.remoteIP();
        remoteClientPort = udp.remotePort();

        int len = udp.read(incomingBuffer, sizeof(incomingBuffer) - 1);
        if (len <= 0) return "";
        incomingBuffer[len] = '\0';
        String msg = String(incomingBuffer);
        msg.trim();

        if (msg == HANDSHAKE_REQ_TOKEN || msg == DISCOVERY_REQ_TOKEN) {
          udp.beginPacket(remoteClientIP, remoteClientPort);
          udp.print("IP:192.168.4.1\n");
          udp.endPacket();
          return "";
        }
        return msg;
      }
      return "";
    }
  };

#elif ACTIVE_TRANSPORT == CONN_BLE
  #include <BLEDevice.h>
  #include <BLEServer.h>
  #include <BLEUtils.h>
  #include <BLE2902.h>

  static bool bleDeviceConnected = false;
  static String bleIncomingCommand = "";
  static portMUX_TYPE bleMux = portMUX_INITIALIZER_UNLOCKED;

  class ShiftCtrlServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      bleDeviceConnected = true;
      Serial.println("[BLE] Client connected!");
    }
    void onDisconnect(BLEServer* pServer) {
      bleDeviceConnected = false;
      Serial.println("[BLE] Client disconnected! Restarting advertising...");
      pServer->startAdvertising();
    }
  };

  class ShiftCtrlCharCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
#if defined(ESP_ARDUINO_VERSION_MAJOR) && ESP_ARDUINO_VERSION_MAJOR >= 3
      String rxVal = pCharacteristic->getValue();
#else
      std::string raw = pCharacteristic->getValue();
      String rxVal = String(raw.c_str());
#endif
      if (rxVal.length() > 0) {
        rxVal.trim();
        portENTER_CRITICAL(&bleMux);
        bleIncomingCommand = rxVal;
        portEXIT_CRITICAL(&bleMux);
      }
    }
  };

  class TransportManager {
  private:
    BLEServer *pServer = nullptr;
    BLECharacteristic *pRxChar = nullptr;

  public:
    void begin() {
      Serial.println("\n[ShiftCtrlHub] Initializing BLE GATT Server...");
      BLEDevice::init(BT_DEVICE_NAME);

      pServer = BLEDevice::createServer();
      pServer->setCallbacks(new ShiftCtrlServerCallbacks());

      BLEService *pService = pServer->createService(BLE_SERVICE_UUID);
      pRxChar = pService->createCharacteristic(
                  BLE_CHARACTERISTIC_UUID,
                  BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_WRITE_NR
                );
      pRxChar->setCallbacks(new ShiftCtrlCharCallbacks());
      pService->start();

      BLEAdvertising *pAdv = BLEDevice::getAdvertising();
      pAdv->addServiceUUID(BLE_SERVICE_UUID);
      pAdv->setScanResponse(true);

      BLEAdvertisementData advData;
      advData.setName(BT_DEVICE_NAME);
      pAdv->setAdvertisementData(advData);

      BLEDevice::startAdvertising();
      Serial.printf("[BLE] Broadcasting as '%s'...\n", BT_DEVICE_NAME);
    }

    String checkIncoming() {
      String cmd = "";
      if (bleIncomingCommand.length() > 0) {
        portENTER_CRITICAL(&bleMux);
        cmd = bleIncomingCommand;
        bleIncomingCommand = "";
        portEXIT_CRITICAL(&bleMux);
        cmd.trim();
      }
      return cmd;
    }
  };

#elif ACTIVE_TRANSPORT == CONN_SPP
  #include <BluetoothSerial.h>
  static BluetoothSerial SerialBT;

  class TransportManager {
  public:
    void begin() {
      Serial.println("\n[ShiftCtrlHub] Initializing Classic Bluetooth SPP...");
      if (!SerialBT.begin(BT_DEVICE_NAME)) {
        Serial.println("[SPP] An error occurred initializing Bluetooth SPP!");
      } else {
        Serial.printf("[SPP] Bluetooth SPP device ready. Pair with '%s'\n", BT_DEVICE_NAME);
      }
    }

    String checkIncoming() {
      if (SerialBT.available()) {
        String msg = SerialBT.readStringUntil('\n');
        msg.trim();
        return msg;
      }
      return "";
    }
  };

#endif
