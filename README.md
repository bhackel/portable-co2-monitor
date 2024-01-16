# portable-co2-monitor
Project for a portable CO2 Monitor

## The Idea

This is the first project where I took the time to think through an ideal version of a CO2 monitor, then I broke it down into small pieces and will be attempting to execute on it

Full End-To-End project
 - IOS App that can sync data from the device
 - Device that monitors CO2 levels, can be attached to person

Software requirements:
- IOS App
	- Want to build in swift
	- Needs to connect to the device over bluetooth, then read some stored data, then clear the stored data on the device, and store it in  app
	- Display a graph that shows CO2 exposure over the day
	- Export button to download CSV with "time: ppm" values
- Microcontroller code
	- Possibly translate it into Rust
	- data should be collected every 30? seconds
	- Poll the Senseair using raw Serial comms
	- First develop on arduino 33 iot
	- Then create PCB design with the components
	- This must happen before 3D print can be done

Hardware requirements:
- Option 1: Wrist
	- 3D print a decently big rectangle that attaches to the wrist using Velcro
	- Power using a wire to a battery bank (need 5V), or integrate battery somehow
	- This seems like the ultimate option, copy a Fitbit in design. highest difficulty
- Option 2: Belt hook
	- 3D print a box that clips onto the belt
	- Power using a wire to a battery bank that is in pocket
	- This is where I should start

Procedure
- We already have a working CO2 monitor
- #### ARDUINO
- Add Bluetooth functionality so that I can connect to it with my laptop and read the current CO2 value
- Add Bluetooth functionality so that I can connect to it, read up to the past 100 values, then these values are cleared from a storage in memory
- #### APP
- ~~Create a swift Hello World app and send it to my phone~~ Done 12/31/23
- Add Bluetooth functionality to the swift app so that it can connect to the Arduino and read the current CO2 value to the screen
- Have it continually read the CO2 value and put it on the screen
- Have it read the past 100 values
- Create a storage for storing CO2 values over time
- Create a graph that shows these values
- Create a table that shows these values
- Create a button that allows to share a CSV with these values
- #### PCB
- Design a PCB that integrates all the components nicely
- Make sure to figure out how to get that microcontroller into there
- Somehow get the power connected with a connector
- I am unfamiliar in this part
- #### 3D Printing
- Create a box that encases the PCB and has poles or something to mount it on
- Don't be lazy and use zipties, make this look professional with screw mounts