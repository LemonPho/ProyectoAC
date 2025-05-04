#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SUMA 1
#define RESTA 2
#define MULTIPLICACION 3
#define DIVISION 4
#define SALIR 5

//definicion del flotante personalizado
typedef struct{
    uint8_t signo; //el signo (1 bit)
    uint8_t exponente; //exponente de 8 bits (2^n)
    uint32_t mantissa; //el valor del numero de 23 bits pero guardadas en un valor de 32 bits
} FLOAT;


//union y funciones para manejar los FLOAT
typedef union{
    float flotante;
    uint32_t temp;
} UNION_FLOAT;
FLOAT FLOAT_convertirAFlotante(float numero);
int FLOAT_valorExponente(FLOAT flotante);
void FLOAT_igualarExponentes(FLOAT *flotante1, FLOAT *flotante2); 
void FLOAT_normalizar(FLOAT *flotante);
void FLOAT_imprimir(FLOAT flotante);
void FLOAT_imprimirBits(FLOAT flotante);

//funciones aritmeticas
FLOAT FLOAT_suma(FLOAT flotante1, FLOAT flotante2);
FLOAT FLOAT_resta(FLOAT flotante1, FLOAT flotante2);
FLOAT FLOAT_multiplicacion(FLOAT flotante1, FLOAT flotante2);
FLOAT FLOAT_division(FLOAT flotante1, FLOAT flotante2);

float poder(float base, int exponente);
void imprimir8Bits(uint8_t valor);
void imprimir32Bits(uint32_t valor);
void enterParaContinuar();

int main(){
    FLOAT flotante1, flotante2;
    int salir=0;
    int opcion;
    do{
        system("cls");
        printf("1. Suma\n");
        printf("2. Resta\n");
        printf("3. Multiplicacion\n");
        printf("4. Division\n");
        printf("5. Salir\n");
        printf("Opcion: ");
        scanf("%d", &opcion);

        float tempFlotante1, tempFlotante2;
        FLOAT resultado;

        switch(opcion){
            case SUMA: {
                printf("Ingresa el primer numero: ");
                scanf("%f", &tempFlotante1);
                printf("Ingresa el segundo numero: ");
                scanf("%f", &tempFlotante2);
                flotante1 = FLOAT_convertirAFlotante(tempFlotante1);
                flotante2 = FLOAT_convertirAFlotante(tempFlotante2);
                FLOAT_imprimir(flotante1);
                printf(" + ");
                FLOAT_imprimir(flotante2);
                printf(" = ");
                resultado = FLOAT_suma(flotante1, flotante2);
                FLOAT_imprimir(resultado);
                printf("\n");
                break;
            }

            case SALIR: {
                salir = 1;
                break;
            }

            default: {
                printf("Opcion invalida\n");
                break;
            }
        }

        enterParaContinuar();
    } while(!salir);
}

//funcion para convertir del tipo de dato float a nuestro FLOAT
FLOAT FLOAT_convertirAFlotante(float numero){
    UNION_FLOAT tempUnion;
    FLOAT nuevoNumero;

    tempUnion.flotante = numero;
    //basicamente dentro del union estamos moviendo los bits directamente del float al FLOAT en la union
    nuevoNumero.signo = (tempUnion.temp >> 31) & 0x1; //los valores hex son para cortar lo extra que tienen, ya que el signo es solamente un bit
    nuevoNumero.exponente = (tempUnion.temp >> 23) & 0xFF; //2 bytes (8 bits)
    nuevoNumero.mantissa = tempUnion.temp & 0x7FFFFF; //23 bits

    return nuevoNumero;
}


//sabemos el signo del exponente en una manera implicita, de aceurdo del estandar IEEE 754:
//255 (128): reservado para reprenstar NaN o infinito
//254 -> 1 (127 -> -126): valores para representar el exponente (2^n)
//0 (-127): para numeros anormales donde no se sabe el valor
//entonces tenemos el rango funcional de n: [-126, 127] para exponentes, donde 2^n
int FLOAT_valorExponente(FLOAT flotante){
    return flotante.exponente - 127;
}

