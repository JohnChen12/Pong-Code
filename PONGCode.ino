#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>


// Define button pins
#define P1_UP_BUTTON 3
#define P1_DOWN_BUTTON 4
#define P2_UP_BUTTON 5
#define P2_DOWN_BUTTON 6


// Pin configuration for the TFT display
#define TFT_CS    8      
#define TFT_RST   9      
#define TFT_DC    10    


// Constants for game elements
const uint8_t PADDLE_HEIGHT = 80;
const uint8_t BALL_SIZE = 20;
const uint8_t SCORE_LIMIT = 9;


// Initialize Adafruit ILI9341 display with defined pins
Adafruit_ILI9341 display = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);


// Game Variables to manage game state
bool game_started = false;
bool game_over, win;
uint8_t left_player_score, right_player_score;
uint16_t ball_x = 160, ball_y = 120;
int8_t ball_dir_x = 1, ball_dir_y = 1;
unsigned long ball_update;
unsigned long paddle_update;
uint16_t left_player_y = 120;
uint16_t right_player_y = 120;
int ball_speed = 3;


// Initialize TFT display and set rotation
void setup() {
 display.begin();
 display.setRotation(3);


 // Set up button pins
 pinMode(P1_UP_BUTTON, INPUT_PULLUP);
 pinMode(P1_DOWN_BUTTON, INPUT_PULLUP);
 pinMode(P2_UP_BUTTON, INPUT_PULLUP);
 pinMode(P2_DOWN_BUTTON, INPUT_PULLUP);


 // Wait for initialization (Delay for 2 seconds to allow time for setup)
 unsigned long start = millis();
 while (millis() - start < 2000);


 // Initialization complete (Initialize game state, draw court, and reset the game)
 game_started = true;
 drawCourt();
 resetGame();
}




void loop() {
 bool update_needed = false;
 unsigned long time = millis();


 // Button states
 static bool p1_up_state = false;
 static bool p1_down_state = false;
 static bool p2_up_state = false;
 static bool p2_down_state = false;


 // Read button states
 p1_up_state |= (digitalRead(P1_UP_BUTTON) == LOW);
 p1_down_state |= (digitalRead(P1_DOWN_BUTTON) == LOW);
 p2_up_state |= (digitalRead(P2_UP_BUTTON) == LOW);
 p2_down_state |= (digitalRead(P2_DOWN_BUTTON) == LOW);


 // Ball movement update logic
 if (time - ball_update >= 1) {
   uint16_t new_x = ball_x + ball_dir_x * ball_speed;
   uint16_t new_y = ball_y + ball_dir_y * ball_speed;




   // Check if we hit the vertical walls
   if (new_x <= 0 || new_x >= 320) {
     ball_dir_x = -ball_dir_x;
     new_x += ball_dir_x * ball_speed * 2;


     // Check which player scored
     if (new_x < 160) {
       right_player_score++;
     } else {
       left_player_score++;
     }


     // Check if either player reached the score limit
     if (left_player_score == SCORE_LIMIT) {
       win = false;
       game_over = true;
     } else if (right_player_score == SCORE_LIMIT) {
       win = true;
       game_over = true;
     }
   }


   // Check if we hit the horizontal walls
   if (new_y <= 0 || new_y >= 240) {
     ball_dir_y = -ball_dir_y;
     new_y += ball_dir_y * ball_speed * 2;
   }


   // Check if we hit the left player paddle
   if (new_x <= 20 && new_y >= left_player_y && new_y <= left_player_y + PADDLE_HEIGHT) {
     ball_dir_x = -ball_dir_x;
     new_x += ball_dir_x * ball_speed * 2;
   }


   // Check if we hit the right player paddle
   if (new_x >= 300 && new_y >= right_player_y && new_y <= right_player_y + PADDLE_HEIGHT) {
     ball_dir_x = -ball_dir_x;
     new_x += ball_dir_x * ball_speed * 2;
   }


   // Erase previous ball position and draw the new one
   display.fillCircle(ball_x, ball_y, BALL_SIZE / 2, ILI9341_BLACK);
   display.fillCircle(new_x, new_y, BALL_SIZE / 2, ILI9341_WHITE);
   ball_x = new_x;
   ball_y = new_y;




   // Update the last update time
   ball_update = time;


   // Flag that an update is needed
   update_needed = true;
 }


 // Paddle movement update logic
 if (time - paddle_update >= 32) {
   paddle_update = time;


   // Erase previous left player paddle position
   display.fillRect(12, left_player_y, 8, PADDLE_HEIGHT, ILI9341_BLACK);


   // Move left player paddle based on button states
   if (p1_up_state) {
     left_player_y -= 5;
   }


   if (p1_down_state) {
     left_player_y += 5;
   }


   // Reset button states
   p1_up_state = p1_down_state = false;


   // Boundary check for left player paddle
   if (left_player_y < 0) {
     left_player_y = 0;
   }


   if (left_player_y + PADDLE_HEIGHT > 239) {
     left_player_y = 239 - PADDLE_HEIGHT;
   }


   // Erase previous right player paddle position
   display.fillRect(308, right_player_y, 8, PADDLE_HEIGHT, ILI9341_BLACK);


   // Move right player paddle based on button states
   if (p2_up_state) {
     right_player_y -= 5;
   }




   if (p2_down_state) {
     right_player_y += 5;
   }


   // Reset button states
   p2_up_state = p2_down_state = false;


   // Boundary check for right player paddle
   if (right_player_y < 0) {
     right_player_y = 0;
   }


   if (right_player_y + PADDLE_HEIGHT > 239) {
     right_player_y = 239 - PADDLE_HEIGHT;
   }


   // Draw the x`updated paddles
   display.fillRect(12, left_player_y, 8, PADDLE_HEIGHT, ILI9341_WHITE);
   display.fillRect(308, right_player_y, 8, PADDLE_HEIGHT, ILI9341_WHITE);


   // Flag that an update is needed
   update_needed = true;
 }


 // Display scores and handle game over
 if (update_needed) {
   if (game_over) {
     // Display game over/winner message
     const char* text = win ? "RIGHT PLAYER WINS" : "LEFT PLAYER WINS";
     display.fillScreen(ILI9341_BLACK);
     display.setCursor(0, 120);
     display.print(text);
     delay(5000);
   
     // Clear screen, reset game, and redraw court
     display.fillScreen(ILI9341_BLACK);
     resetGame();
     drawCourt();
   }




   // Display scores
   display.setTextSize(3);
   display.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
   display.setCursor(125, 0);
   display.print(left_player_score);
   display.setCursor(205, 0);
   display.print(right_player_score);
 }
}




// Draw the initial game court
void drawCourt() {
 display.drawRect(0, 0, 240, 320, ILI9341_BLACK);
}




// Reset the game state
void resetGame() {
 ball_x = 160;
 ball_y = 120;
 ball_dir_x = random(2) == 0 ? -1 : 1;
 ball_dir_y = random(2) == 0 ? -1 : 1;
 left_player_y = 120;
 right_player_y = 120;
 left_player_score = 0;
 right_player_score = 0;
 game_over = false;
}
