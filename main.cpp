#include <simplecpp>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
using namespace simplecpp;

//movingObject.h
#ifndef _MOVINGOBJECT_INCLUDED_
#define _MOVINGOBJECT_INCLUDED_

#include <simplecpp>
#include <vector>
#include <composite.h>
#include <sprite.h>

using namespace simplecpp;

class MovingObject : public Sprite {
 public:
  vector<Sprite*> parts;
  double vx, vy;
  double ax, ay;
  bool paused;
  void initMO(double argvx, double argvy, double argax, double argay, bool argpaused=true) {
    vx=argvx; vy=argvy; ax=argax; ay=argay; paused=argpaused;
  }
 public:
 MovingObject(double argvx, double argvy, double argax, double argay, bool argpaused=true)
    : Sprite() {
    initMO(argvx, argvy, argax, argay, argpaused);
  }
 MovingObject(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) : Sprite() {
   double angle_rad = angle_deg*PI/180.0;
   double argvx = speed*cos(angle_rad);
   double argvy = -speed*sin(angle_rad);
   initMO(argvx, argvy, argax, argay, argpaused);
  }
  void set_vx(double argvx) { vx = argvx; }
  void set_vy(double argvy) { vy = argvy; }
  void set_ax(double argax) { ax = argax; }
  void set_ay(double argay) { ay = argay; }
  double getXPos();
  double getYPos();
  void reset_all(double argx, double argy, double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta);

  void pause() { paused = true; }
  void unpause() { paused = false; }
  bool isPaused() { return paused; }

  void addPart(Sprite* p) {
    parts.push_back(p);
  }
  void nextStep(double t);
  void getAttachedTo(MovingObject *m);
  void bounce_x(){ set_vy(-vy);}
  void bounce_y(){ set_vx(-vx);}
};

#endif

//MovingObject.cpp

void MovingObject::nextStep(double t) {
  if(paused) { return; }
  //cerr << "x=" << getXPos() << ",y=" << getYPos() << endl;
  //cerr << "vx=" << vx << ",vy=" << vy << endl;
  //cerr << "ax=" << ax << ",ay=" << ay << endl;

  for(size_t i=0; i<parts.size(); i++){
    parts[i]->move(vx*t, vy*t);
  }
  vx += ax*t;
  vy += ay*t;
} // End MovingObject::nextStep()

double MovingObject::getXPos() {
  return (parts.size() > 0) ? parts[0]->getX() : -1;
}

double MovingObject::getYPos() {
  return (parts.size() > 0) ? parts[0]->getY() : -1;
}

void MovingObject::reset_all(double argx, double argy, double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) {
  for(size_t i=0; i<parts.size(); i++){
    parts[i]->moveTo(argx, argy);
  }
  double angle_rad = angle_deg*PI/180.0;
  double argvx = speed*cos(angle_rad);
  double argvy = -speed*sin(angle_rad);
  vx = argvx; vy = argvy; ax = argax; ay = argay; paused = argpaused;
} // End MovingObject::reset_all()

void MovingObject::getAttachedTo(MovingObject *m) {
  double xpos = m->getXPos();
  double ypos = m->getYPos();
  for(size_t i=0; i<parts.size(); i++){
    parts[i]->moveTo(xpos, ypos);
  }
  initMO(m->vx, m->vy, m->ax, m->ay, m->paused);
}

//coin.h
#ifndef __COIN_H__
#define __COIN_H__



class Coin : public MovingObject {
  double coin_start_x;
  double coin_start_y;
  double release_speed;
  double release_angle_deg;
  double coin_ax;
  double coin_ay;
  bool coin_captured = false;
  // Moving parts
  Circle coin_circle;

 public:
 Coin(double coin_start_x, double coin_start_y, double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) : MovingObject(speed, angle_deg, argax, argay, argpaused, rtheta) {
    release_speed = speed;
    release_angle_deg = angle_deg;
    coin_ax = argax;
    coin_ay = argay;
    initCoin(coin_start_x, coin_start_y);
  }

  void initCoin(double coin_start_x, double coin_start_y);
  void resetCoin();
  bool check_if_captured();
  void set_coin_captured();
  void bounce_from_x(){ bounce_x();}
  void bounce_from_y(){ bounce_y();}
}; // End class Coin

#endif

// bomb.h
class Bomb : public MovingObject {
  double bomb_start_x;
  double bomb_start_y;
  double release_speed;
  double release_angle_deg;
  double bomb_ax;
  double bomb_ay;
  bool bomb_captured = false;
  // Moving parts
  Circle bomb_circle;
  Line bomb_line;
 public:
 Bomb(double bomb_start_x, double bomb_start_y, double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) : MovingObject(speed, angle_deg, argax, argay, argpaused, rtheta) {
    release_speed = speed;
    release_angle_deg = angle_deg;
    bomb_ax = argax;
    bomb_ay = argay;
    initBomb(bomb_start_x, bomb_start_y);
  }

  void initBomb(double bomb_start_x, double bomb_start_y);
  void resetBomb();
  bool check_if_captured();
  void set_bomb_captured();
  void bounce_from_x(){ bounce_x();}
  void bounce_from_y(){ bounce_y();}
}; // End class Bomb

