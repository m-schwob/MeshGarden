- seems that on board regulator not nessccery LDO and may not fit power requirements.
- make table for the popular boards and compare regulator (max current (and max current), voltage range, power drop)
- make table of comparing solutions - efficiency, life time, complexity.
- the idea is to separate power module from controller. the interface is with 2 option 3.3/5V like most boards. the specific solution will mke sure the right voltage 

** base configuration is 3.3V direct to chip or 5 volt via LDO. **
    - micro usb on board connected to 5V and data.
    - pro configuration use the on board regulator.

- think how to configure voltage rang. maybe one user config and second using build in input measurement


options:
1. base and simple - use replaceable battery (3 AAA batteries, or lithium). connect to 5V input
2. lithume with cnargingg module
3. direct wall adpater standart usb
4. solar panel/wind turbine with batteries or capacitor

-pro: you can make uredelf that make sure voltage and corrent range