#include <iostream>
#include <cmath>
#include <iomanip>
using namespace std;

// Функція рівняння
// f(x) = x*x + 4*sin(x)
double f(double x)
{
    return x * x + 4 * sin(x);
}

// Похідна функції
double df(double x)
{
    return 2 * x + 4 * cos(x);
}

// Створення масиву даних
double* createData()
{
    double* d = new double[10];
    for (int i = 0; i < 10; i++)
        *(d + i) = 0;
    return d;
}

// Введення меж та точності
double* inputData(double* d)
{
    cout << "Enter a, b and eps: ";
    if (!(cin >> *(d + 0) >> *(d + 1) >> *(d + 2)))
    {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Incorrect input." << endl;
        *(d + 9) = 0;
        return d;
    }

    *(d + 5) = 0;
    *(d + 6) = 0;
    *(d + 9) = 0;

    if (*(d + 0) >= *(d + 1) || *(d + 2) <= 0)
    {
        cout << "Incorrect interval or accuracy." << endl;
        return d;
    }

    if (f(*(d + 0)) * f(*(d + 1)) > 0)
    {
        cout << "No sign change at interval ends. Choose another interval." << endl;
        return d;
    }

    *(d + 9) = 1;
    cout << "Interval accepted." << endl;
    return d;
}

// Метод Ньютона
double* newton(double* d)
{
    if (*(d + 9) == 0)
    {
        cout << "Enter a valid interval first." << endl;
        return d;
    }

    double a = *(d + 0);
    double b = *(d + 1);
    double eps = *(d + 2);
    double x;

    // Початкова точка ближча до кореня за |f(x)|
    if (fabs(f(a)) <= fabs(f(b)))
        x = a;
    else
        x = b;

    double mid = (a + b) / 2;
    if (fabs(f(mid)) < fabs(f(x)) && fabs(df(mid)) > 1e-10)
        x = mid;

    int k = 0;
    bool ok = false;

    cout << "\nNewton method:" << endl;
    for (int i = 0; i < 200; i++)
    {
        cout << "Step " << i << ": x = " << x << ", f(x) = " << f(x) << endl;

        if (fabs(f(x)) <= eps)
        {
            ok = true;
            break;
        }

        double p = df(x);
        if (fabs(p) < 1e-12)
        {
            cout << "Derivative is too close to zero." << endl;
            break;
        }

        double next = x - f(x) / p;
        if (!isfinite(next) || next < a || next > b)
        {
            cout << "Newton method left the interval. Choose a narrower interval." << endl;
            break;
        }

        x = next;
        k++;
    }

    if (ok)
    {
        *(d + 3) = x;
        *(d + 5) = 1;
        *(d + 7) = k;
        cout << "Newton root = " << x << endl;
        cout << "Iterations = " << k << endl;
    }
    else
    {
        *(d + 5) = 0;
        cout << "Newton method did not find a root." << endl;
    }

    return d;
}

// Метод половинного ділення
double* bisection(double* d)
{
    if (*(d + 9) == 0)
    {
        cout << "Enter a valid interval first." << endl;
        return d;
    }

    double l = *(d + 0);
    double r = *(d + 1);
    double eps = *(d + 2);
    double x = l;
    int k = 0;
    bool ok = false;

    cout << "\nBisection method:" << endl;

    if (fabs(f(l)) <= eps)
    {
        x = l;
        ok = true;
    }
    else if (fabs(f(r)) <= eps)
    {
        x = r;
        ok = true;
    }
    else
    {
        for (int i = 0; i < 200; i++)
        {
            x = (l + r) / 2;
            k++;
            cout << "Step " << k << ": x = " << x << ", f(x) = " << f(x) << endl;

            if (fabs(f(x)) <= eps && (r - l) / 2 <= eps)
            {
                ok = true;
                break;
            }

            if (f(l) * f(x) <= 0)
                r = x;
            else
                l = x;
        }
    }

    if (ok)
    {
        *(d + 4) = x;
        *(d + 6) = 1;
        *(d + 8) = k;
        cout << "Bisection root = " << x << endl;
        cout << "Iterations = " << k << endl;
    }
    else
    {
        *(d + 6) = 0;
        cout << "Bisection method did not find a root." << endl;
    }

    return d;
}

// Перевірка та порівняння
double* compare(double* d)
{
    if (*(d + 5) == 0 || *(d + 6) == 0)
    {
        cout << "Run both methods first." << endl;
        return d;
    }

    cout << "\nCheck results:" << endl;
    cout << "Newton root = " << *(d + 3) << endl;
    cout << "f(Newton root) = " << f(*(d + 3)) << endl;
    cout << "Bisection root = " << *(d + 4) << endl;
    cout << "f(Bisection root) = " << f(*(d + 4)) << endl;
    cout << "Difference = " << fabs(*(d + 3) - *(d + 4)) << endl;
    return d;
}

int main()
{
    cout << "Laboratory work 8, variant 23" << endl;
    cout << "Student: Sarzhevskyi Daniil Oleksandrovych" << endl;
    cout << "Group: IPZ-12(6)" << endl;

    double* d = createData();
    double* (*command)(double*) = nullptr; // Покажчик на функцію
    int choice;
    cout << fixed << setprecision(10);

    do
    {
        cout << "\nMENU" << endl;
        cout << "1 - Enter interval and accuracy" << endl;
        cout << "2 - Newton method" << endl;
        cout << "3 - Bisection method" << endl;
        cout << "4 - Check and compare" << endl;
        cout << "0 - Exit" << endl;
        cout << "Choose: ";

        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Incorrect choice." << endl;
            continue;
        }

        command = nullptr;
        if (choice == 1) command = inputData;
        else if (choice == 2) command = newton;
        else if (choice == 3) command = bisection;
        else if (choice == 4) command = compare;
        else if (choice != 0) cout << "Incorrect choice." << endl;

        if (command != nullptr)
            d = command(d); // Виклик через покажчик на функцію
    } while (choice != 0);

    delete[] d; // Звільнення пам'яті
    return 0;
}
