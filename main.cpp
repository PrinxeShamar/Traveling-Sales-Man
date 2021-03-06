#include <iostream>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <algorithm>
#include <chrono>
#include <thread>

using namespace std;

int POINTS = 8;
int POP_SIZE = 100;
int GENERATIONS = 100;
int VALUE_MAX = 1000;
int ROUTES = (0.5)*(POINTS-1)*(POINTS);

#include "point.h"

void create_distribution(vector<Path*>& parents, vector<double>& distribution, Path*& best_path, Path*& worst_path){
    worst_path = parents[0]; 
    best_path = parents[0];
    int worst = parents[0]->path_value;
    int best = parents[0]->path_value;
    int total = 0;
    for(int i = 0; i < POP_SIZE; i++){
        total += parents[i]->path_value;
        distribution.push_back(parents[i]->path_value);
        if(parents[i]->path_value > worst){
            worst_path = parents[i];
            worst = parents[i]->path_value;
        }
        if(parents[i]->path_value < best){
            best_path = parents[i];
            best = parents[i]->path_value;
        }
    }
    int new_total = 0;
    for(int i = 0; i < POP_SIZE; i++){
        distribution[i] = worst - distribution[i];
        new_total += distribution[i];
    }
    for(int i = 0; i < POP_SIZE; i++){
        distribution[i] = distribution[i] / new_total;
    }
}

Path* random_distribution(vector<Path*>& parents, vector<double>& distribution){
    double rand_num = rand() / double(RAND_MAX);
    double sum = 0;
    for(int i = 0; i < POP_SIZE; i++){
        sum += distribution[i];
        if(rand_num < sum){
            return parents[i];
        }
    }
    return parents.back();
}

void GeneticAlgorithm(vector<Point*>& points){
    vector<Path*> parents; 
    int generation = 0;
    if(generation == 0){
        int count = 0;
        while(count < POP_SIZE){
            Path* path = new Path;
            vector<Point*> points_left = points;
            while(points_left.size() > 0){
                int index = rand() % points_left.size();
                path->add_point(points_left[index]);
                points_left.erase(points_left.begin() + index);
            }
            parents.push_back(path);
            count++;
        }
    }
    Path best_generation_path = *parents[0];
    best_generation_path.calculate_path_value();
    while(generation < GENERATIONS){
        vector<double> distribution;
        for(int i = 0; i < POP_SIZE; i++){
            parents[i]->calculate_path_value();
        }
        sort(parents.begin(), parents.end(), path_sort);
        Path* best_path, *worst_path;
        create_distribution(parents, distribution, best_path, worst_path);   
        if(best_generation_path.path_value > best_path->path_value){
            best_generation_path = *best_path;
        }
        // cout << "Generation: " << generation << " Best: " << best_path->path_value << " Worst: " << worst_path->path_value << endl;
        vector<Path*> new_generation;
        int count = 0;
        while(count < POP_SIZE){
            Path* parent = random_distribution(parents, distribution);
            Path* new_child = new Path(*parent);
            if(rand() % 100 < 20){
                new_child->swap_mutation();
            }
            new_generation.push_back(new_child);
            count++;
        } 
        for(int i = 0; i < POP_SIZE; i++) delete parents[i];
        generation++;
        parents = new_generation;
    }
    best_generation_path.print_path();
}

void BruteForce(const vector<Point*>& points, Path& path, Path& best_part, int index = 0){
    vector<int> points_left;
    for(int i = 0; i < POINTS; i++){
        if(find(path.path.begin(), path.path.end(), points[i]) == path.path.end()){
            points_left.push_back(i);
        }
    }
    if(points_left.size() > 0){
        for(int i = 0; i < points_left.size(); i++){
            path.add_point(points[points_left[i]]);
            BruteForce(points, path, best_part, points_left[i]);
            path.path.pop_back();
        }
    }else{
        path.calculate_path_value();
        if(best_part.path_value > path.path_value){
            best_part = path;
        }
    }
}

void BruteForce(const vector<Point*>& points){
    Path path, best_path;
    BruteForce(points, path, best_path);
    best_path.print_path();
}

int main(){
    srand(time(0));

    vector<Point*> points;
    for(int i = 0; i < POINTS; i++){
        points.push_back(new Point(i));
    }
    int total = 0;
    for(int i = 0; i < POINTS; i++){
        for(int x = i+1; x < POINTS; x++){
            int value = (rand() % VALUE_MAX) + 1;
            total += value;
            points[i]->add_route(points[x], value);
        }
    }
    
    double value_per_route = (double) total / ROUTES;
    cout << "Routes: " << ROUTES << " AVG: " << value_per_route << endl;

    cout << "BRUTE FORCE BEST ";
    auto brute_force_start = std::chrono::high_resolution_clock::now();
    BruteForce(points);
    auto brute_force_end = std::chrono::high_resolution_clock::now();
    cout << "BRUTE FORCE TIME: " << std::chrono::duration_cast<std::chrono::milliseconds>(brute_force_end-brute_force_start).count() << "ms" << endl;

    cout << "GENETIC ALGORITHM BEST ";
    auto gen_algo_start = std::chrono::high_resolution_clock::now();    
    GeneticAlgorithm(points);
    auto gen_algo_end = std::chrono::high_resolution_clock::now();
    cout << "GENETIC ALGORITHM TIME: " << std::chrono::duration_cast<std::chrono::milliseconds>(gen_algo_end-gen_algo_start).count() << "ms" << endl;
}