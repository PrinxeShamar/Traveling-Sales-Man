#include <iostream>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>

using namespace std;

#include "point.h"

int POINTS = 10;
int POP_SIZE = 100;

int main(){

    srand(time(0));

    vector<Point*> points;
    for(int i = 0; i < POINTS; i++){
        points.push_back(new Point(i));
    }
    for(int i = 0; i < POINTS; i++){
        for(int x = i+1; x < POINTS; x++){
            points[i]->add_route(points[x], (rand() % 100) + 1);
        }
    }
    for(int i = 0; i < POINTS; i++){
        points[i]->print_routes();
    }

    vector<Point*> best_path;
    int best_path_value = 0;

    int count = 0;
    while(count < POP_SIZE){
        vector<Point*> path;
        vector<Point*> points_left = points;
        while(points_left.size() > 0){
            int index = rand() % points_left.size();
            path.push_back(points_left[index]);
            points_left.erase(points_left.begin() + index);
        }
        int path_value = 0;
        // cout << "Path: < ";
        for(int i = 0; i < path.size() - 1; i++){
            // cout << path[i]->point << "->";
            path_value += path[i]->get_value(path[i+1]);
        }
        // cout << path.back()->point << " >" << endl;
        // cout << "Path Value: " << path_value << endl;
        
        if(path_value > best_path_value){
            best_path_value = path_value;
            best_path = path;
        }
        count++;
    }

    cout << "Best Path: < ";
    for(int i = 0; i < best_path.size() - 1; i++){
        cout << best_path[i]->point << "->";
    }
    cout << best_path.back()->point << " >" << endl;
    cout << "Best Path Value: " << best_path_value << endl;
}