//lasso.h
#ifndef __LASSO_H__
#define __LASSO_H__

//#define WINDOW_X 1200
//#define WINDOW_Y 960
#define WINDOW_X 800
#define WINDOW_Y 600

#define STEP_TIME 0.05

#define PLAY_X_START 100
#define PLAY_Y_START 0
#define PLAY_X_WIDTH (WINDOW_X-PLAY_X_START)
#define PLAY_Y_HEIGHT (WINDOW_Y-100)

#define LASSO_X_OFFSET 100
#define LASSO_Y_HEIGHT 100
#define LASSO_BAND_LENGTH LASSO_X_OFFSET
#define LASSO_THICKNESS 5

#define COIN_GAP 0

#define RELEASE_ANGLE_STEP_DEG 5
#define MIN_RELEASE_ANGLE_DEG 0
#define MAX_RELEASE_ANGLE_DEG (360-RELEASE_ANGLE_STEP_DEG)
#define INIT_RELEASE_ANGLE_DEG 45

#define RELEASE_SPEED_STEP 20
#define MIN_RELEASE_SPEED 0
#define MAX_RELEASE_SPEED 300
#define INIT_RELEASE_SPEED 150

#define COIN_SPEED 120
#define COIN_ANGLE_DEG 90
#define BOMB_ANGLE_DEG 90
#define LASSO_G 30
#define COIN_G 30

#define LASSO_SIZE 10
#define LASSO_RADIUS 50
#define COIN_SIZE 5
#define BOMB_SIZE 5
#define COIN_START_X  PLAY_X_START + 200
#define COIN_START_Y  PLAY_Y_HEIGHT
#define BOMB_START_X  PLAY_X_START + 260
#define BOMB_START_Y  PLAY_Y_HEIGHT

class Lasso : public MovingObject {
  double lasso_start_x;
  double lasso_start_y;
  double release_speed;
  double release_angle_deg;
  double lasso_ax;
  double lasso_ay;
  bool check_if_anything_captured = false;
  // Moving parts
  Circle lasso_circle;
  Circle lasso_loop;

  // Non-moving parts
  Line lasso_line;
  Line lasso_band;

  // State info
  bool lasso_looped;
  Coin *the_coin;
  Bomb *the_bomb;
  int num_coins;
  int life;
  void initLasso();
 public:
 Lasso(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) : MovingObject(speed, angle_deg, argax, argay, argpaused, rtheta) {
    release_speed = speed;
    release_angle_deg = angle_deg;
    lasso_ax = argax;
    lasso_ay = argay;
    initLasso();
  }

  void draw_lasso_band();
  void yank();
  void loopit();
  void addAngle(double angle_deg);
  void addSpeed(double speed);

  void nextStep(double t);
  void check_for_coin(Coin *coin);
  int getNumCoins() { return num_coins; }
  void setNumCoins(int a){ num_coins = a;}
  int getNumLifes() { return life; }
  void setNumLifes(int a){ life = a;}
  void check_for_bomb(Bomb *bomb);
  void check_if_captured();
  void set_captured();
  void setCapturedFalse(){ check_if_anything_captured = false;}
}; // End class Lasso

#endif

//coin.cpp

void Coin::initCoin(double coin_start_x, double coin_start_y) {
  this->coin_start_x = coin_start_x;
  this->coin_start_y = coin_start_y;
  coin_circle.reset(this->coin_start_x, this->coin_start_y, COIN_SIZE);
  coin_circle.setColor(COLOR("yellow"));
  coin_circle.setFill(true);
  addPart(&coin_circle);

} // end Coin::initCoin()

void Coin::resetCoin() {
  double coin_angle_deg = COIN_ANGLE_DEG;
  bool paused = true, rtheta = true;
  reset_all(this->coin_start_x, this->coin_start_y, this->release_speed, coin_angle_deg, this->coin_ax, this->coin_ay, paused, rtheta);

} // end Coin::resetCoin()

bool Coin::check_if_captured(){

   if(coin_captured){return true;}
   else {return false;}

}  // end Coin::check_if_captured()

void Coin::set_coin_captured(){

    coin_captured = true;
    coin_circle.setColor(COLOR("white"));
    coin_circle.setFill(true);

}  // end Coin::set_coin_captured()

// bomb.cpp

void Bomb::initBomb(double bomb_start_x, double bomb_start_y) {
  this->bomb_start_x = bomb_start_x;
  this->bomb_start_y = bomb_start_y;
  bomb_circle.reset(this->bomb_start_x, this->bomb_start_y, BOMB_SIZE);
  bomb_circle.setColor(COLOR("black"));
  bomb_circle.setFill(true);
  bomb_line.reset(this->bomb_start_x, this->bomb_start_y, this->bomb_start_x, this->bomb_start_y - 1.5*BOMB_SIZE);
  addPart(&bomb_circle);
  addPart(&bomb_line);

} // end Bomb::initBomb()

