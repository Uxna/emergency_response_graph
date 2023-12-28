#include "Vertex.cpp"
#include "PQ.cpp"
#include "HashTable.cpp"

#include <fstream>
#include <iostream>
#include <limits>
#include <cmath>
/*
The graph will have a set of V vertices and a VxV matrix that store the edges.
A hash map will be used to store the key (1-500000) of each vertex to the index in the vertices set.
*/

using namespace std;

class Graph{
    private:
        int numVertices; //number of vertex currently stored in the graph
        int storageSize; //number of vertex the graph is currently able to store (size of the vertex array)
        Vertex** vertexArray; //each index stores a vertex
        Edge*** edgeMatrix; //stores the edge between two vertices, using their index in the vertex array
        HashTable indexTable; //index of vertices is stored here
        Edge* getEdge(int vA, int vB);
        void setEdge(int vA, int vB, Edge* e);
        void Dijkstra(int source);
        void relax(int source, int destination);

    public:
        Graph();
        ~Graph();
        void load(fstream& fin);
        bool insert(int source, int destination, double distance, double speed);
        string print(int vertex);
        bool delVertex(int vertex);
        string path(int vA, int vB);
        bool updateTraffic(int vA, int vB, double newAFactor);
        bool update(fstream& fin);
        double lowestTime(int vA, int vB);
        //helper functions below
        bool validInput(int source, int destination = 1, double distance = 1, double speed = 1);
};

void Graph::Dijkstra(int source){
    int sourceIndex = indexTable.getIndex(source);
    Vertex* sourceVertex = vertexArray[sourceIndex];
    for (int i = 0; i < numVertices; ++i){
        if (vertexArray[i] != nullptr){
            vertexArray[i]->setTime(numeric_limits<double>::infinity());
            vertexArray[i]->setPredecessor(nullptr);
        }
    }
    sourceVertex->setTime(0);
    //initalize priority queue with all vertex in the graph
    PQ timePQ(vertexArray, numVertices);
    while (timePQ.isEmpty() == false){
        Vertex* currentVertex = timePQ.extractMinTimeVertex();
        int* adjVertices = currentVertex->adjacentVertices();
        for (int i = 0; i < currentVertex->getAdjListSize(); ++i){
            relax(currentVertex->getKey(), adjVertices[i]);
            timePQ.heapifyVertex(adjVertices[i]);
        }
        delete[] adjVertices;
    }
}

void Graph::relax(int source, int destination){
    int sourceIndex = indexTable.getIndex(source);
    int destinationIndex = indexTable.getIndex(destination);
    Vertex* sourceVertex = vertexArray[sourceIndex];
    Vertex* destinationVertex = vertexArray[destinationIndex];
    Edge* edge2Dest = getEdge(source, destination);
    if (isinf(edge2Dest->getTime())){
        return; //adjustment factor is 0, no need to update
    }
    if (destinationVertex->getTime() > sourceVertex->getTime() + edge2Dest->getTime()){
        //time going through source to destination is less than current time at destination
        destinationVertex->setTime(sourceVertex->getTime() + edge2Dest->getTime());
        destinationVertex->setPredecessor(sourceVertex);
    }
}

string Graph::path(int vA, int vB){
    int aIndex = indexTable.getIndex(vA);
    int bIndex = indexTable.getIndex(vB);
    string result = "";
    if (numVertices == 0 || aIndex == -1 || bIndex == -1){ //empty graph or one of vertex is not stored
        return "failure";
    }
    Dijkstra(vA);
    Vertex* currentVertex = vertexArray[bIndex];
    if (isinf(currentVertex->getTime())){ //no path exist
        return "failure";
    }
    while (currentVertex != nullptr){
        result = result + to_string(currentVertex->getKey()) + " ";
        currentVertex = currentVertex->getPredecessor();
    }
    return result;

}

double Graph::lowestTime(int vA, int vB){
    if (numVertices == 0 || indexTable.getIndex(vA) == -1 || indexTable.getIndex(vB) == -1){
        return -1; //return -1 if no vertex currently stored or one of the given vertex is not stored in graph
    }
    Dijkstra(vA);
    double time = vertexArray[indexTable.getIndex(vB)]->getTime();
    if (isinf(time)){
        //we never visited vB from vA, no path exist between the two
        return -1;
    }
    return time;
}

