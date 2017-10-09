#ifndef MOTORDRIVER_H
#define MOTORDRIVER_H

#include <stdint.h>

//------------------------------------------------------------------------------
// enums
//------------------------------------------------------------------------------

// Two modes as defined in VNH2SP30 datasheet. 
typedef enum driverConfiguration
{
    HALFBRIDGE,
    FULLBRIDGE
} DriverConfiguration;
    
    
// Four states as defined in VNH2SP30 datasheet.
// If configured as half bridge, the only available
// modes are CLOCKWISE and BRAKETOGND  
typedef enum motorState
{
    CLOCKWISE,
    COUNTERCLOCKWISE,
    BRAKETOVCC,
    BRAKETOGND  // this will be the default case
} MotorState;


//------------------------------------------------------------------------------
// function declarations
//------------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" 
{
#endif

    typedef struct MotorDriver MotorDriver;
    
    MotorDriver* MotorDriver__create( 
        uint8_t en, uint8_t cs, uint8_t inA, 
        uint8_t inB, uint8_t pwm, DriverConfiguration mode );   // C constructor
    
    void MotorDriver__destroy( MotorDriver* self );       // C destructor


    // member functions
    DriverConfiguration MotorDriver__changeBridgeMode( 
        MotorDriver* self, DriverConfiguration mode );
    
    bool MotorDriver__motorDriverIsFaulty( MotorDriver* self );
    
    unsigned int MotorDriver__checkMotorCurrentDraw( MotorDriver* self );
    
    void MotorDriver__turnOffMotor( MotorDriver* self );
    
    bool MotorDriver__turnOnMotor( MotorDriver* self, MotorState state );
    
    MotorState MotorDriver__getMotorState( MotorDriver* self );

#ifdef __cplusplus
}
#endif


#endif // MOTORDRIVER_H
