   
#include "C:\Users\ellio\OneDrive\Documentos\Intrgracion 3\Integracion-3---TIDE--UCT-2018\Robot CuiCui\CuiCui_Ver_Final.h"

#byte T1CON = 0x0FCD
#byte PIR2 = 0x0FA1
#bit  TMR3IF =PIR2.1
#define  MUSIC_NOTES TRUE
#include <math.h>
#include <tones_c5.c> 
//#define  MUSIC_NOTES TRUE
#use fast_io(A)
#use fast_io(B)
#use fast_io(C)

//Se agrega el caracter nulo al final de una asignacion de tipo string

const char Version[]="1.0\0";    
const int total_bits=10;
const int total_bytes=2;
const int total_datos=25;
const int UP=2;
const int SOUND=3;
const int LEFT=4;
const int GO=5;
const int RIGHT=6;
//const int PAUSE=7;
const int DOWN=8;
const int RESET=9;
const int16 mov_recto=300, mov_giro=200;

const int vel1=10;
const int vel2=15;
const int vel3=20;



int  mov_ant=0, mov_act=0;
int  datos[total_datos]="\0";

int8 valorR;

int16 q, level_ir_b, level_source_b, distancia_0, distancia_1, diferencia_T0, diferencia_T1, distancia_recorrida_1=0, distancia_recorrida_2=0; 
int16 control_M1, control_M2, valor_pwm=0, vel_pwm=0;
int16 T0_Act=0, T1_Act=0, T0_Ant=0, T1_Ant=0, Delta_T0=0, Delta_T1=0, Delta_T0_ant=0; 
int16 counter_sleep=0;

float p, level_ir, level_source;
const float a=72.0, b=0.6, c=0.15, a2=10.0, b2=0.2, c2=0.015;  //b=0
float rV, eV, iV, dV, yV, uV, iV0, eV0, uV2;
float eD, iD, dD, iD0, eD0, uD;
const float max=1000.0, min=0.0;
float velocidad_ref=0, delta_recorrido=0, velocidad_T0=0;


long times_cr[total_bits]="\0";
char sbits[total_bits];
int1 first_ext=0;
int1 start_recived=0;
int i,j,k;
long tt,tti,t;
int ntbits=0;
int1 hay_dato=0;
int contador=0;
int bits[8]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
int bytes[total_bytes]={0x00,0x00};
int identificador=0x01;
int dato_ir=0x00;
int1 wake_up=0, out_sleep=0;
int1 temp;
int1 control_tipo;
//void limpia_bits(void);

void limpia_bits(void){

  for(i=0;i<total_bits;++i){
    sbits[i]='\0';
  }
  ntbits=0;
}
// INTERRUPCION POR TIMER 3
#int_timer3
void timer3_isr(){

set_timer3(34285);

T0_Act=get_timer0();

T1_Act=get_timer1();

Delta_T0 = T0_Act - T0_Ant;
Delta_T1 = T1_Act - T1_Ant;
velocidad_T0=((float)(Delta_T0)+(float)(Delta_T0_ant))/2;

Delta_T0_ant=Delta_T0;
T0_Ant=T0_Act;
T1_Ant=T1_Act;
distancia_recorrida_1 = distancia_recorrida_1 + Delta_T0;
distancia_recorrida_2 = distancia_recorrida_2 + Delta_T1;
//printf("%2.2f,%3.1f\r",velocidad_T0, eD);
//printf("\r\nCorriente = %1.3fA\r\n", corriente_M1);
//printf("%Lu,%2.2f\r", Delta_T0, velocidad_T0);
//printf("%Lu,%Lu,%Lu,%Lu\r", Delta_T0, Delta_T1, distancia_recorrida_1, distancia_recorrida_2);

      
      yV=Delta_T0;
     
     
     //rV=Velocidad_ref;
     rV=30.0;
     eV=rV - yV;
     iV=b*eV + iV0;
     dV=c*(eV-eV0);
     uV=iV + a*eV + dV;
     
     if (uV>max){
         uV=max;}
     else {
         if (uV<min){
         uV=min;}
          }
     control_M1=uV;
     
     eD=(float)(get_timer0()) - (float)(get_timer1());
     iD=b2*eD + iD0;
     dD=c2*(eD-eD0);
     uD=iD + a2*eD + dD;
     
     uV2 = uV + uD;
         
     if (uV2>max){
         uV2=max;}
     else {
         if (uV2<min){
         uV2=min;}
          }
     control_M2 = uV2; 
     set_pwm1_duty(control_M1); //control_M1
     set_pwm2_duty(control_M2);//control_M2
     iV0=iV;
     eV0=eV;
     iD0=iD;
     eD0=eD;
     //printf("\r\n M1= %Lu  M2= %Lu \r\n", control_M1, control_M2);
     
    
}


