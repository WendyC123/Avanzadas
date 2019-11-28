#include "mbed.h"
#include "DTH22.h"
#include "GPS.h"

#define MIN (0.300)
#define MAX (0.900)
#define UVONMAX 15

Serial pc(USBTX, USBRX); // tx, rx
GPS gps(PA_11, PA_12);
Serial device(PA_15, PB_7, 9600);

AnalogIn salida(A0);
DigitalOut referencia(A1);

DigitalOut A(D6);
DigitalOut B(D5);

void Lore(){
   if(gps.sample()) {
   pc.printf("Encontrado en: %f, %f\n", gps.longitude, gps.latitude);
    } else {

    pc.printf("¿Donde estas? n\r");
      }
    
    }
    
void Gaby(){
    int temp ;
    int humidity;
    int error ;
    
    DTH22 myhumtemp(D7);
    error = myhumtemp.getDTH11TH(&temp,&humidity);
    pc.printf("Temp is %ld\r\n",temp);
    pc.printf("Humidity is %ld\r\n",humidity);
    pc.printf("Error is %d\r\n\r\n",error);
    A=0;
    B=0;
    device.putc(humidity);  
    }
    
void Wendy(){
    float uvraw, uv;
        
    uvraw = salida;
    uv = ((UVONMAX/(MAX-MIN))*(uvraw - MIN));
    pc.printf("UV Intensity %2.2f\r\n", uv);   
    
    }

    

Ticker tiempo;


int main()
{
    int i=0;
    int temp ;
    int humidity;
    int error ;
    float uvraw, uv;
    DTH22 myhumtemp(D7);
    
    tiempo.attach(Lore,1800);
    tiempo.attach(Gaby,1800);
    tiempo.attach(Wendy,1800);
    
    while(1) {
    
    if(i==0){
    pc.printf("Oprima\r\n H=HUMEDAD Y T=TEMPERATURA\r\n");  
    pc.printf(" G=UBICACION\r\n");    
    pc.printf(" R=INTENCIDAD DE RAYOS UV\r\n");
    }    
        char c = pc.getc();
        switch (c) {

            case 'H':    // sensor is covered
                error = myhumtemp.getDTH11TH(&temp,&humidity);
                pc.printf("Temp is %ld\r\n",temp);
                pc.printf("Humidity is %ld\r\n",humidity);
                pc.printf("Error is %d\r\n\r\n",error);
                A=0;
                B=0;
                device.putc(humidity);
                i=1;
            
                break;
                
                case 'T':    // sensor is covered
                error = myhumtemp.getDTH11TH(&temp,&humidity);
                pc.printf("Temp is %ld\r\n",temp);
                pc.printf("Humidity is %ld\r\n",humidity);
                pc.printf("Error is %d\r\n\r\n",error);
                A=1;
                B=1;
                device.putc(temp);
                i=1;
            
                break;

            case 'G':    // sensor in dim light

                if(gps.sample()) {
                    pc.printf("Encontrado en: %f, %f\n", gps.longitude, gps.latitude);
                } else {

                    pc.printf("¿Donde estas? n\r");
                }
                i=1;
                
                break;

            case 'R':    // sensor in medium light
                uvraw = salida;
                uv = ((UVONMAX/(MAX-MIN))*(uvraw - MIN));
                pc.printf("UV Intensity %2.2f\r\n", uv);
                i=1;
                
                break;


            default:   // error situation

                pc.printf("Oprima H, G o R");

        }

        wait(0.5);        // delay in between reads for stability

    }
}

