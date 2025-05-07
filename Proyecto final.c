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
FLOAT FLOAT_floatAFLOAT(float numero);
void FLOAT_igualarExponentes(FLOAT *flotante1, FLOAT *flotante2); 
void FLOAT_normalizar(FLOAT *flotante);
void FLOAT_normalizar64Bits(uint64_t bits, FLOAT *flotante);
void FLOAT_imprimir(FLOAT flotante);
void FLOAT_imprimirBits(FLOAT flotante);
int FLOAT_esInfinito(FLOAT flotante);
int FLOAT_esCero(FLOAT flotante);
int FLOAT_esIndefinido(FLOAT flotante);

//funciones aritmeticas
FLOAT FLOAT_suma(FLOAT flotante1, FLOAT flotante2);
FLOAT FLOAT_resta(FLOAT flotante1, FLOAT flotante2);
FLOAT FLOAT_multiplicacion(FLOAT flotante1, FLOAT flotante2);
FLOAT FLOAT_division(FLOAT flotante1, FLOAT flotante2);

float poder(float base, int exponente);
void imprimir8Bits(uint8_t bits);
void imprimir32Bits(uint32_t bits);
void imprimir64Bits(uint64_t bits);
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
                flotante1 = FLOAT_floatAFLOAT(tempFlotante1);
                flotante2 = FLOAT_floatAFLOAT(tempFlotante2);
                FLOAT_imprimir(flotante1);
                printf(" + ");
                FLOAT_imprimir(flotante2);
                printf(" = ");
                resultado = FLOAT_suma(flotante1, flotante2);
                FLOAT_imprimir(resultado);
                printf("\n");
                break;
            }

            case RESTA: {
                printf("Ingresa el primer numero: ");
                scanf("%f", &tempFlotante1);
                printf("Ingresa el segundo numero: ");
                scanf("%f", &tempFlotante2);
                flotante1 = FLOAT_floatAFLOAT(tempFlotante1);
                flotante2 = FLOAT_floatAFLOAT(tempFlotante2);
                FLOAT_imprimir(flotante1);
                printf(" - ");
                FLOAT_imprimir(flotante2);
                printf(" = ");
                resultado = FLOAT_resta(flotante1, flotante2);
                FLOAT_imprimir(resultado);
                printf("\n");
                break;
            }

            case MULTIPLICACION: {
                printf("Ingresa el primer numero: ");
                scanf("%f", &tempFlotante1);
                printf("Ingresa el segundo numero: ");
                scanf("%f", &tempFlotante2);
                flotante1 = FLOAT_floatAFLOAT(tempFlotante1);
                flotante2 = FLOAT_floatAFLOAT(tempFlotante2);
                FLOAT_imprimir(flotante1);
                printf(" * ");
                FLOAT_imprimir(flotante2);
                printf(" = ");
                resultado = FLOAT_multiplicacion(flotante1, flotante2);
                FLOAT_imprimir(resultado);
                printf("\n");
                break;
            }

            case DIVISION: {
                printf("Ingresa el primer numero: ");
                scanf("%f", &tempFlotante1);
                printf("Ingresa el segundo numero: ");
                scanf("%f", &tempFlotante2);
                flotante1 = FLOAT_floatAFLOAT(tempFlotante1);
                flotante2 = FLOAT_floatAFLOAT(tempFlotante2);
                FLOAT_imprimir(flotante1);
                printf("/");
                FLOAT_imprimir(flotante2);
                printf(" = ");
                resultado = FLOAT_division(flotante1, flotante2);
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
FLOAT FLOAT_floatAFLOAT(float numero){
    UNION_FLOAT tempUnion;
    FLOAT nuevoNumero;

    tempUnion.flotante = numero;
    //basicamente dentro del union estamos moviendo los bits directamente del float al FLOAT en la union
    nuevoNumero.signo = (tempUnion.temp >> 31) & 0x1; //los valores hex son para cortar lo extra que tienen, ya que el signo es solamente un bit
    nuevoNumero.exponente = (tempUnion.temp >> 23) & 0xFF; //2 bytes (8 bits)
    nuevoNumero.mantissa = tempUnion.temp & 0x7FFFFF; //23 bits

    return nuevoNumero;
}

void FLOAT_imprimir(FLOAT flotante){
    if(FLOAT_esCero(flotante)){
        printf("%f", 0.0f);
        return;
    } else if(FLOAT_esInfinito(flotante)){
        printf("Infinito");
        return;
    } else if(FLOAT_esIndefinido(flotante)){
        printf("Indefinido");
        return;
    }

    //necesitamos convertir el FLOAT a un float para que sea mas facil imprimirlo
    uint32_t bits = (flotante.signo << 31) | (flotante.exponente << 23) | flotante.mantissa & 0x7FFFFF;
    UNION_FLOAT nuevoFloat; nuevoFloat.temp = bits;
    float numero = nuevoFloat.flotante;

    printf("%f", numero);
}

//funcion para imprimir los bits del FLOAT (utilidad para debug)
void FLOAT_imprimirBits(FLOAT flotante){
    printf("%d ", flotante.signo & 1);
    
    for(int i = 7; i >= 0; i--){
        printf("%d", (flotante.exponente >> i) & 1);
        if (i % 4 == 0) printf(" "); //espacios entre 4 bits
    }
    for(int i = 22; i >= 0; i--){
        printf("%d", (flotante.mantissa >> i) & 1);
        if ((i+1) % 4 == 0) printf(" "); //espacios entre 4 bits
    }
}

void FLOAT_agregar1Implicito(FLOAT* flotante){
    //UL = 32 bits
    //ULL = 64 bits
    //si en vez de 1 << 23 fuera 1 << 32 o mas, se tendria que poner ULL para guardar mas que 32 bits en esa comparacion
    flotante->mantissa = flotante->mantissa | (1UL << 23);
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
    FLOAT_agregar1Implicito(flotante1);
    FLOAT_agregar1Implicito(flotante2);

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
    while(flotante->mantissa >= (1UL << 24)){
        flotante->mantissa = flotante->mantissa >> 1;
        flotante->exponente = flotante->exponente + 1;
    }

    while(flotante->mantissa < (1UL << 23)){
        flotante->mantissa = flotante->mantissa << 1;
        flotante->exponente = flotante->exponente - 1;
    }
}

void FLOAT_normalizar64Bits(uint64_t bits, FLOAT* flotante){
    if(bits > (1ULL << 47)){
        //en este caso tuvimos una multiplicacion que movio al decimal
        //ejemplo: 1.1 * 1.1 = 10.01
        //checamos si los bits van mas de 47, porque al multiplicar dos registros de 24 nos da normalmente 47
        //pero hay veces que por agregar un decimal (por ejemplo 99 -> 100), se tiene que mover el exponente un posicion
        flotante->exponente = flotante->exponente + 1;
    }
    
    //reducir los bits al tamanio 23
    while(bits >= (1ULL << 24)){
        bits = bits >> 1;
    }        

    //grabar los bits del posicion 23 -> 0 y asi eliminamos el 1 implicito que agregamos antes
    flotante->mantissa = bits & 0x7FFFFF;
}

int FLOAT_esInfinito(FLOAT flotante){
    //0xFF = 1111 1111
    return flotante.mantissa == 0x0 && flotante.exponente == 0xFF;
}

int FLOAT_esCero(FLOAT flotante){
    return flotante.exponente == 0x0 && flotante.mantissa == 0x0;
}

int FLOAT_esIndefinido(FLOAT flotante){
    return flotante.exponente == 0xFF && flotante.mantissa >= 1;
}

FLOAT FLOAT_suma(FLOAT flotante1, FLOAT flotante2){
    FLOAT_igualarExponentes(&flotante1, &flotante2);

    FLOAT resultado;
    if(flotante1.signo != flotante2.signo){
        if(flotante1.mantissa > flotante2.mantissa){
            resultado.mantissa = flotante1.mantissa - flotante2.mantissa;
            resultado.signo = flotante1.signo;
        } else {
            resultado.mantissa = flotante2.mantissa - flotante1.mantissa;
            resultado.signo = flotante2.signo;
        }
    } else {
        resultado.mantissa = flotante1.mantissa + flotante2.mantissa;
        resultado.signo = flotante1.signo;
    }
    resultado.exponente = flotante1.exponente;
    FLOAT_normalizar(&resultado);
    
    return resultado;
}

FLOAT FLOAT_resta(FLOAT flotante1, FLOAT flotante2){
    FLOAT_igualarExponentes(&flotante1, &flotante2);
    FLOAT resultado;
    
    if(flotante2.signo == flotante1.signo){
        if(flotante1.mantissa == flotante2.mantissa){
            resultado.exponente = 0;
            resultado.mantissa = 0;
            resultado.signo = 0;
            return resultado;
        }
        resultado.mantissa = flotante1.mantissa - flotante2.mantissa;
        resultado.signo = flotante1.signo;
    } else {
        if (flotante1.mantissa > flotante2.mantissa) {
            resultado.mantissa = flotante1.mantissa + flotante2.mantissa;
            resultado.signo = flotante1.signo;
        } else if(flotante1.mantissa < flotante2.mantissa) {
            resultado.mantissa = flotante2.mantissa + flotante1.mantissa;
            resultado.signo = flotante2.signo;
        } else {
            resultado.mantissa = flotante1.mantissa + flotante2.mantissa;
            resultado.signo = flotante1.signo;
        }
    }
    
    resultado.exponente = flotante1.exponente;

    FLOAT_normalizar(&resultado);

    return resultado;
}

FLOAT FLOAT_multiplicacion(FLOAT flotante1, FLOAT flotante2){
    FLOAT_agregar1Implicito(&flotante1);
    FLOAT_agregar1Implicito(&flotante2);

    FLOAT resultado;
    uint64_t auxMantissa;

    auxMantissa = (uint64_t)flotante1.mantissa * (uint64_t)flotante2.mantissa;
    resultado.exponente = flotante1.exponente + flotante2.exponente - 127;
    resultado.signo = flotante1.signo ^ flotante2.signo;

    FLOAT_normalizar64Bits(auxMantissa, &resultado);

    return resultado;
}

FLOAT FLOAT_division(FLOAT flotante1, FLOAT flotante2){
    FLOAT resultado;

    if(FLOAT_esCero(flotante2)){
        resultado.exponente = 0;
        resultado.mantissa = 1;
        return resultado;
    } else if(FLOAT_esInfinito(flotante1) || FLOAT_esInfinito(flotante2)){
        resultado.mantissa = 0;
        resultado.exponente = 0xFF;
        return resultado;
    }

    FLOAT_agregar1Implicito(&flotante1);
    FLOAT_agregar1Implicito(&flotante2);

    uint64_t dividendo = (uint64_t)flotante1.mantissa << 23; //para mas precision

    resultado.mantissa = dividendo / flotante2.mantissa;
    resultado.exponente = flotante1.exponente - flotante2.exponente + 127;
    resultado.signo = flotante1.signo ^ flotante2.signo;

    FLOAT_normalizar(&resultado);

    return resultado;
}

float poder(float base, int exponente){
    float resultado=base;

    for(int i=1; i <= exponente; i++){
        resultado *= base;
    }

    return resultado;
}

void imprimir8Bits(uint8_t bits){
    for(int i = 7; i >= 0; i--){
        printf("%d", (bits >> i) & 1);
        if(i % 4 == 0) printf(" ");
    }
}

void imprimir32Bits(uint32_t bits){
    for(int i = 31; i >= 0; i--){
        printf("%d", (bits >> i) & 1);
        if (i % 4 == 0) printf(" "); //espacios entre 4 bits
    }
}

void imprimir64Bits(uint64_t bits){
    for(int i = 63; i >= 0; i--){
        printf("%d", (bits >> i) & 1);
        if (i % 4 == 0) printf(" "); //espacios entre 4 bits
    }
}

void enterParaContinuar(){
    while((getchar()) != '\n');
    printf("Presione enter para continuar...");
    getchar();
}