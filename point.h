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

#endif