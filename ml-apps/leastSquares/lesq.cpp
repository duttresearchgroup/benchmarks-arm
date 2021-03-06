
#include <iostream>
#include <random>
#include <algorithm>
#include <vector>
#include <chrono>
#include <fstream>

#include </home/donny/repos/mash/src/runtime/uapi/beats.h>

void printVec(std::vector<float> &v);
void dataGen(std::vector<float>&);
void lg(std::vector<float>&, std::vector<float>&);

static task_beat_info_t *g_vitamins_info = 0;
static task_beat_data_t *g_vitamins_beats = 0;

int main(int argc, char **argv)
{
	unsigned long int N = 100000; //change this for varied performance
	std::vector<float> X, Y;

    /*beats init*/
    g_vitamins_info = task_beat_register_task();
    if(g_vitamins_info){
        g_vitamins_beats = task_beat_create_domain(g_vitamins_info,BEAT_PERF,0);
        if(!g_vitamins_beats)
            fprintf( stderr, " Cannot setup beats!\n");
        else {
            fprintf( stderr, " Beats setup\n");
        }
    }   
    else {
       fprintf( stderr, " Cannot use connect to beats sensing module!\n");
    } 

    X.resize(N);
    Y.resize(N);
    dataGen(X);
    dataGen(Y);
    while(1) {
    //main kernel, annotate here
  	lg(X,Y);
  	//annotate here
        if (g_vitamins_beats) {
            task_beat(g_vitamins_beats);
        }	
    }
    if(g_vitamins_beats && g_vitamins_info) {
        fprintf(stderr, "Beats: %llu beats, rate = %f beats/s\n",
          task_beat_read_total(g_vitamins_beats),task_beat_read_total_rate(g_vitamins_info,g_vitamins_beats));
    }
   return 0;
}


void lg(std::vector<float> &X, std::vector<float> &Y)
{
	float sumx, sumxsq, sumy, sumxy,a0, a1, denom;
 	sumx = 0;
 	sumxsq = 0;
 	sumy = 0;
 	sumxy = 0;
 	int N = X.size();
 	for(int i = 0; i < N; i++){
    	sumx +=  X[i];
    	sumxsq += X[i]*X[i];
    	sumy +=  Y[i];
    	sumxy +=  X[i] * Y[i];
 	}
	denom = (N * sumxsq) - (sumx*sumx);
 	a0 = (sumy * sumxsq-sumx * sumxy) / denom;
 	a1 = (N * sumxy-sumx * sumy) / denom;
 	std::cout << "Fit line is: " << a1 << "x+" << a0 << std::endl;
}

void dataGen(std::vector<float> &v)
{
  auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
  std::mt19937 randEngine(seed);
  for(int i=0; i< v.size(); i++){
    v[i]= (randEngine()%100);
  }
}

void printVec(std::vector<float> &v )
{
  for(int i=0; i<v.size();i++)
    std::cout << v[i] << std::endl;
}
