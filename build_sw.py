# Vitis build script

import sys
import vitis
from pathlib import Path

xsa_file = str(Path('build/pmbus_zc702.xsa').absolute())
vc = vitis.create_client(workspace='./build/vitis_ws')
vpf = vc.create_platform_component(name='zc702_pmbus', hw_design=xsa_file)
vitis.dispose()