// INTERRUPCION por RECEPCION SERIE -------------------------------------------

#int_rda
void serial_isr() {
  if(kbhit()){
    valor_pwm=4*getc();
  }
}
// INTERRUPCION PARA SALIR DEL MODO SLEEP POR MEDIO DEL COMPARADOR------------

#int_comp
void comp_isr(){
   
   counter_sleep=0;
   wake_up=1;
   temp=C1OUT;
   temp=C2OUT;
}


// INTERRUPCION EXT por RB0 --------------------------------------------------

#int_ext
void ext_isr() {

  // Obtengo datos de Timer0
  
  tt = get_timer0();
  t = tt-tti;
  tti= tt;

  if(start_recived==1){
    // Bit "0"
    if(t>58 && t<68){ sbits[ntbits]='0'; 
    times_cr[ntbits]=t;
    }
    // Bit "1"
    if(t>90 && t<100){ sbits[ntbits]='1'; 
    times_cr[ntbits]=t;
    }
    // Si he recibido 10 bits entonces hay dato
    if(++ntbits==total_bits){
      hay_dato=1;
    }
  }

  // Detecto Start Bit
  if(t>153 && t<160){
    start_recived=1;
    limpia_bits();
  }
}

//-----------------------------------------------------------------------------

void calculo_del_error(void){
      if ( mov_act == UP ){
         switch (mov_ant){
            
            case 0:
            set_timer0(0);
            set_timer1(0);
            distancia_0 = mov_recto;
            distancia_1 = mov_recto;
            T0_Ant=0;
            T0_Act=0;
            T1_Ant=0;
            T1_Act=0;
            Delta_T0=0;
            Delta_T1=0;
            break;
     
            case UP:
            set_timer0(diferencia_T0);
            set_timer1(diferencia_T1);
            distancia_0 = mov_recto;
            distancia_1 = mov_recto; 
            T0_Ant=diferencia_T0;
            T0_Act=diferencia_T0;
            T1_Ant=diferencia_T1;
            T1_Act=diferencia_T1;
            Delta_T0=0;
            Delta_T1=0;
            break;
            
            case DOWN:
            set_timer0(diferencia_T1);
            set_timer1(diferencia_T0);
            distancia_0 = mov_recto + diferencia_T1 + diferencia_T0 ;
            distancia_1 = mov_recto + diferencia_T0 + diferencia_T1; 
            T0_Ant=diferencia_T1;
            T0_Act=diferencia_T1;
            T1_Ant=diferencia_T0;
            T1_Act=diferencia_T0;
            Delta_T0=0;
            Delta_T1=0;         
            break;
            
            case LEFT:
            break;
            
            case RIGHT:
            break;
            }
      }
      if ( mov_act == DOWN ){
         switch (mov_ant){
         
            case 0:
            set_timer0(0);
            set_timer1(0);
            distancia_0 = mov_recto;
            distancia_1 = mov_recto;
            T0_Ant=0;
            T0_Act=0;
            T1_Ant=0;
            T1_Act=0;
            Delta_T0=0;
            Delta_T1=0;
            break;
         
            case UP:
            set_timer0(diferencia_T1);
            set_timer1(diferencia_T0);
            distancia_0 = mov_recto + diferencia_T1 + diferencia_T0 ;
            distancia_1 = mov_recto + diferencia_T0 + diferencia_T1; 
            T0_Ant=diferencia_T1;
            T0_Act=diferencia_T1;
            T1_Ant=diferencia_T0;
            T1_Act=diferencia_T0;
            Delta_T0=0;
            Delta_T1=0;            
            break;
         
            case DOWN:
            set_timer0(diferencia_T0);
            set_timer1(diferencia_T1);
            distancia_0 = mov_recto;
            distancia_1 = mov_recto;
            T0_Ant=diferencia_T0;
            T0_Act=diferencia_T0;
            T1_Ant=diferencia_T1;
            T1_Act=diferencia_T1;
            Delta_T0=0;
            Delta_T1=0;
            break;
            
            case LEFT:
            break;
         
            case RIGHT:
            break;
            }
      }
      if ( mov_act == LEFT ){
         switch (mov_ant){
            
            case 0:
            set_timer0(0);
            set_timer1(0);
            distancia_0 = mov_giro;
            distancia_1 = mov_giro;
            T0_Ant=0;
            T0_Act=0;
            T1_Ant=0;
            T1_Act=0;
            Delta_T0=0;
            Delta_T1=0;
            break;
         
            case UP:
            break;
         
            case DOWN:
            break;
         
            case LEFT:
            set_timer0(diferencia_T0);
            set_timer1(diferencia_T1);
            distancia_0 = mov_giro;
            distancia_1 = mov_giro;
            T0_Ant=diferencia_T0;
            T0_Act=diferencia_T0;
            T1_Ant=diferencia_T1;
            T1_Act=diferencia_T1;
            Delta_T0=0;
            Delta_T1=0;
            break;
            
            case RIGHT:
            set_timer0(0);
            set_timer1(0);
            distancia_0 = mov_giro + diferencia_T0;
            distancia_1 = mov_giro + diferencia_T1;
            T0_Ant=0;
            T0_Act=0;
            T1_Ant=0;
            T1_Act=0;
            Delta_T0=0;
            Delta_T1=0;
            break;
            }
      }
      if ( mov_act == RIGHT ){
         switch (mov_ant){
         
            case 0:
            set_timer0(0);
            set_timer1(0);
            distancia_0 = mov_giro;
            distancia_1 = mov_giro;
            T0_Ant=0;
            T0_Act=0;
            T1_Ant=0;
            T1_Act=0;
            Delta_T0=0;
            Delta_T1=0;
            break;
         
            case UP:
            break;
         
            case DOWN:
            break;
         
            case LEFT:
            set_timer0(0);
            set_timer1(0);
            distancia_0 = mov_giro + diferencia_T0;
            distancia_1 = mov_giro + diferencia_T1;
            T0_Ant=0;
            T0_Act=0;
            T1_Ant=0;
            T1_Act=0;
            Delta_T0=0;
            Delta_T1=0;
            break;
         
            case RIGHT:
            set_timer0(diferencia_T0);
            set_timer1(diferencia_T1);
            distancia_0 = mov_giro;
            distancia_1 = mov_giro;
            T0_Ant=diferencia_T0;
            T0_Act=diferencia_T0;
            T1_Ant=diferencia_T1;
            T1_Act=diferencia_T1;
            Delta_T0=0;
            Delta_T1=0;
            break;
            }
         } 
 }

