#!/usr/bin/env python3
"""
=============================================================================
SHIFTCTRLHUB — UNIFIED LOCAL WEB & UDP SERVER
=============================================================================
1. Serves the ShiftCtrlHub web interface to any browser (PC or Mobile):
   - Local PC access : http://localhost:8080/
   - Mobile phone LAN: http://<your-lan-ip>:8080/
2. Bridges touch interactions directly to the robot MCU over raw UDP:
   - Tapping any button/slider sends an instant UDP datagram to port 3333.
   - Zero external pip dependencies (Pure standard library).
=============================================================================
"""

import sys
import os
import socket
import threading
import json
from http.server import HTTPServer, SimpleHTTPRequestHandler
from urllib.parse import urlparse

HTTP_PORT = 8080
DEFAULT_TARGET_IP = "192.168.4.1"  # SoftAP default IP (or set to STA router IP)
DEFAULT_UDP_PORT = 3333
WORKSPACE_DIR = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))

# Global UDP Socket
udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
udp_socket.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)

target_ip = DEFAULT_TARGET_IP
target_port = DEFAULT_UDP_PORT
latest_rx_message = "NO_DATA"
rx_lock = threading.Lock()

def get_lan_ip():
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        s.connect(("8.8.8.8", 80))
        ip = s.getsockname()[0]
        s.close()
        return ip
    except Exception:
        return "127.0.0.1"

# ---------------------------------------------------------------------------
# Background UDP Receiver Loop
# ---------------------------------------------------------------------------
def udp_receiver_loop():
    global latest_rx_message
    rx_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    rx_sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    try:
        rx_sock.bind(("", 3334))  # Listen for robot MCU replies / heartbeats
    except Exception:
        pass

    while True:
        try:
            data, addr = rx_sock.recvfrom(1024)
            msg = data.decode("utf-8", errors="ignore").strip()
            with rx_lock:
                latest_rx_message = f"{addr[0]}:{addr[1]} -> {msg}"
            print(f"[UDP RX from {addr[0]}:{addr[1]}] {msg}")
        except Exception:
            pass

# ---------------------------------------------------------------------------
# HTTP Request Handler (Serving Static Web + UDP API)
# ---------------------------------------------------------------------------
class ShiftCtrlHubHandler(SimpleHTTPRequestHandler):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, directory=WORKSPACE_DIR, **kwargs)

    def _set_cors_headers(self):
        self.send_header("Access-Control-Allow-Origin", "*")
        self.send_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS")
        self.send_header("Access-Control-Allow-Headers", "Content-Type")

    def do_OPTIONS(self):
        self.send_response(200)
        self._set_cors_headers()
        self.end_headers()

    def do_GET(self):
        parsed = urlparse(self.path)
        if parsed.path in ["/", "/index.html", "/remote"]:
            self.path = "/app_redesign_preview.html"
            return super().do_GET()
        elif parsed.path == "/status":
            self.send_response(200)
            self._set_cors_headers()
            self.send_header("Content-Type", "application/json")
            self.end_headers()
            with rx_lock:
                rx_msg = latest_rx_message
            resp = {
                "server": "online",
                "lan_ip": get_lan_ip(),
                "target_ip": target_ip,
                "target_port": target_port,
                "last_rx": rx_msg
            }
            self.wfile.write(json.dumps(resp).encode("utf-8"))
            return
        else:
            return super().do_GET()

    def do_POST(self):
        global target_ip, target_port
        parsed = urlparse(self.path)

        if parsed.path in ["/tx", "/api/tx"]:
            content_length = int(self.headers.get("Content-Length", 0))
            post_body = self.rfile.read(content_length).decode("utf-8")

            token = ""
            try:
                payload = json.loads(post_body)
                token = payload.get("token", "")
                if "ip" in payload and payload["ip"]:
                    target_ip = payload["ip"]
                if "port" in payload and payload["port"]:
                    target_port = int(payload["port"])
            except json.JSONDecodeError:
                token = post_body.strip()

            if token:
                msg_bytes = (token + "\n").encode("utf-8")
                udp_socket.sendto(msg_bytes, (target_ip, target_port))
                print(f"[UDP TX -> {target_ip}:{target_port}] {token}")

            self.send_response(200)
            self._set_cors_headers()
            self.send_header("Content-Type", "application/json")
            self.end_headers()
            self.wfile.write(json.dumps({"status": "sent", "token": token, "target": f"{target_ip}:{target_port}"}).encode("utf-8"))

        elif parsed.path in ["/discover", "/api/discover"]:
            broadcast_msg = b"Discovery_Request\n"
            udp_socket.sendto(broadcast_msg, ("255.255.255.255", target_port))
            print(f"[UDP BROADCAST] Discovery_Request sent to 255.255.255.255:{target_port}")

            self.send_response(200)
            self._set_cors_headers()
            self.send_header("Content-Type", "application/json")
            self.end_headers()
            self.wfile.write(json.dumps({"status": "discovery_broadcast_sent"}).encode("utf-8"))
        else:
            self.send_response(404)
            self.end_headers()

    def log_message(self, format, *args):
        # Keep console clean; only show important outputs
        if "POST" in args[0] or "404" in args[0]:
            print(f"[HTTP] {self.address_string()} - {args[0]}")

def main():
    lan_ip = get_lan_ip()
    print("=================================================================")
    print("   SHIFTCTRLHUB — UNIFIED LOCAL WEB & UDP SERVER (ONLINE)")
    print("=================================================================")
    print(f" * Computer Browser Link: http://localhost:{HTTP_PORT}/")
    print(f" * Mobile Phone Link    : http://{lan_ip}:{HTTP_PORT}/")
    print(f" * Target Robot Socket  : {DEFAULT_TARGET_IP}:{DEFAULT_UDP_PORT}")
    print("-----------------------------------------------------------------")
    print(" * Ready! Open the link above in Chrome on your PC or phone.")
    print("=================================================================\n")

    rx_thread = threading.Thread(target=udp_receiver_loop, daemon=True)
    rx_thread.start()

    server = HTTPServer(("0.0.0.0", HTTP_PORT), ShiftCtrlHubHandler)
    try:
        server.serve_forever()
    except KeyboardInterrupt:
        print("\nShutting down ShiftCtrlHub Server...")
        server.server_close()

if __name__ == "__main__":
    main()
