//
// Created by lenovo on 11.12.2018.
//

#pragma once
#include "BK_tree_class.h"

using value_type= std::wstring;
const value_type SEPARATORS = L"!?)([].,;/*-+\n\t<>=+%&^#№_1234567890 ";

value_type correct_string (BK_tree* bkTree, value_type & src);

void correct_file(BK_tree* bkTree, std::string const& address_in, std::string const& address_out);
