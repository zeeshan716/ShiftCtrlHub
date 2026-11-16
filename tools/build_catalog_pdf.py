import os
import base64
from playwright.sync_api import sync_playwright

WORKSPACE_DIR = os.path.abspath(r"d:\dStuff\Robotics\projects (overall)\myProjects\universal_remote_control2")
IMAGES_DIR = os.path.join(WORKSPACE_DIR, "catalog_images")
HTML_OUT = os.path.join(WORKSPACE_DIR, "ShiftCtrlHub_Components_Catalog.html")
PDF_OUT = os.path.join(WORKSPACE_DIR, "ShiftCtrlHub_Components_Catalog.pdf")
BRAIN_PDF_OUT = os.path.abspath(r"C:\Users\Zee\.gemini\antigravity\brain\0edd4d88-175f-47c7-a84d-2e6e40e62800\ShiftCtrlHub_Components_Catalog.pdf")

# Definition of all catalog components
SECTIONS = [
    {
        "title": "1. Frame, Chassis & Preview Tools",
        "description": "Outer boundary, bezel frame, and desktop testing controls.",
        "items": [
            ("CHASSIS-FRAME", "Device Chassis Bezel", "CHASSIS-FRAME.png",
             "Landscape tablet/controller bezel (960×480 px default) housing all controls, touch thumb wells, and status bars."),
            ("TOOL-THEME-BTN", "Theme Switcher Button", "TOOL-THEME-BTN.png",
             "Desktop testing toolbar button toggling between ShiftCtrlHub Dark Titanium mode and high-contrast Light mode."),
            ("TOOL-FULLSCREEN-BTN", "Fullscreen Expander", "TOOL-FULLSCREEN-BTN.png",
             "Expands the remote interface to fill 100vw × 100vh for distraction-free mobile testing.")
        ]
    },
    {
        "title": "2. Top Ribbon Bar",
        "description": "Header navigation bar housing brand identity, transport selectors, and mode toggles.",
        "items": [
            ("BRAND-WORDMARK", "ShiftCtrlHub Brand Mark", "BRAND-WORDMARK.png",
             "Primary typography mark in header; tapping it opens the Wireless Configuration Drawer."),
            ("BRAND-HEARTBEAT-LED", "Telemetry Heartbeat Pip", "BRAND-HEARTBEAT-LED.png",
             "Pulsing emerald LED dot indicating active background telemetry loop."),
            ("TRANSPORT-CONTAINER", "Transport Mode Ribbon", "TRANSPORT-CONTAINER.png",
             "Segmented container housing the 4 wireless connection choices (SoftAP, STA, BLE, SPP)."),
            ("TRANSPORT-TAB-SOFTAP", "SoftAP Access Point Tab", "TRANSPORT-TAB-SOFTAP.png",
             "Direct Wi-Fi AP mode connecting directly to the robot SoftAP hotspot (192.168.4.1:3333)."),
            ("TRANSPORT-TAB-STA", "Wi-Fi Station Mode Tab", "TRANSPORT-TAB-STA.png",
             "Local WLAN / Router mode with multi-AP auto-failover (WiFiMulti) and UDP discovery."),
            ("TRANSPORT-TAB-BLE", "Bluetooth Low Energy Tab", "TRANSPORT-TAB-BLE.png",
             "Bluetooth Low Energy GATT UART mode communicating with Nordic UART Service."),
            ("TRANSPORT-TAB-SPP", "Bluetooth Classic SPP Tab", "TRANSPORT-TAB-SPP.png",
             "Classic Bluetooth SPP serial mode for pairing with HC-05 modules and Bluetooth serial MCUs."),
            ("TRANSPORT-SUB-BADGE", "Target Socket Address Badge", "TRANSPORT-SUB-BADGE.png",
             "Displays active socket destination IP:Port (192.168.4.1:3333) with drawer trigger gear."),
            ("INPUT-TOGGLE-BTN", "Buttons / Joystick Circular Switch", "INPUT-TOGGLE-BTN.png",
             "52×52 px circular button toggling cockpit between discrete buttons/sliders and virtual joystick."),
            ("SWITCH-RC-LFR", "RC / LFR Segmented Switch", "SWITCH-RC-LFR.png",
             "Primary mode selector switch toggling between manual Radio Control and autonomous Line Follower."),
            ("PILL-RC", "RC Mode Pill", "PILL-RC.png",
             "Left half of RC/LFR switch; activates manual driving cockpit surfaces in burgundy glow."),
            ("PILL-LFR", "LFR Mode Pill", "PILL-LFR.png",
             "Right half of RC/LFR switch; activates autonomous PIDV line following console in royal violet."),
            ("SWITCH-DRIVE", "4v4 / 2WD Drive Switch", "SWITCH-DRIVE.png",
             "Drive configuration switch toggling between 4WD differential-drive and 2WD front-wheel steer."),
            ("PILL-4V4", "4v4 Drive Pill", "PILL-4V4.png",
             "Left half of drive switch; selects 4WD tank skid-steer drive architecture."),
            ("PILL-2WD", "2WD Drive Pill", "PILL-2WD.png",
             "Right half of drive switch; selects 2WD toy-car drive with motor throttle and steering servo.")
        ]
    },
    {
        "title": "3. Cockpit Surface 1: 4WD Rover (Buttons Mode)",
        "description": "Active in RC mode with 4v4 drive and Buttons input. Differential-drive tank mechanics.",
        "items": [
            ("4WD-THROTTLE-READOUT", "4WD Speed Number Badge", "4WD-THROTTLE-READOUT.png",
             "Monospace numerical badge displaying current master drive PWM limit (0–255) with dynamic heat glow."),
            ("4WD-THROTTLE-TRACK", "4WD Throttle Slider Rail", "4WD-THROTTLE-TRACK.png",
             "Vertical concave slider rail (270 px height) with pointer capture and multi-stop gradient power fill."),
            ("4WD-THROTTLE-KNOB", "4WD Throttle Thumb Handle", "4WD-THROTTLE-KNOB.png",
             "Ergonomic tactile thumb handle with flame-red luminous center accent bar; dispatches S:<0-255>."),
            ("4WD-ZONE-RIGHT", "Right Diamond Keypad Pad", "4WD-ZONE-RIGHT.png",
             "230×230 px diamond keypad cluster for right-thumb directional rover navigation."),
            ("KEY-FORWARD", "Forward Motion Button", "KEY-FORWARD.png",
             "Top directional button; dispatches C:F on touch hold and C:S on release."),
            ("KEY-BACKWARD", "Backward Motion Button", "KEY-BACKWARD.png",
             "Bottom directional button; dispatches C:B on touch hold and C:S on release."),
            ("KEY-LEFT", "Left Steer Button", "KEY-LEFT.png",
             "Left directional button; dispatches C:L on touch hold and C:S on release for on-axis spin."),
            ("KEY-RIGHT", "Right Steer Button", "KEY-RIGHT.png",
             "Right directional button; dispatches C:R on touch hold and C:S on release for on-axis spin.")
        ]
    },
    {
        "title": "4. Cockpit Surface 2: 2WD Car (Dual Sliders Mode)",
        "description": "Active in RC mode with 2WD drive and Buttons input. Front-wheel steering servo mechanics.",
        "items": [
            ("2WD-THROTTLE-READOUT", "2WD Throttle Readout Badge", "2WD-THROTTLE-READOUT.png",
             "Monospace badge displaying rear motor forward/reverse percentage (-100% to +100%) with center snap."),
            ("2WD-THROTTLE-TRACK", "2WD Throttle Vertical Track", "2WD-THROTTLE-TRACK.png",
             "Vertical spring-loaded slider rail for drive motor throttle with horizontal 0% neutral datum line."),
            ("2WD-THROTTLE-KNOB", "2WD Throttle Thumb Handle", "2WD-THROTTLE-KNOB.png",
             "Auto-centering spring knob; pushing up drives forward (+), pulling down drives reverse (-)."),
            ("2WD-STEERING-READOUT", "2WD Steering Readout Badge", "2WD-STEERING-READOUT.png",
             "Monospace badge showing front-wheel steering servo deflection angle (-100% Left to +100% Right)."),
            ("2WD-STEERING-TRACK", "2WD Steering Horizontal Track", "2WD-STEERING-TRACK.png",
             "Horizontal spring-loaded slider rail for steering servo with vertical center straight-ahead datum."),
            ("2WD-STEERING-KNOB", "2WD Steering Thumb Handle", "2WD-STEERING-KNOB.png",
             "Auto-centering horizontal spring handle with electric cyan indicator pip; turns front wheels.")
        ]
    },
    {
        "title": "5. Cockpit Surface 3: Analog Virtual Joystick",
        "description": "Active in RC mode with Joystick input. Omnidirectional continuous thumb deflection.",
        "items": [
            ("JOY-GIMBAL-RING", "Joystick Gimbal Pad", "JOY-GIMBAL-RING.png",
             "240×240 px circular recessed gimbal pad with concentric range rings and thumb deflection boundary."),
            ("JOY-STICK-HEAD", "Joystick Knob Head", "JOY-STICK-HEAD.png",
             "360° omnidirectional draggable stick knob (60×60 px) with glowing center optic pip; snaps to center."),
            ("JOY-HUD-BLOCK", "Center HUD Telemetry Block", "JOY-HUD-BLOCK.png",
             "Center telemetry HUD displaying real-time deflection magnitude, polar angle, and drive architecture tag."),
            ("JOY-MAG-READOUT", "Joystick Magnitude Readout", "JOY-MAG-READOUT.png",
             "Monospace live readout displaying active thumbstick displacement percentage (0% to 100%)."),
            ("JOY-ANGLE-READOUT", "Joystick Polar Angle Readout", "JOY-ANGLE-READOUT.png",
             "Angular orientation readout displaying active polar angle in degrees (0° to 360°)."),
            ("JOY-DRIVE-TAG", "Joystick Drive Tag", "JOY-DRIVE-TAG.png",
             "Status tag confirming whether joystick mixing operates in 4v4 DRIVE or 2WD DIFF DRIVE."),
            ("JOY-DEADBAND-SLIDER", "Deadband Sensitivity Slider", "JOY-DEADBAND-SLIDER.png",
             "Range slider (3% to 25%) configuring center neutral dead-zone threshold to prevent accidental drift."),
            ("JOY-DEADBAND-READOUT", "Deadband Value Badge", "JOY-DEADBAND-READOUT.png",
             "Monospace badge displaying active joystick center deadband threshold percentage.")
        ]
    },
    {
        "title": "6. Cockpit Surface 4: Line Follower PIDV Console",
        "description": "Active in LFR mode. Real-time PIDV parameter tuning and line-loss recovery.",
        "items": [
            ("LFR-PRESETS-BAR", "Presets Strip", "LFR-PRESETS-BAR.png",
             "Top preset strip providing instantaneous baseline tuning configurations with one tap."),
            ("LFR-PRESET-DEFAULT", "Default Preset Button", "LFR-PRESET-DEFAULT.png",
             "Benchmark preset loading balanced track settings (Speed=140, Kp=2.5, Ki=0.05, Kd=1.8, Kv=1.2)."),
            ("LFR-PRESET-SPRINT", "High Speed Preset Button", "LFR-PRESET-SPRINT.png",
             "Sprint preset loading aggressive straightaway parameters (Speed=200, Kp=3.8, Ki=0.02, Kd=2.6, Kv=1.8)."),
            ("LFR-PRESET-SHARP", "Hairpin Preset Button", "LFR-PRESET-SHARP.png",
             "Hairpin preset loading tight-turn damping parameters (Speed=110, Kp=4.5, Ki=0.08, Kd=3.2, Kv=2.4)."),
            ("LFR-ROW-SPEED", "Base Speed Row", "LFR-ROW-SPEED.png",
             "Emerald-accented row controlling autonomous base cruise PWM (0–255) with live streaming."),
            ("LFR-ROW-KP", "Proportional Gain (Kp) Row", "LFR-ROW-KP.png",
             "Amber-accented row controlling proportional gain (Kp, 0.0–10.0) for primary line tracking."),
            ("LFR-ROW-KI", "Integral Gain (Ki) Row", "LFR-ROW-KI.png",
             "Amethyst-accented row controlling integral gain (Ki, 0.00–2.00) for steady-state error elimination."),
            ("LFR-ROW-KD", "Derivative Gain (Kd) Row", "LFR-ROW-KD.png",
             "Cyan-accented row controlling derivative gain (Kd, 0.0–10.0) for damping track oscillations."),
            ("LFR-ROW-KV", "Velocity Scaling (Kv) Row", "LFR-ROW-KV.png",
             "Rose-accented row controlling velocity scaling (Kv, 0.0–5.0) for dynamic corner deceleration."),
            ("ROW-STEPPER-BOX", "Stepper Control Cluster", "ROW-STEPPER-BOX.png",
             "Precision adjustment cluster combining decrement button, direct number box, and increment button."),
            ("ROW-STEPPER-MINUS", "Minus Stepper Button", "ROW-STEPPER-MINUS.png",
             "Touch stepper button nudging active gain downward by minimum step resolution."),
            ("ROW-NUMERIC-INPUT", "Direct Numeric Box", "ROW-NUMERIC-INPUT.png",
             "Monospace input field allowing direct keyboard numeric entry of exact gain values."),
            ("ROW-STEPPER-PLUS", "Plus Stepper Button", "ROW-STEPPER-PLUS.png",
             "Touch stepper button nudging active gain upward by minimum step resolution.")
        ]
    },
    {
        "title": "7. Footer Status Rail & Telemetry",
        "description": "Slender bottom status bar providing real-time transmission and protocol status.",
        "items": [
            ("FOOTER-RAIL", "Footer Status Bar", "FOOTER-RAIL.png",
             "Slender bottom status rail (32 px height) providing real-time transmission and protocol telemetry."),
            ("FOOTER-TX-TOKEN", "Last Dispatched Token Badge", "FOOTER-TX-TOKEN.png",
             "Dynamic color badge displaying the exact token dispatched to the robot (C:S, C:F, S:180, V:Kp:2.5)."),
            ("FOOTER-SPEED-VAL", "Master Speed Ceiling Badge", "FOOTER-SPEED-VAL.png",
             "Monospace badge showing the active master PWM speed ceiling limit."),
            ("FOOTER-MODE-STATUS", "Cockpit Mode Readout", "FOOTER-MODE-STATUS.png",
             "Telemetry text readout indicating current cockpit control mode (e.g. RC // 4v4 BUTTONS, LFR // PID)."),
            ("FOOTER-PROTOCOL-STATUS", "Connectivity Status Badge", "FOOTER-PROTOCOL-STATUS.png",
             "Connectivity status indicator confirming active transport medium and UDP port (UDP 3333).")
        ]
    },
    {
        "title": "8. Wireless Configuration Drawer",
        "description": "Slide-out configuration sheet for wireless credentials and socket endpoints.",
        "items": [
            ("DRAWER-SHEET", "Wireless Settings Drawer", "DRAWER-SHEET.png",
             "Slide-out configuration sheet from right bezel for editing IP addresses, ports, and pairing targets."),
            ("DRAWER-CLOSE-BTN", "Drawer Close Button", "DRAWER-CLOSE-BTN.png",
             "Dismiss button in drawer header (✕) closing configuration sheet without applying changes."),
            ("DRAWER-TRANSPORT-SELECT", "Transport Dropdown", "DRAWER-TRANSPORT-SELECT.png",
             "Dropdown selector choosing between SoftAP, Station Mode, BLE, and SPP."),
            ("DRAWER-IP-INPUT", "Target IP Input Field", "DRAWER-IP-INPUT.png",
             "Monospace text input field for configuring the target robot IP address (default: 192.168.4.1)."),
            ("DRAWER-SAVE-BTN", "Save & Apply Settings Button", "DRAWER-SAVE-BTN.png",
             "Primary action button saving wireless settings and dispatching CONN:SYNC handshake.")
        ]
    }
]

