#include <limits>

class Edge{
    private:
        double speed;
        double distance;
        double adjFactor;
        int destination;

    public:
        int getDestination(){
            return destination;
        }
        void setDestination(int d){
            destination = d;
        }
        double getTime(){ //return -1 if the time is infinite
            if (adjFactor == 0){
                return std::numeric_limits<double>::infinity();
            }
            return distance/(speed*adjFactor);
        }
        void updateDS(double dist, double spee){
            distance = dist;
            speed = spee;
        }
        void updateAdj(double adjF){
            adjFactor = adjF;
        }

        Edge(int dest, double dist, double spee): speed{spee}, distance{dist}, destination{dest}, adjFactor{1}{}
        Edge(){}
        ~Edge(){}
};