bool Graph::insert(int source, int destination, double distance, double speed){
    if (validInput(source, destination, distance, speed) == false){ return false; } //invalid input
    //check if source and destination are vertices, if not, create new Vertex objects
    if (indexTable.getIndex(source) == -1){
        Vertex* newVertex = new Vertex(source);
        vertexArray[numVertices] = newVertex; //store the new vertex in both hash table and vertex array
        indexTable.insert(source, numVertices);
        numVertices += 1;
    }
    if (indexTable.getIndex(destination) == -1){
        Vertex* newVertex = new Vertex(destination);
        vertexArray[numVertices] = newVertex; //store the new vertex in both hash table and vertex array
        indexTable.insert(destination, numVertices);
        numVertices += 1;
    }

    //create the edge between two vertex, if edge already exist, update the edge
    int sourceIndex = indexTable.getIndex(source);
    int destinationIndex = indexTable.getIndex(destination);
    if (getEdge(source, destination) == nullptr){
        Edge* newEdge = new Edge(source, distance, speed);
        vertexArray[sourceIndex]->insertAdjvertex(destination);
        vertexArray[destinationIndex]->insertAdjvertex(source);
        setEdge(source, destination, newEdge);
    } else {
        getEdge(source, destination)->updateDS(distance, speed);
    }

    //make sure numVertices is at least 2 less than storage size, we need at least 2 space for next insertion
    if (storageSize - numVertices < 2){
        storageSize = storageSize*2;
        Vertex** newVArray = new Vertex*[storageSize];
        Edge*** newEMatrix = new Edge**[storageSize];
        for (int i = 0; i < storageSize; ++i){ //initialize new elements to nullptr
            newVArray[i] = nullptr;
            newEMatrix[i] = new Edge*[storageSize];
            for (int j = 0; j < storageSize; ++j){
                newEMatrix[i][j] = nullptr;
            }
        }
        for (int i = 0; i < storageSize/2; ++i){ //copy old elements
            newVArray[i] = vertexArray[i];
            for (int j = 0; j < storageSize/2; ++j){
                newEMatrix[i][j] = edgeMatrix[i][j];
            }
        }
        delete[] vertexArray;
        vertexArray = newVArray;
        for (int i = 0; i < storageSize/2; ++i){
            delete[] edgeMatrix[i];
            edgeMatrix[i] = nullptr;
        }
        delete[] edgeMatrix;
        edgeMatrix = newEMatrix;
    }
    return true;
}

void Graph::load(fstream& fin){
    int a, b;
    double d, s;
    while (fin >> a){
        fin >> b; fin >> d; fin >> s;
        if (validInput(a, b, d, s)){
            insert(a, b, d, s);
        }
    }
}

string Graph::print(int vertex){
    int vertexIndex = indexTable.getIndex(vertex);
    if (vertexIndex == -1){ //vertex is not stored 
        return "failure";
    }
    int* adjArray = vertexArray[vertexIndex]->adjacentVertices();
    string result = "";
    for (int i = 0; i < vertexArray[vertexIndex]->getAdjListSize(); ++i){
        result = result + to_string(adjArray[i]) + " ";
    }
    delete[] adjArray;
    return result;
}

