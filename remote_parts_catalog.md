# Universal Remote Control: Component & Parts Catalog

> **Purpose**: This catalog defines standardized names and technical identifiers for every individual component, container, button, slider, track, badge, and indicator in the **ShiftCtrlHub** remote control interface.  
> **How to Use**: When requesting adjustments, simply name the **Part Name** or **Identifier** from this document (e.g., *"Make `4WD-THROTTLE-KNOB` wider"* or *"Change the font of `FOOTER-TX-TOKEN`"*). Only that exact component will be altered, guaranteeing zero unintended side-effects or collateral drift.

---

## Visual Layout Overview

```
+-----------------------------------------------------------------------------------------------------------------------+
| [TOP-BAR-WORDMARK]            [TRANSPORT-TAB-SOFTAP] [STA] [BLE] [SPP]                [INPUT-TOGGLE-BTN]  [RC | LFR]  |
|                                       [TRANSPORT-SUB-BADGE]                                               [4v4 | 2WD] |
+-----------------------------------------------------------------------------------------------------------------------+
|                                                                                                                       |
|   (LEFT ZONE)                                    (CENTER ZONE)                                (RIGHT ZONE)            |
|                                                                                                                       |
|   [4WD-THROTTLE-READOUT]                                                                       [KEY-UP]               |
|   +-------------------+                                                                [KEY-LEFT]    [KEY-RIGHT]      |
|   | [4WD-THROTTLE-    |                      (NEGATIVE-SPACE)                                 [KEY-DOWN]              |
|   |  TRACK / FILL]    |                                                                                               |
|   | [THROTTLE-KNOB]   |                                                                                               |
|   +-------------------+                                                                                               |
|                                                                                                                       |
+-----------------------------------------------------------------------------------------------------------------------+
| [FOOTER-TX-TOKEN] [FOOTER-SPEED-BADGE]                         [FOOTER-MODE-STATUS] | [FOOTER-PROTOCOL-STATUS]        |
+-----------------------------------------------------------------------------------------------------------------------+
```

---

## 1. Frame, Chassis & Preview Tools

| Part Code | Common Name | DOM ID / Selector | Location | Function & Visual Role |
| :--- | :--- | :--- | :--- | :--- |
| `APP-SHELL` | **Outer Page Canvas** | `body` | Fullscreen background | Hosts ambient radial lighting, text selection, and theme variables. |
| `CHASSIS-FRAME` | **Device Chassis** | `#deviceChassis`, `.device-chassis` | Center screen | The landscape tablet/controller bezel (960×480 px default). |
| `TOOL-TOOLBAR` | **Testing Toolbar** | `.toolbar-quick` | Above chassis | Preview helper bar for desktop browsers. |
| `TOOL-THEME-BTN` | **Theme Switcher** | `#btnThemeToggle`, `.quick-btn` | Upper right (desktop bar) | Toggles between Dark Mode and Light Mode. |
| `TOOL-THEME-DOT` | **Theme Indicator Dot** | `#themeDot` | Inside theme button | Visual color pip showing active theme state. |
| `TOOL-FULLSCREEN-BTN` | **Fullscreen Button** | Button with `toggleFullscreen()` | Upper right (desktop bar) | Expands chassis to fill 100vw × 100vh. |

---

## 2. Top Ribbon Bar (`.app-header`)

### 2.1 Brand & Connection Health (Left)
| Part Code | Common Name | DOM ID / Selector | Location | Function & Visual Role |
| :--- | :--- | :--- | :--- | :--- |
| `BRAND-WORDMARK` | **ShiftCtrlHub Wordmark** | `.brand-mark` | Header (far left) | Main typography wordmark; clicking opens Wireless Settings Drawer. |
| `BRAND-HEARTBEAT-LED` | **Telemetry Heartbeat Dot** | `.brand-dot` | Preceding ShiftCtrlHub text | Pulsing emerald LED indicating live telemetry activity. |

