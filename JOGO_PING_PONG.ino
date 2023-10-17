#include <SparkFunColorLCDShield.h>
LCDShield lcd;
//Configurações da tela
#define SCREEN_WIDTH 131 // OLED display width, in pixels
#define SCREEN_HEIGHT 131 // OLED display height, in pixels
#define COLOR_BACKGROUND BLACK
#define COLOR_ENTYTIES WHITE

//Configurações do player
int player_position_x = 60;
int player_position_y = 22; //fixo
int player_thickness = 5; 
int player_width = 16; 

//Configurações do player
int enemy_position_x = 60;
int enemy_position_y = 108; //fixo
int enemy_thickness = 5; 
int enemy_width = 16;
long enemy_last_move_time = 0;
long enemy_speed_of_moving = 1500;

int contador=1;

int direita = 2;

//Var do game
int player_score = 0;
int enemy_score = 0;

//configurações da bola
int ball_position_x = 60;
int ball_position_y = 60;
int ball_radius = 2;
int ball_direction_x = random(-3,3);
int ball_direction_y = 3;
int ball_speed = 15;//9,8,7...1
long ball_last_move_time = 0;

void setup() {
  Serial.begin(9600);

  lcd.init(PHILIPS);
  lcd.contrast(-80);
  lcd.clear(COLOR_BACKGROUND);
  
  nome_jogo();
  delay(1000);
  //Limpa a tela toda definindo a cor de Fundo
  lcd.clear(COLOR_BACKGROUND);
  delay(1000);
  
  //Converte int para char para poder ser exibido na tela
  String string_player_score = String(player_score);
  String string_enemy_score = String(enemy_score);
  char char_enemy_score[2];
  char char_player_score[2];
  string_player_score.toCharArray(char_player_score,2);
  string_enemy_score.toCharArray(char_enemy_score,2);
  
  //Mostra a pontuação 
  lcd.setStr(char_player_score, 0, 0, COLOR_ENTYTIES, COLOR_BACKGROUND);
  lcd.setStr(char_enemy_score, 0, 113, COLOR_ENTYTIES, COLOR_BACKGROUND);
  
  //Cria as linhas de fundo
  lcd.setLine(0, 20, 131, 20, COLOR_ENTYTIES);
  lcd.setLine(0, 110, 131, 110, COLOR_ENTYTIES);

  //Cria uma linha mais grossa, barra de funcionamento do jogo, para o player
  int i;
  for(i=1;i<=player_thickness;i++){
    lcd.setLine(player_position_x,player_position_y+i,player_position_x+player_width,player_position_y+i,COLOR_ENTYTIES);
  }

  //Cria uma linha mais grossa, barra de funcionamento do jogo, para o enemy
  for(i=1;i<=enemy_thickness;i++){
    lcd.setLine(enemy_position_x,enemy_position_y-i,enemy_position_x+enemy_width,enemy_position_y-i,COLOR_ENTYTIES);
  }

  lcd.setCircle(ball_position_x, ball_position_y, ball_radius,COLOR_ENTYTIES,COLOR_BACKGROUND);

  delay(1000);
}

void loop() {

  joystick_check();

  move_ball_and_check_colision();

  move_enemy();

}

void move_enemy(){

  if(millis() > (enemy_speed_of_moving + enemy_last_move_time)){
    int i;
    for(i=1;i<=enemy_thickness;i++){
      lcd.setLine(enemy_position_x,enemy_position_y-i,enemy_position_x+enemy_width,enemy_position_y-i,COLOR_BACKGROUND);
    }
  
    if(ball_position_x < (enemy_position_x + enemy_width/2)){
        enemy_position_x = enemy_position_x - 3;
    }else{
        enemy_position_x = enemy_position_x + 3;
    }

    if(enemy_position_x > 131 - enemy_width) enemy_position_x = 131 - enemy_width;
    if(enemy_position_x < 0) enemy_position_x = 0;

    
    for(i=1;i<=enemy_thickness;i++){
      lcd.setLine(enemy_position_x,enemy_position_y-i,(enemy_position_x+enemy_width),enemy_position_y-i,COLOR_ENTYTIES);
    }
    enemy_last_move_time = millis();
  }
}

void move_ball_and_check_colision(){

  if(millis() > ball_speed*20+ball_last_move_time){
    lcd.setCircle(ball_position_x, ball_position_y, ball_radius,COLOR_BACKGROUND,COLOR_BACKGROUND);
    
    ball_position_y = ball_position_y + ball_direction_y;
    
    if(ball_position_x + ball_direction_x < -1) ball_direction_x = ball_direction_x * -1;
    if(ball_position_x + ball_direction_x > 131) ball_direction_x = ball_direction_x * -1;
    
    ball_position_x = ball_position_x + ball_direction_x;

    lcd.setCircle(ball_position_x, ball_position_y, ball_radius,COLOR_ENTYTIES,COLOR_BACKGROUND);

    ball_last_move_time=millis();
    int i;
    for(i=1;i<=player_thickness;i++){
      lcd.setLine(player_position_x,player_position_y+i,player_position_x+player_width,player_position_y+i,COLOR_ENTYTIES);
    }
    for(i=1;i<=enemy_thickness;i++){
      lcd.setLine(enemy_position_x,enemy_position_y-i,enemy_position_x+enemy_width,enemy_position_y-i,COLOR_ENTYTIES);
    }

    if(ball_position_y < player_position_y){
      newRound("enemy");//player
    }

    if(player_position_y + player_thickness >= ball_position_y - ball_radius && player_position_y <= ball_position_y - ball_radius && player_position_x <= ball_position_x + ball_radius && player_position_x+player_width >= ball_position_x + ball_radius){
      ball_direction_y = 3;
      ball_direction_x = random(-3, 3);
      int i;
      for(i=1;i<=player_thickness;i++){
        lcd.setLine(player_position_x,player_position_y+i,player_position_x+player_width,player_position_y+i,COLOR_ENTYTIES);
      }
    }

    if(ball_position_y > enemy_position_y){
      newRound("player");//player
    }

    if(enemy_position_y - enemy_thickness <= ball_position_y + ball_radius && enemy_position_y >= ball_position_y + ball_radius && enemy_position_x <= ball_position_x + ball_radius && enemy_position_x+enemy_width >= ball_position_x + ball_radius){
      //send the ball to player with random values
      ball_direction_y = -3;
      ball_direction_x = random(-3, 3);
      int i;
      for(i=1;i<=enemy_thickness;i++){
        lcd.setLine(enemy_position_x,enemy_position_y-i,enemy_position_x+enemy_width,enemy_position_y-i,COLOR_ENTYTIES);
      }
    }
  }
}