void Bomb::resetBomb() {
  double Bomb_angle_deg = BOMB_ANGLE_DEG;
  bool paused = true, rtheta = true;
  reset_all(this->bomb_start_x, this->bomb_start_y, this->release_speed, Bomb_angle_deg, this->bomb_ax, this->bomb_ay, paused, rtheta);
  this->bomb_line.reset(this->bomb_start_x, this->bomb_start_y, this->bomb_start_x, this->bomb_start_y - 1.5*BOMB_SIZE);

} // end Bomb::resetBomb()

bool Bomb::check_if_captured(){

   if(bomb_captured){return true;}
   else {return false;}

}  // end Bomb::check_if_captured()

void Bomb::set_bomb_captured(){

    bomb_captured = true;
    bomb_circle.setColor(COLOR("white"));
    bomb_circle.setFill(true);
    bomb_line.setColor(COLOR("white"));

}  // end Bomb::set_bomb_captured()

//lasso.cpp

void Lasso::draw_lasso_band() {
  double len = (release_speed/MAX_RELEASE_SPEED)*LASSO_BAND_LENGTH;
  double arad = release_angle_deg*PI/180.0;
  double xlen = len*cos(arad);
  double ylen = len*sin(arad);
  lasso_band.reset(lasso_start_x, lasso_start_y, (lasso_start_x-xlen), (lasso_start_y+ylen));
  lasso_band.setThickness(LASSO_THICKNESS);
} // End Lasso::draw_lasso_band()

void Lasso::initLasso() {
  lasso_start_x = (PLAY_X_START+LASSO_X_OFFSET);
  lasso_start_y = (PLAY_Y_HEIGHT-LASSO_Y_HEIGHT);
  lasso_circle.reset(lasso_start_x, lasso_start_y, LASSO_SIZE);
  lasso_circle.setColor(COLOR("red"));
  lasso_circle.setFill(true);
  lasso_loop.reset(lasso_start_x, lasso_start_y, LASSO_SIZE/2);
  lasso_loop.setColor(COLOR("green"));
  lasso_loop.setFill(true);
  addPart(&lasso_circle);
  addPart(&lasso_loop);
  lasso_looped = false;
  the_coin = NULL;
  num_coins = 0;
  life = 4;
  lasso_line.reset(lasso_start_x, lasso_start_y, lasso_start_x, lasso_start_y);
  lasso_line.setColor(COLOR("red"));

  lasso_band.setColor(COLOR("blue"));
  draw_lasso_band();

} // End Lasso::initLasso()

void Lasso::yank() {
  bool paused = true, rtheta = true;
  reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
  lasso_loop.reset(lasso_start_x, lasso_start_y, LASSO_SIZE/2);
  lasso_loop.setFill(true);
  lasso_looped = false;
  check_if_captured();
  setCapturedFalse();

} // End Lasso::yank()

void Lasso::loopit() {
  if(lasso_looped) { return; } // Already looped
  lasso_loop.reset(getXPos(), getYPos(), LASSO_RADIUS);
  lasso_loop.setFill(false);
  lasso_looped = true;
} // End Lasso::loopit()

void Lasso::addAngle(double angle_deg) {
  release_angle_deg += angle_deg;
  if(release_angle_deg < MIN_RELEASE_ANGLE_DEG) { release_angle_deg = MIN_RELEASE_ANGLE_DEG; }
  if(release_angle_deg > MAX_RELEASE_ANGLE_DEG) { release_angle_deg = MAX_RELEASE_ANGLE_DEG; }
  bool paused = true, rtheta = true;
  reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
} // End Lasso::addAngle()

void Lasso::addSpeed(double speed) {
  release_speed += speed;
  if(release_speed < MIN_RELEASE_SPEED) { release_speed = MIN_RELEASE_SPEED; }
  if(release_speed > MAX_RELEASE_SPEED) { release_speed = MAX_RELEASE_SPEED; }
  bool paused = true, rtheta = true;
  reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
} // End Lasso::addSpeed()

void Lasso::nextStep(double stepTime) {
  draw_lasso_band();
  MovingObject::nextStep(stepTime);
  if(getYPos() > PLAY_Y_HEIGHT) { yank(); }
  lasso_line.reset(lasso_start_x, lasso_start_y, getXPos(), getYPos());
} // End Lasso::nextStep()

void Lasso::check_for_coin(Coin *coinPtr) {
  double lasso_x = getXPos();
  double lasso_y = getYPos();
  double coin_x = coinPtr->getXPos();
  double coin_y = coinPtr->getYPos();
  double xdiff = (lasso_x - coin_x);
  double ydiff = (lasso_y - coin_y);
  double distance = sqrt((xdiff*xdiff)+(ydiff*ydiff));
  if(distance <= LASSO_RADIUS && !coinPtr->check_if_captured()) {
    the_coin = coinPtr;
    the_coin->getAttachedTo(this);
        num_coins++;
        coinPtr->set_coin_captured();
        set_captured();
    wait(0.5);
    the_coin->resetCoin();
  }
} // End Lasso::check_for_coin()

