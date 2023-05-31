


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
static uint8_t alarma[TIME_SIZE];
static bool evento_alarma_notificado;

void funcion_alarma_notificar(clock_t reloj){
    evento_alarma_notificado = true;
}

//public function implmenetation 

// la forma en como esta escrito permite que siempre siempre se inicie | suitsetup, suitardown
void setUp(void){   //codigo de inicializacion
    static const uint8_t INICIAL[] = {1, 2, 3, 4};
    static const uint8_t INICIAL_ALARMA[] = {1, 2, 3, 5};
    evento_alarma_notificado = false;
    reloj = ClockCreate(TICKS_PER_SECOND, funcion_alarma_notificar);
    ClockSetTime(reloj, INICIAL, sizeof(INICIAL));
    AlarmSetTime(reloj, INICIAL_ALARMA, sizeof(INICIAL_ALARMA));
}

// ‣ Al inicializar el reloj está en 00:00 y con hora invalida.
void test_reloj_arranca_con_hora_invalida(void){
    static const uint8_t ESPERADO[] = {0, 0, 0, 0, 0, 0};
    reloj = ClockCreate(TICKS_PER_SECOND, funcion_alarma_notificar); 
    uint8_t hora[TIME_SIZE] = {0xFF};
    TEST_ASSERT_FALSE(ClockGetTime(reloj,hora,sizeof(hora)));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO,hora,sizeof(ESPERADO));
}
// ‣ Al ajustar la hora el reloj queda en hora y es válida.

void test_ajustar_hora(void){      
    static const uint8_t ESPERADO[] = {1, 2, 3, 4, 0, 0};
    uint8_t hora[TIME_SIZE] = {0xFF};
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


// ‣ Fijar la hora de la alarma y consultarla.
void test_ajustar_alarma(void){      
    static const uint8_t ESPERADO[] = {1, 2, 3, 5, 0, 0};
    uint8_t alarma[TIME_SIZE] = {0xFF};
    TEST_ASSERT_TRUE(AlarmSetTime(reloj, ESPERADO, 6));
    TEST_ASSERT_TRUE(AlarmGetTime(reloj, alarma, sizeof(hora)));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO, alarma, sizeof(ESPERADO));
}

// ‣ Fijar la alarma y avanzar el reloj para que suene. 
void test_alarma_activada(void){   
    SIMULATE_SECONDS(2*60, ClockTick(reloj)); //la hago pasar un minuto de mas por si acaso
    ClockGetTime(reloj, hora, sizeof(hora));
    AlarmGetTime(reloj, alarma, sizeof(hora)); //la alarma es seteada un minuto despues de la hora
    TEST_ASSERT_TRUE(evento_alarma_notificado);
}

void test_alarma_Noactivada(void){   
    SIMULATE_SECONDS(0.5*60, ClockTick(reloj));
    ClockGetTime(reloj, hora, sizeof(hora));
    AlarmGetTime(reloj, alarma, sizeof(hora)); //la alarma es seteada un minuto despues de la hora
    TEST_ASSERT_FALSE(evento_alarma_notificado);
}

// ‣ Fijar la alarma, deshabilitarla y avanzar el reloj para no suene.
void test_alarma_deshabilitada(void){   
    SIMULATE_SECONDS(2*60, ClockTick(reloj)); //la hago pasar un minuto de mas por si acaso
    ClockGetTime(reloj, hora, sizeof(hora));
    AlarmGetTime(reloj, alarma, sizeof(hora)); //la alarma es seteada un minuto despues de la hora
    DesactivarAlarma(reloj);
    TEST_ASSERT_FALSE(isAlarmActive(reloj));
}

