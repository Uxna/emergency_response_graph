#include "Graph.cpp"
#include "illegal_exception.cpp"

#include <iostream>
#include <string>

using namespace std;

int main(){
    Graph graph;
    string command;
    cin >> command;
    while (command != "END"){
        try{ //catch any illegal arguments
        if (command == "LOAD"){
            string filename;
            cin >> filename;
            fstream fin(filename.c_str());
            graph.load(fin);
            cout << "success" << endl;
        }
        else if (command == "INSERT"){
            int a, b;
            double d, s;
            cin >> a; cin >> b;
            cin >> d; cin >> s;
            if (graph.validInput(a, b, d, s) == false){
                throw illegal_exception();
            } else {
                graph.insert(a, b, d, s);
                cout << "success" << endl;
            }
        }
        else if (command == "PRINT"){
            int a;
            cin >> a;
            if (graph.validInput(a) == false){
                throw illegal_exception();
            } else {
                cout << graph.print(a) << endl;
            }
        }
        else if (command == "DELETE"){
            int a;
            cin >> a;
            if (graph.validInput(a) == false){
                throw illegal_exception();
            } else {
                if (graph.delVertex(a) == true){
                    cout << "success";
                } else {
                    cout << "failure";
                }
                cout << endl;
            }
        }
        else if (command == "PATH"){
            int a, b;
            cin >> a; cin >> b;
            if (graph.validInput(a, b) == false){
                throw illegal_exception();
            } else {
                cout << graph.path(a, b) << endl;
            }
        }
        else if (command == "TRAFFIC"){
            int a, b;
            double A;
            cin >> a; cin >> b;
            cin >> A;
            if (graph.validInput(a, b) == false){
                throw illegal_exception();
            } else {
                if (graph.updateTraffic(a, b, A) == true){
                    cout << "success";
                } else {
                    cout << "failure";
                }
                cout << endl;
            }
        }
        else if (command == "UPDATE"){
            string filename;
            cin >> filename;
            fstream fin(filename.c_str());
            if (graph.update(fin) == true){
                cout << "success";
            } else {
                cout << "failure";
            }
            cout << endl;
        }
        else if (command == "LOWEST"){
            int a, b;
            cin >> a; cin >> b;
            if (graph.validInput(a, b) == false){
                throw illegal_exception();
            } else {
                double time = graph.lowestTime(a, b);
                if (time == -1){
                    cout << "failure";
                } else {
                    cout << "lowest is " << time;
                }
                cout << endl;
            }
        }
        else {
            cout << "Invalid command, try again" << endl;
        }
        } //end of the try block
        catch (illegal_exception& e){
            cout << e.getMessage() << endl;
        }

        cin >> command;
    }
}