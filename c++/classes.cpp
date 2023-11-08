
#include<iostream>

using namespace std;
enum cow_purpose {dairy, meat, hide, pet};

class cow{
public:
    cow(string name_i, int age_i, unsigned char purpose_i){
        age = age_i; 
        name = name_i;
        purpose = purpose_i;
    }

    void set_name(string new_name){
        name = new_name;
    }

    string get_name(){
        return name;   
    }
    int get_age(){
        return age;   
    }
    unsigned char get_purpose(){
        return purpose;   
    }

private:
    string name;
    int age;
    unsigned char purpose;
    // private cant be accessed;
};


int main()
{
    cow my_cow("Betsy", 4 , pet);
    
    cout<< my_cow.get_name()<< " is type of cow: " << (int)my_cow.get_purpose() << endl;
    my_cow.set_name("HILLY");
    cout<< my_cow.get_name()<< endl;
    
    cin.get();
    return 0;
}