### 2.2 Transport Mode Ribbon (Center)
| Part Code | Common Name | DOM ID / Selector | Location | Function & Visual Role |
| :--- | :--- | :--- | :--- | :--- |
| `TRANSPORT-CONTAINER` | **Transport Ribbon Bar** | `#transportBar`, `.transport-bar` | Header (center top) | Segmented container housing the 4 wireless connection choices. |
| `TRANSPORT-TAB-SOFTAP` | **SoftAP Tab** | `#tabSoftap`, `.transport-tab` | Transport bar (item 1) | Direct Wi-Fi Access Point mode (`192.168.4.1`). Glowing Emerald pill. |
| `TRANSPORT-TAB-STA` | **STA Tab** | `#tabSta`, `.transport-tab` | Transport bar (item 2) | Local WLAN / Router mode. Glowing Cyan pill. |
| `TRANSPORT-TAB-BLE` | **BLE Tab** | `#tabBle`, `.transport-tab` | Transport bar (item 3) | Bluetooth Low Energy UART mode. Glowing Azure pill. |
| `TRANSPORT-TAB-SPP` | **SPP Tab** | `#tabSpp`, `.transport-tab` | Transport bar (item 4) | Bluetooth Classic SPP mode (HC-05). Glowing Amber pill. |
| `TRANSPORT-TAB-DOT` | **Transport Status Dot** | `.transport-tab-dot` | Inside each transport tab | 6px indicator pip that glows with the protocol's signature color. |
| `TRANSPORT-SUB-BADGE` | **Socket Address Badge** | `.conn-sub-badge`, `#activeConnDetail` | Directly beneath transport bar | Displays active IP/port or device name (e.g. `192.168.4.1:3333`). |
| `TRANSPORT-SETTINGS-GEAR` | **Wireless Settings Gear** | `.conn-gear` | Beside socket address | Gear icon; triggers the Wireless Configuration Drawer on click. |

### 2.3 Mode & Input Control Cluster (Upper Right)
| Part Code | Common Name | DOM ID / Selector | Location | Function & Visual Role |
| :--- | :--- | :--- | :--- | :--- |
| `INPUT-TOGGLE-BTN` | **Buttons / Joystick Circular Button** | `#btnBjToggle`, `.bj-custom-button` | Left of mode switches | 52×52 px circular button. Toggles between discrete buttons/sliders and joystick. |
| `INPUT-TOGGLE-ICON` | **Input Toggle Glyph** | `#bjGlyph`, `.bj-glyph-icon` | Inside circular button | Displays `✛` for Buttons mode, `🕹` for Joystick mode. |
| `INPUT-TOGGLE-LABEL` | **Input Toggle Label** | `#bjTitle`, `.bj-label-text` | Inside circular button | Monospace micro-label (`BUTTONS` vs `JOYSTICK`). |
| `SWITCH-RC-LFR-ZONE` | **RC / LFR Touch Zone** | Wrapper around `#rcLfrSwitch` | Top row of switch stack | Generous touch padding allowing toggle clicks anywhere in vicinity. |
| `SWITCH-RC-LFR` | **RC / LFR Switch** | `#rcLfrSwitch`, `.segmented-switch` | Top row of switch stack | Primary mode selector switch. |
| `PILL-RC` | **RC Mode Pill** | `#pillRC`, `.switch-pill` | Left half of RC/LFR switch | Manual Radio Control driving mode. Active state in Burgundy. |
| `PILL-LFR` | **LFR Mode Pill** | `#pillLFR`, `.switch-pill` | Right half of RC/LFR switch | Autonomous Line Follower tuning mode. Active state in Violet. |
| `SWITCH-DRIVE-ZONE` | **4v4 / 2WD Touch Zone** | Wrapper around `#driveSwitch` | Bottom row of switch stack | Generous touch padding for drive mode selection. |
| `SWITCH-DRIVE` | **4v4 / 2WD Switch** | `#driveSwitch`, `.segmented-switch` | Bottom row of switch stack | Drive configuration selector switch. |
| `PILL-4V4` | **4v4 Drive Pill** | `#pill4v4`, `.switch-pill` | Left half of 4v4/2WD switch | 4WD Differential-Drive / Skid-Steer mode. |
| `PILL-2WD` | **2WD Drive Pill** | `#pill2WD`, `.switch-pill` | Right half of 4v4/2WD switch | 2WD Front-Wheel Steer (real car / servo steer) mode. |

