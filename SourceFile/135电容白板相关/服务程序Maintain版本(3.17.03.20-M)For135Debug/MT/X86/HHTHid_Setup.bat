if /i "%PROCESSOR_IDENTIFIER:~0,3%"=="X86"  hhtdev remove "HID\Vid_1cbe&Pid_0114&Col05"
if /i "%PROCESSOR_IDENTIFIER:~0,3%"=="X86"  hhtdev remove "{C3BF46B7-AECF-4628-B748-25B93E654F32}\HID_DEVICE"


if /i "%PROCESSOR_IDENTIFIER:~0,3%"=="X86"  hhtdev install HHTHid.inf "{C3BF46B7-AECF-4628-B748-25B93E654F32}\HID_DEVICE"
if /i "%PROCESSOR_IDENTIFIER:~0,3%"=="X86"  hhtdev update HHTHidMouFiltr.inf "HID\Vid_1cbe&Pid_0114&Col03"
