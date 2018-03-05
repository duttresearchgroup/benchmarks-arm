#include <iostream>
#include <random>
#include <algorithm>
#include <fstream>

#include </home/donny/repos/mash/src/runtime/uapi/beats.h>


#define N 15000
#define T 25
#define k 3
//#define ERR

static task_beat_info_t *g_vitamins_info = 0;
static task_beat_data_t *g_vitamins_beats = 0;

struct point{
	int x,y;
	char c;
	};

void dataGen(point*, int);
float dist(struct point p1,struct point p2);
void kmeans(point*, point*);
void kmeansApx(point*, point*);
void kmeansApxLevel(point*, point*, int);

//full scale perf = 10.4

int main(int argc, char** argv)
{
	struct point space[N];
    struct point means[T];
    struct point centroids[k];

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

    /*data generation*/
    dataGen(space, N);
	dataGen(means, T);
	dataGen(centroids, k);
	/*init*/
	centroids[0].c = 'r';
	centroids[1].c = 'g';
	centroids[2].c = 'b';
	
    while (1) {
	//main kernel, annotate here
	kmeans(space, centroids);

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

//accurate version
void kmeans(point* space, point* centroids)
{
int flips;
	do{
		flips = 0;
		int rcount =0,gcount=0,bcount =0;
		int rx,ry,gx,gy,bx,by;
		rx = ry = bx = by = gx = gy = 0;

		for(int i=0; i<N; i++){
	 		if(space[i].c == 'r'){
	 			rcount++;
	 		 	rx += space[i].x;
	 		 	ry += space[i].y;
	 	 	}
	 	 	if(space[i].c == 'g'){
	 			gcount++;
	 			gx += space[i].x;
	 			gy += space[i].y;
	 	 	}
	 	 	if(space[i].c == 'b'){
	 			bcount++;
	 			bx += space[i].x;
	 			by += space[i].y;
	 	 	}
	  	}
	 
	 	for(int i=0; i<k; i++){
			if(i==0 && rcount!=0){
				centroids[i].x = rx/rcount;
			 	centroids[i].y = ry/rcount;
		 	}
		 	else if(i==1 && gcount!=0){
				centroids[i].x = gx/gcount;
			 	centroids[i].y = gy/gcount;
		 	}
		 	else if(i==2 && bcount!=0){
				centroids[i].x = bx/bcount;
			 	centroids[i].y = by/bcount;
		 	}
	 	}
		
		char cc;
		for(int i=0; i<N; i++){
			float d, min=1000000;
			for(int j=0; j<k; j++){
	 			d = dist(space[i], centroids[j]);
				if(d < min){
					min = d;
					cc = centroids[j].c;
				}
			}
			if(space[i].c != cc){
				space[i].c = cc;
				flips++;
			}
		}

		std::cout << "Flips = " << flips << std::endl;
	}while(flips!=0);
}


void printPoint(point p)
{
	std::cout << "Point: (" << p.x << "," << p.y << ")--" << p.c << std::endl;
}

void dataGen(point* train, int s)
{
	int p;
	int seed = 7;
	std::mt19937 rand(seed);
	for(int i=0; i<s; i++){
		p = rand()%3;

		if(p==0)
		  train[i].c = 'r';
		else if(p==1)
		  train[i].c = 'g';
		else
			train[i].c = 'b';
		
		train[i].x = rand()%1000;
		train[i].y = rand()%600;

	}
}

float dist(struct point p1, struct point p2)
{
	float Dist = sqrt((p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y));
	return Dist;
}
