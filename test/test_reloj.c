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
#include "unity.h"

/**
 ** \author Balthazar Martin
 ** \date 14/06/23
 ** \brief Test Driven Development del modulo reloj
 **
 ** \addtogroup reloj reloj.h
 ** \brief Banco de pruebas para las funciones y el funcionaminento del reloj
 ** @{ */

/* === Macros definitions ====================================================================== */

#define TICKS_PER_SECOND 5

#define SIMULATE_SECONDS(VALUE, function)                           \
    for ( int count=0; count < VALUE; count++) {                    \
        for( int index = 0; index < TICKS_PER_SECOND ; index++){    \
            function;                                               \
        }                                                           \
    }                                                               \


/* === Private data type declarations ========================================================== */

static clock_t reloj;
static uint8_t hora[TIME_SIZE];
static uint8_t alarma[TIME_SIZE];
static bool evento_alarma_notificado;
// static bool evento_alarma_state;
// static bool evento_alarma_pospuesta;

/* === Private variable declarations =========================================================== */

/* === Private function declarations =========================================================== */
                                //les falta static a esto no?
void funcion_alarma_notificada(clock_t reloj){
    evento_alarma_notificado = true;
}

void funcion_alarma_nonotificada(clock_t reloj){ //dudo de tener que hacer esto //StopAlarm();
    evento_alarma_notificado = false;
}

// void funcion_alarma_habilitada(clock_t reloj){
//     evento_alarma_state = true;
// }

// void funcion_alarma_deshabilitada(clock_t reloj){
//     evento_alarma_state = false;
// }

// void funcion_alarma_pospuesta(clock_t reloj){
//     evento_alarma_pospuesta = true;
// }

/* === Public variable definitions ============================================================= */

/* === Private variable definitions ============================================================ */

/* === Private function implementation ========================================================= */

/* === Public function implementation ========================================================== */

// la forma en como esta escrito permite que siempre siempre se inicie | suitsetup, suitardown
void setUp(void){   //codigo de inicializacion
    static const uint8_t INICIAL[] = {1, 2, 3, 4};
    static const uint8_t INICIAL_ALARMA[] = {1, 2, 3, 6};
    evento_alarma_notificado = false;
    // evento_alarma_state = false;
    // evento_alarma_pospuesta = false;
    reloj = ClockCreate(TICKS_PER_SECOND, funcion_alarma_notificada);
    ClockSetTime(reloj, INICIAL, sizeof(INICIAL));
    ClockSetAlarm(reloj, INICIAL_ALARMA, sizeof(INICIAL_ALARMA));
}

//! Test para verificar que: Al inicializar el reloj está en 00:00 y con hora invalida.
void test_reloj_arranca_con_hora_invalida(void){
    static const uint8_t ESPERADO[] = {0, 0, 0, 0, 0, 0};
    reloj = ClockCreate(TICKS_PER_SECOND, funcion_alarma_notificada); 
    uint8_t hora[TIME_SIZE] = {0xFF};
    TEST_ASSERT_FALSE(ClockGetTime(reloj,hora,sizeof(hora)));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO,hora,sizeof(ESPERADO));
}

//! Tests para verificar que: Al ajustar la hora el reloj queda en hora y es válida.
void test_ajustar_hora(void){      
    static const uint8_t ESPERADO[] = {1, 2, 3, 4, 0, 0};
    uint8_t hora[TIME_SIZE] = {0xFF};
    ClockSetTime(reloj, ESPERADO, 6);
    TEST_ASSERT_TRUE(ClockGetTime(reloj,hora,sizeof(hora)));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO,hora,sizeof(ESPERADO));
}

//! Tests para verificar que: Después de n ciclos de reloj la hora avanza un segundo
void test_incrementar_segundos_unidad(void){   
    static const uint8_t ESPERADO[] = {1, 2, 3, 4, 0, 1};
    SIMULATE_SECONDS(1, ClockTick(reloj));
    ClockGetTime(reloj, hora, sizeof(hora));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO,hora, sizeof(ESPERADO));
}

//! Test para verificar que: Después de n ciclos de reloj la hora avanza diez segundos
void test_incrementar_segundos_decena(void){   
    static const uint8_t ESPERADO[] = {1, 2, 3, 4, 1, 0};
    SIMULATE_SECONDS(10, ClockTick(reloj));
    ClockGetTime(reloj, hora, sizeof(hora));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO,hora, sizeof(ESPERADO));
}

//! Test para verificar que: Después de n ciclos de reloj la hora avanza un minuto
void test_incrementar_minutos_unidad(void){   
    static const uint8_t ESPERADO[] = {1, 2, 3, 5, 0, 0};
    SIMULATE_SECONDS(60, ClockTick(reloj));
    ClockGetTime(reloj, hora, sizeof(hora));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO,hora, sizeof(ESPERADO));
}