bool Graph::delVertex(int vertex){
    int vertexIndex = indexTable.getIndex(vertex);
    if (numVertices == 0 || vertexIndex == -1){ return false; } //graph empty or vertex not stored
    //loop through the adjacency list, for every vertex encountered, delete at at vertex, and delete in edgeMatrix
    int* adjArray = vertexArray[vertexIndex]->adjacentVertices();
    for (int i = 0; i < vertexArray[vertexIndex]->getAdjListSize(); ++i){
        int adjVertex = adjArray[i];
        int adjIndex = indexTable.getIndex(adjVertex);
        delete getEdge(vertex, adjVertex);
        edgeMatrix[vertexIndex][adjIndex] = nullptr;
        edgeMatrix[adjIndex][vertexIndex] = nullptr;
        vertexArray[adjIndex]->deleteAdjVertex(vertex);
    }
    indexTable.remove(vertex);
    delete vertexArray[vertexIndex];
    vertexArray[vertexIndex] = nullptr;
    delete[] adjArray;
    return true;
    // if (vertexIndex == numVertices - 1){ //just deleted the last element
    //     numVertices -= 1;
    //     delete[] adjArray;
    //     return true;
    // }
    // //put the last element in the array into the index that was just deleted
    // Vertex* lastVertex = vertexArray[numVertices-1];
    // int lastIndex = indexTable.getIndex(lastVertex->getKey());
    // vertexArray[vertexIndex] = lastVertex;
    // indexTable.setIndex(lastVertex->getKey(), vertexIndex);
    // numVertices -= 1;
    // //move the edges into the correct slots in the matrix
    // int* lastVAdjArray = lastVertex->adjacentVertices();
    // for (int i = 0; i < lastVertex->getAdjListSize(); ++i){
    //     int adjVertex = lastVAdjArray[i];
    //     int adjIndex = indexTable.getIndex(adjVertex);
    //     edgeMatrix[vertexIndex][adjIndex] = edgeMatrix[lastIndex][adjIndex];
    //     edgeMatrix[adjIndex][vertexIndex] = edgeMatrix[adjIndex][lastIndex];
    //     edgeMatrix[lastIndex][adjIndex] = nullptr;
    //     edgeMatrix[adjIndex][lastIndex] = nullptr;
    // }
    // delete[] lastVAdjArray;
    // delete[] adjArray;
    // return true;
}

bool Graph::updateTraffic(int vA, int vB, double newAFactor){
    int Aindex = indexTable.getIndex(vA);
    int Bindex = indexTable.getIndex(vB);
    Edge* edge = getEdge(vA, vB);
    if (Aindex != -1 && Bindex != -1 && edge != nullptr && newAFactor >= 0 && newAFactor <=1){
        edge->updateAdj(newAFactor);
        return true;
    }
    return false;
}

bool Graph::update(fstream& fin){
    int vA, vB;
    double adjFactor;
    bool success = false;
    while (fin >> vA){
        fin >> vB;
        fin >> adjFactor;
        if (updateTraffic(vA, vB, adjFactor) == true){
            success = true;
        }
    }
    return success;
}

Graph::Graph(): numVertices{0}, storageSize{10}{
    vertexArray = new Vertex*[storageSize];
    edgeMatrix = new Edge**[storageSize]; //initialize an array of array pointers, initalize all pointers to null
    for (int row = 0; row < storageSize; ++row){
        vertexArray[row] = nullptr;
        edgeMatrix[row] = new Edge*[storageSize];
        for (int i = 0; i < storageSize; ++i){
            edgeMatrix[row][i] = nullptr;
        }
    }
}

Graph::~Graph(){
    for (int i = 0; i < storageSize; ++i){
        delete vertexArray[i];
        if (edgeMatrix[i] != nullptr){
            for (int col = 0; col < storageSize; ++col){
                delete edgeMatrix[i][col];
            }
            delete[] edgeMatrix[i];
        }
    }
    delete[] vertexArray;
    delete[] edgeMatrix;
}

bool Graph::validInput(int source, int destination, double distance, double speed){
    bool vS = (source > 0) && (source <= 500000);
    bool vDest = (destination > 0) && (destination <= 500000);
    bool vDist = distance > 0;
    bool vSpee = speed > 0;
    return (vS && vDest && vDist && vSpee);
}

Edge* Graph::getEdge(int vA, int vB){
    int aIndex = indexTable.getIndex(vA);
    int bIndex = indexTable.getIndex(vB);
    if (aIndex < bIndex){
        return edgeMatrix[aIndex][bIndex];
    }
    return edgeMatrix[bIndex][aIndex];
}

void Graph::setEdge(int vA, int vB, Edge* e){
    int aIndex = indexTable.getIndex(vA);
    int bIndex = indexTable.getIndex(vB);
    if (aIndex < bIndex){
        edgeMatrix[aIndex][bIndex] = e;
        return;
    }
    edgeMatrix[bIndex][aIndex] = e;
    return;
}