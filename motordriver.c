#include "motordriver.h"
#include "Arduino.h"
#include "assert.h"


//------------------------------------------------------------------------------
// C OOP Implementation
//------------------------------------------------------------------------------

// create a struct MotorDriver (Analog to C++ Class) 
// containing control pins for each motor driver chip
// on Sparkfun Monster Motor shield
typedef struct MotorDriver
{
    uint8_t en;                  // enable input/output (ANALOG)
    uint8_t cs;                  // current sense output (ANALOG)
    uint8_t inA;                 // clockwise input
    uint8_t inB;                 // counterclockwise input
    uint8_t pwm;                 // pwm input
    DriverConfiguration mode; // HALFBRIDGE or FULLBRIDGE
    MotorState state;         // state of connected motor
} MotorDriver;

////////////////////////////////////////////////////////////////////////////////

// Function to initialize variables
void MotorDriver__init( MotorDriver* self, 
                        uint8_t en, 
                        uint8_t cs, 
                        uint8_t inA,
                        uint8_t inB, 
                        uint8_t pwm, 
                        DriverConfiguration mode )
{ 
    self->en = en;
    self->cs = cs;
    self->inA = inA;
    self->inB = inB;
    self->pwm = pwm;
    self->mode = mode;
    self->state = BRAKETOGND;
}

////////////////////////////////////////////////////////////////////////////////

// Function to create a MotorDriver instance
MotorDriver* MotorDriver__create( uint8_t en, 
                                  uint8_t cs, 
                                  uint8_t inA,
                                  uint8_t inB, 
                                  uint8_t pwm, 
                                  DriverConfiguration mode )    // C constructor
{
    // allocate memory
    MotorDriver* md = ( MotorDriver* ) malloc( sizeof( MotorDriver ) );
    assert( md );
//    if( !md )
//    {
//        return null;
//    }

    // initialize with variables
    MotorDriver__init( md, en, cs, inA, inB, pwm, mode );
    
    // map Arduino<->Motor Shield pins
    pinMode( md->inA, OUTPUT );
    pinMode( md->inB, OUTPUT );
    pinMode( md->pwm, OUTPUT );
    pinMode( md->cs, INPUT );
    digitalWrite( md->inA, LOW );
    digitalWrite( md->inB, LOW );
    
    return md;  
}

////////////////////////////////////////////////////////////////////////////////

void MotorDriver__reset( MotorDriver* self )
{
  
}

////////////////////////////////////////////////////////////////////////////////

// Function to destruct MotoDriver instance
void MotorDriver__destroy( MotorDriver* md )
{
    if ( md ) // if allocated memory i.e. not null
    {
        MotorDriver__reset( md );
        free( md );
    }
}

////////////////////////////////////////////////////////////////////////////////

// Function to change the driver configuration (HALFBRIDGE OR FULLBRIDGE)
DriverConfiguration MotorDriver__changeBridgeMode( MotorDriver* self, 
                                                   DriverConfiguration mode )
{
    if ( MotorDriver__motorDriverStatus( self ) && 
         self->mode != mode )   // if no fault, i.e. is true and 
                                // different mode specified
    {
        MotorState current_state = self->state;   // save current state
        MotorDriver__turnOffMotor( self );    // turn off motor
        self->mode = mode;    // update mode
        MotorDriver__turnOnMotor( self, current_state );
    }

    // otherwise, do nothing
}

////////////////////////////////////////////////////////////////////////////////

// Function to check motor driver status
// Returns: true if fault is detected (shutdown), false if none 
bool MotorDriver__motorDriverIsFaulty( MotorDriver* self )
{
    // en pin is already pulling HIGH in hardware
    // but if a fault causes shutdown,
    // this pin will output LOW
    ( digitalRead( self->en ) == LOW ) ? true : false;
}

////////////////////////////////////////////////////////////////////////////////

// Function to check motor current draw
// Returns: current draw value mapped from 0 to 1024
unsigned int MotorDriver__checkMotorCurrentDraw( MotorDriver* self )
{
    return analogRead( self->cs );
}

////////////////////////////////////////////////////////////////////////////////

// Function to turn off motor
// Returns: nothing
void MotorDriver__turnOffMotor( MotorDriver* self )
{
    digitalWrite( self->inA, LOW );
    
    if ( self->mode == FULLBRIDGE )
    {
        digitalWrite( self->inB, LOW );
    }
    
    analogWrite( self->pwm, 0 );
    self->state = BRAKETOGND;
}

////////////////////////////////////////////////////////////////////////////////

// Function to make motor operate in any of four states (two if HALFBRIDGE MODE)
// If returns true, the chosen state is saved to instance
// Precondition: state < CLOCKWISE || state > BRAKETOGND
//               mode = HALFBRIDGE || FULLBRIDGE
// Parameter: state - one of four(two) states - see enum motorState above
// Returns: true if operation is successful, false if state is incompatible
bool MotorDriver__turnOnMotor( MotorDriver* self, MotorState state )
{    
    if ( self->mode == HALFBRIDGE )
    {
        if ( state == CLOCKWISE )
        {
            digitalWrite( self->inA, HIGH );
        }
            
        else if ( state == BRAKETOGND )
        {
            digitalWrite( self->inA, LOW );
        }

        else
        {
            return false;
        }
    }

    else // if mode == FULLBRIDGE
    {
        // set GPIO states
        ( state == CLOCKWISE || state == BRAKETOVCC ) ? 
            digitalWrite( self->inA, HIGH ) : 
            digitalWrite( self->inA, LOW );

        ( state == COUNTERCLOCKWISE || state == BRAKETOVCC )?
            digitalWrite( self->inB, HIGH ) : 
            digitalWrite( self->inB, LOW );
    }

    self->state = state;
    return true;
}

////////////////////////////////////////////////////////////////////////////////

// Function to check operating state of Motor Driver
MotorState MotorDriver__getMotorState( MotorDriver* self )
{
    return self->state;
}

