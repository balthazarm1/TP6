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

/* === Headers files inclusions =============================================================== */

#include "reloj.h"
#include <string.h>

/* === Macros definitions ====================================================================== */

#define TIME_SIZE 6

/* === Private data type declarations ========================================================== */

//! 
struct clock_s{
    clock_event_t handler;
    uint8_t hora_actual[TIME_SIZE]; //!<
    uint8_t alarma_actual[TIME_SIZE];
    int tics_por_segundo;   //!<
    int ticks;  //!<
    bool valida;    //!<
    bool alarma_estado;
    bool alarma_activa;
};
    

/* === Private variable declarations =========================================================== */

/* === Private function declarations =========================================================== */

/* === Public variable definitions ============================================================= */

/* === Private variable definitions ============================================================ */

/* === Private function implementation ========================================================= */

/* === Public function implementation ========================================================== */

clock_t ClockCreate(int tics_por_segundo, clock_event_t handler){
    static struct clock_s self[1];
    memset(self, 0, sizeof(self));
    self->handler = handler;
    self->tics_por_segundo = tics_por_segundo;
    return self;
}

bool ClockGetTime(clock_t reloj, uint8_t * hora, int size){
    //memset(hora, 0, size);
    memcpy(hora, reloj->hora_actual, size);
    return reloj->valida;
}

bool ClockSetTime(clock_t reloj, const uint8_t * hora, int size){
    //memset(hora, 0, size);
    memcpy(reloj->hora_actual, hora,size);
    reloj->valida = true;
    return true;
}

bool AlarmGetTime(clock_t reloj, uint8_t * hora, int size){
    //memset(hora, 0, size);
    memcpy(hora, reloj->alarma_actual, size);
    return reloj->alarma_estado;
}

bool AlarmSetTime(clock_t reloj, const uint8_t * hora, int size){
    //memset(hora, 0, size);
    memcpy(reloj->alarma_actual, hora,size);
    reloj->alarma_estado = true;
    return reloj->alarma_estado;
}

// bool isAlarmActive(clock_t reloj){   
//     return(reloj->alarma_activa);
// }

void CheckAlarmActive(clock_t reloj){
    if (memcmp(reloj->alarma_actual, reloj->hora_actual, TIME_SIZE) == 0){
        reloj->handler(reloj);
     }
}

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





//isAlarmActive()   //ActivarAlarma     //DesactivarAlarma  //setAlarma     //getAlarma
//posponerAlarma    //AceptadaAlarma

//otra forma es que clock tnga alarma_activa(clock)     //otra forma es crear una hal alarm on y alarm off



/* === End of documentation ==================================================================== */

/** @} End of module definition for doxygen */