# Car-Emotions-Gauge
By using an ELM327 as a translator between your car to the ESP32 micro-controller, the device is able to gain an understanding of the vehicles status and use tis information to display different emotions. This connection to the vehicle also provides us with the ability to monitor engine fault codes and clear them when desired.

# Components

- ESP32 [Amazon](https://www.amazon.com/Development-Microcontroller-Integrated-ESP-WROOM-32-NodeMCU-32S/dp/B09WXZ9441/ref=sr_1_3?crid=1Z3HTN915WHPA&keywords=esp32+38pin&qid=1686247838&sprefix=esp32+38pin%2Caps%2C63&sr=8-3)
- ELM327 OBD-II Wi-Fi Adapter [Amazon](https://www.amazon.com/Scanner-Android-iPhone-Launchh-Diagnostic/dp/B07Z675TJ3/ref=sr_1_2?crid=3SPV5KZVFA9VA&keywords=elm327+wifi&qid=1686247857&sprefix=elm327+wifi%2Caps%2C69&sr=8-2)
- 1.28" LCD Module [WaveShare](https://www.waveshare.com/1.28inch-lcd-module.htm)
- Momentary Push Buttons [Amazon](https://www.amazon.com/Gikfun-6x6x5mm-Switch-Button-Arduino/dp/B00R17XUFC/ref=sr_1_3?crid=7Q5EG8XC75FZ&keywords=pcb+buttons&qid=1686247882&sprefix=pcb+buttons%2Caps%2C124&sr=8-3)
- Custom PCB [JLCPCB]()

# Assembly

Using the SPI communication elements within the `Sylo_Prototype_3_Final.ino` file under the variable `Arduino_DataBus *bus` and the variable `Arduino_GFX *gfx` you will be able to connect the Display directly to the ESP32 using the pre-crimped wires.

With the display conencted, you can upload the script to the ESP32 using the Arduino IDE. It is also important you use the `Sketch Data Upload` tool in order to upload the contents of the `data` folder that include the .GIF files that act as the car's emotions.

Once the data and the source code have been uploaded you should see display turn on and the character will blink twice on screen. In the serial monitor you will see that the ESP32 is searching for the ELM327 Adapter. Once the Adapater is connected to your car, the character will then begin to emote the "feelings" of your car.

# Engine Fault Codes

If you would like to view your engine fault codes you can hold down button one which will bring up the menu. Using the buttons as navigation shown near the bottom of the screen, you can navigate to `DTC Codes` and then click button two to enter into it. It will then display the number of current codes and ask if the user would like to view them. You may view up to six different codes.

Moving back to the Menu, you can scroll one option below and you will see the `Clear DTC` option. It will then prompt the user warning them once more that the codes will be cleared. From here if cleared the engine codes in the vehicle will be erased and you will be directed back to the main menu. To return back to the emotions page, scroll to the `Home` tab.

