# Finger Scale
The finger scale is a free and open source finger strength training device. It helps you measure your max strength, time your hangs, and overall improve your finger strength.
## Physical build
1. Connect the HX711 to the load cells as the instructions included.
1.1 (OPTIONAL) Put the HX711 in 80Hz mode by disconnecting pin 15 from ground and connecting it to VCC (pin 16). Your scale will work if you don't do this, bit it is reccomended to do, because it will increase responsiveness a lot.
1.2 (OPTIONAL) Connect E- to ground; E- is supposed to be ground, but it isn't actually connected to ground; not connecting it will give a decent and somewhat acurate scale, but if you do connect it it will be more accurate.
2. Connect The Vcc from the HX711 module to the 5V of your ESP8266 board, the Ground to Ground, the sck to Arduino pin 5, and the dt to Arduino pin 4.
3. Cut a piece of wood the size of the scale you want.
4. Either 3d print or cutout holders for the load sensors. Several printable files can be found [here](https://www.thingiverse.com/search?q=50kg+load+cell+holder). If you, like me, do not have a 3d printer you can cut a holder out off cardboard or wood.
5. Attach the load cells to their holders and the holders to the wood that is your scale.

## One time set up
1. Set up `config.h`: Rename [example_config.h](example_config.h) to `config.h` and replace all values. \
1.5 (OPTIONAL) Set up the ip address of your scale as static ip on your router: If your the administrator of your network you can assign your desired ip to the mac address of your board. This will guarantee that the ip is never taken.
2. Install the ESP8266 Board in the [Arduino IDE](https://www.arduino.cc/en/software): Go to file->preferences->Additional board manager URLs: and paste `https://arduino.esp8266.com/stable/package_esp8266com_index.json`. Then go to tools->Board->Board manager and type `esp8266` and install.
3. Install the libraries: Easiest way is to use the Arduino IDE to install the libraries; The libraries you need are [ESP Async WebServer](https://github.com/mathieucarbou/ESPAsyncWebServer), [ESPAsyncTCP](https://github.com/dvarrel/ESPAsyncTCP), and [HX711_ADC](https://github.com/olkal/HX711_ADC).
4. Calibrate the scale: With the HX711_ADC library comes a calibration example. In the Arduino IDE go to file -> Examples -> HX711_ADC -> Calibration. Upload the code, and follow the instructions printed to serial, Make sure to save to EEPROM.
5. Install the filesystem uploader: Install the uploader from [here](https://github.com/earlephilhower/arduino-littlefs-upload/releases) and move the `arduino-littlefs-upload-1.5.0.vsix` to `*/.arduinoIDE/plugins`.
6. Upload the filesystem: In the Arduino IDE hit ctrl+shit+p and type "Upload LittleFS to Pico/ESP8266/ESP32" and hit enter.
7. Upload scale.ino.

## Usage
In any browser on a device connected to the same network as your scale go to http://\<the ip you choose\>/ and you should see the webpage defined [here](/data/index.html). Stand on the scale, click 'Reset 0' on the webpage, now the 'Current force' and 'Max force' should both say '0kg'. Start pulling on the fingerboard, 'Current force' will display the current force, and 'Max force' will show the biggest force you have pulled. You can reset the 'Max force' by clicking 'Reset max'. When you click 'Start continuous measurement' it will start showing a graph of force over time, click 'Stop continuous measurement' to stop graphing and 'Download csv file' to download .csv file containing the last continuous measurements. The first column is time in milliseconds, and the second column is force in whichever units you used for calibration. \
Optionally you can install a browser extension like [Redirector](https://addons.mozilla.org/en-GB/firefox/addon/redirector/) to redirect a name of your choosing to the scale's ip. 