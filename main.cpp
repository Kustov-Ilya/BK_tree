#include "correct_file.h"
#include <locale>




int main(int argc, char *argv[]){
    setlocale(LC_ALL, "");
    if(argc == 4){
        BK_tree BK;
        BK.init(argv[2]);
        correct_file(&BK, argv[1], argv[3]);
    }else{
        std::cout << "Enter 3 files";
    }


    return 0;
}