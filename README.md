# Arduino Accelerometer Project
#### Author: Junle Yan | Client: Tom Wood

#### User Guide

- `\arduino` contain source code for the Arduino device
    - `INTERVALTIME` in the code can be changed to adjust the reading interval

- `\processing` contain a Windows executable to process TXT file
    - By running `txt2csv.exe`, it generates a CSV file for each TXT file detected within the folder
    - Processed CSV file should contain the following headers:
        - `interval` - reading interval of the sensors
        - `hour`, `minute`, and `second` - time recorded from RTC time module
        - `acX`, `acY`, and `acX` - the linear accelerations of the XYZ axis
        - `gyX`, `gyY`, and `gyZ` - the angular accelerations of the XYZ axis
    - `\source` contain the source code for the Windows executable

- To view live data through Bluetooth, you would need to download Bluetooth LE terminal on your mobile device. For my testing, I been using Bluetooth Terminal on Apple App Store. Once you have finished downloading the App, you have to connect to the Arduino within the App. After that, you can now view live data being sent to your device from the Arduino. Demo video: https://cdn.discordapp.com/attachments/933860483067412480/1053562517332430898/RPReplay_Final1671259238.mov