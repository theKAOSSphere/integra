# Integra LV2 Plugin (Skeleton)

This directory contains a minimal skeleton for an LV2 plugin named *Integra*.

Files created:

- `Makefile.mk` — top-level wrapper which calls `source/Makefile.mk`.
- `source/Makefile.mk` — build rules that compile `integra.cpp` into `integra.so`.
- `source/integra.cpp` — blank placeholder plugin source (implement DSP here).
- `source/integra.lv2/manifest.ttl` — LV2 manifest describing the plugin binary and TTL references.
- `source/integra.lv2/integra.ttl` — plugin TTL including I/O ports and a control port.
- `source/integra.lv2/modgui.ttl` — minimal modgui metadata.
- `source/integra.lv2/modgui/` — minimal UI files (an SVG icon, stylesheet placeholder, and `knobs/` directory).

How to use:

1. Add your LV2 DSP implementation to `source/integra.cpp`.
2. Update `integra.ttl` metadata to match your plugin's name, ports, etc.
3. Run the build via WSL or a Linux-like toolchain:

```pwsh
# From the repo root (Windows PowerShell example)
make -C integra
```

Notes:
- This is a minimal skeleton; it does not implement a fully LV2-compliant plugin or UI. Use the `ricochet` and `one-button-looper` examples as references for full setups.

