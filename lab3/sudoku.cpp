#include <stdio.h>
#include <stdlib.h>

using namespace std;

class CP_sudoku {
    private:
        int backtrace_num_ = 0;
    public:

        struct SudokuField {
            int field[9][9];
        };

        template <size_t rows, size_t cols>

        SudokuField *solve(int (&init_values)[rows][cols]){
            backtrace_num_ = 0;
		    SudokuField* sf = new SudokuField();
            
            // Init SudokuField
            for(int i = 0; i < 9; i++){
                for(int j = 0; j < 9; j++){
                    sf->field[i][j] = init_values[i][j];
                }
            }

            if(solve_(sf)){
                return sf;
            }

            else{
                return NULL;
            }
        }

        bool solve_(SudokuField* sf){

            backtrace_num_++;
            pair<int, int> empty = find_(sf);
            int row = empty.first;
            int col = empty.second;

            if (row == -1){
                return true;
            }

            for(int num = 1; num < 10; num++){
                if(valid_(sf, num, row, col)){

                    sf->field[row][col] = num;

                    if(solve_(sf)){
                        return true;
                    }

                    sf->field[row][col] = -1;
                }
            }

            return false;

        }

        bool valid_(SudokuField* sf, int num, int row, int col){

            // Check columns and rows
            for(int i = 0; i < 9; i++){
                if(sf->field[i][col] == num || sf->field[row][i] == num){
                    return false;
                }
            }

            // Check box
            int start_x = row / 3 * 3;
            int start_y = col / 3 * 3;

            for(int i = start_x; i < start_x + 3; i++){
                for(int j = start_y; j < start_y + 3; j++){
                    if(sf->field[i][j] == num){
                        return false;
                    }
                }
            }

            return true;
        }

        pair<int, int> find_(SudokuField* sf){
            bool found_empty = false;
            int min_possible_vals = 10;
            int current_possible_vals = 0;
            pair<int, int> solution;
            for(int i = 0; i < 9; i++){
                for(int j = 0; j < 9; j++){
                    if (sf->field[i][j] == -1){
                        found_empty = true;
                        current_possible_vals = 0;
                        for(int num=1; num < 10; num++){
                            if(valid_(sf, num, i, j)){
                                current_possible_vals++;
                            }
                        }
                        if (current_possible_vals < min_possible_vals){
                            min_possible_vals = current_possible_vals;
                            solution = make_pair(i, j);
                        }
                    }
                }
            }
            if(!found_empty){
                return make_pair(-1, -1);
            }
            return solution;
        }
        
        int getBacktraceCalls(){
            return backtrace_num_;
        }
};