#include <AberLED.h>
#define EMPTY_POSITION 0
#define SHIP_PRESENT 1
int grid[8][8]; 

void initGrid(){
  for(int x=0;x<8;x++){
    for(int y=0;y<8;y++){
      grid[x][y]=EMPTY_POSITION;
    } 
  }
}

//Ship Model____________________________________Ship Model
#define PT_BOAT 2
#define FRIGATE 3
#define SUBMARINE 3
#define DESTROYER 4

int shipLen;
int noOfShips=0;
int sx;
int sy;

bool settingShip = false;

void createShip(int shipClass){
  shipLen=shipClass;
  sx = 4;
  sy = 4;
  noOfShips++;
  settingShip = true;
}

void moveShip(){
  if(AberLED.getButtonDown(3)){
    sx++;
    if(sx>7){
      sx--; 
    }
  }
  else if(AberLED.getButtonDown(4)){
    sx--;
    if(sx<0){
      sx++; 
    }
  }
  else if(AberLED.getButtonDown(1)){
    sy--;
    if(sy<0){
      sy++; 
    }
  }
  else if(AberLED.getButtonDown(2)){
    sy++;
    if(sy+shipLen>8){
      sy--; 
    }
  }
}

void renderShip(){
  for(int i=0;i<shipLen;i++){
    AberLED.set(sx,sy+i,GREEN); 
  } 
}

void setShipPosition(){
  for(int i=0;i<shipLen;i++){
    grid[sx][sy+i]=SHIP_PRESENT;
  }

  settingShip = false;
}

void showHiddenShips(){
  for(int x=0;x<8;x++){
    for(int y=0;y<8;y++){
      if(grid[x][y] == SHIP_PRESENT){
        AberLED.set(x,y,GREEN);
      } 
    }
  }
}

void HideShips(){
  for(int x=0;x<8;x++){
    for(int y=0;y<8;y++){
      if(grid[x][y] == SHIP_PRESENT){
        AberLED.set(x,y,BLACK);
      } 
    }
  }
}

void selectWhichShipToMake(){
  if(settingShip != true){
    switch(noOfShips){
    case 0:
      createShip(PT_BOAT);
      break;
    case 1:
      createShip(FRIGATE);
      break;
    case 2:
      createShip(SUBMARINE);
      break;
    case 3:
      createShip(DESTROYER);
      break;

    }

  }
}
//Cannon model______________________________Cannon model
int rx=0;
int ry=0;
int cannonShells=20;
int noOfHits=0;
#define HIT 2
#define MISS 3

void moveReticule(){
  if(AberLED.getButtonDown(3)){
    rx++; 
  }
  else if(AberLED.getButtonDown(4)){
    rx--; 
  }
  else if(AberLED.getButtonDown(1)){
    ry--; 
  }
  else if(AberLED.getButtonDown(2)){
    ry++; 
  }
}

void fire(){
  if(grid[rx][ry] == SHIP_PRESENT){
    grid[rx][ry] = HIT;
    noOfHits++;
  }
  else{
    grid[rx][ry] = MISS;
  }
  cannonShells--;
}

void renderHits(){
  for(int x=0;x<8;x++){
    for(int y=0;y<8;y++){
      if(grid[x][y] == HIT){
        AberLED.set(x,y,RED);
      } 
    }
  }
}

void renderMisses(){
  for(int x=0;x<8;x++){
    for(int y=0;y<8;y++){
      if(grid[x][y] == MISS){
        AberLED.set(x,y,GREEN);
      } 
    }
  } 
}

void renderReticule(){
  AberLED.set(rx,ry,YELLOW); 
}

//Main Loop___________________________________Main Loop
#define S_INVALID -1
#define S_START 0
#define S_SHIP_BUILDING 1
#define S_ATTACKING 2
#define S_PLAYER_WIN 3
int state=0;

void goToState(int s){
 state = s; 
}

void renderBox(int WinningPlayer){
  if(WinningPlayer == 1){
    for(int i=0;i<8;i++){
      AberLED.set(i,0,GREEN);
      AberLED.set(i,7,GREEN);
      AberLED.set(0,i,GREEN);
      AberLED.set(7,i,GREEN);
    }
    AberLED.set(3,3,GREEN); 
    AberLED.set(4,2,GREEN);
    AberLED.set(4,3,GREEN);
    AberLED.set(4,4,GREEN);
    AberLED.set(4,5,GREEN);
    AberLED.set(4,6,GREEN);
    AberLED.set(5,6,GREEN);
    AberLED.set(3,6,GREEN);
  }
  else if(WinningPlayer == 2){
    for(int i=0;i<8;i++){
      AberLED.set(i,0,RED);
      AberLED.set(i,7,RED);
      AberLED.set(0,i,RED);
      AberLED.set(7,i,RED);
    }
    AberLED.set(3,2,RED);
    AberLED.set(4,2,RED);
    AberLED.set(5,3,RED);
    AberLED.set(3,4,RED);
    AberLED.set(4,4,RED);
    AberLED.set(3,5,RED);
    AberLED.set(5,6,RED);
    AberLED.set(4,6,RED);
    AberLED.set(3,6,RED);
  } 
}

void renderStart(){
  for(int y=0;y<8;y++){
    for(int x=0;x<8;x++){
      if(y<=3){
        AberLED.set(x,y,GREEN);
      }
      else if(y>=4){
        AberLED.set(x,y,RED);
      } 
    }
  }
  AberLED.set(1, 2,RED);
  AberLED.set(5, 1,RED);
  AberLED.set(2, 2,RED);
  AberLED.set(6, 1,RED);
  AberLED.set(4, 1,RED);
  AberLED.set(7, 1,RED);

  AberLED.set(2, 6,GREEN);
  AberLED.set(4, 7,GREEN);
  AberLED.set(4, 5,GREEN);
  AberLED.set(4, 6,GREEN);
  AberLED.set(2, 5,GREEN);
}

void setup(){
  AberLED.begin();
  initGrid(); 
}

void handleInput(){
  switch(state){
  case 0:
    initGrid();
    cannonShells=20;
    noOfHits = 0;
    noOfShips = 0;
    if(AberLED.getButtonDown(5)){
      goToState(S_SHIP_BUILDING);
    }
    break;
  case 1:

    if(AberLED.getButtonDown(5) && settingShip ==true){
      setShipPosition(); 
    }
    else if(noOfShips == 4 && settingShip != true){
      goToState(S_ATTACKING); 
    }
    break;
  case 2:
    moveReticule();
    if(AberLED.getButtonDown(5)){
      fire();

    }
    break;
  case 3:
    if(AberLED.getButtonDown(5)){
      state = S_START;
    }
    break;
  }
}

void update(){
  if(state == S_SHIP_BUILDING){
    selectWhichShipToMake();
    if(settingShip==true){
      moveShip(); 
    }
  }
  else if(state == S_ATTACKING && cannonShells == 0 || noOfHits>=12){
    state = S_PLAYER_WIN;
  }
}

void render(){
  AberLED.clear();
  switch(state){
  case 0:
    renderStart();
    break;
  case 1:
    renderShip();
    showHiddenShips(); 
    break;
  case 2:
    HideShips();
    renderReticule();
    renderHits();
    renderMisses();
    break;
  case 3:
    if(noOfHits >= 12){
      renderBox(2);
    }
    else if(noOfHits < 12){
      renderBox(1); 
    }
    break;

  }
  AberLED.swap();
}

void loop(){
  handleInput();
  update();
  render();
}




