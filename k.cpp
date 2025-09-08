#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <sstream>
using namespace std;

struct Customer {
    double income;
    double spending;
    int cluster; // cluster id
};

struct Centroid {
    double income;
    double spending;
};

// Euclidean distance between customer and centroid
double distance(const Customer& c, const Centroid& cent) {
    return sqrt(pow(c.income - cent.income, 2) + pow(c.spending - cent.spending, 2));
}

// K-Means algorithm
void kMeans(vector<Customer>& customers, int k, int maxIterations = 100) {
    int n = customers.size();
    vector<Centroid> centroids(k);

    // Random initialization of centroids
    srand(time(0));
    for (int i = 0; i < k; i++) {
        int idx = rand() % n;
        centroids[i].income = customers[idx].income;
        centroids[i].spending = customers[idx].spending;
    }

    for (int iter = 0; iter < maxIterations; iter++) {
        bool changed = false;

        // Step 1: Assign each customer to nearest centroid
        for (int i = 0; i < n; i++) {
            double minDist = 1e9;
            int clusterId = -1;
            for (int j = 0; j < k; j++) {
                double d = distance(customers[i], centroids[j]);
                if (d < minDist) {
                    minDist = d;
                    clusterId = j;
                }
            }
            if (customers[i].cluster != clusterId) {
                customers[i].cluster = clusterId;
                changed = true;
            }
        }

        if (!changed) break; // converged

        // Step 2: Update centroids
        vector<double> sumIncome(k, 0), sumSpending(k, 0);
        vector<int> count(k, 0);

        for (int i = 0; i < n; i++) {
            int c = customers[i].cluster;
            sumIncome[c] += customers[i].income;
            sumSpending[c] += customers[i].spending;
            count[c]++;
        }

        for (int j = 0; j < k; j++) {
            if (count[j] > 0) {
                centroids[j].income = sumIncome[j] / count[j];
                centroids[j].spending = sumSpending[j] / count[j];
            }
        }
    }

    // Print final clusters
    cout << "Final Clusters:\n";
    for (int i = 0; i < n; i++) {
        cout << "Customer " << i+1 << " (Income=" << customers[i].income
             << ", Spending=" << customers[i].spending
             << ") -> Cluster " << customers[i].cluster << "\n";
    }

    cout << "\nCentroids:\n";
    for (int j = 0; j < k; j++) {
        cout << "Cluster " << j << ": Income=" << centroids[j].income
             << ", Spending=" << centroids[j].spending << "\n";
    }
}

// Load data from CSV (Income,Spending)
vector<Customer> loadData(const string& filename) {
    vector<Customer> customers;
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string incomeStr, spendingStr;
        if (getline(ss, incomeStr, ',') && getline(ss, spendingStr, ',')) {
            Customer c;
            c.income = stod(incomeStr);
            c.spending = stod(spendingStr);
            c.cluster = -1;
            customers.push_back(c);
        }
    }
    return customers;
}

int main() {
    string filename = "customers.csv"; // CSV with Income,Spending
    vector<Customer> customers = loadData(filename);

    if (customers.empty()) {
        cout << "No data found. Please provide customers.csv\n";
        return 0;
    }

    int k = 3; // number of clusters
    kMeans(customers, k);

    return 0;
}
