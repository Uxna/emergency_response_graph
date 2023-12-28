#include <string>

class illegal_exception{
    public:
        illegal_exception(){};
        ~illegal_exception(){};
        std::string getMessage(){
            return message;
        }
    private:
        std::string message{"illegal argument"};
};