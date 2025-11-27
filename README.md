# Integra LV2 Plugin

Integra is a simple but effective EQ plugin inspired by the functionality of the TC Electronic Integrated Preamp. It provides classic Bass and Treble controls and a clean volume adjustment, making it a great utility for subtle tone shaping.

This plugin is fully compatible with the MOD Audio ecosystem and includes a custom MODGUI.

## Features

*   **Bass Control:** Boost or cut low frequencies by up to 15 dB.
*   **Treble Control:** Boost or cut high frequencies by up to 15 dB.
*   **Volume Control:** Adjust the output level from -90 dB to +25 dB.

## Building the Plugin

To build the plugin, you need a standard LV2 development environment and toolchain (e.g., `build-essential` on Debian/Ubuntu).

From the root of this repository, run the following command:

```bash
make
```

This will compile the plugin source code (`integra.cpp`) and generate the `integra.so` binary inside the `source/integra.lv2` bundle directory.

## Installation

Once built, you can install the plugin by copying the entire `source/integra.lv2` directory to your system's LV2 plugins path (e.g., `~/.lv2/` or `/usr/lib/lv2/`).

For use with MOD devices, you can use the `mod-plugin-builder` toolchain to create a package for deployment.

