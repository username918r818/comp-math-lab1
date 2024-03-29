#include <iostream>
#include <vector>

void makeDiagMatrix(std::vector<std::vector<double>>& a, std::vector<int>& indX);
void printMatrix(const std::vector<std::vector<double>>& a);
bool check(const std::vector<std::vector<double>>& a);
std::vector<double> calculate(const std::vector<std::vector<double>>& c, const std::vector<std::vector<double>>& d, const std::vector<std::vector<double>>& x);
double checkPrecision(const std::vector<std::vector<double>>& x);
std::vector<int> get_reverse_permutation(const std::vector<int>& permutation);
std::vector<std::vector<double>> randomGenerator(int m, int n);
std::vector<std::vector<double>> cheatMakeDiagMatrix(const std::vector<std::vector<double>>& a);


double abs(double a) {
    return a < 0 ? -a : a;
}

int main() {
    char read_from_file;
    std::cout << "Read from file? (Y/n)\n";
    std::cin >> read_from_file;
    if (read_from_file == 'Y' || read_from_file == 'y') {
        if (freopen("input.txt", "r", stdin) == nullptr) {
            std::cerr << "Failed to open input file." << std::endl;
            return 1; // or handle the error in an appropriate way
        }
        if (freopen("output.txt", "w", stdout) == nullptr) {
            std::cerr << "Failed to open output file." << std::endl;
            return 1; // or handle the error in an appropriate way
        }
    }
    double precision;
    std::cout << "Enter precision\n";
    std::cin >> precision;

    int n;
    std::cout << "Enter n\n";
    std::cin >> n;

    std::vector<std::vector<double>> a(n, std::vector<double>(n));
    std::vector<std::vector<double>> b(1, std::vector<double>(n));

    std::cout << "Enter matrix A\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            std::cin >> a[j][i];
        }
    }

    std::cout << "Enter matrix B\n";
    for (int i = 0; i < n; i++) {
        std::cin >> b[0][i];
    }

    #ifdef DEBUG
    n = 20;
    a = randomGenerator(n, n);
    a = cheatMakeDiagMatrix(a);
    b = randomGenerator(1, n);
    std::cout << "DEBUGINFO\n";
    std::cout << "Matrix A\n";
    printMatrix(a);
    std::cout << "Matrix B\n";
    for (int i = 0; i < n; i++) {
        std::cout << b[0][i] << " ";
    }
    std::cout << std::endl;
    #endif

    std::vector<int> indX(n);
    for (int i = 0; i < n; i++) {
        indX[i] = i;
    }

    makeDiagMatrix(a, indX);
    bool breakFlag = !check(a);
    if (breakFlag) {
        std::cout << "no diagonal dominance\n";
        return 0;
    }

    std::vector<std::vector<double>> c(n, std::vector<double>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) {
                c[i][j] = 0;
                continue;
            }
            c[i][j] = -a[i][j] / a[i][i];
        }
    }

    std::vector<std::vector<double>> d(1, std::vector<double>(n));
    for (int i = 0; i < n; i++) {
        d[0][i] = b[0][i] / a[i][i];
    }

    std::vector<std::vector<double>> x;
    int counter = 0;
    double max = 0;
    x.push_back(d[0]);
    do {
        x.push_back(calculate(c, d, x));
        counter++;
        max = checkPrecision(x);
    } while (max > precision);


    
    std::cout << "Result: x\n";
    std::vector<double> result(n);
    for (int i = 0; i < n; i++) {
        result[indX[i]] = x[counter][i];
    }
    for (int i = 0; i < n; i++) {
        std::cout << result[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "Iterations: " << counter << "\n";
    std::cout << "Precision: " << max << "\n";


    #ifdef DEBUG
    std::cout << "DEBUGINFO\n"; 
    std::cout << "x_n\n";
    for (int i = 0; i < x.size(); i++) {
        for (int j = 0; j < n; j++) {
            std::cout << x[i][j] << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "indX\n";
    for (int i = 0; i < n; i++) {
        std::cout << indX[i] << " ";
    }

    std::cout << std::endl;
    std::cout << "c\n";
    printMatrix(c);
    std::cout << "d\n";
    for (int i = 0; i < n; i++) {
        std::cout << d[0][i] << " ";
    }
    std::cout << std::endl;
    #endif

    return 0;
}

void makeDiagMatrix(std::vector<std::vector<double>>& a, std::vector<int>& indX) {
    int n = a.size();

    for (int i = 0; i < n; i++) { // проходим по строкам
        double max = a[i][i];

        int maxIndex = i;
        for (int j = 0; j < n; j++) { // по столбцам
            if (a[j][i] > max) {
                max = a[j][i];
                maxIndex = j;
            }
        }
        if (maxIndex != i) {
            int tmp = indX[i];
            indX[i] = indX[maxIndex];
            indX[maxIndex] = tmp;
            for (int j = 0; j < n; j++) {
                double tmp2 = a[maxIndex][j];
                a[maxIndex][j] = a[i][j];
                a[i][j] = tmp2;
            }
        }
    }
}

void printMatrix(const std::vector<std::vector<double>>& a) {
    int n = a.size();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            std::cout << a[j][i] << " ";
        }
        std::cout << std::endl;
    }
}

bool check(const std::vector<std::vector<double>>& a) {
    int n = a.size();
    for (int i = 0; i < n; i++) {
        double sum = -a[i][i];
        for (int j = 0; j < n; j++) {
            sum += a[j][i];
        }
        if (a[i][i] < sum) {
            return false;
        }
    }
    return true;
}

std::vector<double> calculate(const std::vector<std::vector<double>>& c, const std::vector<std::vector<double>>& d, const std::vector<std::vector<double>>& x) {
    int n = c.size();
    int counter = x.size();
    std::vector<double> newX(n);
    for (int i = 0; i < n; i++) {
        newX[i] = d[0][i];
        for (int j = 0; j < n; j++) {
            newX[i] += c[j][i] * x[counter - 1][j];
        }
    }

    return newX;
}

double checkPrecision(const std::vector<std::vector<double>>& x) {
    int n_x = x.size();
    int n = x[0].size();
    double max = 0;
    for (int i = 0; i < n; i++) {
        if (abs(x[n_x - 1][i] - x[n_x - 2][i]) > max) {
            max = abs(x[n_x - 1][i] - x[n_x - 2][i]);
        }
    }
    return max;
}

std::vector<int> get_reverse_permutation(const std::vector<int>& permutation) {
    int n = permutation.size();
    std::vector<int> reverse_permutation(n);
    for (int i = 0; i < n; i++) {
        // permutation[i]
    }
    return reverse_permutation;
}

std::vector<std::vector<double>> randomGenerator(int m, int n) {
    std::vector<std::vector<double>> a(m, std::vector<double>(n));
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            a[i][j] = rand() % 100;
        }
    }
    return a;
}

std::vector<std::vector<double>> cheatMakeDiagMatrix(const std::vector<std::vector<double>>& a) {
    int n = a.size();
    std::vector<std::vector<double>> c(n, std::vector<double>(n));
    double sum = 0;
    for (int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            c[i][j] = a[i][j];
            if (i!=j) {
                sum += abs(a[i][j]);
            }
        }
    }

    for (int i = 0; i < n; i++) {
        c[i][i] = sum + 1;
    }

    return c;
}