void FLOAT_imprimir(FLOAT flotante){
    int exponente = FLOAT_valorExponente(flotante);
    if(exponente == -127) {
        printf("Numero anormal");
        return;
    }

    if(exponente == 128){
        printf("Numero infinito");
        return;
    }

    //necesitamos convertir el FLOAT a un float para que sea mas facil imprimirlo
    uint32_t bits = ((uint32_t)flotante.signo << 31) | ((uint32_t)flotante.exponente << 23) | flotante.mantissa & 0x7FFFFF;
    UNION_FLOAT nuevoFloat; nuevoFloat.temp = bits;
    float numero = nuevoFloat.flotante;

    printf("%f", numero);
}

//funcion para imprimir los bits del FLOAT (utilidad para debug)
void FLOAT_imprimirBits(FLOAT flotante){
    printf("%d ", flotante.signo & 1);
    
    for(int i = 7; i >= 0; i--){
        printf("%d", (flotante.exponente >> i) & 1);
        if (i % 4 == 0) printf(" ");
    }
    for(int i = 22; i >= 0; i--){
        printf("%d", (flotante.mantissa >> i) & 1);
        if ((i+1) % 4 == 0) printf(" "); //espacios entre 4 bits
    }
}

//esta funcion iguala los valores en los exponentes de los floats, para luego poder hacer operaciones aritmeticas
//los punteros es para editar directamente los variables, pero las variables originales no deberian ser punteros 
//y deberian ser pasados con el operador &
//esta funcion deberia ser trabajada solo con copias de los flotantes originales, no queremos modificar los originales porque
//pueden perder precision por el movimiento de exponentes
//lo mejor seria ponerlos unicamente en las funciones aritmeticas, ya que ya reciben copias de las variables
void FLOAT_igualarExponentes(FLOAT *flotante1, FLOAT *flotante2){
    int diferencia;

    //agregando los 1's implicitos del estandar ieee 754
    flotante1->mantissa = flotante1->mantissa | (1 << 23);
    flotante2->mantissa = flotante2->mantissa | (1 << 23);

    if(flotante1->exponente < flotante2->exponente){
        
        diferencia = flotante2->exponente - flotante1->exponente;
        flotante1->exponente = flotante2->exponente;
        flotante1->mantissa = flotante1->mantissa >> diferencia;
    } else if(flotante2->exponente < flotante1->exponente) {
        diferencia = flotante1->exponente - flotante2->exponente;
        flotante2->exponente = flotante1->exponente;
        flotante2->mantissa = flotante2->mantissa >> diferencia;
    }
}


void FLOAT_normalizar(FLOAT *flotante){
    while(flotante->mantissa >= (1 << 24)){
        flotante->mantissa = flotante->mantissa >> 1;
        flotante->exponente = flotante->exponente + 1;
    }
}


FLOAT FLOAT_suma(FLOAT flotante1, FLOAT flotante2){
    FLOAT_igualarExponentes(&flotante1, &flotante2);

    FLOAT resultado;
    resultado.mantissa = flotante1.mantissa + flotante2.mantissa;
    resultado.exponente = flotante1.exponente;
    resultado.signo = flotante1.signo;
    FLOAT_normalizar(&resultado);
    
    return resultado;
}

FLOAT FLOAT_resta(FLOAT flotante1, FLOAT flotante2){
    FLOAT_igualarExponentes(&flotante1, &flotante2);
    //TODO
}

FLOAT FLOAT_multiplicacion(FLOAT flotante1, FLOAT flotante2){
    FLOAT_igualarExponentes(&flotante1, &flotante2);
    //TODO
}

FLOAT FLOAT_division(FLOAT flotante1, FLOAT flotante2){
    FLOAT_igualarExponentes(&flotante1, &flotante2);
    //TODO
}

float poder(float base, int exponente){
    float resultado=base;

    for(int i=1; i <= exponente; i++){
        resultado *= base;
    }

    return resultado;
}

void imprimir8Bits(uint8_t valor){
    for(int i = 7; i >= 0; i--){
        printf("%d", (valor >> i) & 1);
        if(i % 4 == 0) printf(" ");
    }
}

void imprimir32Bits(uint32_t valor){
    for(int i = 31; i >= 0; i--){
        printf("%d", (valor >> i) & 1);
        if (i % 4 == 0) printf(" "); //espacios entre 4 bits
    }
}

void enterParaContinuar(){
    while((getchar()) != '\n');
    printf("Presione enter para continuar...");
    getchar();
}