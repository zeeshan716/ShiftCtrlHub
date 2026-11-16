---
name: ShiftCtrlHub Universal Remote
description: Modern minimalism, architectural luxury hardware design system for ground robotics
colors:
  # ShiftCtrlHub Core Palette (from Spec)
  black: "#000000"
  burgundy: "#6D001A"
  white: "#FFFFFF"

  # Onyx Dark Palette (Default)
  dark-bg: "#050507"
  dark-chassis: "#0C0C0E"
  dark-card: "#141418"
  dark-key: "#1B1B22"
  dark-border: "#272730"
  dark-accent: "#6D001A"
  dark-accent-glow: "rgba(109, 0, 26, 0.35)"
  dark-text-primary: "#FFFFFF"
  dark-text-muted: "#8F8F9D"

  # Gallery Light Palette (Sunlight Architectural Contrast)
  light-bg: "#EFEFF2"
  light-chassis: "#FFFFFF"
  light-card: "#F7F7FA"
  light-key: "#FFFFFF"
  light-border: "#D8D8DF"
  light-accent: "#6D001A"
  light-text-primary: "#000000"
  light-text-muted: "#575764"

typography:
  family: "-apple-system, BlinkMacSystemFont, 'SF Pro Display', 'Helvetica Neue', sans-serif"
  mono: "'SF Mono', 'Cascadia Mono', ui-monospace, monospace"
---

# Design System: ShiftCtrlHub Modern Minimalism

## 1. Visual Language & Palette
- **Pure Black (`#000000`):** Monolithic architectural void, dark chassis, negative space.
- **Deep Burgundy (`#6D001A`):** Signature jewel tone — active directional key presses, glowing throttle fill, active switch pills, and status indicator.
- **Crisp White (`#FFFFFF`):** High-contrast typography, directional chevrons, speed numeral.

## 2. Ultra-Minimalist Ergonomic Cockpit
- **Center Area (Zero Clutter):** Completely removed the circular compass disc, status pills, and descriptive helper text. Pure negative breathing space.
- **Left Thumb (Slender Throttle):** A slim, stylish 26px precision vertical rail with glowing burgundy level fill and floating thumb pill (`0–255`, `S:<val>`).
- **Right Thumb (Motion Diamond):** 4-movement directional diamond (Forward `C:F`, Reverse `C:B`, Left `C:L`, Right `C:R`) with pure white chevrons that sink and illuminate in burgundy on press.
- **Simultaneous Operation:** Independent pointer tracking (`pointerId`) allows smooth speed gliding and directional tapping/holding simultaneously.

## 3. Ribbon Layout (Center Transports & Upper-Right Switches)
- **Left:** Pure `ShiftCtrlHub` wordmark with glowing jewel dot.
- **Center of the Ribbon (4 Ways to Connect):**
  * Sleek segmented transport selector: `[ SoftAP | STA | BLE | SPP ]`.
  * Instant 1-tap switching between transports:
    - `SoftAP`: Wi-Fi direct connection (`192.168.4.1:3333`)
    - `STA`: Local Wi-Fi router network (`UDP 3333`)
    - `BLE`: Bluetooth Low Energy (`BLE GATT UART Service`)
    - `SPP`: Bluetooth Classic (`HC-05 / Raspberry Pi SPP`)
  * Live status pill underneath (`192.168.4.1:3333 ⚙`) opens the wireless configuration drawer.
- **Right Upper End (Kodular-Inspired Control Station):**
  * **Immediate Left:** Redesigned dedicated **Buttons / Joystick button (`btnBjToggle`)**:
    - Distinct tactile button with dynamic glyph (`✛` in Buttons mode, `🕹` in Joystick mode).
    - Dynamic label: `BUTTONS` vs `JOYSTICK`.
    - Illuminated jewel accent strip and 3D tactile feedback.
  * **Right End:** Vertically stacked segmented switches in one line:
    - Switch 1: `[ RC | LFR ]`
    - Switch 2: `[ 4v4 | 2WD ]`
  * Generous vicinity touch targets: tapping anywhere near switches toggles them instantly.

## 4. Cockpit Modes & Ergonomics
- **Headings Removed:** All redundant text labels like "Throttle", "Steering", or "Heading" have been eliminated because the directional buttons and slider geometries are self-evident.
- **Adjusted Throttle Numerical Value (4v4 Mode):**
  * In 4v4 mode, the live throttle number (`180`) is positioned directly above the center of the vertical slider track in a crisp monospace badge (`.throttle-value-badge`), providing perfect geometric balance.
- **2WD Tank Drive (Sliders Mode):**
  * Left: Vertical Throttle slider with centered live readout (`0%`, dispatches `S:FB:<val>`). Spring-return auto-centers to 0% neutral on release.
  * Right: Horizontal Steering slider with centered live readout (`0%`, dispatches `S:RL:<val>`). Spring-return auto-centers to 0% neutral on release.
  * Fail-safe auto-centering acts as an instant brake with zero separate stop button.
3. **2WD Diff Drive (Joystick Mode):**
   * Full 360° virtual joystick available in 2WD mode!
   * Implements Kodular's exact `Directions_2WD` differential steering algorithm from `Screen1.bky`:
     - Dispatches `C:F`, `C:B`, `C:FR`, `C:FL`, `C:BR`, `C:BL`, `C:L`, `C:R`, and `C:S` on release/deadband.
4. **4WD Rover (Joystick Mode):**
   * Implements Kodular's `Directions_4v4` algorithm with dedicated strafe/turn-in-place vectors (`C:F`, `C:B`, `C:R`, `C:L`, `C:FR`, `C:FL`, `C:BR`, `C:BL`, `C:S`).
5. **Line Follower Console (LFR PID):**
   * 5 parameters: `Base Speed`, `Kp`, `Ki`, `Kd`, `Kv`.
   * Real-time sliders, stepper nudgers (`-`/`+`), and direct numeric inputs.
   * Presets: `Default`, `High Speed`, `Hairpin`.
   * **Transmit All Button:**
     - In live tuning, moving a slider sends individual real-time tokens (`V:Kp:2.5`).
     - **Transmit All** sends all 5 parameters simultaneously in a single burst (`V:ALL:<speed>:<kp>:<ki>:<kd>:<kv>`). This is essential when switching presets, recovering from a brief reconnect, or after typing values manually so the microcontroller receives the complete coherent tuning state without intermediate stutter.

