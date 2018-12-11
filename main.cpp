#include "correct_file.h"




int main(int argc, char *argv[]){
    //setlocale(LC_ALL, "");
    system("chcp 65001");
    /*BK_tree BK;
    if(argc == 4){
        BK.init(argv[2]);
        correct_file(&BK, argv[1], argv[3]);
    }

    std::string str = "f_word_list.txt";
    std::string in = "input.txt";
    str = "f_russian.txt";
    in = "input_rus.txt";
    std::string out = "output.txt";*/

    std::ifstream file_in("f_russian.txt", std::fstream::in);

    if (file_in.is_open())
    {
        std::string str;
        while (getline(file_in, str))
        {
            std::cout << str;
        }
    }
    file_in.close();


    return 0;
}