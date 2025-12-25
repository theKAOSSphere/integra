# Integra & Pulverize – LV2 Preamplifier Plugins

**Two distinct preamp pedals built on the same core engine, optimized for the MOD Audio platform.**

This repository contains two LV2 plugins:
*   **Integra:** A faithful emulation of the versatile TC Electronic Integrated Preamplifier.
*   **Pulverize:** An aggressive, cutting clean boost emulating the Fortin Grind.

Both plugins are designed to provide simplicity, accurate control feel, and seamless integration with the MOD Audio ecosystem.

<table align="center" border="0" cellpadding="8" cellspacing="0">
  <tr>
    <th>Integra</th>
    <th>Pulverize</th>
  </tr>
  <tr>
    <td align="center">
      <img src="source/kaoss-integra.lv2/modgui/screenshot-integra.png" alt="Integra" width="160" />
    </td>
    <td align="center">
      <img src="source/kaoss-pulverize.lv2/modgui/screenshot-pulverize.png" alt="Pulverize" width="160" />
    </td>
  </tr>
</table>

---

## Features

### Common Features
*   **Shared Core Engine:** Both plugins are built on the same high-quality DSP foundation.
*   **Volume Control:** Mimics the behavior of a physical volume knob, providing a range from silent to +25 dB.
*   **Analog-style Rolloff:** A gentle low-pass filter (18 kHz) is applied at the output for a realistic, warm sound.
*   **Accurate faceplate design:** The visual design closely resembles the original hardware for an authentic user experience.
*   **Faceplate-accurate controls:** The plugin's knobs match the behaviour of the original pedal's knobs.

### Integra
*   **Bass / Treble:** Each control uses a first-order shelving filter with +/-15 dB of gain available internally.
*   **Dynamic filter frequencies:** Treble and Bass frequency centers shift with knob position to better emulate the hardware's behavior.

### Pulverize
*   **Aggressive High-Gain Voicing:** Features an aggressive low-cut to remove mud, a treble cut to tame fizz, and a pronounced mid-range hump to add clarity and punch.
*   **Simple One-Knob Operation:** A single "Volume" knob controls the overall output boost.

---

## Quick Settings

Here are some recommended starting points for each plugin:

### Integra
*   **Clean Boost:** Bass: 0, Treble: 0, Volume: 5+
*   **Early Meshuggah Tone:** Bass: -10, Treble: +2, Volume: 4.5+

### Pulverize
*   **Modern Metal Boost:** Simply place it before a high-gain amp sim and adjust the **Volume** knob to taste (typically between 5 and 7) to tighten the low-end and add aggressive character.

---

## Installation

**Note on Versioning:** Both Integra and Pulverize are developed within the same repository and share a single version number. Each release package will contain both plugin bundles.

It is recommended to download the pre-built plugins from the **[Releases Page](https://github.com/theKAOSSphere/integra/releases)**.

1.  Go to the [Releases Page](https://github.com/theKAOSSphere/integra/releases).
2.  Download the latest release archive. You will find both `kaoss-integra.lv2` and `kaoss-pulverize.lv2` bundles.
3.  Unzip the files. You will have two folders: `kaoss-integra.lv2` and `kaoss-pulverize.lv2`.

### For MOD Audio Devices

1.  **Transfer the Plugins:** Copy both the `kaoss-integra.lv2` and `kaoss-pulverize.lv2` directories to your MOD device.
    ```bash
    # Example command
    scp -r ~/Downloads/kaoss-integra.lv2 ~/Downloads/kaoss-pulverize.lv2 root@192.168.51.1:/data/plugins/
    ```
2.  **Restart the Host:** Connect to your device via `ssh` and restart the `mod-host` service.
    ```bash
    ssh root@192.168.51.1
    systemctl restart mod-host
    ```
3.  **Refresh the Web UI:** Reload the MOD web interface. Both plugins should now be available.

### For Linux Desktops

1.  **Copy the LV2 Bundles:** Copy both folders to your user's LV2 directory.
    ```bash
    cp -r ~/Downloads/kaoss-integra.lv2 /path/to/lv2/directory/
    cp -r ~/Downloads/kaoss-pulverize.lv2 /path/to/lv2/directory/
    ```
2.  **Scan for Plugins:** Your LV2 host should detect the new plugins on its next scan.

---

## Building From Source

<details>
<summary><strong>► Build for MOD Audio Devices (using mod-plugin-builder)</strong></summary>

This project is configured to be built using **`mod-plugin-builder`**. The repository contains package definitions for both `integra` and `pulverize`.

#### Build Steps

1.  **Link the Repository:**
    Ensure the `integra` repository is available to `mod-plugin-builder`. A common way is to clone it into `plugins/package`.
    ```bash
    cd /path/to/mod-plugin-builder/plugins/package
    git clone https://github.com/theKAOSSphere/integra
    ```
2.  **Create Symlink for Pulverize:**
    You must create a symlink so `mod-plugin-builder` can see the `pulverize` package definition.
    ```bash
    cd /path/to/mod-plugin-builder/plugins/package
    ln -s ./integra/pulverize pulverize
    ```
3.  **Update Config.in:**
    Add `source "plugins/package/pulverize/Config.in"` to the main `Config.in` file in `mod-plugin-builder`.

4.  **Run the Build:**
    Navigate to the root of `mod-plugin-builder` and run the build command for the desired plugin.
    ```bash
    cd /path/to/mod-plugin-builder

    # To build Integra
    ./build <target> integra

    # To build Pulverize
    ./build <target> pulverize
    ```
    Replace `<target>` with your device (e.g., `modduox-new`). The compiled bundles will be in `/path/to/mod-workdir/<target>/target/usr/local/lib/lv2`.

</details>

<details>
<summary><strong>► Build for Linux Desktop (Standalone)</strong></summary>

For testing on a standard Linux desktop.

### Prerequisites

You must have `build-essential`, `libtool`, `pkg-config`, and `lv2-dev` installed.

### Build Steps

The `source/Makefile` uses a `VARIANT` variable to determine which plugin to build.

1.  **Navigate to the Source Directory:**
    ```bash
    cd source/
    ```
2.  **Compile the Desired Plugin:**
    ```bash
    # To build Integra
    make VARIANT=integra

    # To build Pulverize
    make VARIANT=pulverize
    ```
    This will create the corresponding `.lv2` bundle inside the `source/` directory. You can then follow the desktop installation instructions.
    
3.  **Install to System:**
    To install directly into `/usr/local/lib/lv2/`, you can use the `install` target:
    ```bash
    sudo make VARIANT=integra install
    sudo make VARIANT=pulverize install
    ```

</details>

---

## License

This plugin is licensed under GPLv3. See the `LICENSE` file for details. The project contains the `SPDX-License-Identifier: GPL-3.0-or-later` header in the source files.

This software is not affiliated with or endorsed by TC Electronic or Fortin Amplification. All trademarks are the property of their respective owners.