---

## 3. Cockpit Surface 1: 4WD Rover (Buttons Mode)

Active when `MODE: RC`, `DRIVE: 4v4`, and `INPUT: Buttons`. (DOM: `#view4wdButtons`)

| Part Code | Common Name | DOM ID / Selector | Location | Function & Visual Role |
| :--- | :--- | :--- | :--- | :--- |
| `4WD-ZONE-LEFT` | **Left Throttle Zone** | `.throttle-zone-vertical` | Left side (width: 90px) | Houses the vertical master throttle slider and live numeric readout. |
| `4WD-THROTTLE-READOUT` | **4WD Speed Number Badge** | `#speedNum4wd`, `.throttle-value-badge` | Above throttle track | Crisp monospace badge displaying current PWM (`0`–`255`). Dynamic heat glow. |
| `4WD-THROTTLE-TRACK` | **4WD Throttle Slider Rail** | `#throttleTrack4wd`, `.slim-vertical-track` | Left thumb well (height: 270px) | Concave recessed track with pointer capture for smooth vertical dragging. |
| `4WD-THROTTLE-FILL` | **4WD Throttle Heat Bar** | `#throttleFill4wd`, `.slim-vertical-fill` | Inside throttle track | Multi-stop gradient fill (Burgundy $\rightarrow$ Ruby $\rightarrow$ Flame Red) showing power. |
| `4WD-THROTTLE-KNOB` | **4WD Throttle Thumb Handle** | `#throttleKnob4wd`, `.slim-slider-thumb-v` | On throttle track (58×38 px) | Knurled tactile handle for left thumb dragging. Dispatches `S:<0-255>`. |
| `4WD-THROTTLE-KNOB-PIP` | **4WD Knob Center Accent Bar** | `.knob-accent-bar-v` | Center of throttle knob | Luminous horizontal tick glowing with flame red accent. |
| `4WD-ZONE-CENTER` | **Cockpit Negative Space** | `.center-open-space` | Center screen | Uncluttered negative space preventing accidental thumb collision. |
| `4WD-ZONE-RIGHT` | **Right Diamond Keypad Zone** | `.diamond-zone`, `.keys-diamond-pad` | Right side (230×230 px) | Houses the 4 directional motion keypads for right thumb operation. |
| `KEY-FORWARD` | **Forward Motion Button** | `#btnFwd`, `.ctrl-key.key-up` | Diamond top | Sends `C:F` on hold, `C:S` on release. Up arrow SVG. |
| `KEY-BACKWARD` | **Backward Motion Button** | `#btnRev`, `.ctrl-key.key-down` | Diamond bottom | Sends `C:B` on hold, `C:S` on release. Down arrow SVG. |
| `KEY-LEFT` | **Left Steer Button** | `#btnLeft`, `.ctrl-key.key-left` | Diamond left | Sends `C:L` on hold, `C:S` on release. Left arrow SVG. |
| `KEY-RIGHT` | **Right Steer Button** | `#btnRight`, `.ctrl-key.key-right` | Diamond right | Sends `C:R` on hold, `C:S` on release. Right arrow SVG. |
| `KEY-SVG-ICON` | **Key Directional Arrow Icons** | `.ctrl-key svg` | Center of each key | Precision vector strokes; light up with drop-shadow glow on press. |

---

## 4. Cockpit Surface 2: 2WD Car (Dual Sliders Mode)

Active when `MODE: RC`, `DRIVE: 2WD`, and `INPUT: Buttons`. (DOM: `#view2wdSliders`)

