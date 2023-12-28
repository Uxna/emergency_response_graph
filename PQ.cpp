#include "Vertex.cpp"

#include <iostream>
class PQ{
    private:
        Vertex** timePQ; //every node is has smaller or equal time than its children
        int numElements;
        void heapify(int queueIndex){
            int leftIndex = 2*queueIndex;
            int rightIndex = 2*queueIndex + 1;
            int smallest = queueIndex;
            if (leftIndex < numElements && timePQ[leftIndex]->getTime() < timePQ[smallest]->getTime()){
                smallest = leftIndex;
            }
            if (rightIndex < numElements && timePQ[rightIndex]->getTime() < timePQ[smallest]->getTime()){
                smallest = rightIndex;
            }
            if (smallest != queueIndex){
                Vertex* temp = timePQ[smallest];
                timePQ[smallest] = timePQ[queueIndex];
                timePQ[queueIndex] = temp;
                heapify(smallest);
            }
        }
        void heapifyUp(int queueIndex){
            int parentIndex = queueIndex/2;
            if (queueIndex > 0 && timePQ[parentIndex]->getTime() > timePQ[queueIndex]->getTime()){
                Vertex* temp = timePQ[parentIndex];
                timePQ[parentIndex] = timePQ[queueIndex];
                timePQ[queueIndex] = temp;
                heapifyUp(parentIndex);
            }
        }
    public:
        PQ(Vertex** vertexArray, int numVertices): numElements{numVertices}{
            timePQ = new Vertex*[numElements];
            int skipNum = 0;
            for (int i = 0; i < numElements; ++i){
                if (vertexArray[i] == nullptr){
                    //this vertex was deleted, skip it
                    skipNum += 1;
                } else {
                    timePQ[i - skipNum] = vertexArray[i];
                }
            }
            numElements -= skipNum;
            //build the heap in constructor
            for (int i = numElements/2 - 1; i >= 0; --i){
                heapify(i);
            }
        }
        ~PQ(){
            delete[] timePQ;
        }
        bool isEmpty(){
            return numElements == 0;
        }
        Vertex* extractMinTimeVertex(){
            Vertex* max = timePQ[0];
            timePQ[0] = timePQ[numElements - 1];
            numElements -= 1;
            heapify(0);
            return max;
        }
        void heapifyVertex(int vertex){
            //go through the queue to find the vertex that was modified, heapfiy at that index
            for (int i = 0; i < numElements; ++i){
                if (timePQ[i]->getKey() == vertex){
                    heapifyUp(i);
                }
            }
        }
};
