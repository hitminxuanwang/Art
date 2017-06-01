if /i "%PROCESSOR_IDENTIFIER:~0,3%"=="X86" (echo 32bit) ELSE  hhtdev remove "HID\Vid_1cbe&Pid_0114&Col05"
if /i "%PROCESSOR_IDENTIFIER:~0,3%"=="X86" (echo 32bit) ELSE  hhtdev remove "{C3BF46B7-AECF-4628-B748-25B93E654F32}\HID_DEVICE"
