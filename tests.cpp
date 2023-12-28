#include <iostream>
#include <vector>
#include "playground.hpp"
#include "player.hpp"

#define TEST_SUCCESS EXIT_SUCCESS
#define TEST_FAILURE EXIT_FAILURE
#define TEST_VISUAL_SUCCESS 0b100
#define TEST_VISUAL_FAILURE 0b010

#define TEST_IS_VISUAL(X) (bool)(X & ~0b1)

using namespace std;

void gp(playground& g, bool less)
{
    if (!less) printf("board view:\n");
    for (int y = 0; y < 9; y++)
    {
        for (int x = 0; x < 9; x++)
        {
            unsigned char c = g.get_cell(coord_to_pos(x, y));
            char ctc = cell_to_char(c);
            if (ctc == ' ') ctc = '_';
            putchar(ctc);
        }
        putchar('\n');
    }
    putchar('\n');
    if (less) return;
    printf("bits view:\n");
    printf("cell to char  |  byte\n");
    for (int i = 0; i < PLAYGROUND_BYTES; i++) {
        unsigned char gb = *(g.get_ground() + i);

        for (int b = 0; b < sizeof(unsigned char) * 8; b += 2) {
            putchar(cell_to_char((gb >> b) & (unsigned char)0b11));
            putchar(' ');
        }
        printf("      |  ");
        for (int b = 0; b < sizeof(unsigned char) * 8; b += 2) {
            putchar(((gb >>  b   ) & (unsigned char)0b1) ? '1' : '0');
            putchar(((gb >> (b+1)) & (unsigned char)0b1) ? '1' : '0');
            putchar(' ');
        }
        putchar('\n');
    }
}
int playground_get_set_test(bool visual)
{
    playground ground = playground();
    if (visual) goto vis;
    return TEST_VISUAL_FAILURE;


    // for (unsigned char fill = 0b00; fill <= 0b11; fill++)
    //     for (int y = 0; y < 9; y++)
    //         for (int x = 0; x < 9; x++)
    //         {
    //             ground.set_cell(x, y, fill);
    //             for (int yy = 0; yy < 9; yy++)
    //                 for (int xx = 0; xx < 9; xx++)
    //                 {
    //                     unsigned char cc = ground.get_cell(xx, yy);
    //                     if (xx == x && yy == x) {
    //                         if (cc != fill) return TEST_FAILURE;
    //                     }
    //                     else if (cc != CELL_SPACE) return TEST_FAILURE;
    //                 }
    //             ground.set_cell(x, y, CELL_SPACE);
    //         }

    // return TEST_SUCCESS;



    vis:
    printf("press any key to see playground get set test!\n");
    for (unsigned char fill = 0b01; fill < 0b100; fill--)
        for (int y = 0; y < 9; y++)
            for (int x = 0; x < 9; x++)
            {
                std::cin.get();
                ground.set_cell(coord_to_pos(x, y), fill);
                system("clear");
                gp(ground, false);
            }
            
    return TEST_VISUAL_SUCCESS;
}

// char test_for_cell_to_char(vector<cell_t>& arr) {
//     bool verify = cell_to_char(CELL_SPACE) == ' ';
//     for (const auto& elem : arr) {
//         switch (elem)
//         {
//         case CELL_SPACE:
//             verify = cell_to_char(CELL_SPACE) == ' '; 
//             return verify == 1 ? TEST_SUCCESS : TEST_FAILURE;
//             break;
//         case CELL_PL1:
//             verify = cell_to_char(CELL_PL1) == 'x';
//             return verify == 1 ? TEST_SUCCESS : TEST_FAILURE;
//             break;
//         case CELL_PL2:
//             verify = cell_to_char(CELL_PL2) == 'o';
//             return verify == 1 ? TEST_SUCCESS : TEST_FAILURE;
//             break;
//         case CELL_ERR:
//             verify = cell_to_char(CELL_ERR) == 'E';
//             return verify == 1 ? TEST_SUCCESS : TEST_FAILURE;
//             break;
//         default:
//             verify = cell_to_char(CELL_ERR) == '?';
//             return verify == 1 ? TEST_SUCCESS : TEST_FAILURE;
//             break;
//         }
//     }
// }



