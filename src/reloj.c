/************************************************************************************************
Copyright (c) 2023, Balthazar Martin <balthazarm8@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute,
sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial
portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

SPDX-License-Identifier: MIT
*************************************************************************************************/

/**
 ** \author Balthazar Martin
 ** \date 14/06/23
 ** \brief Implemtancion de la Libreria del modulo reloj
 **
 ** \addtogroup reloj reloj.h
 ** \brief Implementacion de las funciones del reloj
 ** @{ */

/* === Headers files inclusions =============================================================== */

#include "reloj.h"
#include <string.h>

/* === Macros definitions ====================================================================== */

/* === Private data type declarations ========================================================== */

//! Estructura publica del reloj con la hora y alarma
struct clock_s{
    clock_event_t handler; //!< Funcion de CallBack para activar la alarma
    uint8_t hora_actual[TIME_SIZE]; //!< Arreglo de 1 byte para indicar la hora actual del reloj
    uint8_t alarma_actual[TIME_SIZE]; //!< Arreglo para indicar la alarma actual del reloj
    uint8_t alarma_reservada[TIME_SIZE]; //!< Arreglo para guardar la alarma por si se la pospone
    int tics_por_segundo; //!< Indica cuantos ticks(pulsos) por segundo hara el reloj al ser creado
    int ticks;  //!< Indica cuantos ticks(pulsos) va haciendo el reloj
    bool reloj_estado; //!< Booleano para indicar si la hora del reloj esta configurado o no
    bool alarma_estado; //!< Indica si la alarma del reloj esta activa o no
    bool alarma_postergada; //!< Indica si la alarma fue postergada o no
    //bool alarma_activa;
};

/* === Public data type declarations =========================================================== */
    
/* === Public variable declarations =========================================================== */

/* === Private variable declarations =========================================================== */

/* === Private function declarations =========================================================== */

//  FALTA DOCUMENTACION DE ESTO
//! A
void TraducirMinutos(uint8_t minutos, uint8_t tiempo_post[]);

//! A
void SumarHorarios(uint8_t alarma_actual[], uint8_t time_post[], uint8_t resultado[]);

/* === Public variable definitions ============================================================= */

/* === Private variable definitions ============================================================ */

/* === Private function implementation ========================================================= */

/* === Public function implementation ========================================================== */

//! A
clock_t ClockCreate(int tics_por_segundo, clock_event_t handler){
    static struct clock_s self[1];
    memset(self, 0, sizeof(self));
    self->handler = handler;
    self->tics_por_segundo = tics_por_segundo;
    return self;
}

//! A
bool ClockGetTime(clock_t reloj, uint8_t * hora, int size){
    memcpy(hora, reloj->hora_actual, size);
    return reloj->reloj_estado;
}

//! A
void ClockSetTime(clock_t reloj, const uint8_t * hora, int size){
    memcpy(reloj->hora_actual, hora,size);
    reloj->reloj_estado = true;
}

//! A
bool ClockGetAlarm(clock_t reloj, uint8_t * hora, int size){
    memcpy(hora, reloj->alarma_actual, size);
    return reloj->alarma_estado;
}

//! A
void ClockSetAlarm(clock_t reloj, const uint8_t * hora, int size){
    memcpy(reloj->alarma_actual, hora,size);
    reloj->alarma_estado = true;
}

//! A
void CheckAlarmActive(clock_t reloj){
    if ( memcmp(reloj->alarma_actual, reloj->hora_actual, TIME_SIZE) == 0  && reloj->alarma_estado ){
        reloj->handler(reloj);
     }
}

//! A
void ClockAlarmToggle(clock_t reloj){
    reloj->alarma_estado = !reloj->alarma_estado;   
}

//! A
void ClockStopAlarm(clock_t reloj){
    if(reloj->alarma_postergada && reloj->alarma_estado){
        memcpy(reloj->alarma_actual, reloj->alarma_reservada, sizeof(reloj->alarma_reservada));
        reloj->alarma_postergada = false;
    }
}

//! A
void TraducirMinutos(uint8_t minutos, uint8_t tiempo_post[]) {
    // Obtener las horas
    uint8_t horas = minutos / 60;

    // Obtener las decenas y unidades de la hora
    tiempo_post[0] = horas / 10;
    tiempo_post[1] = horas % 10;

    // Obtener las decenas y unidades de los minutos
    tiempo_post[2] = (minutos % 60) / 10;
    tiempo_post[3] = (minutos % 60) % 10;

}

//! A
void SumarHorarios(uint8_t alarma_actual[], uint8_t time_post[], uint8_t resultado[]) {
    uint8_t carry = 0; // Variable para llevar el acarreo

        // Suma de los minutos
        resultado[3] = (alarma_actual[3] + time_post[3] + carry) % 10;
        carry = (alarma_actual[3] + time_post[3] + carry) / 10;

        // Suma de las decenas de minutos
        resultado[2] = (alarma_actual[2] + time_post[2] + carry) % 6;
        carry = (alarma_actual[2] + time_post[2] + carry) / 6;

        // Suma de las horas
        resultado[1] = (alarma_actual[1] + time_post[1] + carry) % 10;
        carry = (alarma_actual[1] + time_post[1] + carry) / 10;

        // Suma de las decenas de horas
        resultado[0] = (alarma_actual[0] + time_post[0] + carry) % 3;
        carry = (alarma_actual[1] + time_post[1] + carry) % 3;

}

//! A
void ClockPostponeAlarm(clock_t reloj, uint8_t tiempo){
    uint8_t time_post[TIME_SIZE];
    uint8_t time[TIME_SIZE];

    memcpy(reloj->alarma_reservada, reloj->alarma_actual, TIME_SIZE); //dudo si puedo usar time_size
    reloj->alarma_postergada = true;
    TraducirMinutos(tiempo, time_post);
    SumarHorarios(reloj->alarma_actual, time_post, time);
    memcpy(reloj->alarma_actual, time, sizeof(time));
}

//! A
void ClockTick(clock_t reloj){
    reloj->ticks++;

    if (reloj->ticks == reloj->tics_por_segundo) {  //segundos unidad
        reloj->ticks = 0;
        reloj->hora_actual[5]++;
    }
    if (reloj->hora_actual[5] == 10){   //segundos decena
        reloj->hora_actual[5] = 0;
        reloj->hora_actual[4]++;
    }
    if (reloj->hora_actual[4] == 6){    //minutos unidad
        reloj->hora_actual[4] = 0;
        reloj->hora_actual[3]++;
        CheckAlarmActive(reloj);
    }
    if (reloj->hora_actual[3] == 10){   //minutos decena
        reloj->hora_actual[3] = 0;
        reloj->hora_actual[2]++;
    }
    if (reloj->hora_actual[2] == 6){   //hora unidad
        reloj->hora_actual[2] = 0;
        reloj->hora_actual[1]++;
    }
    if (reloj->hora_actual[1] == 10){   //hora decena
        reloj->hora_actual[1] = 0;
        reloj->hora_actual[0]++;
    }
    if (reloj->hora_actual[0] == 2 && reloj->hora_actual[1] == 4){   //un dia entero
        reloj->hora_actual[0] = 0;
        reloj->hora_actual[1] = 0;
    }

}

/* === End of documentation ==================================================================== */

/** @} End of module definition for doxygen */