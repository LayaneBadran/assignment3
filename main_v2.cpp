#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>

void process_pbm(std::string filepath, std::string name, std::string member_id) {
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
    
    int cell_height = std::floor((height - TOP - BOTTOM) / (float)NUM_ROWS); // we don't have to use floor here, but it makes it clearer than integer division
    int cell_width = std::floor((width - LEFT - RIGHT) / (float)NUM_COLS); // we can't calculate cell_size with only the width since each cell is skewed in the picture
    
    // because our cells aren't perfectly square, we need to resize our page (eg ignore stuff that isn't divisible by our cell height/width)
    height = cell_height * NUM_ROWS + TOP;
    width = cell_width * NUM_COLS + LEFT;

    //bool array[2][2][2] fits in stack memory! But bool array[480][98][98] does not. 
    //instead of using the heap with the "new" command, we can just create a vector so we don't have to worry about memory cleanup
    
    std::vector < std::vector < std::vector < std::vector < int > > > > image_array\
    (NUM_ROWS, std::vector < std::vector < std::vector < int > > > (NUM_COLS, std::vector < std::vector < int > > (cell_height, std::vector < int > (cell_width, 2))));

    int character_number = 0;
    int repetition_number = 0;

    //for each row we append onto the applicable column
    for (int i = 0; i < height; i++) {
        std::getline(pbm, line);

        if (i <= TOP) {
            continue;  // ignores top margin with no content
        }

        std::istringstream linestream(line);

        if ((i - TOP) % cell_height == 0) {
            character_number ++;
        }

        repetition_number = 0;
        for (int j = 0; j < width; j++) {

            std::getline(linestream, word, ' ');

            if (j <= LEFT) {
                continue;  // ignores left margin with no content
            }

            if ((j - LEFT) % cell_width == 0) {
                repetition_number ++;
            }

            image_array[character_number][repetition_number][(i - TOP) % cell_height][(j - LEFT) % cell_width] = std::stoi(word);
        }
            
    }

    //testing that it works! It's a bit offset due to skewed paper but shows we are able to parse and partition each cell  

    // std::ofstream output_file;

    // for (int c = 0; c < NUM_ROWS; c++) {
    //     //std::string x = "./" + std::to_string(c);
    //     for(int r = 0; r < NUM_COLS; r++) {
    //         //output_file.open("./" + std::to_string(c) + "_" + member_id + "_" + std::to_string(r) + ".pbm", std::ios_base::app); // append instead of overwrite
    //         output_file << "P1\n" << width << " " << height << "\n";

    //         for(int i = 0; i < cell_height; i++) {
    //             for(int j = 0; j < cell_width; j++) {
    //                 output_file << image_array[c][r][i][j] << " ";
    //             }
    //             output_file << "\n";
    //         }
    //     }
    // }
    
    

    // we ran out of time to do the last part -- writing a loop to output files. But we showed that our array stores all the files we need!

    pbm.close();
}


int main() {
    process_pbm(".\\dila.pbm", "dila", 0);
    return 0;
}