void test_for_move() {
    playground gp = playground();

    int count = 0;
    int success = 0;
    int fail = 0;

    for (int i = 0; i < PLAYGROUND_BYTES; i++) gp.get_ground()[i] = 0;
    // for (int i = 7; i < PLAYGROUND_BYTES - 10; i++) gp.get_ground()[i] = 2;
    // for (int i = 14; i < PLAYGROUND_BYTES; i++) gp.get_ground()[i] = 3;

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if(gp.move(j, i)) {
                printf("move %d & %d - success\n", j, i);
                success++;
            } else {
                printf("move %d & %d - fail\n", j, i);
                fail++;
            }
            count++;
            cout << count << endl;
        }
    }
    cout << "Количсетво успешных ходов: " << success << endl;
    cout << "Количсетво фейлов ходов: " << fail << endl;
}

void test_for_swap_players() {
    playground gp = playground();

    if ((cell_t)(gp.get_ground()[20] & 0b1100) == 4) {
        gp.swap_players();

        if ((cell_t)(gp.get_ground()[20] & 0b1100) == 8) {
            cout << "success" << endl;
        } else {
            cout << "fail" << endl;
        } 
    }
    else if ((cell_t)(gp.get_ground()[20] & 0b1100) == 8) {
        gp.swap_players();
        if ((cell_t)(gp.get_ground()[20] & 0b1100) == 4) {
            cout << "success" << endl;
        } else {
            cout << "fail" << endl;
        }
    } else {
        cout << "error" << endl;
    };
}


void test_for_get_who_moves() {
    playground gp = playground();

    cell_t res = gp.get_who_moves();
    gp.swap_players();

    if (res == 1 && gp.get_who_moves() == 2) {
        cout << "success" << endl;
    } else {
        cout << "fail" << endl;
    };
}



int playground_transform_test(bool visual) {
    if (!visual) return TEST_VISUAL_FAILURE;

    playground g = playground();
    g.set_cell(coord_to_pos(0, 0), CELL_PL1);
    g.set_cell(coord_to_pos(1, 1), CELL_PL1);
    g.set_cell(coord_to_pos(1, 0), CELL_PL1);
    g.set_cell(coord_to_pos(0, 1), CELL_PL1);

    g.set_cell(coord_to_pos(7, 7), CELL_ERR);
    g.set_cell(coord_to_pos(8, 8), CELL_ERR);
    g.set_cell(coord_to_pos(8, 7), CELL_ERR);
    g.set_cell(coord_to_pos(7, 8), CELL_ERR);

    g.set_cell(coord_to_pos(7, 0), CELL_PL2);
    g.set_cell(coord_to_pos(8, 1), CELL_PL2);
    g.set_cell(coord_to_pos(8, 0), CELL_PL2);
    g.set_cell(coord_to_pos(7, 1), CELL_PL2);

    g.set_cell(coord_to_pos(0, 7), CELL_ERR);
    g.set_cell(coord_to_pos(1, 8), CELL_ERR);
    g.set_cell(coord_to_pos(1, 7), CELL_ERR);
    g.set_cell(coord_to_pos(0, 8), CELL_ERR);


    printf("initial state:\n");
    gp(g, true);

    printf("press any key to see playground transform test\n");
    std::cin.get();
    for (unsigned char i = 0; i < 8; i++) {
        printf("%i-th transform:\n", (int)i);
        playground tg = transform(g, (int)i);
        gp(tg, true);
        std::cin.get();
    }
    return TEST_VISUAL_SUCCESS;
}





int main(int argc, char** argv)
{
    using std::vector;

    // vector<int (*)(bool)> tests = vector<int (*)(bool)>();
    
    // bool visual = true;


    // tests.push_back(playground_get_set_test);
    // tests.push_back(playground_transform_test);


    // int total = 0;
    // int failure = 0;
    // int failure_visual = 0;
    // for (int i = 0; i < tests.size(); i++) {
    //     total++;
    //     int result = tests[i](visual);
    //     if (result == TEST_FAILURE) {
    //         failure++;
    //         printf("test[%i] failure\n", i);
    //     }
    //     if (result == TEST_VISUAL_FAILURE) {
    //         failure_visual++;
    //         printf("test[%i] only is visual\n", i);
    //     }
    // }
    // printf("total: %i, %i failure\n", total, failure);

    // vector<cell_t> array_of_values;

    // array_of_values.push_back(CELL_SPACE);
    // array_of_values.push_back(CELL_PL2);
    // array_of_values.push_back(CELL_PL1);
    // array_of_values.push_back(CELL_ERR);

    // test_for_cell_to_char(array_of_values);

    test_for_get_who_moves();
    test_for_swap_players();
    test_for_move();

}