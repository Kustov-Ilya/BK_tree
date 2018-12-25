#include <utility>

//
// Created by lenovo on 10.12.2018.
//

#ifndef BK_TREE_BK_TREE_CLASS_H
#define BK_TREE_BK_TREE_CLASS_H

#include <time.h>
#include <iostream>
#include <string>
#include <map>
#include <queue>
#include <vector>
#include <algorithm>
#include <exception>
#include <fstream>

class BK_tree
{
public:
    using value_type= std::wstring;
    using size_type = size_t;

private:
    struct Node
    {
        value_type data;
        std::map<size_type, Node*> ways;

        Node() = default;

        //Done
        explicit Node(const value_type& tmp){
            data = tmp;
        }

        //Done
        ~Node()
        {
            if (!ways.empty())
            {
                for (auto &tmp : ways) delete tmp.second;
            }
        }

        //Done
        size_type damerau_levenshtein(const value_type& word)
        {
            //if (word.empty()) std::exception("Word is empty");
            value_type this_word = this->data;
            size_type size1 = this_word.size();
            size_type size2 = word.size();
            std::vector<std::vector<size_type>> dist(3, std::vector<size_type>(size2 + 1));
            for (size_type i = 0; i <= size1; i++)
            {
                for (size_type j = 0; j <= size2; j++)
                {
                    if (i == 0 || j == 0)
                    {
                        dist[i % 3][j] = i + j;
                        continue;
                    }
                    size_type insert_dist = dist[i % 3][j - 1] + 1;
                    size_type delete_dist = dist[(i - 1) % 3][j] + 1;
                    size_type match_dist = dist[(i - 1) % 3][j - 1] + (this_word[size1 - i] != word[size2 - j]);
                    dist[i % 3][j] = std::min({ insert_dist, delete_dist, match_dist });

                    if (i > 1 && j > 1 && this_word[size1 - i] == word[size2 - j + 1] && this_word[size1 - i + 1 ] == word[size2 - j])
                    {
                        dist[i % 3][j] = std::min({ dist[i % 3][j], dist[(i - 2) % 3][j - 2] + 1 });
                    }
                }
            }
            return dist[size1 % 3][size2];
        }


    };

    size_type Num_of_mistakes;
    Node* root;

    value_type corrected_word(const value_type& word) {
        auto founded = find_in_branch(root, word);
        if (founded.empty()) return value_type();
        size_type size = word.size();
        value_type corr_w;
        size_type min_mistakes = 10, min_rem = 10;
        while (!founded.empty()) {

            auto tmp = founded.front();
            size_type rem = abs(tmp.second.size()-size);
            if (tmp.first < min_mistakes)
            {
                min_rem = rem;
                corr_w = tmp.second;
                min_mistakes = tmp.first;
            }else if(tmp.first == min_mistakes && rem<min_rem){
                min_rem = rem;
                corr_w = tmp.second;
            }
            founded.pop();
        }
        return corr_w;
    }


    std::queue<std::pair<size_type, value_type>> find_in_branch(Node* tmp, const value_type& word) {
        std::queue<std::pair<size_type, value_type>> queue1;
        size_type dist = tmp->damerau_levenshtein(word);
        if (dist <= Num_of_mistakes) queue1.push(std::make_pair(dist, tmp->data));
        for (size_type i = dist - Num_of_mistakes; i <= dist + Num_of_mistakes; i++) {
            auto it = tmp->ways.find(i);
            if (it != tmp->ways.end()) {
                auto queue2 = find_in_branch(it->second, word);
                while (!queue2.empty()) {
                    queue1.push(queue2.front());
                    queue2.pop();
                }
            }
        }
        return queue1;

    }
public:


    //Done
    BK_tree()
            :root(nullptr), Num_of_mistakes(1) {}

    BK_tree(size_type num_of_mistakes)
            :root(nullptr), Num_of_mistakes(num_of_mistakes){}

    //Done
    BK_tree(BK_tree const& tmp)
            :root(tmp.root), Num_of_mistakes(tmp.Num_of_mistakes) {}


    //Done
    BK_tree(BK_tree&& tmp) noexcept
            :root(tmp.root), Num_of_mistakes(tmp.Num_of_mistakes)
    {
        tmp.root = nullptr;
    }


    //Done
    ~BK_tree()
    {
        if (root != nullptr)
        {
            delete root;
            root = nullptr;
        }
    }


    //Done
    bool empty()
    {
        return (root == nullptr);
    }



    void add(const value_type& tmp)
    {
        //if (tmp.empty()) std::exception("Why do u add void word?");
        if (empty())
        {
            root = new Node(tmp);
            return;
        }
        Node *that = root;
        size_type dist;
        for (;;)
        {
            size_type dist = that->damerau_levenshtein(tmp);
            auto it = that->ways.find(dist);
            if (it == that->ways.end())
            {
                Node* data = new Node(tmp);
                that->ways.insert(std::make_pair(dist, data));
                return;
            }
            else
            {
                that = it->second;
            }
        }
    }

    value_type find(const value_type& word){
        //if (word.empty()) std::exception("Why do u want to find null word?");
        //if (empty()) std::exception("BK is null");
        Node *that = root;
        for (;;)
        {
            size_type dist = that->damerau_levenshtein(word);
            if (dist == 0)
            {
                return that->data;
            }

            auto it = that->ways.find(dist);
            if(it!=that->ways.end())
            {
                that = it->second;
            }
            else
            {
                return value_type();
            }
        }
    }


    //Done
    BK_tree& operator=(BK_tree const& tmp) = default;


    //Done
    BK_tree& operator=(BK_tree&& tmp) noexcept{
        root = tmp.root;
        Num_of_mistakes = tmp.Num_of_mistakes;
        tmp.root = nullptr;
        return *this;
    }


    value_type find_with_mistakes(const value_type& word) {
        //if (word.empty()) std::exception("U can't find mistakes in void word");
        if (empty()) return value_type();
        value_type rez = find(word);
        return !rez.empty() ? rez : corrected_word(word);
    }



    void init(std::string const& address){
        std::wifstream file(address, std::fstream::in);
        value_type str;
        if (file.is_open())
        {
            while (getline(file, str))
            {
                this->add(str);
            }
        }
        file.close();
    }
};




#endif //BK_TREE_BK_TREE_CLASS_H
