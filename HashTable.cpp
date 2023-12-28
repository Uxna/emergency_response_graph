class Node{
    public:
        int vertexNumber;
        int arrayIndex;
        Node* nextNode;
        Node(int v, int i): vertexNumber{v}, arrayIndex{i}, nextNode{nullptr}{}
};

class HashTable{
    private: 
        static const int tableSize = 13;
        Node* table[tableSize];
    
    public:
        HashTable(){
            for (int i = 0; i < tableSize; ++i){
                table[i] = nullptr;
            }
        }
        void insert(int vertex, int index){
            int hashIndex = vertex % tableSize;
            Node* newNode = new Node(vertex, index);
            newNode->nextNode = table[hashIndex];
            table[hashIndex] = newNode;
        }
        int getIndex(int vertex){
            int hashIndex = vertex % tableSize;
            Node* currentNode = table[hashIndex];
            while (currentNode != nullptr){
                if (currentNode->vertexNumber == vertex){
                    return currentNode->arrayIndex;
                }
                currentNode = currentNode->nextNode;
            }
            return -1; //return -1 if the vertex is not stored in the hash table
        }
        void setIndex(int vertex, int ind){
            int hashIndex = vertex % tableSize;
            Node* currentNode = table[hashIndex];
            while (currentNode != nullptr){
                if (currentNode->vertexNumber == vertex){
                    currentNode->arrayIndex = ind;
                    return;
                }
                currentNode = currentNode->nextNode;
            }
        }
        void remove(int vertex){
            int hashIndex = vertex % tableSize;
            Node* currentNode = table[hashIndex];
            Node* previousNode = nullptr;
            while (currentNode != nullptr && currentNode->vertexNumber != vertex){
                previousNode = currentNode;
                currentNode = currentNode->nextNode;
            }
            if (currentNode != nullptr){
                if (previousNode != nullptr){
                    previousNode->nextNode = currentNode->nextNode;
                } else {
                    table[hashIndex] = currentNode->nextNode;
                }
                delete currentNode;
            }
        }
        ~HashTable(){ //delete the allocated memory
            for (int i = 0; i < tableSize; ++i){
                Node* currentNode = table[i];
                while (currentNode != nullptr){
                    Node* nextNode = currentNode->nextNode;
                    delete currentNode;
                    currentNode = nextNode;
                }
            }
        }
};