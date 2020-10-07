    //
    //  stack.h
    //  mortal-stack
    //
    //  Created by Alexander Makhov on 8/10/20.
    //

#ifndef stack_h
#define stack_h

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <string.h>

#define EPS     1e-9
#define NOLL    NAN

#define сonstrStack( pseudo_address, start_size )                  \
char* var_name = (char*) calloc( 72, sizeof ( char ) );            \
strcpy( var_name, # pseudo_address );                              \
constrStack( pseudo_address, start_size, var_name + 1 );           \
free( var_name )

typedef double elem_t;

typedef enum OPERATION
{
    PUSH,
    POP
} OPERATION;

typedef enum ERROR_MESSAGE
{
    OK,
    NULL_POINTER_STACK,
    STACK_CANARY_A,
    STACK_CANARY_B,
    DATA_CANARY_A,
    DATA_CANARY_B,
    NULL_POINTER_DATA,
    ZERO_CAPACITY,
    ZERO_SIZE,
    SIZE_GR_CAPACITY,
    CHECKSUM_INVALID,
    NAN_ELEMENT
} ERROR_MESSAGE;

typedef struct Stack
{
    long int* stack_canarry_a;
    
    size_t  size;
    size_t  capacity;
    elem_t  * data;
    char    * var_name;
    int     checksum;
    
    long int* stack_canary_a;
    long int* stack_canary_b;
    
    long int* stack_canarry_b;
} Stack;

int checkSum( Stack* stack );

void constrStack( Stack* stack, const size_t start_capacity, const char* var_name );

Stack* newStack( const size_t start_capacity );

void stackPush( Stack* stack, const elem_t value );

elem_t topStack( Stack* stack );

void stackPop( Stack* stack );

void deleteStack( Stack* stack );

ERROR_MESSAGE stackOK( Stack* stack );

void stackDump( Stack* stack );

#endif /* stack_h */
