# VNH2SP30 H-Bridge Driver for DC motors For SparkFun MonsterMoto Shields and  
el cheapo Chinese VNH2SP30 motor driver modules


The original example sketch by [Jim Lindblom](http://cdn.sparkfun.com/  
datasheets/Dev/Arduino/Shields/MonsterMoto_Shield_Example.pde) 
is now made object-oriented programming friendly.  A few extra functions have 
been added to the library.  See below for details. 

This library is written in object-oriented C.  I could have written a C++ Class 
instead but would like to get a hang of C-style OOP.

---

## Preparing the SparkFun MonsterMoto Shield for half-bridge mode operation:

When activating half bridge mode on the SparkFun 
MonsterMoto Shield: 
1. remove R2 and short pads, 
2. short InA and InB, 
3. short OutA and OutB;
4. Remove any header pins on InB and Out B.

---

## Using the library:

1. Make sure the files in this repository are located in your Arduino library 
folder or your Arduino sketch folder.

2. Define your pins in the Arduino Sketch.  For example:

```C
// Sparkfun MonsterMoto motor driver pins
#define MD_A_EN A0
#define MD_A_CS A2
#define MD_A_INA 7
#define MD_A_INB 8  // put a placeholder value i.e. 255 when using halfbridge mode
#define MD_A_PWM 5

// and if you are also using the other VNH2SP30 driver chip
#define MD_B_EN A1
#define MD_B_CS A3
#define MD_B_INA 4
#define MD_B_INB 9  // put a placeholder value i.e. 255 when using halfbridge mode 
#define MD_B_PWM 6
```

3. Create instances of the motorDriver "class" in your Arduino file.

```C
MotorDriver* motorDriver_A = MotorDriver__create( MD_A_EN, MD_A_CS, MD_A_INA,
    MD_A_INB, MD_A_PWM, FULLBRIDGE );
    
MotorDriver* motorDriver_B = MotorDriver__create( MD_B_EN, MD_B_CS, MD_B_INA,
    MD_B_INB, MD_B_PWM, FULLBRIDGE );
```

---

## Available member functions:

Check the C implementation file for more details.  But in summary:

* Change bridge mode - YOU HAVE TO FIGURE OUT SOME WAY TO UNDO THE 
**HALF-BRIDGE MODIFICATIONS** IN SITU
```C
void MotorDriver__changeBridgeMode( MotorDriver* self, DriverConfiguration mode );
```

* Check if motor driver is faulty
```C
bool MotorDriver__motorDriverIsFaulty( MotorDriver* self );
```

* Check motor current draw
```C
unsigned int MotorDriver__checkMotorCurrentDraw( MotorDriver* self );
```

* Turn off motor
```C
void MotorDriver__turnOffMotor( MotorDriver* self );
```

* Turn on motor
```C
bool MotorDriver__turnOnMotor( MotorDriver* self, MotorState state );
```

* Get motor state
```C 
MotorState MotorDriver__getMotorState( MotorDriver* self );
```
