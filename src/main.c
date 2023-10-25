#include "api/api.h"

void draw(int *arr)
{
    for (unsigned int i = 0; i < HEIGHT_NUM; i++) {
        for (unsigned int j = 0; j < WIDTH_NUM; j++) {
            setPixel(j, i, arr[i * WIDTH_NUM + j]);
        }
    }
}

void calc(int *prev_gen, int *curr_gen)
{
    for (int i = 0; i < HEIGHT_NUM; i++) {
        for (int j = 0; j < WIDTH_NUM; j++) {
            int prev_status = prev_gen[i * WIDTH_NUM + j];
            int neighbours = prev_status == 1 ? -1 : 0;
            for (int neighbour_i = -1; neighbour_i <= 1; neighbour_i++) {
                for (int neighbour_j = -1; neighbour_j <= 1; neighbour_j++) {
                    neighbours += prev_gen[((i + neighbour_i + HEIGHT_NUM) % HEIGHT_NUM) * WIDTH_NUM +
                                           (j + neighbour_j + WIDTH_NUM) % WIDTH_NUM];
                }
            }
            int is_alive = 0;
            if (neighbours == 3) {
                is_alive = 1;
            }
            if (neighbours == 2 && prev_status == 1) {
                is_alive = 1;
            }
            curr_gen[i * WIDTH_NUM + j] = is_alive;
        }
    }
}

int main(int argc, char *argv[])
{
    int prev_gen_arr[HEIGHT_NUM * WIDTH_NUM];
    int curr_gen_arr[HEIGHT_NUM * WIDTH_NUM];
    int *prev_gen = prev_gen_arr;
    int *curr_gen = curr_gen_arr;

    for (unsigned int i = 0; i < HEIGHT_NUM; i++) {
        for (unsigned int j = 0; j < WIDTH_NUM; j++) {
            prev_gen[i * WIDTH_NUM + j] = generate() % 2;
        }
    }
    init(WIDTH_NUM, HEIGHT_NUM);
    draw(prev_gen);
    display();
    while (1) {
        calc(prev_gen, curr_gen);
        draw(curr_gen);
        display();
        int *tmp = prev_gen;
        prev_gen = curr_gen;
        curr_gen = tmp;
    }
    return 0;
}