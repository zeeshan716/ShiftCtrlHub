#!/usr/bin/env python3
"""
=============================================================================
SHIFTCTRLHUB — LOCAL UDP RELAY BRIDGE FOR BROWSER TESTING
=============================================================================
Enables desktop web browsers (Chrome/Firefox/Edge) to transmit and receive
real-time raw UDP datagrams to/from the robot MCU on the local network.

Runs entirely on Python 3 Standard Library (ZERO pip dependencies required).
Listens on HTTP port 8765 for commands from app_redesign_preview.html,
and transmits raw UDP packets to the robot MCU (port 3333).
=============================================================================
"""

import sys
import socket
import threading
import json
from http.server import HTTPServer, BaseHTTPRequestHandler
from urllib.parse import urlparse, parse_qs

HTTP_PORT = 8765
DEFAULT_TARGET_IP = "192.168.4.1" # Default SoftAP IP (or set to STA router IP)
DEFAULT_UDP_PORT = 3333

# Global UDP Socket
udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
udp_socket.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)

target_ip = DEFAULT_TARGET_IP
target_port = DEFAULT_UDP_PORT
latest_rx_message = "NO_DATA"
rx_lock = threading.Lock()

# ---------------------------------------------------------------------------
# Background UDP Receiver Thread
# ---------------------------------------------------------------------------
def udp_receiver_loop():
    global latest_rx_message
    rx_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    rx_sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    try:
        rx_sock.bind(("", 3334)) # Local listen port for replies
    except Exception:
        pass

    while True:
        try:
            data, addr = rx_sock.recvfrom(1024)
            msg = data.decode('utf-8', errors='ignore').strip()
            with rx_lock:
                latest_rx_message = f"{addr[0]}:{addr[1]} -> {msg}"
            print(f"[UDP RX from {addr[0]}:{addr[1]}] {msg}")
        except Exception as e:
            pass

class ShiftCtrlHubBridgeHandler(BaseHTTPRequestHandler):
    def _set_cors_headers(self):
        self.send_header("Access-Control-Allow-Origin", "*")
        self.send_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS")
        self.send_header("Access-Control-Allow-Headers", "Content-Type")

    def do_OPTIONS(self):
        self.send_response(200)
        self._set_cors_headers()
        self.end_headers()

    def do_POST(self):
        global target_ip, target_port
        parsed = urlparse(self.path)

        if parsed.path == "/tx":
            content_length = int(self.headers.get('Content-Length', 0))
            post_body = self.rfile.read(content_length).decode('utf-8')
            
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
                # Send raw UDP datagram to robot MCU
                msg_bytes = (token + "\n").encode('utf-8')
                udp_socket.sendto(msg_bytes, (target_ip, target_port))
                print(f"[UDP TX -> {target_ip}:{target_port}] {token}")

            self.send_response(200)
            self._set_cors_headers()
            self.send_header("Content-Type", "application/json")
            self.end_headers()
            self.wfile.write(json.dumps({"status": "sent", "token": token, "ip": target_ip}).encode('utf-8'))

        elif parsed.path == "/discover":
            # Broadcast Discovery_Request to 255.255.255.255:3333
            broadcast_msg = b"Discovery_Request\n"
            udp_socket.sendto(broadcast_msg, ("255.255.255.255", target_port))
            print(f"[UDP BROADCAST] Discovery_Request sent to 255.255.255.255:{target_port}")

            self.send_response(200)
            self._set_cors_headers()
            self.send_header("Content-Type", "application/json")
            self.end_headers()
            self.wfile.write(json.dumps({"status": "discovery_broadcast_sent"}).encode('utf-8'))
        else:
            self.send_response(404)
            self.end_headers()

    def do_GET(self):
        parsed = urlparse(self.path)
        if parsed.path == "/status":
            self.send_response(200)
            self._set_cors_headers()
            self.send_header("Content-Type", "application/json")
            self.end_headers()
            with rx_lock:
                rx_msg = latest_rx_message
            resp = {
                "bridge": "online",
                "target_ip": target_ip,
                "target_port": target_port,
                "last_rx": rx_msg
            }
            self.wfile.write(json.dumps(resp).encode('utf-8'))
        else:
            self.send_response(200)
            self._set_cors_headers()
            self.send_header("Content-Type", "text/plain")
            self.end_headers()
            self.wfile.write(b"ShiftCtrlHub UDP Relay Bridge is Active. Send POST to /tx with token.")

    def log_message(self, format, *args):
        # Suppress noisy HTTP request logging
        return

def main():
    print("=================================================================")
    print("   SHIFTCTRLHUB — LOCAL UDP RELAY BRIDGE (ACTIVE)")
    print("=================================================================")
    print(f" * HTTP Bridge Endpoint : http://localhost:{HTTP_PORT}")
    print(f" * Default Target Robot : {DEFAULT_TARGET_IP}:{DEFAULT_UDP_PORT}")
    print(" * Ready for ShiftCtrlHub browser commands...")
    print("=================================================================\n")

    receiver_thread = threading.Thread(target=udp_receiver_loop, daemon=True)
    receiver_thread.start()

    server = HTTPServer(("127.0.0.1", HTTP_PORT), ShiftCtrlHubBridgeHandler)
    try:
        server.serve_forever()
    except KeyboardInterrupt:
        print("\nShutting down bridge...")
        server.server_close()

if __name__ == "__main__":
    main()
