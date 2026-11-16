# Product

<!-- impeccable:product-schema 1 -->

## Platform

android

## Stack

Flutter / Dart (cross-platform native mobile, GitHub Actions automated CI APK releases)

## Users

Robotics engineers, hobbyists, students, and competition makers (Line Follower Robot competitions, autonomous rovers, differential-drive bots, lab microcontrollers). Fast two-thumb mobile operation in dynamic environments (outdoor sunlit arenas, indoor lab workbenches).

## Product Purpose

A universal, zero-cost, zero-ad, high-reliability standalone mobile transmitter application for microcontrollers (ESP32, Arduino, Raspberry Pi, STM32). Provides instant, rock-solid direct wireless control (Bluetooth Low Energy, Bluetooth Classic SPP, Wi-Fi SoftAP, Wi-Fi STA UDP) with dedicated tactile control surfaces.

## Positioning

Unlike bloated cloud IoT dashboards or sluggish webview remotes, this is a low-latency, responsive, physical-transmitter-grade control surface built for actual robotics field testing. It replaces multi-screen clutter with dedicated, purpose-built cockpits: 4WD Cardinal D-Pad, 2WD Dual Sliders (auto-centering to neutral, zero stop-button clutter), Virtual Analog Joystick, and Line Follower Robot (LFR) Autonomous Mode with live PID + Kv parameter tuning.

## Operating Context

- **Physical Grip:** Two-thumb horizontal landscape orientation on mobile phones.
- **Environments:** Outdoor direct sunlight (requires high-contrast Light Mode) and dark robotics laboratories/pits (requires deep Dark Mode).
- **Communication Real-Time:** Deterministic single-character tokens (`C:F`, `C:S`, `C:L`, `C:R`, `S:<val>`, `V:Kp:<val>`, `V:Kv:<val>`) over serial/UDP streams with heartbeat telemetry.

## Capabilities and Constraints

- **Transports:**
  - Bluetooth Low Energy (BLE) for ESP32/modern chips (Service UUID `4fafc201-1fb5-459e-8fcc-c5c9c331914b`)
  - Bluetooth Classic SPP for HC-05/HC-06/Raspberry Pi
  - Wi-Fi SoftAP (Direct AP at `192.168.4.1:3333` UDP)
  - Wi-Fi STA (Local network UDP broadcast `Discovery_Request` / `IP:<addr>` auto-discovery)
- **Robotics Control Cockpits:**
  - **4WD Rover:** High-tactile Cardinal Buttons (Forward, Backward, Left, Right) with instantaneous hold/release commands and speed slider.
  - **2WD Differential Drive:** Independent Left & Right Tank-Drive Sliders with spring auto-centering to neutral. **Explicit constraint: NO separate Stop button.**
  - **Analog Joystick:** 360-degree polar vector drive with deadband and dynamic differential wheel speed calculation.
  - **LFR Autonomous Tuning:** Live parameter adjustment for Base Speed, `Kp`, `Ki`, `Kd`, and `Kv`. 3-way input: precision steppers, responsive sliders, and direct numerical typing.
- **Visual & UX Constraints:**
  - Must NOT cram all controls onto one screen.
  - Dedicated full-screen surfaces per mode, switchable via minimal top navigation.
  - Tactile micro-interactions (authentic physical depression, micro-LED active state indicators, tabular numerals `tabular-nums` for rock-solid non-jittering readings).
  - High performance: no frame drops, no obstructive animations during driving.

## Design & Developer Commitments

- **Forever Free & Ad-Free:** ShiftCtrlHub will forever remain **100% free**, Inshallah, and will **always stay completely ad-free** — zero advertisements, zero tracking, and zero feature paywalls.
- **Minimalist Hardware Instrument Craft:** High-craft physical instrument design (tactile physical console, precision machined aesthetics, tactile thumb wells, zero generic web-card clutter).
- **Legibility & Contrast:** Clear, readable system typography and high-contrast status feedback for both sunlit arenas and dark labs.