| Part Code | Common Name | DOM ID / Selector | Location | Function & Visual Role |
| :--- | :--- | :--- | :--- | :--- |
| `2WD-THROTTLE-READOUT` | **2WD Throttle Readout Badge** | `#readout2wdThrottle`, `.throttle-value-badge` | Above vertical slider | Displays forward/reverse percentage (`0%`, `+80%`, `-50%`). Snaps to 0%. |
| `2WD-THROTTLE-TRACK` | **2WD Throttle Vertical Track** | `#track2wdThrottle`, `.slim-vertical-track` | Left side (height: 270px) | Vertical spring-loaded slider track for drive motor throttle. |
| `2WD-THROTTLE-DATUM` | **2WD Throttle Center Datum Line** | `.track-datum-h` | 50% vertical center of track | Horizontal line marking 0% neutral rest position. |
| `2WD-THROTTLE-KNOB` | **2WD Throttle Thumb Handle** | `#knob2wdThrottle`, `.slim-slider-thumb-v` | On vertical track | Auto-centering spring knob. Up = Forward (`+`), Down = Reverse (`-`). |
| `2WD-STEERING-READOUT` | **2WD Steering Readout Badge** | `#readout2wdSteering`, `.throttle-value-badge` | Above horizontal slider | Displays steering wheel angle (`0%`, `+45%`, `-45%`). Cyan tint on steer. |
| `2WD-STEERING-TRACK` | **2WD Steering Horizontal Track** | `#track2wdSteering`, `.slim-horizontal-track` | Right side (width: 270px) | Horizontal spring-loaded slider track for front-wheel steering servo. |
| `2WD-STEERING-DATUM` | **2WD Steering Center Datum Line** | `.track-datum-v` | 50% horizontal center of track | Vertical line marking straight-ahead (0%) wheel angle. |
| `2WD-STEERING-KNOB` | **2WD Steering Thumb Handle** | `#knob2wdSteering`, `.slim-slider-thumb-h` | On horizontal track (38×58 px) | Auto-centering spring knob. Left = Turn Left, Right = Turn Right. |
| `2WD-STEERING-KNOB-PIP` | **2WD Steering Knob Center Pip** | `.knob-accent-bar-h` | Center of steering knob | Vertical tick illuminated in electric cyan. |

---

## 5. Cockpit Surface 3: Analog Virtual Joystick

Active when `MODE: RC` and `INPUT: Joystick` (works in both 4v4 and 2WD). (DOM: `#viewJoystick`)

| Part Code | Common Name | DOM ID / Selector | Location | Function & Visual Role |
| :--- | :--- | :--- | :--- | :--- |
| `JOY-GIMBAL-RING` | **Joystick Gimbal Well** | `#joyGimbal`, `.joy-gimbal-ring` | Left side (240×240 px) | Circular recessed pad with radial gradient and thumb deflection constraint. |
| `JOY-RINGS-GUIDE` | **Concentric Range Guide Rings** | `.joy-gimbal-ring::before`, `::after` | Inside gimbal ring | Dashed guideline circles indicating 50% and 25% deflection radiuses. |
| `JOY-STICK-HEAD` | **Joystick Knob Head** | `#joyStickHead`, `.joy-stick-knob` | Inside gimbal (60×60 px) | 360° omnidirectional draggable stick knob. Snaps to center on release. |
| `JOY-CENTER-JEWEL` | **Joystick Center Optic Pip** | `.joy-stick-knob::after` | Center of joystick head | 14px illuminated center pip. Shifts to flame red at full deflection. |
| `JOY-HUD-BLOCK` | **Center HUD Telemetry Block** | `.joy-data-block` | Center screen | Houses real-time deflection telemetry readouts. |
| `JOY-MAG-READOUT` | **Joystick Magnitude Readout** | `#joyMagText`, `.joy-data-val` | Center HUD | Displays active deflection percentage (`0%` to `100%`). |
| `JOY-ANGLE-READOUT` | **Joystick Polar Angle Readout** | `#joyAngleText`, `.joy-data-sub` | Center HUD | Displays polar angle in degrees (`0°` to `360°`). |
| `JOY-DRIVE-TAG` | **Joystick Drive Architecture Tag** | `#joyDriveTag`, `.tag-mini` | Below angle readout | Displays `4v4 DRIVE` or `2WD DIFF DRIVE`. |
| `JOY-DEADBAND-COLUMN` | **Deadband Configuration Zone** | Container around `#joyDeadband` | Right side | Houses the joystick center deadband threshold controls. |
| `JOY-DEADBAND-READOUT` | **Deadband Value Badge** | `#dbVal`, `.throttle-value-badge` | Above deadband slider | Shows current deadband threshold (`10%`). |
| `JOY-DEADBAND-SLIDER` | **Deadband Sensitivity Slider** | `#joyDeadband`, `.fader-slider` | Below deadband badge | Range slider (3% to 25%) controlling neutral dead-zone size. |

