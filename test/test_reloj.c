

// diez segundos, un minutos, diez minutos, una hora, diez 
// horas y un día completo.
// ‣ Fijar la hora de la alarma y consultarla.
// ‣ Fijar la alarma y avanzar el reloj para que suene. 
// ‣ Fijar la alarma, deshabilitarla y avanzar el reloj para no 
// suene.
// ‣ Hacer sonar la alarma y posponerla. 
// ‣ Hacer sonar la alarma y cancelarla hasta el otro dia..

#include "reloj.h"
#include "unity.h"

#define TICKS_PER_SECOND 5
#define TIME_SIZE 6

#define SIMULATE_SECONDS(VALUE, function)                           \
    for ( int count=0; count < VALUE; count++) {                    \
        for( int index = 0; index < TICKS_PER_SECOND ; index++){    \
            function;                                               \
        }                                                           \
    }                                                               \


static clock_t reloj;
static uint8_t hora[TIME_SIZE];

//public function implmenetation 

// la forma en como esta escrito permite que siempre siempre se inicie | suitsetup, suitardown
void setUp(void){   //codigo de inicializacion
    static const uint8_t INICIAL[] = {1, 2, 3, 4};
    reloj = ClockCreate(TICKS_PER_SECOND);
    ClockSetTime(reloj, INICIAL, sizeof(INICIAL));
}

// ‣ Al inicializar el reloj está en 00:00 y con hora invalida.
void test_reloj_arranca_con_hora_invalida(void){
    static const uint8_t ESPERADO[] = {0, 0, 0, 0, 0, 0};
    reloj = ClockCreate(TICKS_PER_SECOND); 
    uint8_t hora[TIME_SIZE] = {0xFF};
    TEST_ASSERT_FALSE(ClockGetTime(reloj,hora,sizeof(hora)));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO,hora,sizeof(ESPERADO));
}
// ‣ Al ajustar la hora el reloj queda en hora y es válida.

void test_ajustar_hora(void){      
    static const uint8_t ESPERADO[] = {1, 2, 3, 4, 0, 0};
    uint8_t hora[TIME_SIZE] = {0xFF};
    clock_t reloj = ClockCreate(TICKS_PER_SECOND);
    TEST_ASSERT_TRUE(ClockSetTime(reloj, ESPERADO, 6));
    TEST_ASSERT_TRUE(ClockGetTime(reloj,hora,sizeof(hora)));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO,hora,sizeof(ESPERADO));
}

// ‣ Después de n ciclos de reloj la hora avanza un segundo, 
// diez segundos, un minutos, diez minutos, una hora, diez 
// horas y un día completo.

void test_incrementar_segundos_unidad(void){   
    static const uint8_t ESPERADO[] = {1, 2, 3, 4, 0, 1};
    SIMULATE_SECONDS(1, ClockTick(reloj));
    ClockGetTime(reloj, hora, sizeof(hora));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO,hora, sizeof(ESPERADO));
}

void test_incrementar_segundos_decena(void){   
    static const uint8_t ESPERADO[] = {1, 2, 3, 4, 1, 0};
    SIMULATE_SECONDS(10, ClockTick(reloj));
    ClockGetTime(reloj, hora, sizeof(hora));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO,hora, sizeof(ESPERADO));
}

void test_incrementar_minutos_unidad(void){   
    static const uint8_t ESPERADO[] = {1, 2, 3, 5, 0, 0};
    SIMULATE_SECONDS(60, ClockTick(reloj));
    ClockGetTime(reloj, hora, sizeof(hora));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO,hora, sizeof(ESPERADO));
}

void test_incrementar_minutos_decena(void){   
    static const uint8_t ESPERADO[] = {1, 2, 4, 0, 0, 0};
    SIMULATE_SECONDS(60*6, ClockTick(reloj));
    ClockGetTime(reloj, hora, sizeof(hora));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO,hora, sizeof(ESPERADO));
}

void test_incrementar_hora_unidad(void){   
    static const uint8_t ESPERADO[] = {1, 3, 0, 0, 0, 0};
    SIMULATE_SECONDS(60*26, ClockTick(reloj));
    ClockGetTime(reloj, hora, sizeof(hora));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO,hora, sizeof(ESPERADO));
}

void test_incrementar_hora_decena(void){   
    static const uint8_t ESPERADO[] = {2, 2, 3, 4, 0, 0};
    SIMULATE_SECONDS(60*60*10, ClockTick(reloj));
    ClockGetTime(reloj, hora, sizeof(hora));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO,hora, sizeof(ESPERADO));
}

void test_incrementar_dia(void){   
    static const uint8_t ESPERADO[] = {1, 2, 3, 4, 0, 0};
    SIMULATE_SECONDS(60*60*24, ClockTick(reloj));
    ClockGetTime(reloj, hora, sizeof(hora));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO,hora, sizeof(ESPERADO));
}

