//
// Created by lenovo on 11.12.2018.
//

#include "correct_file.h"


value_type correct_string(BK_tree *bkTree, value_type &src) {
    value_type result;
    value_type::size_type first_it=0, second_it, len=src.length();
    if(len == 0) return value_type();
    //first symbol in separator or not
    if(SEPARATORS.find(src[0])!=value_type::npos){
        second_it = src.find_first_not_of(SEPARATORS, first_it);
        result+=src.substr(first_it, second_it-first_it);
        first_it = second_it;
    }
    //find first word
    second_it = src.find_first_of(SEPARATORS, first_it);
    //find all word and correct them
    while(second_it != value_type::npos){
        //word
        value_type tmp = bkTree->find_with_mistakes(src.substr(first_it, second_it-first_it));
        result+= tmp.empty() ? src.substr(first_it, second_it-first_it): tmp;
        //not word
        first_it = src.find_first_not_of(SEPARATORS, second_it);
        result+=src.substr(second_it, first_it-second_it);
        second_it = src.find_first_of(SEPARATORS, first_it);
    }
    //last word if it is
    if(len>first_it){
        value_type tmp = bkTree->find_with_mistakes(src.substr(first_it, second_it-first_it));
        result+= tmp.empty() ? src.substr(first_it, second_it-first_it): tmp;
    }
    return result;
}

void correct_file(BK_tree *bkTree, std::string const &address_in, std::string const &address_out) {
    std::wifstream file_in(address_in, std::fstream::in);
    std::wofstream file_out(address_out, std::fstream::out);

    if (file_in.is_open()&&file_out.is_open())
    {
        value_type str;
        while (getline(file_in, str))
        {
            std::transform(str.begin(),str.end(),str.begin(),towlower);
            file_out << correct_string(bkTree, str) << std::endl;
        }
    }
    file_in.close();
    file_out.close();
}