---

## 6. Cockpit Surface 4: Line Follower PIDV Console

Active when `MODE: LFR`. (DOM: `#viewLFR`)

### 6.1 Presets Bar
| Part Code | Common Name | DOM ID / Selector | Location | Function & Visual Role |
| :--- | :--- | :--- | :--- | :--- |
| `LFR-PRESETS-BAR` | **Presets Bar** | `.lfr-preset-bar` | Top of LFR console | Row containing baseline quick-tuning presets. |
| `LFR-PRESET-DEFAULT` | **Default Preset Button** | `#prDefault`, `.quick-btn` | Left of presets bar | Loads default benchmark gains (`Speed=140, Kp=2.5, Ki=0.05, Kd=1.8, Kv=1.2`). |
| `LFR-PRESET-SPRINT` | **High Speed Preset Button** | `#prSprint`, `.quick-btn` | Center of presets bar | Loads sprint tuning (`Speed=200, Kp=3.8, Ki=0.02, Kd=2.6, Kv=1.8`). Ruby glow. |
| `LFR-PRESET-SHARP` | **Hairpin Preset Button** | `#prSharp`, `.quick-btn` | Right of presets bar | Loads tight turn tuning (`Speed=110, Kp=4.5, Ki=0.08, Kd=3.2, Kv=2.4`). Amber glow. |

### 6.2 Parameter Rows (Speed, Kp, Ki, Kd, Kv)
| Part Code | Common Name | DOM ID / Selector | Color Accent | Purpose & Telemetry Token |
| :--- | :--- | :--- | :--- | :--- |
| `LFR-ROW-SPEED` | **Base Speed Row** | `.lfr-clean-row.lfr-row-speed` | **Emerald Mint** | Controls base drive cruise PWM (`0`–`255`). Dispatches `V:Speed:<val>`. |
| `LFR-ROW-KP` | **Proportional Gain (Kp) Row** | `.lfr-clean-row.lfr-row-kp` | **Warm Amber** | Primary line tracking reaction (`0.0`–`10.0`). Dispatches `V:Kp:<val>`. |
| `LFR-ROW-KI` | **Integral Gain (Ki) Row** | `.lfr-clean-row.lfr-row-ki` | **Royal Amethyst** | Steady-state error compensation (`0.00`–`2.00`). Dispatches `V:Ki:<val>`. |
| `LFR-ROW-KD` | **Derivative Gain (Kd) Row** | `.lfr-clean-row.lfr-row-kd` | **Electric Cyan** | Damping / oscillation suppression (`0.0`–`10.0`). Dispatches `V:Kd:<val>`. |
| `LFR-ROW-KV` | **Velocity Scaling (Kv) Row** | `.lfr-clean-row.lfr-row-kv` | **Coral Rose** | Dynamic cornering speed reduction (`0.0`–`5.0`). Dispatches `V:Kv:<val>`. |

### 6.3 Individual Controls Inside Each Parameter Row
| Control Element | Common Name | Example ID / Selector | Function & Visual Role |
| :--- | :--- | :--- | :--- |
| `ROW-TITLE` | **Parameter Title** | `.lfr-row-name` | Parameter name label (e.g. `Base Speed`, `Kp`). |
| `ROW-TAG-PILL` | **Semantic Tag Pill** | `.lfr-tag-pill` | Colored badge (`PWM`, `PROP`, `INTEG`, `DERIV`, `SCALE`). |
| `ROW-FADER-SLIDER` | **Gain Sweep Slider** | `#slSpeed`, `#slKp`, `#slKi`, `#slKd`, `#slKv` | Fine touch sweep rail with color-accented thumb pip. |
| `ROW-STEPPER-BOX` | **Stepper Control Group** | `.stepper-box` | Container grouping decrement, numeric box, and increment controls. |
| `ROW-STEPPER-MINUS` | **Minus Stepper Button** | `.stepper-btn` (`−`) | Nudges parameter downward by its minimum step. |
| `ROW-NUMERIC-INPUT` | **Direct Numeric Box** | `#inSpeed`, `#inKp`, `#inKi`, `#inKd`, `#inKv` | Monospace input box for exact keyboard value entry. |
| `ROW-STEPPER-PLUS` | **Plus Stepper Button** | `.stepper-btn` (`+`) | Nudges parameter upward by its minimum step. |

