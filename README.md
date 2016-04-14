### Simple altimeter testing tool via serial port.

# How to use

First of all u have to select a correct serial port. It can be done by clicking on `Properties` tool icon as shown below. 
![Properties button](http://i.imgur.com/6quaNRc.png)

Default settings are fine, so you just need to select correct serial port and click `Apply`.
![Serial port selection](http://i.imgur.com/FLnXclU.png)

That is all. Now you can connect to your device by clicking on `Connect` icon. Connection status will be shown in bottom side of the window. 

![](http://i.imgur.com/UWf3Fkr.png)  

### Now you can: 

- Request current data by clicking on `Request data` button. You can automate this process by checking `Auto` check box (request every 80 ms). 
- Change device address. For this, you have to set `Current address`, `New address` and click `Set address button`.
- Request device type by clicking `Request type` (address must be set in `Altimeter data` group box). 
- Some warning and all I/O data (as hexadecimal) will be shown in `Logging` group box area. You can save them by clicking on `Save to file` (as plaint text) button. 

# How to compile. 

It's simple. You just need to download [Qt](https://www.qt.io/download-open-source/), and open *.pro file in Qt Creator. 

### License GPL3 