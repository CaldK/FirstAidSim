#include "SimRandom.h"

int SimRandom::seed = chrono::high_resolution_clock::now().time_since_epoch().count();
mt19937 SimRandom::generator = mt19937(seed);
double SimRandom::urgentRatio = 0;
discrete_distribution<int> SimRandom::disc_distribution = discrete_distribution<int>();
exponential_distribution<double> SimRandom::exp_distribution = exponential_distribution<double>(1/50.f);
uniform_int_distribution<int> SimRandom::urgent_uni_distribution = uniform_int_distribution<int>(30,90);
uniform_int_distribution<int> SimRandom::nonUrgent_uni_distribution = uniform_int_distribution<int>(10,20);
map<int, uniform_int_distribution<int>> SimRandom::map_dist_distribution = {};
uniform_real_distribution<double> SimRandom::real_distribution = uniform_real_distribution<double>(0.0, 1.0);