#include  <stdio.h>
#include <stdint.h>

#define Light 0
#define Fan 1
#define TV 2
#define AC 3
#define Headphones 4
#define Speaker 5
#define Curtain 6
#define Dim_LED 7

#define LONG_PRESS_TIME 2000

typedef enum {
MOVIE,
MUSIC,
WORK,
REGULAR
}MODES;

typedef enum{
EVENT_NONE,
EVENT_LONG_PRESS,
EVENT_SHORT_PRESS
}EVENT;


uint8_t devices;
MODES current_mode;
EVENT event;

unsigned long timer = 0;
unsigned long press_start = 0;
unsigned long press_duration = 0;
uint8_t button_now = 0;
uint8_t button_prev = 0;


void toggle_light(void){
devices ^= (1U << Light);
printf("just toggled the light 💡 bro!\n");
}
void curtain_open(void){

devices ^= (1U << Curtain);
printf("just opened the curtains 🪟 for you! \n");
}

void pause_music(void){

devices ^= (1U << Headphones);
printf("just paused the music🎧\n");
}
void pause_movie(void) {

devices ^= (1U << TV);

}

void all_off(void){
   devices = 0x00;
}

void movie(void){
   all_off();
   devices |= (0x170);
printf("movie mode on\n");
}

void music(void) {
    int audio_output;
      printf("pick one enter 4 for headphones and 5 for  speaker : \n");
      scanf("%d",&audio_output);
   if(audio_output == Headphones){
     printf("HEY!nice choice headphones are the best enjoy\n");
     all_off();
     devices |= ((1U << Headphones) | (1U << Curtain));
   }
 else if (audio_output == Speaker){
   printf("Blasting the speakers right now ENJOY!! 👯 /n");
   all_off();
   devices |= ((1U << Speaker) | (1U << Curtain));
 }
}

void regular(void){
all_off();
devices |= (0x03);
printf("regular mode on\n");
}

void work(void){
all_off();
devices |= (1U << Curtain);
}

void read_user_input(void){
int button;
printf("\nby default we will start with regular mode\n");

printf("enter 1 and 0 as buttons\n");
scanf("%d",&button);

if(button == 1){
button_now = 1;
}
else if(button == 0)
button_now = 0;
}
void button_handling(void){
event = EVENT_NONE;

     if((button_now == 1) && (button_prev == 0)){
       press_start = timer;
    }
     if((button_now == 0) && (button_prev == 1)){

      press_duration = timer - press_start;
         if(press_duration >= LONG_PRESS_TIME){
           event = EVENT_LONG_PRESS;
         }
         else{
          event = EVENT_SHORT_PRESS;
         }

    }

button_prev = button_now;
}

void smart_controller(void){

     switch (current_mode){

     case REGULAR:

      regular();
          if(event == EVENT_LONG_PRESS){

          current_mode = WORK;
         }
        break;

    case WORK:
         work();
          if(event == EVENT_LONG_PRESS){

            current_mode = MUSIC;
          }
         break;

   case MUSIC:
       music();
        if(event == EVENT_LONG_PRESS){

        current_mode = MOVIE;
        }
       break;

   case MOVIE:
     movie();
     if(event == EVENT_LONG_PRESS){

      current_mode = REGULAR;
    }
    break;



   }

}

void short_press(void){
if(event != EVENT_SHORT_PRESS) return;

       switch (current_mode){

       case REGULAR:

          toggle_light();

        break;


     case WORK:
      curtain_open();
      break;


    case MOVIE:
     pause_movie();
     break;


     case MUSIC:
   pause_music();
    break;

      }

}


int main() {


while(1){

timer++;

read_user_input();

button_handling;

smart_controller();

short_press();

}





}

