//============================================================================//
//          ACIONAMENTO DO MOTOR BRUSHLESS COM MODELAGEM SIX-STEP             //
//                                                                            //
// Acionamento do tipo Sensorless                                             //
// Utiliza Potenciômetro para controlar a velocidade                          //
// Utiliza Botão do Usuário para mudar o sentido e parar                      //
// Programa para placa NUCLEO F303RE com X-NUCLEO IHM 07M1                    // 
//============================================================================//


//============================= BIBLIOTECAS ==================================//
#include "mbed.h"       //Inclusão da Biblioteca Mbed


//=============================== PINAGEM ====================================//
PwmOut in1(PA_8);
PwmOut in2(PA_9);       //Pinos de entrada da placa IHM07M1
PwmOut in3(PA_10);
    
DigitalOut en1(PC_10);
DigitalOut en2(PC_11);   //Pinos de enable da placa IHM07M1
DigitalOut en3(PC_12);         

DigitalIn botao(PC_13); //Pino do User Button
AnalogIn pot(PB_1);     //Pino do potenciômetro da placa IHM07M1


//============================= CONSTANTES ===================================//
#define maxDelay 0.0200f    //Máximo delay entre a comutação das fases
#define minDelay 0.0010f    //Mínimo delay entre a comutação das fases


//============================= VARIÁVEIS ====================================//
//Matriz que contém a sequência para os pinos de enable
//                  U,V,W
int enSeq[7][3] = {{1,0,1},{0,1,1},{1,1,0},{1,0,1},{0,1,1},{1,1,0},{0,0,0}};
//Matriz que contém a sequência para os pinos de entrada
//                  U,V,W
int inSeq[7][3] = {{1,0,0},{0,1,0},{0,1,0},{0,0,1},{0,0,1},{1,0,0},{0,0,0}};

float delay = 0;    //Variável usada para calcular o delay aplicado
int i = 0;          //Índice para contagem da sequência de acionamento
int sentido = 0;    //Variável usada para controlar o sentido e a parada
bool press = 0;     //Flag que monitora o botão


//========================= PROGRAMA PRINCIPAL ===============================//
int main(){
    en1.write(0);
    en2.write(0);       //Coloca os pinos de enable em Alta Impedância
    en3.write(0);
    
    in1.period_ms(1);
    in2.period_ms(1);   //Configura o período do PWM
    in3.period_ms(1);
    
    in1.write(0);
    in2.write(0);       //Configura o duty-cycle para 0%
    in3.write(0);


//============================ LOOP INFINITO =================================//
    while(1){
        
        in1.write(inSeq[i][0]);     //Seta as entradas conforme a combinação da
        in2.write(inSeq[i][1]);     //matriz, selecionando as linhas pelo índice
        in3.write(inSeq[i][2]);     //e mantendo as colunas fixas
        
        en1.write(enSeq[i][0]);     //Seta os enables conforme a combinação da
        en2.write(enSeq[i][1]);     //matriz, selecionando as linhas pelo índice
        en3.write(enSeq[i][2]);     //e mantendo as colunas fixas
        
        switch(sentido){
            case 0: i = 6; break;                   //Pára o motor
            case 1: i++; if(i > 5){i = 0;} break;   //Incrementa o índice
            case 2: i = 6; break;                   //Pára o motor
            case 3: i--; if(i < 0){i = 5;} break;   //Decrementa o índice
            default: break;
        }
        
        if(botao == 0 && press == 0){press = 1;}    //Botão presionado
        if(press == 1 && botao == 1){               //Botão solto
            sentido++;                              //Incrementa sentido
            if(sentido > 3){sentido = 0;}           //Zera sentido
            press = 0;                              //Zera a flag do botão
        }
        
        //Calcula delay com valores entre "minDelay" e "maxDelay"
        delay = (pot*(maxDelay - minDelay)) + minDelay;
        wait(delay);    //Delay conforme o resultado
    }
}

