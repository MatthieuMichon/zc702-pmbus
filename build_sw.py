# Vitis build script

import vitis
from pathlib import Path

xsa_file = str(Path('build/pmbus_zc702.xsa').absolute())
vc = vitis.create_client(workspace='./build/vitis_ws')

# platform component

vpf = vc.create_platform_component(
    name='zc702_pmbus', hw_design=xsa_file, os="standalone",
        cpu="ps7_cortexa9_0",domain_name="standalone_ps7_cortexa9_0")
vpf.build()
xpfm = vc.find_platform_in_repos(name='zc702_pmbus')

# app component

app_component = vc.create_app_component(
    name='pmbus_cmp', platform=xpfm, domain='standalone_ps7_cortexa9_0')
app_component.import_files('./ps-src/main.c', dest_dir_in_cmp = "src")

cmp = vc.get_component(name='pmbus_cmp')
cmp.build()

vitis.dispose()