void Lasso::check_for_bomb(Bomb *bombPtr) {
  double lasso_x = getXPos();
  double lasso_y = getYPos();
  double coin_x = bombPtr->getXPos();
  double coin_y = bombPtr->getYPos();
  double xdiff = (lasso_x - coin_x);
  double ydiff = (lasso_y - coin_y);
  double distance = sqrt((xdiff*xdiff)+(ydiff*ydiff));
  if(distance <= LASSO_RADIUS && !bombPtr->check_if_captured()) {
    the_bomb = bombPtr;
    the_bomb->getAttachedTo(this);
        life--;
        bombPtr->set_bomb_captured();
        set_captured();
    wait(0.5);
    the_bomb->resetBomb();
  }
} // end Lasso::check_for_bomb()

void Lasso::check_if_captured(){

   if(check_if_anything_captured){return ;}
   else {life--; return ;}

} // end Lasso::check_if_captured()

void Lasso::set_captured(){

    check_if_anything_captured = true;

} // end Lasso::set_captured()


int main() {


  initCanvas("Lasso", WINDOW_X, WINDOW_Y);
  int level = 1;
  // Draw lasso at start position
  double release_speed = INIT_RELEASE_SPEED; // m/s
  double release_angle_deg = INIT_RELEASE_ANGLE_DEG; // degrees
  double lasso_ax = 0;
  double lasso_ay = LASSO_G;
  bool paused = true;
  bool rtheta = true;
  Lasso lasso(release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);

  Line b1(0, PLAY_Y_HEIGHT, WINDOW_X, PLAY_Y_HEIGHT);
  b1.setColor(COLOR("blue"));
  Line b2(PLAY_X_START, 0, PLAY_X_START, WINDOW_Y);
  b2.setColor(COLOR("blue"));

  // initial screen with buttons
  {
       Rectangle r1(WINDOW_X/2 - 100,WINDOW_Y/2,80,20);
       Rectangle r2(WINDOW_X/2,WINDOW_Y/2,80,20);
       Rectangle r3(WINDOW_X/2 + 100,WINDOW_Y/2,80,20);
       Rectangle r4(WINDOW_X/2 + 200,WINDOW_Y/2,80,20);
       Text t1(WINDOW_X/2 - 100,WINDOW_Y/2,"start");
       Text t2(WINDOW_X/2,WINDOW_Y/2,"controls");
       Text t3(WINDOW_X/2 + 100,WINDOW_Y/2,"instructions");
       Text t4(WINDOW_X/2 + 200,WINDOW_Y/2,"exit");

       while(true){
            int u=getClick(), x, y;
            x = u/65536;
            y = u%65536;
          //  break;
            if(y < WINDOW_Y/2 + 10 && y > WINDOW_Y/2 - 10){
                if(x < WINDOW_X/2 - 60 && x > WINDOW_X/2 - 140){
                    break;
                }
                if(x < WINDOW_X/2 + 40 && x > WINDOW_X/2 - 40){
                    Text t4(WINDOW_X/2, WINDOW_Y/2 + 25, "press t to throw lasso");
                    Text t5(WINDOW_X/2, WINDOW_Y/2 + 45, "press y to yank lasso");
                    Text t6(WINDOW_X/2, WINDOW_Y/2 + 65, "press l to loop lasso");
                    Text t7(WINDOW_X/2, WINDOW_Y/2 + 85, "press [ or ] to change release angle");
                    Text t8(WINDOW_X/2, WINDOW_Y/2 + 105, "press + or - to change release speed");
                    getClick();
                }
                if(x < WINDOW_X/2 + 140 && x > WINDOW_X/2 + 60){
                    Text t9(WINDOW_X/2 + 50, WINDOW_Y/2 + 25, "catch all coins within time limit to complete level.");
                    Text t10(WINDOW_X/2 + 50, WINDOW_Y/2 + 45, "if in a throw nothing is captured or if bomb is captured life decreases");
                    Text t11(WINDOW_X/2 + 50, WINDOW_Y/2 + 65, "time limit: 30 units for level 1 and 15 units for level 2");
                    Text t12(WINDOW_X/2 + 50, WINDOW_Y/2 + 85, "if all lives lost game over");
                    getClick();
                }
                if(x < WINDOW_X/2 + 240 && x > WINDOW_X/2 + 160){
                    return 0;
                }
            }
        }
  }

 // level 1 start

  if(level == 1){
  int stepCount = 0;
  float stepTime = STEP_TIME;
  float runTime = -1; // sec; -ve means infinite
  float currTime = 0;
  int time = 0;

  //  make game info apprear on screen

  string msg("Cmd: _");
  Text charPressed(PLAY_X_START+50, PLAY_Y_HEIGHT+20, msg);
  char coinScoreStr[256],timerstr[256], LifeStr[256];
  sprintf(coinScoreStr, "Coins: %d", lasso.getNumCoins());
  Text coinScore(PLAY_X_START+50, PLAY_Y_HEIGHT+50, coinScoreStr);
  sprintf(timerstr, "Time: %d", time);
  Text timer(PLAY_X_START+50, PLAY_Y_HEIGHT+80, timerstr);
  sprintf(LifeStr, "Lives: %d", lasso.getNumLifes());
  Text lives(PLAY_X_START+150, PLAY_Y_HEIGHT+50, LifeStr);
  paused = true; rtheta = true;

  // coin and bomb attributes

  double coin_speed4 = 3*COIN_SPEED;
  double coin_speed3 = 4*COIN_SPEED;
  double coin_speed2 = 3*COIN_SPEED;
  double coin_speed1 = 4*COIN_SPEED;
  double bomb_speed1 = 3.5*COIN_SPEED;
  double bomb_speed2 = 5*COIN_SPEED;
  double bomb_speed3 = 3.5*COIN_SPEED;

  double coin_angle_deg = COIN_ANGLE_DEG;
  double bomb_angle_deg = BOMB_ANGLE_DEG;

  double coin_ax = 0;
  double bomb_ax = 0;

  double coin_ay4 = 9*COIN_G;
  double coin_ay3 = 16*COIN_G;
  double coin_ay2 = 9*COIN_G;
  double coin_ay1 = 16*COIN_G;
  double bomb_ay1 = 3.5*3.5*COIN_G;
  double bomb_ay2 = 25*COIN_G;
  double bomb_ay3 = 3.5*3.5*COIN_G;

  double coin_start_x = COIN_START_X;
  double coin_start_y = COIN_START_Y;
  double bomb_start_x = BOMB_START_X;
  double bomb_start_y = BOMB_START_Y;

  // construct bomb and coin on canvas

  Coin coin(coin_start_x, coin_start_y, coin_speed1, coin_angle_deg, coin_ax, coin_ay1, paused, rtheta);
  Coin coin2(coin_start_x + (PLAY_X_WIDTH - 100)/5, coin_start_y, coin_speed2, coin_angle_deg, coin_ax, coin_ay2, paused, rtheta);
  Coin coin3(coin_start_x + 2*(PLAY_X_WIDTH - 100)/5, coin_start_y, coin_speed3, coin_angle_deg, coin_ax, coin_ay3, paused, rtheta);
  Coin coin4(coin_start_x + 3*(PLAY_X_WIDTH - 100)/5, coin_start_y, coin_speed4, coin_angle_deg, coin_ax, coin_ay4, paused, rtheta);
  Bomb bomb(bomb_start_x, bomb_start_y, bomb_speed1, bomb_angle_deg, bomb_ax, bomb_ay1, paused, rtheta);
  Bomb bomb2(bomb_start_x + (PLAY_X_WIDTH - 100)/5, bomb_start_y, bomb_speed2, bomb_angle_deg, bomb_ax, bomb_ay2, paused, rtheta);
  Bomb bomb3(bomb_start_x + 2*(PLAY_X_WIDTH - 100)/5, bomb_start_y, bomb_speed3, bomb_angle_deg, bomb_ax, bomb_ay3, paused, rtheta);

  // After every COIN_GAP sec, make the coin jump
  double last_coin_jump_end = 0;
  double last_coin_jump_end2 = 0;
  double last_coin_jump_end3 = 0;
  double last_coin_jump_end4 = 0;
  double last_bomb_jump_end = 0;
  double last_bomb_jump_end2 = 0;
  double last_bomb_jump_end3 = 0;

  // When t is pressed, throw lasso
  // If lasso within range, make coin stick
  // When y is pressed, yank lasso
  // When l is pressed, loop lasso
  // When q is pressed, quit

  for(;;) {
    if((runTime > 0) && (currTime > runTime)) { break; }
    XEvent e;
    bool pendingEv = checkEvent(e);
    if(pendingEv) {
      char c = charFromEvent(e);
      msg[msg.length()-1] = c;
      charPressed.setMessage(msg);
      switch(c) {
      case 't':
	lasso.unpause();
	break;
      case 'y':
	lasso.yank();
	break;
      case 'l':
	lasso.loopit();
	lasso.check_for_coin(&coin);
	lasso.check_for_coin(&coin2);
    lasso.check_for_coin(&coin3);
    lasso.check_for_coin(&coin4);
    lasso.check_for_bomb(&bomb);
    lasso.check_for_bomb(&bomb2);
    lasso.check_for_bomb(&bomb3);

	wait(STEP_TIME*5);
	break;
      case '[':
	if(lasso.isPaused()) { lasso.addAngle(-RELEASE_ANGLE_STEP_DEG);	}
	break;
      case ']':
	if(lasso.isPaused()) { lasso.addAngle(+RELEASE_ANGLE_STEP_DEG); }
	break;
      case '-':
	if(lasso.isPaused()) { lasso.addSpeed(-RELEASE_SPEED_STEP); }
	break;
      case '=':
	if(lasso.isPaused()) { lasso.addSpeed(+RELEASE_SPEED_STEP); }
	break;
      case 'q':
	exit(0);
      default:
	break;
      }
    }

    lasso.nextStep(stepTime);
    coin.nextStep(stepTime);
    coin2.nextStep(stepTime);
    coin3.nextStep(stepTime);
    coin4.nextStep(stepTime);
    bomb.nextStep(stepTime);
    bomb2.nextStep(stepTime);
    bomb3.nextStep(stepTime);
    if(coin.isPaused()) {
      if((currTime-last_coin_jump_end) >= COIN_GAP) {
	coin.unpause();
      }
    }

    if(coin.getYPos() > PLAY_Y_HEIGHT) {
      coin.resetCoin();
      last_coin_jump_end = currTime;
    }

    if(coin2.isPaused()) {
      if((currTime-last_coin_jump_end2) >= COIN_GAP) {
	coin2.unpause();
      }
    }

    if(coin2.getYPos() > PLAY_Y_HEIGHT) {
      coin2.resetCoin();
      last_coin_jump_end2 = currTime;
    }
    if(coin3.isPaused()) {
      if((currTime-last_coin_jump_end3) >= COIN_GAP) {
	coin3.unpause();
      }
    }

    if(coin3.getYPos() > PLAY_Y_HEIGHT) {
      coin3.resetCoin();
      last_coin_jump_end3 = currTime;
    }

    if(coin4.isPaused()) {
      if((currTime-last_coin_jump_end4) >= COIN_GAP) {
	coin4.unpause();
      }
    }

    if(coin4.getYPos() > PLAY_Y_HEIGHT) {
      coin4.resetCoin();
      last_coin_jump_end4 = currTime;
    }

    if(bomb.isPaused()) {
      if((currTime-last_bomb_jump_end) >= COIN_GAP) {
	bomb.unpause();
      }
    }

    if(bomb.getYPos() > PLAY_Y_HEIGHT) {
      bomb.resetBomb();
      last_bomb_jump_end = currTime;
    }

    if(bomb2.isPaused()) {
      if((currTime-last_bomb_jump_end2) >= COIN_GAP) {
	bomb2.unpause();
      }
    }

    if(bomb2.getYPos() > PLAY_Y_HEIGHT) {
      bomb2.resetBomb();
      last_bomb_jump_end2 = currTime;
    }

    if(bomb3.isPaused()) {
      if((currTime-last_bomb_jump_end3) >= COIN_GAP) {
	bomb3.unpause();
      }
    }

    if(bomb3.getYPos() > PLAY_Y_HEIGHT) {
      bomb3.resetBomb();
      last_bomb_jump_end3 = currTime;
    }

    sprintf(coinScoreStr, "Coins: %d", lasso.getNumCoins());
    coinScore.setMessage(coinScoreStr);

    sprintf(LifeStr, "Lives: %d", lasso.getNumLifes());
    lives.setMessage(LifeStr);

    stepCount++;
    currTime += stepTime;
    if(stepCount%10 == 0){
        time++;
        sprintf(timerstr, "Time: %d", time);
        timer.setMessage(timerstr);
        if(time == 30){
            if(lasso.getNumCoins()!=4){
                Text message(WINDOW_X/2, WINDOW_Y/2, "game over");
                wait(3);
                break;
            }
        }
    }
    wait(stepTime);
    if(lasso.getNumLifes()== 0){
        Text message(WINDOW_X/2, WINDOW_Y/2, "game over");
        wait(3);
        break;
    }
    if(lasso.getNumCoins()==4){
        Text message1(WINDOW_X/2, WINDOW_Y/2, "level 1 complete");
        level++;
        wait(3);
        break;
    }
  } // End for(;;)
 }

 // level 2 start

  if(level == 2){
  int stepCount = 0;
  float stepTime = STEP_TIME;
  float runTime = -1; // sec; -ve means infinite
  float currTime = 0;
  int time = 0;
  lasso.setNumCoins(0);
  lasso.yank();
  //lasso.setNumLifes(4);  // start level 2 with 4 lives
  wait(0.1);
  { Text message2(WINDOW_X/2, WINDOW_Y/2, "level 2");
    wait(1);
  }

  //  make game info apprear on screen
  string msg("Cmd: _");
  Text charPressed(PLAY_X_START+50, PLAY_Y_HEIGHT+20, msg);
  char coinScoreStr[256],timerstr[256], LifeStr[256];
  sprintf(coinScoreStr, "Coins: %d", lasso.getNumCoins());
  Text coinScore(PLAY_X_START+50, PLAY_Y_HEIGHT+50, coinScoreStr);
  sprintf(timerstr, "Time: %d", time);
  Text timer(PLAY_X_START+50, PLAY_Y_HEIGHT+80, timerstr);
  sprintf(LifeStr, "Lives: %d", lasso.getNumLifes());
  Text lives(PLAY_X_START+150, PLAY_Y_HEIGHT+50, LifeStr);
  paused = true; rtheta = true;

  // coin and bomb attributes

  double coin_speed4 = 3*COIN_SPEED;
  double coin_speed3 = 4*COIN_SPEED;
  double coin_speed2 = 3*COIN_SPEED;
  double coin_speed1 = 4*COIN_SPEED;
  double bomb_speed1 = 3.3*COIN_SPEED;
  double bomb_speed2 = 3.3*COIN_SPEED;
  double bomb_speed3 = 3.3*COIN_SPEED;

  double coin_angle_deg = COIN_ANGLE_DEG - 30;
  double coin_angle_deg2 = COIN_ANGLE_DEG - 45;
  double coin_angle_deg3 = COIN_ANGLE_DEG + 45;
  double coin_angle_deg4 = COIN_ANGLE_DEG + 30;
  double bomb_angle_deg = BOMB_ANGLE_DEG + 30;
  double bomb_angle_deg2 = BOMB_ANGLE_DEG + 10;
  double bomb_angle_deg3 = BOMB_ANGLE_DEG - 30;

  double coin_ax = 0;
  double bomb_ax = 0;

  double coin_ay4 = 9*COIN_G;
  double coin_ay3 = 16*COIN_G;
  double coin_ay2 = 9*COIN_G;
  double coin_ay1 = 16*COIN_G;
  double bomb_ay1 = 3.3*3.3*COIN_G;
  double bomb_ay2 = 3.3*3.3*COIN_G;
  double bomb_ay3 = 3.3*3.3*COIN_G;

  double coin_start_x = COIN_START_X;
  double coin_start_y = COIN_START_Y;
  double bomb_start_x = BOMB_START_X;
  double bomb_start_y = BOMB_START_Y;

  // construct coin and bomb on canvas

  Coin coin(coin_start_x, coin_start_y, coin_speed1, coin_angle_deg, coin_ax, coin_ay1, paused, rtheta);
  Coin coin2(coin_start_x + (PLAY_X_WIDTH - 100)/5, coin_start_y, coin_speed2, coin_angle_deg2, coin_ax, coin_ay2, paused, rtheta);
  Coin coin3(coin_start_x + 2*(PLAY_X_WIDTH - 100)/5, coin_start_y, coin_speed3, coin_angle_deg3, coin_ax, coin_ay3, paused, rtheta);
  Coin coin4(coin_start_x + 3*(PLAY_X_WIDTH - 100)/5, coin_start_y, coin_speed4, coin_angle_deg4, coin_ax, coin_ay4, paused, rtheta);
  Bomb bomb(bomb_start_x, bomb_start_y, bomb_speed1, bomb_angle_deg, bomb_ax, bomb_ay1, paused, rtheta);
  Bomb bomb2(bomb_start_x + (PLAY_X_WIDTH - 100)/5, bomb_start_y, bomb_speed2, bomb_angle_deg2, bomb_ax, bomb_ay2, paused, rtheta);
  Bomb bomb3(bomb_start_x + 2*(PLAY_X_WIDTH - 100)/5, bomb_start_y, bomb_speed3, bomb_angle_deg3, bomb_ax, bomb_ay3, paused, rtheta);

  double last_coin_jump_end = 0;
  double last_coin_jump_end2 = 0;
  double last_coin_jump_end3 = 0;
  double last_coin_jump_end4 = 0;
  double last_bomb_jump_end = 0;
  double last_bomb_jump_end2 = 0;
  double last_bomb_jump_end3 = 0;

  for(;;) {
    if((runTime > 0) && (currTime > runTime)) { break; }
    XEvent e;
    bool pendingEv = checkEvent(e);
    if(pendingEv) {
      char c = charFromEvent(e);
      msg[msg.length()-1] = c;
      charPressed.setMessage(msg);
      switch(c) {
      case 't':
	lasso.unpause();
	break;
      case 'y':
	lasso.yank();
	break;
      case 'l':
	lasso.loopit();
	lasso.check_for_coin(&coin);
	lasso.check_for_coin(&coin2);
    lasso.check_for_coin(&coin3);
    lasso.check_for_coin(&coin4);
    lasso.check_for_bomb(&bomb);
    lasso.check_for_bomb(&bomb2);
    lasso.check_for_bomb(&bomb3);

	wait(STEP_TIME*5);
	break;
      case '[':
	if(lasso.isPaused()) { lasso.addAngle(-RELEASE_ANGLE_STEP_DEG);	}
	break;
      case ']':
	if(lasso.isPaused()) { lasso.addAngle(+RELEASE_ANGLE_STEP_DEG); }
	break;
      case '-':
	if(lasso.isPaused()) { lasso.addSpeed(-RELEASE_SPEED_STEP); }
	break;
      case '=':
	if(lasso.isPaused()) { lasso.addSpeed(+RELEASE_SPEED_STEP); }
	break;
      case 'q':
	exit(0);
      default:
	break;
      }
    }

    lasso.nextStep(stepTime);
    coin.nextStep(stepTime);
    coin2.nextStep(stepTime);
    coin3.nextStep(stepTime);
    coin4.nextStep(stepTime);
    bomb.nextStep(stepTime);
    bomb2.nextStep(stepTime);
    bomb3.nextStep(stepTime);

    if(coin.isPaused()) {
      if((currTime-last_coin_jump_end) >= COIN_GAP) {
	coin.unpause();
      }
    }

    if(coin.getYPos() > PLAY_Y_HEIGHT) {
      coin.bounce_from_x();
      last_coin_jump_end = currTime;
    }

    if(coin.getXPos() < PLAY_X_START + 200) {
      coin.bounce_from_y();
      last_coin_jump_end = currTime;
    }

    if(coin.getXPos() > PLAY_X_START + 200 + 3*(PLAY_X_WIDTH - 100)/5) {
      coin.bounce_from_y();
      last_coin_jump_end = currTime;
    }

    if(coin2.isPaused()) {
      if((currTime-last_coin_jump_end2) >= COIN_GAP) {
	coin2.unpause();
      }
    }

    if(coin2.getYPos() > PLAY_Y_HEIGHT) {
      coin2.bounce_from_x();
      last_coin_jump_end2 = currTime;
    }

    if(coin2.getXPos() < PLAY_X_START + 200) {
      coin2.bounce_from_y();
      last_coin_jump_end2 = currTime;
    }

    if(coin2.getXPos() > PLAY_X_START + 200 + 3*(PLAY_X_WIDTH - 100)/5) {
      coin2.bounce_from_y();
      last_coin_jump_end2 = currTime;
    }

    if(coin3.isPaused()) {
      if((currTime-last_coin_jump_end3) >= COIN_GAP) {
	coin3.unpause();
      }
    }

    if(coin3.getYPos() > PLAY_Y_HEIGHT) {
      coin3.bounce_from_x();
      last_coin_jump_end3 = currTime;
    }

    if(coin3.getXPos() < PLAY_X_START + 200) {
      coin3.bounce_from_y();
      last_coin_jump_end3 = currTime;
    }

    if(coin3.getXPos() > PLAY_X_START + 200 + 3*(PLAY_X_WIDTH - 100)/5) {
      coin3.bounce_from_y();
      last_coin_jump_end3 = currTime;
    }

    if(coin4.isPaused()) {
      if((currTime-last_coin_jump_end4) >= COIN_GAP) {
	coin4.unpause();
      }
    }

    if(coin4.getYPos() > PLAY_Y_HEIGHT) {
      coin4.bounce_from_x();
      last_coin_jump_end4 = currTime;
    }

    if(coin4.getXPos() < PLAY_X_START + 200) {
      coin4.bounce_from_y();
      last_coin_jump_end4 = currTime;
    }

    if(coin4.getXPos() > PLAY_X_START + 200 + 3*(PLAY_X_WIDTH - 100)/5) {
      coin4.bounce_from_y();
      last_coin_jump_end4 = currTime;
    }

    if(bomb.isPaused()) {
      if((currTime-last_bomb_jump_end) >= COIN_GAP) {
	bomb.unpause();
      }
    }

    if(bomb.getYPos() > PLAY_Y_HEIGHT) {
      bomb.bounce_from_x();
      last_bomb_jump_end = currTime;
    }

    if(bomb.getXPos() < PLAY_X_START + 200) {
      bomb.bounce_from_y();
      last_bomb_jump_end = currTime;
    }

    if(bomb.getXPos() > PLAY_X_START + 200 + 3*(PLAY_X_WIDTH - 100)/5) {
      bomb.bounce_from_y();
      last_bomb_jump_end = currTime;
    }

    if(bomb2.isPaused()) {
      if((currTime-last_bomb_jump_end2) >= COIN_GAP) {
	bomb2.unpause();
      }
    }

    if(bomb2.getYPos() > PLAY_Y_HEIGHT) {
      bomb2.bounce_from_x();
      last_bomb_jump_end2 = currTime;
    }

    if(bomb2.getXPos() < PLAY_X_START + 200) {
      bomb2.bounce_from_y();
      last_bomb_jump_end2 = currTime;
    }

    if(bomb2.getXPos() > PLAY_X_START + 200 + 3*(PLAY_X_WIDTH - 100)/5) {
      bomb2.bounce_from_y();
      last_bomb_jump_end2 = currTime;
    }

    if(bomb3.isPaused()) {
      if((currTime-last_bomb_jump_end3) >= COIN_GAP) {
	bomb3.unpause();
      }
    }

    if(bomb3.getYPos() > PLAY_Y_HEIGHT) {
      bomb3.bounce_from_x();
      last_bomb_jump_end3 = currTime;
    }

    if(bomb3.getXPos() < PLAY_X_START + 200) {
      bomb3.bounce_from_y();
      last_bomb_jump_end3 = currTime;
    }

    if(bomb3.getXPos() > PLAY_X_START + 200 + 3*(PLAY_X_WIDTH - 100)/5) {
      bomb3.bounce_from_y();
      last_bomb_jump_end3 = currTime;
    }

    sprintf(coinScoreStr, "Coins: %d", lasso.getNumCoins());
    coinScore.setMessage(coinScoreStr);

    sprintf(LifeStr, "Lives: %d", lasso.getNumLifes());
    lives.setMessage(LifeStr);

    stepCount++;
    currTime += stepTime;
    if(stepCount%10 == 0){
        time++;
        sprintf(timerstr, "Time: %d", time);
        timer.setMessage(timerstr);
        if(time == 15){
            if(lasso.getNumCoins()!=4){
                Text message(WINDOW_X/2, WINDOW_Y/2, "game over");
                wait(3);
                break;
            }
        }
    }
    wait(stepTime);
    if(lasso.getNumLifes()== 0){
        Text message(WINDOW_X/2, WINDOW_Y/2, "game over");
        wait(3);
        break;
    }
    if(lasso.getNumCoins()==4){
        Text message1(WINDOW_X/2, WINDOW_Y/2, "level 2 complete");
        wait(3);
        break;
    }
  } // End for(;;)
 }
}
