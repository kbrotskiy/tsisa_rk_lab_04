#include <iostream>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <iomanip>
#include <vector>

const double x_down = 0.;
const double x_up = 2.;
const double y_down = -2.;
const double y_up = 2.;
const int count_point = 6;
const double percent_mutations = .25;

struct Point {
    double x;
    double y;
};

using Population = std::vector<Point>;

double Function(const double x, const double y) {
    return sin(x) * sin(x) / (1 + x*x + y*y);
}

double Random(const double down, const double up) {
    return down + rand() * 1. / RAND_MAX * (up - down);
}

bool operator <(const Point& lhs, const Point& rhs) {
    return Function(lhs.x, lhs.y) < Function(rhs.x, rhs.y);
}

Point random_of_Point(const double x_Down, const double x_Up,
                      const double y_Down, const double y_Up) {
    return {Random(x_Down, x_Up), Random(y_Down, y_Up)};
}

void print_Border() {
    std::cout << '+' << std::string(5, '-') << '+' << std::string(12, '-')
              << '+' << std::string(12, '-') << '+' << std::string(12, '-')
              << '+' << std::string(12, '-') << '+' << std::string(12, '-')
              << '+' << std::endl;
}

void printHeadTable() {
    print_Border();
    std::cout << '|' << "  N  " << '|' << "     X      "
              << '|' << "     Y      " << '|' << "    FIT     "
              << '|' << "    MAX     " << '|' << "  AVERAGE   "
              << '|' << std::endl;
    print_Border();
}

void printPopulation(const int generation, const Population& population) {
    for (size_t i = 0; i < population.size(); ++i) {
        if (i == 0) {
            std::cout << '|' << std::setw(4) << generation << " | ";
        } else {
            std::cout << '|' << std::setw(7) << " | ";
        }
        std::cout << std::setw(10) << population[i].x << " | "
                  << std::setw(10) << population[i].y << " | "
                  << std::setw(10) << Function(population[i].x, population[i].y) << " | ";
        if (i == 0) {
            double max = Function(population[0].x, population[0].y), average = 0;
            for (auto i : population) {
                if (Function(i.x, i.y) > max) max = Function(i.x, i.y);
                average += Function(i.x, i.y);
            }
            average /= population.size();
            std::cout << std::setw(10) << max << " | "
                      << std::setw(10) << average << " |\n";
        } else {
            std::cout << std::setw(12) << '|' << std::setw(14) << "|\n";
        }
    }
    print_Border();
}

void Algorithm_of_genetics(const double x_Down, const double x_Up,
                           const double y_Down, const double y_Up,
                           const int pointsNumber, const int new_generate_Population,
                           const double mutationProbability) {
    Population Populat;
    for (size_t i = 0; i < pointsNumber; ++i) {
        Populat.push_back(random_of_Point(x_Down, x_Up, y_Down, y_Up));
    }
    std::sort(Populat.rbegin(), Populat.rend());
    const double eps = .5;

    printHeadTable();
    for(int i = 1; i <= new_generate_Population; ++i) {
        printPopulation(i - 1, Populat);
        Population New_population;
        New_population.push_back(Point{Populat[0].x, Populat[1].y});
        New_population.push_back(Point{Populat[1].x, Populat[0].y});
        New_population.push_back(Point{Populat[0].x, Populat[2].y});
        New_population.push_back(Point{Populat[2].x, Populat[0].y});
        New_population.push_back(Point{Populat[0].x, Populat[3].y});
        New_population.push_back(Point{Populat[3].x, Populat[0].y});
        for (size_t j = 0; j < pointsNumber; ++j) {
            double probability = Random(0., 1.);
            if (probability < mutationProbability) {
                double deltaX = Random(-eps / 2, eps / 2);
                double deltaY = Random(-eps / 2, eps / 2);
                New_population[j].x += deltaX;
                if (New_population[j].x < x_Down) New_population[j].x = x_Down;
                if (New_population[j].x > x_Up) New_population[j].x = x_Up;
                New_population[j].y += deltaY;
                if (New_population[j].y < y_Down) New_population[j].y = y_Down;
                if (New_population[j].y > y_Up) New_population[j].y = y_Up;
            }
        }
        std::sort(New_population.rbegin(), New_population.rend());
        Populat = New_population;
    }
    printPopulation(new_generate_Population, Populat);
}


int main() {
    std::cout << "Вариант 10. \nФункция: sin(x)^2 / (1 + x^2 + y^2), область допустимых значений D: ("
              << x_down << ", " << x_up << ") Х (" << y_down << ", " << y_up << ")\n";
    std::cout << "  Вероятность мутации: 25%\n";
    std::cout << "  Количество точек: 6\n";
    srand(time(nullptr));
    std::cout << "                             Для 10 поколений:\n";
    Algorithm_of_genetics(x_down, x_up, y_down, y_up, count_point,
                          10, percent_mutations);
    std::cout << "\n\n\n\n";
    std::cout << "                             Для 100 поколений:\n";
    Algorithm_of_genetics(x_down, x_up, y_down, y_up, count_point,
                          100, percent_mutations);
    return 0;
}