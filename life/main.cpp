#include <iostream>
#include <unistd.h>

void print_field(bool **array, const int width, const int height) {
    // функция для вывода двумерного массива на экран
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {

            char output = 0;
            if (array[i][j] == 0) {
                output = '-';
            } else {
                output = '*';
            }
            std::cout << output << ' ';
        }
        std::cout << std::endl;
    }
}

void fill_field(bool **array, const int width, const int height) {
    // функция для заполнения массива клеток живыми клетками
    // TODO доделать чтение данных из файла, пока хардкод
    int alive_count = 10;
    short **alive_positions = new short *[alive_count];
    for (int i = 0; i < alive_count; ++i) {
        alive_positions[i] = new short[alive_count];
    }
    alive_positions[0][0] = 2;
    alive_positions[0][1] = 3;
    alive_positions[1][0] = 2;
    alive_positions[1][1] = 4;
    alive_positions[2][0] = 3;
    alive_positions[2][1] = 4;
    alive_positions[3][0] = 3;
    alive_positions[3][1] = 5;
    alive_positions[4][0] = 3;
    alive_positions[4][1] = 6;
    alive_positions[5][0] = 3;
    alive_positions[5][1] = 7;
    alive_positions[6][0] = 5;
    alive_positions[6][1] = 0;
    alive_positions[7][0] = 5;
    alive_positions[7][1] = 1;
    alive_positions[8][0] = 5;
    alive_positions[8][1] = 2;
    alive_positions[9][0] = 6;
    alive_positions[9][1] = 2;

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            for (int pos = 0; pos < alive_count; ++pos) {
                if (i == alive_positions[pos][0] && j == alive_positions[pos][1]) {
                    array[i][j] = 1;
                }
            }
        }
    }

    for (int i = 0; i < alive_count; ++i) {
        delete[] alive_positions[i];
    }
    delete[] alive_positions;

}

int get_alive_count(bool **array, const int width, const int height) {
    // функция подсчета живых клеток на поле
    int count = 0;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (array[i][j] == 1) {
                count += 1;
            }
        }
    }
    return count;
}

bool has_alive(bool **array, const int width, const int height) {
    // функция проверки, есть ли живые клетки
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (array[i][j] == 1) {
                return true;
            }
        }
    }
    return false;
}

int count_neighbours(bool **array, const int width, const int height,
                     const int x_pos, const int y_pos) {
    // функция подсчета соседей клетки по ее координатам
    short neighbours_count = 0;

    int min_x, min_y, max_x, max_y;

    if (x_pos == 0) { min_x = x_pos; } else { min_x = x_pos - 1; }
    if (y_pos == 0) { min_y = y_pos; } else { min_y = y_pos - 1; }
    if (x_pos == height - 1) { max_x = x_pos; } else { max_x = x_pos + 1; }
    if (y_pos == width - 1) { max_y = y_pos; } else { max_y = y_pos + 1; }

    for (int i = min_x; i <= max_x; ++i) {
        for (int j = min_y; j <= max_y; ++j) {
            int neighbour_cell = array[i][j];
            if (neighbour_cell == 1) {
                neighbours_count += 1;
            }
        }
    }

    if (array[x_pos][y_pos] == 1) {
        return neighbours_count - 1;
    }
    return neighbours_count;
}

void collect_members_for_all_cells(bool **array, short **neighbours,
                                   const int width, const int height) {
    // функция для заполнения массива количества соседей актальными значениями
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            neighbours[i][j] = count_neighbours(array, width, height, i, j);
        }
    }
}

bool update_cells(bool **array, short **neighbours,
                  const int width, const int height) {
    // функция обновления состояния всех клеток на основе данных об их соседях
    bool changed = false;

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (array[i][j] == 0 & neighbours[i][j] == 3) {
                // рождение клетки
                array[i][j] = 1;
                changed = true;
            }
            if (array[i][j] == 1 & neighbours[i][j] != 2 & neighbours[i][j] != 3) {
                // смерть клетки
                array[i][j] = 0;
                changed = true;
            }
        }
    }
    return changed;
}

bool next_generation(bool **array, short **neighbours,
                     const int width, const int height) {
    // функция обработки очередного поколения
    // возвращает итог, были ли изменения за это поколение
    collect_members_for_all_cells(array, neighbours, width, height);
    bool updated = update_cells(array, neighbours, width, height);
    print_field(array, width, height);
    return updated;
}

void live(bool **array, short **neighbours,
          const int width, const int height) {
    // функция запуска жизни поколений
    int generation = 1;
    std::cout << "Generation: " << generation << ". Alive count: " << get_alive_count(array, width, height)
              << std::endl;


    while (has_alive(array, width, height)) {
        bool changed = next_generation(array, neighbours, width, height);
        generation += 1;
        std::cout << "Generation: " << generation << ". Alive count: " << get_alive_count(array, width, height)
                  << std::endl;
        if (!changed) {
            std::cout << "The world is stagnated. Game over" << std::endl;
            break;
        }
        sleep(1);
    }
    if (!has_alive(array, width, height)) {
        std::cout << "All cells are dead. Game over" << std::endl;
    }
}


int main(int argc, char **argv) {

    const int width = 30;
    const int height = 20;

    // создание двумерного массива клеток в динамической памяти
    bool **field = new bool *[height];
    for (int i = 0; i < height; ++i) {
        field[i] = new bool[width]();
    }

    // создание двумерного массива для хранения количества соседей клеток в динамической памяти
    short **neighbours = new short *[height];
    for (int i = 0; i < height; ++i) {
        neighbours[i] = new short[width];
    }

    // заполнение поля живыми клетками
    fill_field(field, width, height);

    // вывод поля на экран
    print_field(field, width, height);

    // запуск жизни
    live(field, neighbours, width, height);

    // удаление двумерного массива для хранения количества соседей клеток в динамической памяти
    for (int i = 0; i < height; ++i) {
        delete[] neighbours[i];
    }
    delete[] neighbours;

    // удаление двумерного массива клеток в динамической памяти
    for (int i = 0; i < height; ++i) {
        delete[] field[i];
    }
    delete[] field;

    return 0;
}