void newRound(String winner){
  
  if(winner == "enemy"){
    enemy_score++;
    ball_direction_y = 3;
  }else{
    player_score++;
    ball_direction_y = -3;  
  }

  lcd.clear(COLOR_BACKGROUND);

  //Converte int para char para poder ser exibido na tela
  String string_player_score = String(player_score);
  String string_enemy_score = String(enemy_score);
  char char_enemy_score[3];
  char char_player_score[3];
  string_player_score.toCharArray(char_player_score,3);
  string_enemy_score.toCharArray(char_enemy_score,3);
  
  //Mostra a pontuação 
  lcd.setStr(char_player_score, 0, 0, COLOR_ENTYTIES, COLOR_BACKGROUND);
  lcd.setStr(char_enemy_score, 0, 113, COLOR_ENTYTIES, COLOR_BACKGROUND);
  
  //Cria as linhas de fundo
  lcd.setLine(0, 20, 131, 20, COLOR_ENTYTIES);
  lcd.setLine(0, 110, 131, 110, COLOR_ENTYTIES);

  enemy_position_x = 60;
  enemy_last_move_time = 0;
  
  player_position_x = 60;

  ball_position_x = 60;
  ball_position_y = 60;
  ball_last_move_time = 0;

  int i;
  for(i=1;i<=player_thickness;i++){
    lcd.setLine(player_position_x,player_position_y+i,player_position_x+player_width,player_position_y+i,COLOR_ENTYTIES);
  }

  //Cria uma linha mais grossa, barra de funcionamento do jogo, para o enemy
  for(i=1;i<=enemy_thickness;i++){
    lcd.setLine(enemy_position_x,enemy_position_y-i,enemy_position_x+enemy_width,enemy_position_y-i,COLOR_ENTYTIES);
  }

  lcd.setCircle(ball_position_x, ball_position_y, ball_radius,COLOR_ENTYTIES,COLOR_BACKGROUND);
  
  if((player_score+enemy_score)%10 == 0){
      if(ball_speed > 3) ball_speed = ball_speed - 1; //10,9,8...
  }

  if((player_score+enemy_score)%3 == 0){
    if(enemy_speed_of_moving > 1) enemy_speed_of_moving = enemy_speed_of_moving * 0.7; //2000,1300,910,637...
  }

  delay(500); // Pause for 0.5 seconds
}

void joystick_check(){
  int val = analogRead(A10);
  if(800 < val){
    if(player_position_x>0){
      int i;
      for(i=1;i<=player_thickness;i++){
        lcd.setLine(player_position_x,player_position_y+i,player_position_x+player_width,player_position_y+i,COLOR_BACKGROUND);
      }
      player_position_x = player_position_x - 3;
      for(i=1;i<=player_thickness;i++){
        lcd.setLine(player_position_x,player_position_y+i,player_position_x+player_width,player_position_y+i,COLOR_ENTYTIES);
      }
    }else{
      int i;
      for(i=1;i<=player_thickness;i++){
        lcd.setLine(player_position_x,player_position_y+i,player_position_x+player_width,player_position_y+i,COLOR_BACKGROUND);
      }
      player_position_x = 0;
      for(i=1;i<=player_thickness;i++){
        lcd.setLine(player_position_x,player_position_y+i,player_position_x+player_width,player_position_y+i,COLOR_ENTYTIES);
      }
    }
  }else if(400 > val){
     if(player_position_x+player_width<131){
      int i;
      for(i=1;i<=player_thickness;i++){
        lcd.setLine(player_position_x,player_position_y+i,player_position_x+player_width,player_position_y+i,COLOR_BACKGROUND);
      }
      player_position_x = player_position_x + 3;
      for(i=1;i<=player_thickness;i++){
        lcd.setLine(player_position_x,player_position_y+i,player_position_x+player_width,player_position_y+i,COLOR_ENTYTIES);
      }
    }else{
      int i;
      for(i=1;i<=player_thickness;i++){
        lcd.setLine(player_position_x,player_position_y+i,player_position_x+player_width,player_position_y+i,COLOR_BACKGROUND);
      }
      player_position_x = 131 - player_width;
      for(i=1;i<=player_thickness;i++){
        lcd.setLine(player_position_x,player_position_y+i,player_position_x+player_width,player_position_y+i,COLOR_ENTYTIES);
      }
    }
  }
}

void nome_jogo(){
  lcd.setStr("Ping Pong",60,20,BLACK,WHITE);
}