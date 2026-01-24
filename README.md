# Auto-plant


it consists in two esp32, one for *sensing and pumping* moisture from the soil and water to the plants respecetevly, and the other to control a *robot hose* to point the water to the different pots.





## sensing-pumping

this board function is to perform sensor readings, tell the robot hose to which location to move and start pumping water when the robot has reached the desired location

additionally, it will have an access point to render a website with the readings


### pumping 
code to control a 6v water pump connected with the board through a relay, will have hardcoded how much time to power the pump considering the water from the water flowing from the pipe and how much water is desired to be placed on the plant


## robot hose