//! Test para verificar que: Después de n ciclos de reloj la hora avanza diez minutos
void test_incrementar_minutos_decena(void){   
    static const uint8_t ESPERADO[] = {1, 2, 4, 0, 0, 0};
    SIMULATE_SECONDS(60*6, ClockTick(reloj));
    ClockGetTime(reloj, hora, sizeof(hora));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO,hora, sizeof(ESPERADO));
}

//! Test para verificar que: Después de n ciclos de reloj la hora avanza una hora
void test_incrementar_hora_unidad(void){   
    static const uint8_t ESPERADO[] = {1, 3, 0, 0, 0, 0};
    SIMULATE_SECONDS(60*26, ClockTick(reloj));
    ClockGetTime(reloj, hora, sizeof(hora));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO,hora, sizeof(ESPERADO));
}

//! Test para verificar que: Después de n ciclos de reloj la hora avanza diez horas
void test_incrementar_hora_decena(void){   
    static const uint8_t ESPERADO[] = {2, 2, 3, 4, 0, 0};
    SIMULATE_SECONDS(60*60*10, ClockTick(reloj));
    ClockGetTime(reloj, hora, sizeof(hora));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO,hora, sizeof(ESPERADO));
}

//! Test para verificar que: Después de n ciclos de reloj la hora avanza un día completo
void test_incrementar_dia(void){   
    static const uint8_t ESPERADO[] = {1, 2, 3, 4, 0, 0};
    SIMULATE_SECONDS(60*60*24, ClockTick(reloj));
    ClockGetTime(reloj, hora, sizeof(hora));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO,hora, sizeof(ESPERADO));
}


//! Test para verificar que: Fijar la hora de la alarma y consultarla.
void test_ajustar_alarma(void){      
    static const uint8_t ESPERADO[] = {1, 2, 3, 5, 0, 0};
    uint8_t alarma[TIME_SIZE] = {0xFF};
    ClockSetAlarm(reloj, ESPERADO, 6);
    TEST_ASSERT_TRUE(ClockGetAlarm(reloj, alarma, sizeof(hora)));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO, alarma, sizeof(ESPERADO));
}

//! Test para verificar que: Fijar la alarma y avanzar el reloj para que suene. 
void test_alarma_activada(void){   
    SIMULATE_SECONDS(3*60, ClockTick(reloj)); //la hago pasar un minuto de mas por si acaso
    TEST_ASSERT_TRUE(evento_alarma_notificado);
}

//! Test para verificar que:  Fijar la alarma y avanzar el reloj para que no suene.
void test_alarma_Noactivada(void){   
    SIMULATE_SECONDS(1*60, ClockTick(reloj));
    TEST_ASSERT_FALSE(evento_alarma_notificado);
}

//! Test para verificar que: Fijar la alarma, deshabilitarla y avanzar el reloj para no suene. 
void test_alarma_deshabilitada(void){   
    ClockAlarmToggle(reloj);
    SIMULATE_SECONDS(2*60, ClockTick(reloj)); //la hago pasar un minuto de mas por si acaso
    TEST_ASSERT_FALSE(evento_alarma_notificado);
}

//! Test para verificar que: Hacer sonar la alarma y posponerla. Chequear si alarma no se modifica para el prox. dia
void test_alarma_activada_y_posponerla(void){ 
    uint8_t tiempo_posponer = 30; //minutos
    SIMULATE_SECONDS(2*60, ClockTick(reloj)); 
    TEST_ASSERT_TRUE(evento_alarma_notificado);

    ClockPostponeAlarm(reloj, tiempo_posponer);
    funcion_alarma_nonotificada(reloj);
    TEST_ASSERT_FALSE(evento_alarma_notificado);

    SIMULATE_SECONDS(30*60, ClockTick(reloj));
    TEST_ASSERT_TRUE(evento_alarma_notificado);

    ClockStopAlarm(reloj);  //dudo si debe ser clock o no
    funcion_alarma_nonotificada(reloj);
    TEST_ASSERT_FALSE(evento_alarma_notificado);

    SIMULATE_SECONDS((24*60*60), ClockTick(reloj)); 
    TEST_ASSERT_TRUE(evento_alarma_notificado);
}

//! Test para verificar que: Hacer sonar la alarma y cancelarla hasta el otro dia..
void test_alarma_activada_para_prox_dia(void){ 
    SIMULATE_SECONDS(2*60, ClockTick(reloj)); 
    TEST_ASSERT_TRUE(evento_alarma_notificado);

    SIMULATE_SECONDS(1*60, ClockTick(reloj)); 
    funcion_alarma_nonotificada(reloj);
    TEST_ASSERT_FALSE(evento_alarma_notificado);


    SIMULATE_SECONDS((23*60*60)+(59*60), ClockTick(reloj));
    TEST_ASSERT_TRUE(evento_alarma_notificado);
}
