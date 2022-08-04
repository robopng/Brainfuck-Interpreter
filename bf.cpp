#include <iostream>
#include <fstream>
#include <stack>
#include <vector>
#include <string>
#include <regex>
#include "brainfuck.h"

#define VERSION "1.0A\0"
#define NAME "Brainfuck Interpreter\0"

using namespace std;

//template <class Type, class Container = deque<Type> > class stack;

int main(int argc, char *argv[]){
    int safemode = 0;

    while(*(++argv) != NULL){
        string str = *argv;

        if(!str.compare("-h") || !str.compare("--help")){
            print_help();
            exit(EXIT_SUCCESS);
        }else if(!str.compare("--safemode")){
            safemode = 1;
        }
    }

    ifstream file(*(--argv));
    string code;
    if(file.is_open()){
        while(file){
            string temp;
            file >> temp;
            code += temp;
        }
    }else{
        printf("File name is invalid or no file name given!\n", *argv);
        exit(EXIT_FAILURE);
    }

    if(safemode == 1 && !safe_check(code)){
        printf("Unsafe code! Improper loop");
        exit(EXIT_FAILURE);
    }

    interpret(code);
    cout << endl;
    exit(0);
}

void interpret(const auto &code){
    static vector<unsigned> cells(10, 0);
    static auto vp = cells.begin();
    static auto ep = cells.end();
    static string in;

    for(int i = 0; i < code.size(); ++i){
        switch(code[i]){
            case '+':
                ++(*vp); // order of operations is scary
                break;
            case '-':
                *vp -= 1; // desired behavior is overflow in 0 case
                break;
            case '>':
                ++vp;
                break;
            case '<':
                --vp;
                break;
            case '.':
                cout << char(*vp);
                break;
            case ',':
                cin >> in;
                *vp = !in.compare("NULL") ? 0 : int(in[0]);
                break;
            case '[':
                while_loop(code, vp, ++i); // way to remove i? for : each would be nice
                break;
            case ']':
                // included only for clarity
                break;
            default: // redundancy
                break;
        }

        if(vp == ep){
            cells.push_back(0);
            // ++vp; untested but this absolutely causes bugs
            ++ep;
        }
        if(vp < cells.begin()){
            printf("Invalid code! Negative memory index at character %d", i);
            for(auto c : cells){
                printf(" %d, ", c);
            }
            exit(EXIT_FAILURE);
        }
    }
}

/*
 * Potentially costly in event of code like [[[[[[]]]]]]
 * Dequeue would help but adds code complexity
 */
void while_loop(const auto &code, auto &vp, auto &index){
    int start = index;
    int found;
    stack<int> loops;
    loops.push(0);

    // find corresponding end bracket
    while(!loops.empty()){
        ++index;
        switch(code[index]){
            case '[':
                loops.push(0);
                break;
            case ']':
                found = index;
                loops.pop();
                break;
        }
    }

    // no safety against infinite loops
    while(*vp){
        interpret(code.substr(start, index-start));
    }
}

bool safe_check(const auto &code){
    stack<int> s;

    for(const auto &c : code){
        switch(c){
            case '[':
                s.push(0);
                break;
            case ']':
                if(s.empty()) return false;
                s.pop();
                break;
        }
    }

    return s.empty();
}

void print_help(){
    printf("\n");
    printf("%s version %s\n", NAME, VERSION);
    printf("%s [options] file\n\n", NAME);
    printf("-h | --help: prints this message\n");
    printf("--safemode: verifies integrity of brainfuck loops before execution\n\n");
}