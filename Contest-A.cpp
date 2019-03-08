#include <iostream>
#include <random>
#include <chrono>
#include <map>
#include <iomanip>
#include <limits.h>

using namespace std;

int findLowerBound(int step, int start, double value)
{

    for (int i = 0; ;++i)
    {
        if (i * step + start > value)
            return (i-1) * step + start;
    }
}

int main()
{
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count()); // Random number generator with seed time since bootup
    normal_distribution<> normalX(15000, 4500); // Normal distribution with Mean 15000, Std deviation 4500
    uniform_real_distribution<> uniformC1(0, 1), uniformC2(80, 100); //UniformC1 generate real value in [0, 1], uniformC2 generates real value in [80, 100]

    int nRolls = 1000000;
    int fixedCost = 1000000;
    map<int, int> histC1, histC2, histX, histProfit;
    int cntLoss = 0;
    double mx = -1e30, mn = 1e30, avg = 0;

    int stepProfit = 200000, startProfit = -1500000;
    int stepX = 1000, startX = 1500;
    for (int i = 0; i < nRolls; ++i)
    {
        double c1, c2, x, profit;

        c1 =uniformC1(rng);
        if (c1 <= 0.1)
            c1 = 43;
        else if (c1 <= 0.3)
            c1 = 44;
        else if (c1 <= 0.7)
            c1 = 45;
        else if (c1 <= 0.8)
            c1 = 46;
        else
            c1 = 47;
        histC1[(int)c1]++;

        c2 = uniformC2(rng);
        histC2[(int)c2]++;

        x = normalX(rng);
        if (x >= 1500 && x <= 28500)
            histX[findLowerBound(stepX, startX, x)]++;

        double pr = (249 - c1 - c2) * x - fixedCost;
        mx = max(mx, pr);
        mn = min(mn, pr);
        if (pr < 0)
            cntLoss++;
        avg += pr/nRolls;
        histProfit[findLowerBound(stepProfit, startProfit, pr)]++;

    }
    freopen("outputOneMillion.txt", "w", stdout);
    cout << fixed << setprecision(4);
    cout << "Probability of loss: " << 1.0 * cntLoss/ nRolls << '\n';
    cout << "Maximum Profit: " << mx << '\n';
    cout << "Maximum Loss: " << min(mn, 0.0) << '\n';
    cout << "Average Profit: " << avg << '\n';
    cout << "\n\n";

    int nStars = 500;
    cout << "Profit histogram: \n\n";
    for (int j = 0; j < 20; ++j)
    {
        int l = j * stepProfit + startProfit, r = (j + 1) * stepProfit + startProfit;
        cout << l  << " - " << setw(15) << r << ": ";
        int cntStars = 1.0 * histProfit[l]/nRolls * nStars;
        cout << string(cntStars, '*');
        cout << '\n';
    }
    cout << '\n';


    cout << "Demand per Year histogram: \n\n";
    for (int j = 0; j < 27; ++j)
    {
        int l = j * stepX + startX, r = (j+1) * stepX + startX;
        cout << setw(5) <<  l << " - " << setw(5) << r << ": ";
        int cntStars = 1.0 * histX[l]/nRolls * nStars;
        cout << string(cntStars, '*');
        cout << '\n';
    }
    cout << '\n';

    cout << "C1 Histogram: \n\n";
    for (int j = 43; j <= 47; ++j)
    {
        cout << j << ": ";
        cout << string(1.0 * histC1[j]/nRolls * nStars, '*');
        cout << '\n';
    }
    cout << '\n';

    cout << "C2 Histogram: \n\n";
    for (int j = 80; j < 100; ++j)
    {
        cout << j << "-" << setw(3) <<j+1 << ": ";
        cout << string(1.0 * histC2[j]/nRolls * nStars, '*');
        cout << '\n';
    }
    cout << '\n';

}