def img_to_base64(filepath):
    if os.path.exists(filepath):
        with open(filepath, "rb") as f:
            data = base64.b64encode(f.read()).decode("utf-8")
            return f"data:image/png;base64,{data}"
    return ""

def build_html():
    html = """<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <title>ShiftCtrlHub — Component & Parts Catalog</title>
  <style>
    @page {
      size: A4 portrait;
      margin: 14mm 14mm 16mm 14mm;
      @bottom-right {
        content: counter(page) " / " counter(pages);
        font-family: 'SF Pro Text', -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif;
        font-size: 8pt;
        color: #71717A;
      }
    }

    * {
      box-sizing: border-box;
      margin: 0;
      padding: 0;
    }

    body {
      font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Helvetica, Arial, sans-serif;
      font-size: 9.5pt;
      line-height: 1.45;
      color: #18181B;
      background: #FFFFFF;
      -webkit-print-color-adjust: exact;
      print-color-adjust: exact;
    }

    /* Document Header */
    .doc-header {
      border-bottom: 2px solid #800020;
      padding-bottom: 12px;
      margin-bottom: 20px;
      display: flex;
      justify-content: space-between;
      align-items: flex-end;
    }

    .doc-title-block h1 {
      font-size: 19pt;
      font-weight: 800;
      color: #09090B;
      letter-spacing: -0.5px;
      margin-bottom: 3px;
    }

    .doc-subtitle {
      font-size: 9.5pt;
      color: #71717A;
      font-weight: 500;
    }

    .doc-badge-block {
      text-align: right;
    }

    .hub-pill {
      display: inline-block;
      background: #800020;
      color: #FFFFFF;
      font-weight: 700;
      font-size: 8pt;
      text-transform: uppercase;
      letter-spacing: 0.8px;
      padding: 4px 10px;
      border-radius: 4px;
    }

    .doc-meta {
      font-family: ui-monospace, SFMono-Regular, Menlo, Monaco, Consolas, monospace;
      font-size: 7.5pt;
      color: #71717A;
      margin-top: 4px;
    }

    /* Purpose Box */
    .callout-box {
      background: #FAFAFA;
      border-left: 3.5px solid #800020;
      border-top: 1px solid #E4E4E7;
      border-right: 1px solid #E4E4E7;
      border-bottom: 1px solid #E4E4E7;
      border-radius: 0 6px 6px 0;
      padding: 10px 14px;
      margin-bottom: 22px;
      font-size: 8.5pt;
      color: #27272A;
    }

    .callout-box strong {
      color: #800020;
    }

    /* Section Styling */
    .section-container {
      margin-bottom: 26px;
      page-break-inside: auto;
    }

    .section-title {
      font-size: 12pt;
      font-weight: 700;
      color: #09090B;
      border-bottom: 1.5px solid #E4E4E7;
      padding-bottom: 5px;
      margin-bottom: 6px;
      page-break-after: avoid;
      break-after: avoid;
    }

    .section-desc {
      font-size: 8.5pt;
      color: #71717A;
      margin-bottom: 10px;
      page-break-after: avoid;
      break-after: avoid;
    }

    .table-caption {
      font-size: 8pt;
      font-weight: 700;
      text-transform: uppercase;
      letter-spacing: 0.6px;
      color: #52525B;
      margin-bottom: 5px;
      page-break-after: avoid;
      break-after: avoid;
    }

    /* 3-Column Table */
    table.catalog-table {
      width: 100%;
      border-collapse: collapse;
      table-layout: fixed;
      margin-bottom: 16px;
      background: #FFFFFF;
      border: 1px solid #E4E4E7;
      border-radius: 6px;
      overflow: hidden;
    }

    table.catalog-table th {
      background: #F4F4F5;
      color: #18181B;
      font-size: 8pt;
      font-weight: 700;
      text-transform: uppercase;
      letter-spacing: 0.5px;
      text-align: left;
      padding: 7px 10px;
      border-bottom: 1.5px solid #D4D4D8;
    }

    table.catalog-table td {
      padding: 8px 10px;
      border-bottom: 1px solid #F4F4F5;
      vertical-align: middle;
      font-size: 8.5pt;
      color: #27272A;
    }

    table.catalog-table tr {
      page-break-inside: avoid;
      break-inside: avoid;
    }

    table.catalog-table tr:last-child td {
      border-bottom: none;
    }

    /* Table Column Widths */
    col.col-name { width: 30%; }
    col.col-img  { width: 34%; }
    col.col-desc { width: 36%; }

    /* Column 1: Identifiers */
    .part-code {
      font-family: ui-monospace, SFMono-Regular, Menlo, Monaco, Consolas, monospace;
      font-weight: 700;
      font-size: 8.5pt;
      color: #800020;
      background: #FFF1F2;
      padding: 2px 5px;
      border-radius: 3px;
      border: 1px solid #FFE4E6;
      display: inline-block;
      margin-bottom: 3px;
    }

    .part-name {
      font-weight: 600;
      font-size: 8.5pt;
      color: #09090B;
      display: block;
    }

    /* Column 2: Image Container */
    .img-box {
      background: #09090B;
      border: 1px solid #27272A;
      border-radius: 6px;
      padding: 6px;
      display: flex;
      align-items: center;
      justify-content: center;
      min-height: 48px;
      max-height: 95px;
      overflow: hidden;
    }

    .img-box img {
      max-width: 100%;
      max-height: 80px;
      object-fit: contain;
      display: block;
      border-radius: 3px;
    }

    /* Column 3: Detail */
    .part-detail {
      font-size: 8.2pt;
      color: #3F3F46;
      line-height: 1.4;
    }

    .page-break {
      page-break-before: always;
      break-before: page;
    }
  </style>
</head>
<body>

  <!-- HEADER BLOCK -->
  <header class="doc-header">
    <div class="doc-title-block">
      <h1>ShiftCtrlHub — Components Catalog</h1>
      <div class="doc-subtitle">Standardized Technical Directory & Isolated Modification Reference</div>
    </div>
    <div class="doc-badge-block">
      <span class="hub-pill">Version 2.0 • Hardware & UI</span>
      <div class="doc-meta">Target: Universal MCU Remote</div>
    </div>
  </header>

  <!-- CALLOUT -->
  <div class="callout-box">
    <strong>Granular Modification Guarantee:</strong> Every entry below specifies a unique <code>Part Code</code> paired with an exact cropped snapshot of that component as rendered in the ShiftCtrlHub interface. To request updates, simply reference the <code>Part Code</code> (e.g. <em>"In <code>4WD-THROTTLE-KNOB</code>, increase width by 4px"</em>). Only that designated component will be touched, guaranteeing 100% immunity against collateral layout drift.
  </div>
"""

    for idx, sec in enumerate(SECTIONS):
        # Section breaks
        if idx in [2, 4, 6]:
            html += '\n  <div class="page-break"></div>\n'

        html += f"""
  <section class="section-container">
    <h2 class="section-title">{sec['title']}</h2>
    <p class="section-desc">{sec['description']}</p>
    <div class="table-caption">Table {idx+1}: {sec['title']} Component Inventory</div>
    <table class="catalog-table">
      <colgroup>
        <col class="col-name">
        <col class="col-img">
        <col class="col-desc">
      </colgroup>
      <thead>
        <tr>
          <th>Component Identifier</th>
          <th>Cropped Visual Reference</th>
          <th>Functional Role & Specification (1–2 Lines)</th>
        </tr>
      </thead>
      <tbody>
"""
        for code, name, img_name, desc in sec["items"]:
            img_path = os.path.join(IMAGES_DIR, img_name)
            img_b64 = img_to_base64(img_path)
            html += f"""        <tr>
          <td>
            <span class="part-code">{code}</span>
            <span class="part-name">{name}</span>
          </td>
          <td>
            <div class="img-box">
              <img src="{img_b64}" alt="{code}">
            </div>
          </td>
          <td>
            <div class="part-detail">{desc}</div>
          </td>
        </tr>
"""
        html += """      </tbody>
    </table>
  </section>
"""

    html += """
</body>
</html>
"""
    return html

