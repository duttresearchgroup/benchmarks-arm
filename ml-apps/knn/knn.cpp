#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>
#include <fstream>

#include </home/donny/repos/mash/src/runtime/uapi/beats.h>


// full scale perf = 6.4

static task_beat_info_t *g_vitamins_info = 0;
static task_beat_data_t *g_vitamins_beats = 0;

struct point{
	int x;
	int y;
	char val;
	char _class;

};

struct D{
	point p;
	float dist;
};

float distance (point&, point&);
std::vector<float> distanceVec(point, std::vector<point>);
void populateCluster(std::vector<point>&);
bool compareDist(const D &, const D &);
void printPoint(point);
int  countClass(std::vector<D>, char);
void usage();
void knn (int, std::vector<point> &, std::vector<point> &);


int main(int argc, char const *argv[])
{
	int K, clusterSize, testSize;
	//K = std::atoi(argv[2]);
	K = 6; //change this to vary performance
	std::cout << "K = " << K << std::endl;
	std::vector<point> training_data;
	std::vector<point> test_data;
	//training_data.resize(std::atoi(argv[3]));
	training_data.resize(1250); //change this to vary performance
	std::cout << "Training data size = " << training_data.size() << std::endl;
	//test_data.resize(std::atoi(argv[4]));
	test_data.resize(20); //change this to vary performance
	std::cout << "Test data size = " << test_data.size() << std::endl;
	populateCluster(training_data);
	populateCluster(test_data);

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

        while(1) {
		//main kernel, annotate here
		knn(K, training_data, test_data);
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

void knn (int K, std::vector<point> &training_data, std::vector<point> &test_data)
{
	for(int i=0; i< test_data.size(); i++){
		std::vector<D> d;
		d.resize(training_data.size());
		for(int j=0; j< training_data.size(); j++){
			d[j].p = training_data[j];
			d[j].dist = distance(test_data[i], training_data[j]);
		}
		std::sort (d.begin(), d.end(), compareDist);
		int count =0, index=0;
		for(int i=0; i<K ; i++){
			int tmpcount = countClass(d, d[i].p._class);
			if(tmpcount > count){
				count = tmpcount;
				index = i;
			}
		}
		test_data[i]._class = d[index].p._class;
	}
}


float distance (point &p1, point &p2)
{
	return sqrt((((p1).x)-p2.x)*(p1.x-p2.x))+((p1.y-p2.y)*(p1.y-p2.y));
}

std::vector<float> distanceVec (point p, std::vector<point> C)
{
	std::vector<float> dist;
	dist.resize(C.size());
	for(int i=0; i< C.size(); i++)
		dist.push_back(sqrt(((p.x-C[i].x)*(p.x-C[i].x))+((p.y-C[i].y)*(p.y-C[i].y))));
	return dist;
}


void populateCluster(std::vector<point> &cluster)
{
	auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	std::mt19937 _rand(seed);
	for(int i=0; i< cluster.size(); i++){
		cluster[i].x = _rand()%150;
		cluster[i].y = _rand()%100;
		cluster[i].val = char(65+_rand()%5);
		cluster[i]._class = 'a';
	}
}

bool compareDist(const D &d1, const D &d2)
{
	return d1.dist < d2.dist;
}

void printPoint(point p)
{
	std::cout << "(" << p.x << "," << p.y << ")---" << p.val << "---"  << p._class << std::endl;
}

int countClass(std::vector<D> d, char c)
{
	int cnt=0;
	for(int i=0; i< d.size(); i++){
		if(d[i].p._class == c)
			cnt++;
	}
	return cnt;
}

void usage()
{
	std::cout << "Usage:" << std::endl << "./exe times K train test" << std::endl;
	std::cout << "times: no.of.inner loops\t K: no.of.means \t train: training data size \t test: test data size" << std::endl;
}