void melodia_inicio(void){
  
  delay_ms(187);
  generate_tone(391, 151);
  delay_ms(35);
  generate_tone(391, 187);
  delay_ms(187);
  generate_tone(391, 187);
  generate_tone(440, 187);
  delay_ms(187);
  generate_tone(466, 187);
  delay_ms(187);
  generate_tone(493, 187);
  delay_ms(562);
  generate_tone(987, 187);
}

void melodia_sleep(void){
    
  delay_ms(187);
  generate_tone(466, 187);
  delay_ms(187);
  generate_tone(493, 187);
  delay_ms(562);
  generate_tone(987, 187);
}

Void melodia_wake_up(void){
     delay_ms(187);
     generate_tone(391, 151);
     delay_ms(35);
     generate_tone(391, 187);
     delay_ms(187);
     generate_tone(391, 187);
     generate_tone(440, 187);
     delay_ms(187);
     setup_vref(VREF_HIGH | VREF_F5 | 8);
}

void melodia_baja_bateria(void){
     generate_tone(391, 187);
     delay_ms(187);
     generate_tone(391, 187);
     generate_tone(440, 187);
     delay_ms(187);
}


Void verificar_level_ir(void){

     set_adc_channel(0);
     delay_us(20);
     level_ir_b = read_adc();
     level_ir = (3.3 * level_ir_b / 1024.0);
     if ( level_ir >= 2.8)
         vel_pwm = vel1;
     if ( level_ir >= 2.3 && level_ir <2.8)
         vel_pwm = vel2;
     if ( level_ir < 2.3)
         vel_pwm = vel3;
}
Void level_source_ref(void){

     set_adc_channel(3);
     delay_us(20);
     level_source_b = read_adc();
     level_source = (3.3 * level_source_b / 1024.0)*2;
     if (level_source <=4.5){
        melodia_baja_bateria();
        melodia_baja_bateria();
     }
     //printf("\r\nVoltage = %01.2fV\r\n", level_source);
}
void ejecucion_pwm(void){



     setup_timer_2(t2_div_by_4, 255, 1);  //PR2=255, Tpwm=204,8 us F=4,8kHz
     setup_timer_0(T0_EXT_L_TO_H | T0_DIV_1);
     setup_timer_1(T1_EXTERNAL_SYNC | T1_DIV_BY_1);
     setup_timer_3(T3_INTERNAL | T3_DIV_BY_8);
     //calculo_del_error();
     set_timer0(0);
     set_timer1(0);
     set_timer3(34285);
     enable_interrupts(int_timer3);
     disable_interrupts(int_rda);
     disable_interrupts(int_ext);
      
     eV=0;
     iV=0;
     dV=0;
     uV=0;
     uV2=0;
     iV0=0;
     eV0=0;
     iD0=0;
     eD0=0;
     control_M1=0;
     control_M2=0;
     delta_recorrido=0;
     T0_Ant = 0;
     T1_Ant = 0;
     T0_Act = 0;
     T1_Act = 0;
     Delta_T0 = 0;
     Delta_T1 = 0;
     set_pwm1_duty(control_M1);
     set_pwm2_duty(control_M2);
     setup_ccp1(ccp_pwm); 
     setup_ccp2(ccp_pwm);
     distancia_0=300;
     distancia_1=300;
     enable_interrupts(global);
     
     //for(i=0;i<11;++i){
     //valor_pwm=100*(int16)i;
     //set_pwm1_duty(valor_pwm);
     //set_pwm2_duty(valor_pwm);
     //delay_ms(3000);
     //}
     //valor_pwm=500;
     //set_pwm1_duty(valor_pwm);
     //set_pwm2_duty(valor_pwm);
     //delay_ms(10000);
     //valor_pwm=600;
     //set_pwm1_duty(valor_pwm);
     //set_pwm2_duty(valor_pwm);
     //delay_ms(10000);
     
     if ( control_tipo ==1){
         velocidad_ref = 10;
         while ((get_timer0() <= distancia_0) | (get_timer1() <=distancia_1) );
         setup_ccp1(ccp_off);
         setup_ccp2(ccp_off);
         output_low(PIN_B3);
         output_low(PIN_C2);
         output_B(input_b() & 0b00001111);
         
     }
     else {
         
         
         //velocidad_ref = vel_pwm;
         delay_ms(330);
         disable_interrupts(int_timer3);
         verificar_level_ir();
         //if ( level_ir < 2.1){
            setup_ccp1(ccp_off);
            setup_ccp2(ccp_off);
            output_low(PIN_C2);
            output_low(PIN_B3);
         //}
         //else{
         //    setup_ccp1(ccp_off);
         //    setup_ccp2(ccp_off);
         //    output_low(PIN_B3);
         //    output_low(PIN_C2);
         //    output_B(input_b() & 0b00001111);
         //}
       
       //printf("\r\nV= %01.2fV\r\n", level_ir);
     }
     
     //delay_ms(1000);
     
     //printf("\r\n\r Contador 0 = %5Lu y Contador 1 = %5Lu \r\n", get_timer0(), get_timer1());
     //level_source_ref();
    
     
     diferencia_T0 = get_timer0() - distancia_0;
     diferencia_T1 = get_timer1() - distancia_1;
     mov_ant = mov_act;
     setup_timer_1(T1_INTERNAL | T1_DIV_BY_1);
     setup_timer_0(RTCC_INTERNAL | RTCC_DIV_128 );
     //enable_interrupts(int_rda);
     enable_interrupts(int_ext);
     enable_interrupts(global);
}  