def main():
    print("Building HTML for Catalog...")
    html_content = build_html()
    with open(HTML_OUT, "w", encoding="utf-8") as f:
        f.write(html_content)
    print(f"Saved HTML to: {HTML_OUT}")

    print("Generating PDF via Playwright Chromium...")
    with sync_playwright() as p:
        browser = p.chromium.launch()
        page = browser.new_page()
        page.set_content(html_content, wait_until="load")
        page.pdf(
            path=PDF_OUT,
            format="A4",
            print_background=True,
            margin={"top": "14mm", "bottom": "16mm", "left": "14mm", "right": "14mm"},
            display_header_footer=True,
            header_template="<div></div>",
            footer_template="""
            <div style="font-family: -apple-system, sans-serif; font-size: 8pt; color: #71717A; width: 100%; display: flex; justify-content: space-between; padding: 0 14mm;">
              <span>ShiftCtrlHub — Universal Remote Control</span>
              <span>Page <span class="pageNumber"></span> of <span class="totalPages"></span></span>
            </div>
            """
        )
        browser.close()

    print(f"PDF Successfully Generated: {PDF_OUT}")

    # Copy to Brain Artifacts directory
    import shutil
    shutil.copyfile(PDF_OUT, BRAIN_PDF_OUT)
    print(f"PDF Mirrored to Artifacts: {BRAIN_PDF_OUT}")

if __name__ == "__main__":
    main()
