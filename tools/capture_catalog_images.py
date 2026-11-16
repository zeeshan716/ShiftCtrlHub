import os
import sys
from playwright.sync_api import sync_playwright

OUTPUT_DIR = os.path.abspath(r"d:\dStuff\Robotics\projects (overall)\myProjects\universal_remote_control2\catalog_images")
os.makedirs(OUTPUT_DIR, exist_ok=True)

HTML_URL = "file:///" + os.path.abspath(r"d:\dStuff\Robotics\projects (overall)\myProjects\universal_remote_control2\app_redesign_preview.html").replace("\\", "/")

def capture_element(locator, filename):
    path = os.path.join(OUTPUT_DIR, filename)
    try:
        box = locator.bounding_box()
        if box and box['width'] > 0 and box['height'] > 0:
            locator.screenshot(path=path)
            print(f"Captured: {filename} ({int(box['width'])}x{int(box['height'])})")
            return True
        else:
            print(f"Skipped (zero size): {filename}")
            return False
    except Exception as e:
        print(f"Error capturing {filename}: {e}")
        return False

def main():
    print("Starting Playwright capture...")
    with sync_playwright() as p:
        browser = p.chromium.launch()
        context = browser.new_context(
            viewport={"width": 1280, "height": 900},
            device_scale_factor=2
        )
        page = context.new_page()
        page.goto(HTML_URL)
        page.wait_for_timeout(500)

        # 1. Outer & Chassis
        capture_element(page.locator("#deviceChassis"), "CHASSIS-FRAME.png")
        capture_element(page.locator("#btnThemeToggle"), "TOOL-THEME-BTN.png")
        capture_element(page.locator(".toolbar-quick button:has-text('Fullscreen')"), "TOOL-FULLSCREEN-BTN.png")

        # 2. Header
        capture_element(page.locator(".brand-mark"), "BRAND-WORDMARK.png")
        capture_element(page.locator(".brand-dot"), "BRAND-HEARTBEAT-LED.png")
        capture_element(page.locator("#transportBar"), "TRANSPORT-CONTAINER.png")
        capture_element(page.locator("#tabSoftap"), "TRANSPORT-TAB-SOFTAP.png")
        capture_element(page.locator("#tabSta"), "TRANSPORT-TAB-STA.png")
        capture_element(page.locator("#tabBle"), "TRANSPORT-TAB-BLE.png")
        capture_element(page.locator("#tabSpp"), "TRANSPORT-TAB-SPP.png")
        capture_element(page.locator(".conn-sub-badge"), "TRANSPORT-SUB-BADGE.png")
        capture_element(page.locator("#btnBjToggle"), "INPUT-TOGGLE-BTN.png")
        capture_element(page.locator("#rcLfrSwitch"), "SWITCH-RC-LFR.png")
        capture_element(page.locator("#driveSwitch"), "SWITCH-DRIVE.png")
        capture_element(page.locator("#pillRC"), "PILL-RC.png")
        capture_element(page.locator("#pillLFR"), "PILL-LFR.png")
        capture_element(page.locator("#pill4v4"), "PILL-4V4.png")
        capture_element(page.locator("#pill2WD"), "PILL-2WD.png")

        # 3. 4WD Surface (Default active)
        capture_element(page.locator("#speedNum4wd"), "4WD-THROTTLE-READOUT.png")
        capture_element(page.locator("#throttleTrack4wd"), "4WD-THROTTLE-TRACK.png")
        capture_element(page.locator("#throttleKnob4wd"), "4WD-THROTTLE-KNOB.png")
        capture_element(page.locator(".keys-diamond-pad"), "4WD-ZONE-RIGHT.png")
        capture_element(page.locator("#btnFwd"), "KEY-FORWARD.png")
        capture_element(page.locator("#btnRev"), "KEY-BACKWARD.png")
        capture_element(page.locator("#btnLeft"), "KEY-LEFT.png")
        capture_element(page.locator("#btnRight"), "KEY-RIGHT.png")

        # 4. Footer
        capture_element(page.locator(".app-footer"), "FOOTER-RAIL.png")
        capture_element(page.locator("#lastTxToken"), "FOOTER-TX-TOKEN.png")
        capture_element(page.locator("#footerSpeedVal"), "FOOTER-SPEED-VAL.png")
        capture_element(page.locator("#activeModeStatus"), "FOOTER-MODE-STATUS.png")
        capture_element(page.locator("#protocolStatus"), "FOOTER-PROTOCOL-STATUS.png")

        # 5. Switch to 2WD mode
        print("\nSwitching to 2WD mode...")
        page.click("#pill2WD")
        page.wait_for_timeout(300)
        capture_element(page.locator("#readout2wdThrottle"), "2WD-THROTTLE-READOUT.png")
        capture_element(page.locator("#track2wdThrottle"), "2WD-THROTTLE-TRACK.png")
        capture_element(page.locator("#knob2wdThrottle"), "2WD-THROTTLE-KNOB.png")
        capture_element(page.locator("#readout2wdSteering"), "2WD-STEERING-READOUT.png")
        capture_element(page.locator("#track2wdSteering"), "2WD-STEERING-TRACK.png")
        capture_element(page.locator("#knob2wdSteering"), "2WD-STEERING-KNOB.png")

        # 6. Switch to Joystick mode
        print("\nSwitching to Joystick mode...")
        page.click("#btnBjToggle")
        page.wait_for_timeout(300)
        capture_element(page.locator("#joyGimbal"), "JOY-GIMBAL-RING.png")
        capture_element(page.locator("#joyStickHead"), "JOY-STICK-HEAD.png")
        capture_element(page.locator(".joy-data-block"), "JOY-HUD-BLOCK.png")
        capture_element(page.locator("#joyMagText"), "JOY-MAG-READOUT.png")
        capture_element(page.locator("#joyAngleText"), "JOY-ANGLE-READOUT.png")
        capture_element(page.locator("#joyDriveTag"), "JOY-DRIVE-TAG.png")
        capture_element(page.locator("#dbVal"), "JOY-DEADBAND-READOUT.png")
        capture_element(page.locator("#joyDeadband"), "JOY-DEADBAND-SLIDER.png")

        # 7. Switch to LFR mode
        print("\nSwitching to LFR mode...")
        page.click("#pillLFR")
        page.wait_for_timeout(300)
        capture_element(page.locator(".lfr-preset-bar"), "LFR-PRESETS-BAR.png")
        capture_element(page.locator("#prDefault"), "LFR-PRESET-DEFAULT.png")
        capture_element(page.locator("#prSprint"), "LFR-PRESET-SPRINT.png")
        capture_element(page.locator("#prSharp"), "LFR-PRESET-SHARP.png")
        capture_element(page.locator(".lfr-row-speed"), "LFR-ROW-SPEED.png")
        capture_element(page.locator(".lfr-row-kp"), "LFR-ROW-KP.png")
        capture_element(page.locator(".lfr-row-ki"), "LFR-ROW-KI.png")
        capture_element(page.locator(".lfr-row-kd"), "LFR-ROW-KD.png")
        capture_element(page.locator(".lfr-row-kv"), "LFR-ROW-KV.png")
        capture_element(page.locator(".lfr-row-kp .stepper-box"), "ROW-STEPPER-BOX.png")
        capture_element(page.locator(".lfr-row-kp .stepper-btn:first-child"), "ROW-STEPPER-MINUS.png")
        capture_element(page.locator("#inKp"), "ROW-NUMERIC-INPUT.png")
        capture_element(page.locator(".lfr-row-kp .stepper-btn:last-child"), "ROW-STEPPER-PLUS.png")

        # 8. Open Drawer
        print("\nOpening Wireless Settings Drawer...")
        page.click(".conn-gear")
        page.wait_for_timeout(400)
        capture_element(page.locator("#drawerSheet"), "DRAWER-SHEET.png")
        capture_element(page.locator("#transportSelect"), "DRAWER-TRANSPORT-SELECT.png")
        capture_element(page.locator("#cfgIp"), "DRAWER-IP-INPUT.png")
        capture_element(page.locator("#cfgPort"), "DRAWER-PORT-INPUT.png")
        capture_element(page.locator("#cfgBtName"), "DRAWER-BT-NAME-INPUT.png")
        capture_element(page.locator(".drawer-sheet button:has-text('Save & Apply Settings')"), "DRAWER-SAVE-BTN.png")

        browser.close()
        print("\nAll component screenshots successfully captured!")

if __name__ == "__main__":
    main()