void convierte_bits_a_bytes(void){

  for(i=0;i<2;++i){
    Bytes[i]=0x00;
    for(j=0;j<5;++j){
      if(sbits[(i*5)+j]=='1'){
        bytes[i]=bytes[i]|Bits[j];
      }
    }
  }
  dato_ir=bytes[0];
  identificador=bytes[1];
}

void muestra_bits(void){

  printf("\r\n\PULSO RECIBIDO -----------------------\r\n");

  for(i=0;i<2;++i){
    printf("Byte %u = \r",i+1);
    for(k=0;k<2;++k){
      //if(k==1){
      //  printf(" %u ", i+1);
      //}
      for(j=0;j<5;++j){
        switch(k){
          case 0: printf("%2u ",6-((i*-5)+j+1));
                  break;
          case 1: printf("%2c ",sbits[4-((i*-5)+j)]);
                  break;
        }
      }
      if(k==0){ printf("\r\n"); }
    }
    printf(" Valor = %u\r\n",Bytes[i]);
  }
  printf("Address = %u Command = %u\r\n",Bytes[1],Bytes[0]);
  ntbits=0;
  printf("Byte = \r");
  for(i=0;i<10;++i){
  printf("%2Lu \r",times_cr[i]);
  }

}

void main() {
  
  disable_interrupts(global);
  setup_adc_ports(AN0_TO_AN3 | VSS_VDD);
  setup_adc(ADC_CLOCK_DIV_32);
  setup_spi(FALSE);
  //setup_counters(RTCC_INTERNAL,RTCC_DIV_2);
  setup_WDT(WDT_OFF);
  setup_timer_1(T1_INTERNAL | T1_DIV_BY_1);
  setup_timer_2(T2_DISABLED,0,1);
  setup_timer_3(T3_DISABLED);
  setup_comparator(NC_NC_NC_NC);
  setup_vref(VREF_HIGH | VREF_F5 | 8);
  port_b_pullups(FALSE);
   
  setup_timer_0(T0_INTERNAL | T0_DIV_128 );
  set_tris_a(0b00011011); 
  set_tris_b(0b00000001);
  set_tris_c(0b10000001);
  output_a(0x00);
  output_b(0x00);
  output_c(0x00);
  delay_ms(500);
   
  printf("\r\n");
  printf("Hola Soy el Robot CUICUI 2.0 :D %s\r\n",version);
 
 
  ext_int_edge(H_TO_L);      //Interrupci�n de high a Low
  first_ext=0;
  start_recived=0;
  hay_dato=0;

  limpia_bits();
                            //Melod�a de Inicio
  //melodia_inicio();
  //Recibo de un dato por arduino
  
  while(True){
   valorR = kbhit();
   if(ValorR == 1){
      melodia_inicio();
      valorR=0;
   }
  }
  
  disable_interrupts(int_timer3);
  setup_timer_3(T3_INTERNAL | T3_DIV_BY_8);
  set_timer3(0);
  
  disable_interrupts(int_rda);
  enable_interrupts(int_ext);
  
  set_adc_channel(3);
  delay_us(20);
  level_source_b = read_adc();
  level_source = 2*(3.3 * level_source_b / 1024);
     //if (level_source <=4.5){
     //   melodia_baja_bateria();
     //   melodia_baja_bateria();
     //}
     printf("\r\nVoltage = %01.2fV\r\n", level_source);
  
  
  
  
  enable_interrupts(global);
  TMR3IF = 0;
  counter_sleep = 0;
  
  
  
  
  do {
     if( TMR3IF == 1){
         TMR3IF = 0;
         counter_sleep = counter_sleep + 1;
                     if ( counter_sleep >= 286){        //Tiempo estimado 2 min ==>1145...30s =286
                          disable_interrupts(global);
                          melodia_sleep();
                          setup_vref( VREF_HIGH | 15);    //Tension de referencia Vref=2,475 para Vcc = 3,3 V y Vref=3,06 para Vcc = 5,16 V
                          setup_comparator(A0_VR_A1_VR);
                          disable_interrupts(int_ext);
                          enable_interrupts(INT_COMP);
                          enable_interrupts(global);
                          
                          while ( out_sleep == 0){
                                sleep();
                                delay_cycles(1);
                                disable_interrupts(INT_COMP);
                                delay_us(100);
                                //printf("\r\nDespierta para verificar\r\n");
                                enable_interrupts(int_ext);
                                while ( hay_dato == 0 );
                               
                                convierte_bits_a_bytes();
                                if (bytes[1]== 7 && wake_up == 1){
                                out_sleep=1;
                                disable_interrupts(int_ext);
                                //printf("\r\nDespierto...era para mi\r\n");
                                }
                                else{
                                disable_interrupts(int_ext);
                                enable_interrupts(INT_COMP);
                                wake_up = 0;
                                out_sleep = 0;
                                //printf("\r\nMe voi a Dormir\r\n");
                                }
                                hay_dato=0;
                          }
                          
                          hay_dato = 0;
                          wake_up = 0;
                          out_sleep = 0;
                          disable_interrupts(INT_COMP);
                          setup_vref(false);
                          setup_comparator(NC_NC_NC_NC);
                          melodia_wake_up();
                          enable_interrupts(int_ext);
                     }                    
     }



    if(hay_dato==1){
       counter_sleep=0;
       disable_interrupts(global);
       hay_dato=0;
       convierte_bits_a_bytes();
       //muestra_bits();
       //set_adc_channel(3);
       //delay_us(20);
       //q = read_adc();
       //p = (3.3 * q / 1024.0)*2;
       //printf("\r\nVoltage = %01.2fV\r\n", p);
              
       
       if (bytes[1] == 7){
       if (bit_test(bytes[0],4) == 0){
                                                                                //Verificar tensi�n 
       
       
            switch (bytes[0]){
            case UP :
                 //printf("Robot Avanza \r");
                 //output_B((input_b() & 0b00001111) | 0b01100000 ); 
                 //ejecucion_pwm();
                 //verificar_level_ir();
                 control_tipo=0;
                 output_B((input_b() & 0b00001111) | 0b01100000 ); //(M2B, M2A, M1B, M1A, PWM2, RB2, RB1, IRDATA)                 
                 vel_pwm = vel3;
                 output_high(PIN_B1);
                 
                 ejecucion_pwm();
                 output_low(PIN_B1);
                 //setup_timer_2(t2_div_by_1, 224, 1);  //PR2=224, Tpwm=225 us F= 4.4kHz
                 //set_pwm1_duty(vel_pwm);
                 //set_pwm2_duty(vel_pwm);
                 //setup_ccp1(ccp_pwm);
                 //setup_ccp2(ccp_pwm);
                 //delay_ms(400);
                 //setup_ccp1(ccp_off);
                 //setup_ccp2(ccp_off);
                 //output_low(PIN_C2);
                 //output_low(PIN_B3);
                 //output_B((input_b() & 0b00001111));
                 
                 break;
            case DOWN :
                 //printf("Robot Retrocede \r");
                 //verificar_level_ir();
                 control_tipo=0;
                 output_B((input_b() & 0b00001111) | 0b10010000 ); //(M2B, M2A, M1B, M1A, PWM2, RB2, RB1, IRDATA)    
                 vel_pwm = vel3;
                 output_high(PIN_B1);
                 ejecucion_pwm();
                 output_low(PIN_B1);
                //setup_timer_2(t2_div_by_1, 224, 1);  //PR2=224, Tpwm=225 us
                 //set_pwm1_duty(vel_pwm);
                 //set_pwm2_duty(vel_pwm);
                 //setup_ccp1(ccp_pwm);
                 //setup_ccp2(ccp_pwm);
                 //delay_ms(400);
                 //setup_ccp1(ccp_off);
                 //setup_ccp2(ccp_off);
                 //output_low(PIN_C2);
                 //output_low(PIN_B3);
                 //output_B((input_b() & 0b00001111));          
                 
                 break;
            case LEFT :
                 //printf("Robot Gira a la Izquierda \r");
                 //verificar_level_ir();
                 
                 control_tipo=0;
                 output_B((input_b() & 0b00001111) | 0b10100000 ); //(M2B, M2A, M1B, M1A, PWM2, RB2, RB1, IRDATA)                 
                 vel_pwm = vel3;
                 output_high(PIN_B1);
                 ejecucion_pwm();
                 output_low(PIN_B1);
                 //setup_timer_2(t2_div_by_1, 224, 1);  //PR2=224, Tpwm=225 us
                 //set_pwm1_duty(vel_pwm);
                 //set_pwm2_duty(vel_pwm);
                 //setup_ccp1(ccp_pwm);
                 //setup_ccp2(ccp_pwm);
                 //delay_ms(400);
                 //setup_ccp1(ccp_off);
                 //setup_ccp2(ccp_off);
                 //output_low(PIN_C2);
                 //output_low(PIN_B3);
                 //output_B((input_b() & 0b00001111));
                 
                 break;
            case RIGHT :
                 //printf("Robot Gira a la Derecha \r");
                 //verificar_level_ir();
                 control_tipo=0;
                 output_B((input_b() & 0b00001111) | 0b01010000 ); //(M2B, M2A, M1B, M1A, PWM2, RB2, RB1, IRDATA)                 
                 vel_pwm = vel3;
                 output_high(PIN_B1);
                 ejecucion_pwm();
                 output_low(PIN_B1);
                 //setup_timer_2(t2_div_by_1, 224, 1);  //PR2=224, Tpwm=225 us
                 //set_pwm1_duty(vel_pwm);
                 //set_pwm2_duty(vel_pwm);
                 //setup_ccp1(ccp_pwm);
                 //setup_ccp2(ccp_pwm);
                 //delay_ms(400);
                 //setup_ccp1(ccp_off);
                 //setup_ccp2(ccp_off);
                 //output_low(PIN_C2);
                 //output_low(PIN_B3);
                 //output_B((input_b() & 0b00001111));
                 
                 break;
            case SOUND :
                 printf("Emite un sonido \r");
                 generate_tone(391, 187);
                 generate_tone(523, 187);
                 generate_tone(659, 187);
                 generate_tone(783, 187);
                 delay_ms(187);
                 generate_tone(659, 187);
                 generate_tone(783, 1125);   
                 break;        
            }
       }
       else{
             
       bit_clear(bytes[0],4);
    
       switch (bytes[0]){
            case GO :
                  //printf("!RSET\r");
                 for(i=0;i<contador;++i){
                     
                     switch (datos[i]){
                     case UP:
                          mov_act = UP;
                          control_tipo=1;
                          output_B((input_b() & 0b00001111) | 0b01100000 ); 
                          output_high(PIN_B1);
                          ejecucion_pwm();
                          output_low(PIN_B1);
                          break;
                     case DOWN:
                          mov_act = DOWN;
                          control_tipo=1;
                          output_B((input_b() & 0b00001111) | 0b10010000 );
                          output_high(PIN_B1);
                          ejecucion_pwm();
                          output_low(PIN_B1);
                          break;
                     case LEFT:
                          mov_act = LEFT;
                          control_tipo=1;
                          output_B((input_b() & 0b00001111) | 0b01010000 );
                          output_high(PIN_B1);
                          ejecucion_pwm();
                          break;
                     case RIGHT:
                          mov_act = RIGHT;
                          control_tipo=1;
                          output_B((input_b() & 0b00001111) | 0b10100000 );
                          output_high(PIN_B1);
                          ejecucion_pwm();
                          output_low(PIN_B1);
                          break;
                     case SOUND: 
                          generate_tone(391, 187);
                          generate_tone(523, 187);
                          generate_tone(659, 187);
                          generate_tone(783, 187);
                          delay_ms(187);
                          generate_tone(659, 187);
                          generate_tone(783, 1125);
                          break;  
                     default :
                     delay_us(100);
                     break;
                     }
                     
                     
                                         
                 }
                     generate_tone(659, 152);
                     delay_ms(35);
                     generate_tone(659, 562);
                     distancia_recorrida_1=0;
                     distancia_recorrida_2=0;
                       
                 break;
            case RESET :
                        for(i=0;i<total_datos;++i){
                            datos[i]='\0';
                            }
                            contador=0;
                        generate_tone(1000, 180);
                        generate_tone(600, 240);
                        generate_tone(280, 136);
                        generate_tone(1500, 200);
         
                 break;        
            default :
                  datos[contador]=bytes[0];
                  
                  switch (datos[contador]){
                     case UP:
                          generate_tone(C_NOTE[0], 500);                          
                          break;
                     case DOWN:
                          generate_tone(E_NOTE[0], 500);
                          break;
                     case LEFT:
                          generate_tone(A_NOTE[0], 500);
                          break;
                     case RIGHT:
                          generate_tone(C_NOTE[1], 500);
                          break;
                     case SOUND:
                          generate_tone(E_NOTE[1], 500); 
                          
                          break;             
                     }
                  contador++;
                  
                  
                  
                 break;
            }       
       }
       }
       limpia_bits();
       enable_interrupts(global);
       
    
    }
    
  } 
  while (TRUE);
  
}
