#!/usr/bin/env python3
"""Compile actual SDK policy with host platform boundaries, no device access."""
from pathlib import Path
import os, subprocess, tempfile
root=Path(__file__).resolve().parents[2]
source=(root/'tl_zigbee_sdk/proj/drivers/drv_b85_ota.c').read_text()
# Device/SDK types are provided by the boundary harness; executable body is exact.
source='\n'.join(line for line in source.splitlines() if not line.startswith('#include'))
test=(Path(__file__).with_name('guard_test.c')).read_text().replace('/* POLICY_SOURCE */',source)
with tempfile.TemporaryDirectory(prefix='b85-guard-') as directory:
    p=Path(directory); (p/'test.c').write_text(test)
    subprocess.run([os.environ.get('CC','cc'),'-std=c99','-Wall','-Wextra','-Werror',
        '-fsanitize=address,undefined','-fno-sanitize-recover=all',str(p/'test.c'),'-o',str(p/'test')],check=True)
    subprocess.run([str(p/'test')],check=True)
