#include <iostream>
#include <vector>
using namespace std;

vector<vector<char>> map(10,vector<char>(10));

struct vec{
  int x;
  int y;
  vec(int a, int b){x = a; y = b;}
  vec operator+(const vec& v){
    return vec(x+v.x,y+v.y);
  }
};

enum Dir{
  up,down,left,right
};

const vector<vec> dirs = {vec(-1,0),vec(1,0),vec(0,-1),vec(0,1)};

bool vecOk(vec v){
  return v.x >= 0 and v.x < 10 and v.y >= 0 and v.y < 10;
}



bool paintable(vec p, const vec& ini, Dir dini, vec& v, int& xmin, int& xmax, int& ymin, int& ymax){
  if(map[p.x][p.y] == 'd'){
    if(p.x > xmax) xmax = p.x;
    if(p.x < xmin) xmin = p.x;
    if(p.y < ymin) ymin = p.y;
    if(p.y > ymax) ymax = p.y;
    map[p.x][p.y] = 'r';
    return paintable(p+dirs[up],ini,dini,v,xmin,xmax,ymin,ymax) or paintable(p+dirs[down],ini,dini,v,xmin,xmax,ymin,ymax) or paintable(p+dirs[Dir::left],ini,dini,v,xmin,xmax,ymin,ymax) or paintable(p+dirs[Dir::right],ini,dini,v,xmin,xmax,ymin,ymax);
  }
  else if(map[p.x][p.y] == 'p'){
    v = p;
    if(p.x > xmax) xmax = p.x;
    if(p.x < xmin) xmin = p.x;
    if(p.y < ymin) ymin = p.y;
    if(p.y > ymax) ymax = p.y;
  }
}

void paint(int x, int y){

}

int main(){

  //2,6

  for(int i = 0; i < 10; ++i){
    for(int j = 0; j < 10; ++j)
      cin >> map[i][j];
  }



}
