#include<iostream>
#include<vector>
#include<cmath>
#include<random>
#include"Engine.hpp"

const int WIDTH=1024;
const int HEIGHT=1024;
const int NUM_AGENTS=10000;
using namespace std;
struct Agent{
    float x,y;
    float vx,vy;
    float ax,ay;
};

void setTrail(float *grid,int x,int y,float value){
    if(x>=0 && x<WIDTH && y>=0 && y<HEIGHT){
        grid[y*WIDTH+x] =value;
    }
};

int main(){
    RenderEngine engine(WIDTH,HEIGHT);
    if(!engine.init()){
        cerr<<"Failed to initialize engine!"<<endl;
        return -1;
    }
    float *trailGrid =new float[WIDTH*HEIGHT]();
    Agent *agents=new Agent[NUM_AGENTS];
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<float>  disPos(0.0f,static_cast<float>(WIDTH));
    uniform_real_distribution<float> disDir(0.0f,1.0f);
    float speed=2.0f;
    float centerX= WIDTH/2.0f;
    float centerY=HEIGHT/2.0f;
    for(int i=0;i<NUM_AGENTS;i++){
        agents[i].x=disPos(gen);
        agents[i].y=disPos(gen);
        float dx=agents[i].x-centerX;
        float dy=agents[i].y-centerY;
        float dist = sqrt(dx*dx+dy*dy);
        //float angle=atan2(dy,dx);
        if(dist ==0) dist=0.001f;
        float tangentX=-dy/dist;
        float tangentY=dx/dist;
        if(disDir(gen) >0.5f){
            tangentX=-tangentX;
            tangentY= -tangentY;
        }
        agents[i].vx=tangentX*speed;
        agents[i].vy=tangentY*speed;
    }
    while(!engine.shouldClose()){
        fill(trailGrid,trailGrid+WIDTH*HEIGHT, 0.0f);
        for(int i=0;i<NUM_AGENTS;++i){
           float dx=centerX-agents[i].x;
           float dy=centerY-agents[i].y;
           float dist =sqrt(dx*dx+dy*dy);
           if(dist >0.001f){
            float pullStrength =(speed*speed)/dist;
            agents[i].ax=(dx/dist)*pullStrength;
            agents[i].ay=(dy/dist)*pullStrength;
           }
           agents[i].vx +=agents[i].ax;
           agents[i].vy+=agents[i].ay;
           float currentSpeed=sqrt(agents[i].vx*agents[i].vx+agents[i].vy*agents[i].vy);
           agents[i].vx=(agents[i].vx/currentSpeed)*speed;
           agents[i].vy=(agents[i].vy/currentSpeed)*speed;
           agents[i].x += agents[i].vx;
           agents[i].y += agents[i].vy;
           if(agents[i].x<0) agents[i].x +=WIDTH;
           if(agents[i].x>WIDTH) agents[i].x -=WIDTH;
           if(agents[i].y <0) agents[i].y+=HEIGHT;
           if(agents[i].y>HEIGHT) agents[i].y -=HEIGHT;
           int gridX = static_cast<int>(agents[i].x);
           int gridY = static_cast<int>(agents[i].y);
           setTrail(trailGrid,gridX,gridY,1.0f);       
        }
        engine.renderFrame(trailGrid);
    }
    delete[] trailGrid;
    delete[] agents;
    engine.shutdown();
    return 0;
}