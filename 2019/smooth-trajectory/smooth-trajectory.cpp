#include <iostream>
#include <cmath>
#include <cstdio>
#include <array>

#include <armadillo>

struct Params
{
    double a, b, c, d, e, f;
};

Params findParams(double T)
{
    std::array<double, 6> Ts;
    Ts[0] = 1;
    for (int i = 1; i < Ts.size(); ++i)
    {
        Ts[i] = Ts[i - 1] * T;
    }

    arma::Mat<double> A{
            {Ts[5], Ts[4], Ts[3]},
            {5 * Ts[4], 4 * Ts[3], 3 * Ts[2]},
            {20 * Ts[3], 12 * Ts[2], 6 * Ts[1]}};

    //std::cout << A << std::endl;

    arma::Col<double> B{1., 0., 0.};

    arma::Mat<double> sol = arma::solve(A, B);
    //std::cout << sol << std::endl;

    return {sol(0, 0), sol(1, 0), sol(2, 0), 0, 0};
}

int main()
{
    double T = 3.;
    //std::cin >> T;

    auto params = findParams(T);

    double dT = T / 100.;

    std::printf("%s\n", "plot '-' using 1:2 with lines");

    for (double x = 0; x < T; x += dT)
    {
        // plot position
        //auto t = std::pow(x, 5) * params.a + std::pow(x, 4) * params.b + std::pow(x, 3) * params.c;

        // plot speed
        auto t = 5 * params.a * pow(x, 4) + 4 * params.b * pow(x, 3) + 3 * params.c * pow(x, 2);

        // plot acceleration
        //auto t = 20 * params.a * pow(x, 3) + 12 * params.b * pow(x, 2) + 6 * params.c * x;

        std::printf("%f\t%f\n", x, t);
    }

    printf("%s\n", "e");

    return 0;
}
