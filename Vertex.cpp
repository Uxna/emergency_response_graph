#ifndef VERTEX_CPP
#define VERTEX_CPP

#include "Edge.cpp"

#include <string>
#include <limits>

using namespace std;

class Vertex{
    private:
        int key;
        int* adjVertices;
        int currentSize;
        int listSize;
        double timeFromSource;
        Vertex* predecessor;
    
    public:
        Vertex(int k): key{k}, currentSize{0}, listSize{5}{
            adjVertices = new int[listSize];
            timeFromSource = numeric_limits<double>::infinity();
            predecessor = nullptr;
        }
        ~Vertex(){
            delete[] adjVertices;
        }
        int getAdjListSize(){
            return currentSize;
        }
        int* adjacentVertices(){
            int* result = new int[currentSize];
            for (int i = 0; i < currentSize; ++i){
                result[i] = adjVertices[i];
            }
            return result;
        }
        void insertAdjvertex(int vertex){
            adjVertices[currentSize] = vertex;
            currentSize += 1;
            if (currentSize == listSize){
                listSize = listSize*2;
                int* newArray = new int[listSize];
                for (int i = 0; i < listSize/2; ++i){
                    newArray[i] = adjVertices[i];
                }
                delete[] adjVertices;
                adjVertices = newArray;
            }
        }
        void deleteAdjVertex(int vertex){
            for (int i = 0; i < currentSize; ++i){
                if (adjVertices[i] == vertex){ //delete from array, move all elements after back 1 space
                    for (int j = i; j < currentSize - 1; ++j){
                        adjVertices[j] = adjVertices[j + 1];
                    }
                    currentSize -= 1;
                }
            }
        }
        int getKey(){
            return key;
        }
        double getTime(){
            return timeFromSource;
        }
        void setTime(double t){
            timeFromSource = t;
        }
        Vertex* getPredecessor(){
            return predecessor;
        }
        void setPredecessor(Vertex* pre){
            predecessor = pre;
        }
};

#endif