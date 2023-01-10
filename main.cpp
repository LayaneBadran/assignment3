#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>

void process_pbm(std::string filepath, std::string name) {
    std::ifstream pbm;

    pbm.open(filepath);
    
    if (!pbm) { // checks that the file opened properly
        std::cerr << "Unable to open " << filepath;
        exit(1);
    }

    std::string line;
    std::string header = "P1";

    std::getline(pbm, line);
    if (line != header) { // reads the first line and makes sure it's "P1" which represents pbm files
        std::cerr << "Unable to find 'P1' header text";
        exit(1);
    }

    int height;
    int width;
    std::getline(pbm, line);

    // we have a line of text with two values in it -- we can treat the line as a string stream and parse it with getline()
    std::istringstream linestream(line);
    std::string word;
    std::getline(linestream, word, ' ');
    width = std::stoi(word);
    std::getline(linestream, word);
    height = std::stoi(word);
    
    //margins
    const int LEFT = 85;
    const int TOP = 570;
    const int RIGHT = 61;
    const int BOTTOM = 487;

    const int NUM_COLS = 30;
    const int NUM_ROWS = 16;
    
    int cell_size = (width - RIGHT - LEFT) / NUM_COLS; // we can calculate cell_size with only the width since each cell is a square
    //bool array[2][2][2] fits in stack memory! But bool array[480][98][98] does not. 
    //instead of using the heap with the "new" command, we can just create a vector so we don't have to worry about memory cleanup
    
    std::vector < std::vector < std::vector < std::vector < int > > > > image_array\
    (NUM_ROWS, std::vector < std::vector < std::vector < int > > > (NUM_COLS, std::vector < std::vector < int > > (cell_size, std::vector < int > (cell_size, 0))));

    int character_number = 0;
    int repetition_number = 0;

    //for each row we append onto the applicable column
    for (int i = 0; i < height; i++) {
        std::getline(pbm, line);

        if (i <= BOTTOM || (i >= (height - BOTTOM - TOP))) {
            continue;  // ignores top/bottom margins with no content
        }

        std::istringstream linestream(line);
        
        character_number = ((i - BOTTOM) - ((i - BOTTOM) % cell_size)) / cell_size;

        for (int j = 0; j < width; j++) {
            std::getline(linestream, word, ' ');

            if (j <= LEFT || (j >= (width - LEFT - RIGHT))) {
                continue;  // ignores left/right margins with no content
            }

            repetition_number = ((j - LEFT) - ((j - LEFT) % cell_size)) / cell_size;
            image_array[character_number][repetition_number][(i - BOTTOM) % cell_size][(j - LEFT) % cell_size] = std::stoi(word);
        }
            
    }

    for(int a = 0; a < cell_size; a++)
    {
        for(int b = 0; b < cell_size; b++)
        {
            std::cout << image_array[0][0][a][b] << " ";
        }
        std::cout << "\n";
    }

    pbm.close();
}

// the issue we have is that we can read the file but it's skewed so it's difficult to perform calculations. Cells are not square, and margins are uneven.

int main() {
    process_pbm(".\\dila.pbm", "dila");
    return 0;
}