# Opt-in B85 512 KiB dual boot

This patch enables `CHIP_TYPE=TLSR_8258_512K`, `BOOT_LOADER_MODE=0`,
`SDK_B85_512K_DUAL_BOOT=1` with the SDK4 concurrent Zigbee/BLE application.
The default 1/2/4 MiB configurations are unchanged. The BLE OTA library is unchanged.

| Range | Use |
|---|---|
| 00000–33fff | Firmware A, maximum 208 KiB including checksum |
| 34000–3ffff | Zigbee NV modules 0–5 |
| 40000–73fff | Firmware B, maximum 208 KiB including checksum |
| 74000–75fff | BLE NV |
| 76000–77fff | MAC and calibration |
| 78000–79fff | Install code and factory reset flag |
| 7a000–7ffff | APP and key-pair NV (two 8 KiB banks, 1 KiB index each) |

Call `blc_ota_setFirmwareSizeAndBootAddress(208, MULTI_BOOT_ADDR_0x40000)`
before `drv_platform_init`. Call `drv_b85_flash_guard_init` after cold platform
initialization. Install `drv_b85_flash_set_voltage_check` with a fresh flash-VDD
measurement callback before application NV/OTA initialization; it runs before
each permitted low-level erase/program and before OTA preparation. Compile `proj/drivers/drv_b85_ota.c` in the application and use
`drv_b85_ota_prepare` when opening a physical update window. Route that window's
OTA GATT writes through `drv_b85_ota_write`; normal vendor sequencing, CRC,
flash writes, cleanup and activation remain in `libble_b85.a`. The progress callback
receives 1 after readback and 2 before END verification. The application owns
advertising, battery admission, local controls, feedback and retry policy.

Preparation checks the flash JEDEC capacity and requires exactly one valid boot
marker. It sets `ota_program_offset` explicitly; the configured alternate boot
address is not evidence of the running slot. Both/no-marker states refuse OTA
without erasing either image. Successful activation alone permits the vendor's
one-byte invalidation of the old image. Page programming and sector erase check
the policy at the actual B85 flash driver entry points. Normal NV writes remain
possible; addresses crossing image boundaries or the 512 KiB end are rejected.
This is software protection. Coarse hardware block locks cannot independently
protect the 208 KiB image while allowing the adjacent NV bank.

The header policy buffers the first 16-byte PDU until manufacturer, image type,
marker and size are known. It accepts a private application image type passed by
the caller. It is not cryptographic authentication or a downgrade policy. Use the
vendor CRC postprocessor on every delivered image. B85 accepts extended PDUs
16/32/48/64/80; larger PDUs return OTA_MCU_NOT_SUPPORTED, despite the common header's
240-byte description. The final PDU is padded to a multiple of 16, not necessarily
the negotiated full PDU size.

## Verification

`python3 tests/b85_512k/run.py` compiles the real guard implementation with minimal
platform/vendor boundaries under ASan/UBSan. It checks both slot directions,
ambiguous markers, overflow, capacity, activation, model checks, buffered headers
and repeated START. It does not emulate the vendor library. It requires a working
host C compiler; for example `docker run --rm -v "$PWD:/sdk" -w /sdk rust:1.88
python3 tests/b85_512k/run.py`.

Compile `layout_checks.c` using the concurrent application's target compiler and
include/definition flags to assert every boundary and NV bank size.

Additional integration validation was run on TC32 using tc32-devtools revision
2e1bcaed762e3124b024047db981172d2b0395ba: unmodified vendor OTA machine code, two-way
full-image transfer with exact readback, invalid packets/images, power cuts at
activation, and SDK3/SDK4 NV C with 66 cuts during write/compaction. Hardware tests
have not been run. This CPU/NOR harness does not establish RF behavior, physical
flash timing, retention current, full network migration, or ROM priority when
both markers survive a power cut. There is no automatic trial-image rollback.
These are release validation requirements for the integrator.
