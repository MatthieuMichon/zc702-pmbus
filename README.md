This repository provides a simple use case of the PMBus feature present on the power management ICs of the ZC702 FPGA evaluation board.

# Required software

Xilinx's Vivado and Vitis software are required for configuring the FPGA and also rebuilding the project output files.

# Encountered issues

## Misleading error message from Vitis

Upon creating the hardware platform:

```python
xsa_file = '../pmbus_zc702.xsa'
vc = vitis.create_client(workspace='./build/vitis_ws')
vpf = vc.create_platform_component(name='zc702_pmbus', hw_design=xsa_file)
```

Transcript message:
```
Platform zc702_pmbus creation started.
Failed to create platform: zc702_pmbus.Invalid XSA path '/home/mm/Documents/gh/pmbus_zc702'.
Traceback (most recent call last):
  File "/home/mm/Documents/gh/zc702-pmbus/build_sw.py", line 7, in <module>
    vpf = vc.create_platform_component(name='zc702_pmbus', hw_design=xsa_file)
  File "/opt/Xilinx/2025.1/Vitis/cli/vitis/cli_client.py", line 1713, in create_platform_component
    raise exc_type(f"{exc_value}") from None
Exception: Cannot create platform, status = StatusCode.INVALID_ARGUMENT,
            details = 'Invalid project location '/home/mm/Documents/gh/zc702-pmbus/build/vitis_ws/zc702_pmbus'.''
Shutting down Vitis server running on port '32827'
make: *** [Makefile:11: sw] Error 1
```

Log file contents:

```log
12:26:46 INFO  : Platform zc702_pmbus creation started.
12:26:46 ERROR : Failed to create platform: zc702_pmbus.Invalid XSA path '/home/mm/Documents/gh/pmbus_zc702'.
12:26:46 ERROR : Failed to create platform: zc702_pmbus. Invalid XSA path '/home/mm/Documents/gh/pmbus_zc702'.
```

Notes:

- According to the log file, the issue is an invalid XSA path.
- According to the transcript, the project is in an invalid location. This directly contradicts the log file.
- A space is missing in the log file before the word `Invalid`.
```diff
-12:26:46 ERROR : Failed to create platform: lol.Invalid XSA path '/home/mm/Documents/gh/pmbus_zc702'.
+12:26:46 ERROR : Failed to create platform: lol. Invalid XSA path '/home/mm/Documents/gh/pmbus_zc702'.
```
- The log file has a duplicate entry.
- The fix was to use an absolute path for the XSA file.

# License

Contents of this repository are licensed under the strongly reciprocal variant of the [CERN Open Hardware Licence](/LICENSE.txt) (CERN-OHL-S).
