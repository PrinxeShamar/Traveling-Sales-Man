#ifndef _point_h_
#define _point_h_

class Point {
public:
    Point(int _point) : point(_point) {}
    void add_route(Point* end, int value){
        pair<map<Point*, int>::iterator, bool> ret = routes.insert(make_pair(end, value));
        if(ret.second){
            end->add_route(this, value);
        }
    }
    int get_value(Point* next){
        return routes[next];
    }
    void print_routes(){
        cout << point << ": < "; 
        for(map<Point*, int>::iterator route_itr = routes.begin(); route_itr != routes.end(); route_itr++){
            cout << route_itr->first->point << '=' << route_itr->second << ' ';
        }
        cout << ">" << endl; 

    }
    map<Point*, int> routes;
    int point;
};

class Path {
public:
    Path(){
        path_value = 0;
    }
    Path(const Path& p){
        path = p.path;
        path_value = p.path_value;
    }
    Path* operator=(const Path& p){
        path = p.path;
        path_value = p.path_value;
        return this;
    }
    void add_point(Point* next){
        path.push_back(next);
    }
    int calculate_path_value(){
        path_value = 0;
        for(int i = 0; i < path.size() - 1; i++){
            path_value += path[i]->get_value(path[i+1]);
        }
        return path_value;
    }
    void swap_mutation(){
        int i = rand() % path.size();
        int x = rand() % path.size();
        swap(path[i], path[x]);
    }
    void print_path(){
        cout << "Path: < "; 
        for(int i = 0; i < path.size(); i++){
            cout << path[i]->point << ' ';
        }
        cout << "> Value: " << path_value << " AVG: " << (float) path_value / POINTS << endl;
    }
    bool operator<(const Path& b){
        return path_value > b.path_value;
    }
    vector<Point*> path;
    int path_value;
};

bool path_sort(Path* a, Path* b){
    return a->path_value > b->path_value;
}


#endif