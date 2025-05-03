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
void FLOAT_imprimirFlotante(FLOAT flotante);
void FLOAT_igualarExponentes(FLOAT *flotante1, FLOAT *flotante2); 

//funciones aritmeticas
void FLOAT_suma(FLOAT floatante1, FLOAT floatante2);
void FLOAT_resta(FLOAT floatante1, FLOAT floatante2);
void FLOAT_multiplicacion(FLOAT floatante1, FLOAT floatante2);
void FLOAT_division(FLOAT floatante1, FLOAT floatante2);

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
        switch(opcion){
            case SUMA: {
                printf("Ingresa el primer numero: ");
                scanf("%f", &tempFlotante1);
                printf("Ingresa el segundo numero: ");
                scanf("%f", &tempFlotante2);
                flotante1 = FLOAT_convertirAFlotante(tempFlotante1);
                flotante2 = FLOAT_convertirAFlotante(tempFlotante2);
                FLOAT_imprimirFlotante(flotante1);
                printf(" + ");
                FLOAT_imprimirFlotante(flotante2);
                printf("\n");
                break;
            }

            case SALIR: {
                salir = 1;
                break;
            }
        }
    } while(!salir);
}

FLOAT FLOAT_convertirAFlotante(float numero){
    UNION_FLOAT tempUnion;
    FLOAT nuevoNumero;
    tempUnion.flotante = numero;
    nuevoNumero.signo = (tempUnion.temp >> 31) & 0x1;
    nuevoNumero.exponente = (tempUnion.temp >> 23) & 0xFF;
    nuevoNumero.mantissa = tempUnion.temp & 0x7FFFFF;

    return nuevoNumero;
}

int FLOAT_valorExponente(FLOAT flotante){
    //sabemos el signo del exponente en una manera implicita, de aceurdo del estandar IEEE 754:
    //255 (128): reservado para reprenstar NaN o infinito
    //254 -> 1 (127 -> -126): valores para representar el exponente (2^n)
    //0 (-127): para numeros anormales donde no se sabe el valor
    //entonces tenemos el rango funcional de n: [-126, 127] para exponentes, donde 2^n
    return flotante.exponente - 127;
}

void FLOAT_imprimirFlotante(FLOAT flotante){
    int exponente = FLOAT_valorExponente(flotante);
    if(exponente == -127) {
        printf("Numero anormal");
        return;
    }

    if(exponente == 128){
        printf("Numero infinito");
        return;
    }

    printf("%s", flotante.signo ? "+" : "-");
    printf("0x%06X", flotante.mantissa);
    printf(" * 2^%d", exponente);
}

//esta funcion iguala los valores en los exponentes de los floats, para luego poder hacer operaciones aritmeticas
//los punteros es para editar directamente los variables, pero las variables originales no deberian ser punteros 
//y deberian ser pasados con el operador &
void FLOAT_igualarExponentes(FLOAT *flotante1, FLOAT *flotante2){
    if(flotante1->exponente < flotante2->exponente){
        flotante2->exponente = flotante1->exponente;
    } else {
        flotante1->exponente = flotante2->exponente;
    }
}

void FLOAT_suma(FLOAT floatante1, FLOAT floatante2){

}