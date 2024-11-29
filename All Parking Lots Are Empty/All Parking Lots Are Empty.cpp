#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iomanip> // For setw
#include <queue>   // For BFS

using namespace std;

// Color codes (for Linux/macOS terminal, adjustment may be required for Windows)
#define RESET "\033[0m"
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define BLUE "\033[1;34m"
#define YELLOW "\033[1;33m"

// Function Declarations
pair<int, int> getUserLocation(int size);
vector<vector<int>> generateParkingLot(int size);
void printParkingLot(const vector<vector<int>>& parkingLot, pair<int, int> bestSpot = { -1, -1 }, pair<int, int> userLocation = { -1, -1 });
pair<pair<int, int>, int> findNearestParking(const vector<vector<int>>& parkingLot, pair<int, int> userLocation);

int main() {
    int size = 10; // Parking lot size
    vector<vector<int>> parkingLot = generateParkingLot(size);

    printParkingLot(parkingLot);

    pair<int, int> userLocation = getUserLocation(size);
    auto result = findNearestParking(parkingLot, userLocation);

    if (result.first.first != -1) {
        cout << "Nearest available parking spot: (" << result.first.first + 1 << ", " << result.first.second + 1 << ")"
            << ", Distance: " << result.second << endl;
        cout << YELLOW << setw(3) << "Y" << RESET;
        cout << "=Your starting location." << endl;
        cout << BLUE << setw(3) << "A" << RESET;
        cout << "=Your available parking spot." << endl;
    }
    else {
        cout << "No available parking spots found." << endl;
    }

    cout << "\nVisualized parking lot map:\n";
    printParkingLot(parkingLot, result.first, userLocation);

    return 0;
}

// User Location Input
pair<int, int> getUserLocation(int size) {
    int x, y;
    while (true) {
        cout << "Enter the row of your starting location (1-" << size << "): ";
        cin >> y;
        cout << "Enter the column of your starting location (1-" << size << "): ";
        cin >> x;

        if (y >= 1 && y <= size && x >= 1 && x <= size) {
            return { y - 1, x - 1 }; // Convert 1-based coordinate to 0-based index
        }
        else {
            cout << "Invalid coordinates! Please try again.\n";
        }
    }
}

// Parking Lot Map and Occupancy Status
vector<vector<int>> generateParkingLot(int size) {
    vector<vector<int>> parkingLot(size, vector<int>(size));
    srand(time(0));

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            parkingLot[i][j] = 0;
        }
    }
    return parkingLot;
}

// Print Parking Lot Map (With Coordinate Labels)
void printParkingLot(const vector<vector<int>>& parkingLot, pair<int, int> bestSpot, pair<int, int> userLocation) {
    int size = parkingLot.size();

    // Horizontal coordinate headers (starts from 1)
    cout << "   ";
    for (int j = 1; j <= size; ++j) {
        cout << setw(3) << j;
    }
    cout << endl;

    // Print map row by row
    for (int i = 0; i < size; ++i) {
        cout << setw(3) << i + 1; // Vertical coordinate headers (starts from 1)
        for (int j = 0; j < size; ++j) {
            if (make_pair(i, j) == userLocation) {
                cout << YELLOW << setw(3) << "Y" << RESET; // User's starting point (yellow)
            }
            else if (make_pair(i, j) == bestSpot) {
                cout << BLUE << setw(3) << "A" << RESET; // Best parking spot (blue)
            }
            else if (parkingLot[i][j] == 1) {
                cout << RED << setw(3) << "X" << RESET; // Occupied parking spot (red)
            }
            else {
                cout << GREEN << setw(3) << "O" << RESET; // Available parking spot (green)
            }
        }
        cout << endl;
    }
}

pair<pair<int, int>, int> findNearestParking(const vector<vector<int>>& parkingLot, pair<int, int> userLocation) {
    int size = parkingLot.size();
    vector<vector<bool>> visited(size, vector<bool>(size, false)); // Tracks visited nodes
    queue<pair<pair<int, int>, int>> q; // Queue storing {position, distance}

    // Add the starting point to the queue
    q.push({ userLocation, 0 });
    visited[userLocation.first][userLocation.second] = true;

    // Movement directions: up, down, left, right
    int directions[4][2] = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };

    while (!q.empty()) {
        // Get an element from the queue
        pair<pair<int, int>, int> currentElement = q.front();
        q.pop();

        pair<int, int> position = currentElement.first; // Current coordinate
        int distance = currentElement.second;          // Current distance

        int y = position.first;  // Row
        int x = position.second; // Column

        // If this is an empty parking spot, return the result
        if (parkingLot[y][x] == 0) {
            return { position, distance };
        }

        // Visit neighbors
        for (auto& dir : directions) {
            int newY = y + dir[0];
            int newX = x + dir[1];

            // If valid and not visited, add to the queue
            if (newY >= 0 && newY < size && newX >= 0 && newX < size && !visited[newY][newX]) {
                visited[newY][newX] = true;
                q.push({ {newY, newX}, distance + 1 });
            }
        }
    }

    // If no empty parking spot is found
    return { {-1, -1}, -1 };
}