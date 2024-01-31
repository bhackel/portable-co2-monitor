# portable-co2-monitor

## Overview

Recent research has shown possible correlations between high CO2 concentration in indoor environments and decreased cognitive function. As someone that is curious about optimizing my brain for productivity, I wanted to investigate this correlation.

This idea started when I attended an office hour for my algorithms class. This took place in a very small room, packed full of students. I immediately felt the thickness of the air in the room, and shortly after, lost focus and forgot my question. I left for a bit to reconcentrate, believing that I might be tired from a lack of sleep. However, when I went back into the room, I lost my focus once again. I heard about CO2 fatigue from a YouTube video and was curious about it.

![Office Hour Room](todo-get-image)

I looked around on Amazon for a cheap monitor, but the majority of the devices I found were above $100 (there are a lot of new devices below $40 now). I was not willing to spend so much money on a curiosity, so I decided to look for a cheap alternative.

## V0 Prototype Design

I looked on AliExpress to see if I could find a cheap sensor to use. I quickly found the SenseAir S8 for around $20, and decided to buy one.

I used a spare Arduino Nano 33 IoT that I had on hand and attempted to connect it to the SenseAir S8. I found [this article](https://karlduino.org/CO2monitor/) describing how someone else connected their Arduino to this sensor, so I followed their process to create my code. I had to slightly modify the code to work with the hardware serial connections instead of using the SoftwareSerial package, but otherwise had minor issues.

After verifying that the sensor works, I decided to begin designing ways to make the device more portable. This began with a CAD model.

![Fusion 360 Image](docs/images/Fusion%20360%201.png)

My goal here was to try to visualize the parts so that I could think of ideas on how to integrate everything. I printed this at my local makerspace and proceeded with integration.

I roughly put the parts on a sheet of perfboard and measured out the width that I decided on for my CAD model. Everything seemed to fit, so I proceeded to cut the board.

![Perfboard Design](docs/images/Design%201.jpg)

I decided to flip the CO2 sensor because the foam diffuser needs access to air, and I planned to face this towards the side of the mount.

![Cut Perfboard with parts](docs/images/Design%203.jpg)

I connected up the pins of the Arduino and the CO2 sensor using solder and wires. Yes, I lack good soldering skills.

The wiring is as follows

- Green Wire: Arduino TX Pin <-> SenseAir S8 RX Pin
- Blue Wire: Arduino RX Pin <-> SenseAir S8 TX Pin
- Red Wire: Arduino VUSB <-> SenseAir S8 G+ Pin
- Black Wire: Arduino GND <-> SenseAir S8 G0 Pin

![Back side of perfboard with soldered wires](docs/images/Design%202.jpg)

The next step was to somehow mount the board into the case. However, although I was planning on powering the device over USB, I realized at this point that this was not feasible because of a lack of space. I then decided to cut a broken USB cable and solder it directly to the board.

![Board and mount with USB cable](docs/images/Assembling%201.jpg)

Next was to shove the board into the case and mount it somehow. I decided to put a screw through the case and the board, and then attach it with a washer.

![Board inside of case](docs/images/Assembling%203.jpg)

After using some very ugly hot glue to attach the side panel of the case, I ended with the final design.

![Completed build](docs/images/Final%202.jpg)
![Completed build on belt](docs/images/Final%201.jpg)

## iOS App

The second half of this project involves an accompanying iOS app that connects to the device over Bluetooth.

More to come later