---

## 7. Footer Status Rail (`.app-footer`)

| Part Code | Common Name | DOM ID / Selector | Location | Function & Visual Role |
| :--- | :--- | :--- | :--- | :--- |
| `FOOTER-RAIL` | **Footer Status Bar** | `.app-footer` | Bottom edge (height: 32px) | Compact status bar showing real-time transmission and protocol state. |
| `FOOTER-TX-TOKEN` | **Last Dispatched Token Badge** | `#lastTxToken`, `.tag-mini` | Footer (far left) | Dynamic color badge showing the exact token transmitted (`C:S`, `C:F`, `S:180`, `V:Kp:2.5`). |
| `FOOTER-SPEED-VAL` | **Master Speed Badge** | `#footerSpeedVal`, `.tag-mini` | Footer (beside token) | Monospace badge showing master PWM speed limit. |
| `FOOTER-MODE-STATUS` | **Cockpit Mode Readout** | `#activeModeStatus` | Footer (right side) | Text string showing state (e.g. `RC // 4v4 BUTTONS`, `LFR // PID`). |
| `FOOTER-PROTOCOL-STATUS` | **Protocol / Socket Status** | `#protocolStatus` | Footer (far right) | Displays active connection medium and port (e.g. `WIFI SOFTAP 3333`). |

---

## 8. Wireless Configuration Drawer (Slide-Out Sheet)

| Part Code | Common Name | DOM ID / Selector | Location | Function & Visual Role |
| :--- | :--- | :--- | :--- | :--- |
| `DRAWER-SCRIM` | **Drawer Backdrop Scrim** | `#drawerScrim`, `.drawer-scrim` | Fullscreen overlay | Dark translucent blurred backdrop; tapping closes drawer. |
| `DRAWER-SHEET` | **Wireless Settings Drawer** | `#drawerSheet`, `.drawer-sheet` | Slides from right edge (300px) | Dedicated configuration sheet for IP, port, and Bluetooth pairing. |
| `DRAWER-CLOSE-BTN` | **Drawer Close Button** | Button inside drawer header (`✕`) | Drawer top right | Dismisses the drawer without saving changes. |
| `DRAWER-TRANSPORT-SELECT`| **Transport Dropdown** | `#transportSelect` | Inside drawer | Dropdown menu to switch between SoftAP, STA, BLE, and SPP. |
| `DRAWER-IP-INPUT` | **Target IP Input Box** | `#cfgIp` | Inside drawer | Text input for router IP address in Wi-Fi STA mode (default: `192.168.1.105`). |
| `DRAWER-PORT-INPUT` | **Target Port Input Box** | `#cfgPort` | Inside drawer | Numeric input for UDP/TCP communication port (default: `3333`). |
| `DRAWER-BT-NAME-INPUT` | **Bluetooth Device Name Input** | `#cfgBtName` | Inside drawer | Text input for target BLE/SPP device identifier (default: `ShiftCtrlHub`). |
| `DRAWER-SAVE-BTN` | **Save Settings Button** | Button calling `saveWirelessSettings()` | Drawer bottom | Applies new network configuration and dispatches `CONN:SYNC`. |

---

## 9. Quick Adjustment Command Reference

When requesting future modifications, use this simple format:

> *"Please update **`Part Code`** (or **Common Name**): [Your instructions]."*

### Examples:
* *"In `4WD-THROTTLE-READOUT`, make the font size 14px."*
* *"In `KEY-FORWARD`, make the up arrow icon 32px."*
* *"On `TRANSPORT-TAB-BLE`, change the hover background to a darker blue."*
* *"In `LFR-ROW-KP`, increase the stepper button size to 30px."*
* *"In `FOOTER-TX-TOKEN`, add a 2px left border."*

This naming convention ensures pinpoint precision during any subsequent development pass.
