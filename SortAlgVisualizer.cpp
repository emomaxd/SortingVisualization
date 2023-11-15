#include "raylib.h"
#include <vector>
#include <sstream>
#include <algorithm>
#include <random>
#include <chrono>
#include <thread>
#include <iostream>

const int screenWidth = 800;
const int screenHeight = 450;
const int numOfSticks = 100;
int sticks[numOfSticks];
Color stickColors[numOfSticks];
int comparisons = 0;
int arrayAccesses = 0;

void Swap(int i, int j) {
    int temp = sticks[i];
    sticks[i] = sticks[j];
    sticks[j] = temp;
}

void DrawSticks(int selected, Color selectedColor) {
    int stickWidth = screenWidth / numOfSticks;

    for (int i = 0; i < numOfSticks; i++) {

        if (i == selected) {
            DrawRectangle(i * stickWidth, screenHeight - sticks[i], stickWidth, sticks[i], selectedColor);
        }
        else {
            DrawRectangle(i * stickWidth, screenHeight - sticks[i], stickWidth, sticks[i], stickColors[i]);
        }
    }
}


void BubbleSortVisualization() {
    

    for (int i = 0; i < numOfSticks - 1; i++) {
        for (int j = 0; j < numOfSticks - i - 1; j++) {
            ++comparisons;

            if (sticks[j] > sticks[j + 1]) {
                arrayAccesses += 4; // Four array accesses: 2 reads, 2 writes
                Swap(j, j + 1);

                // Draw and display counts
                BeginDrawing();
                ClearBackground(BLACK);
                std::stringstream comparisonText, accessText, totalText;
                comparisonText << "Comparisons: " << comparisons;
                accessText << "Array Accesses: " << arrayAccesses;
                totalText << "Total Elements: " << numOfSticks;
                DrawText(comparisonText.str().c_str(), 10, 10, 20, WHITE);
                DrawText(accessText.str().c_str(), 260, 10, 20, WHITE);
                DrawText(totalText.str().c_str(), 510, 10, 20, WHITE);
                DrawSticks(j + 1, RED);
                EndDrawing();

                for (int d = 0; d < 1000000; d++) {}
            }
        }
    }
}

int Partition(int low, int high) {
    int pivot = sticks[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (sticks[j] < pivot) {
            i++;
            Swap(i, j);
            ++comparisons;
            BeginDrawing();
            ClearBackground(BLACK);
            std::stringstream comparisonText, accessText, totalText;
            comparisonText << "Comparisons: " << comparisons;
            accessText << "Array Accesses: " << arrayAccesses;
            totalText << "Total Elements: " << numOfSticks;
            DrawText(comparisonText.str().c_str(), 10, 10, 20, WHITE);
            DrawText(accessText.str().c_str(), 260, 10, 20, WHITE);
            DrawText(totalText.str().c_str(), 510, 10, 20, WHITE);
            DrawSticks(j, WHITE);
            EndDrawing();

            for (int d = 0; d < 1000000; d++) {}
        }
    }
    Swap(i + 1, high);

    BeginDrawing();
    ClearBackground(BLACK);
    DrawSticks(high, RED);
    EndDrawing();

    for (int d = 0; d < 1000000; d++) {}
    arrayAccesses += 2;
    return (i + 1);
}



void QuickSortVisualization(int low, int high) {
    if (low < high) {
        int pivot = Partition(low, high);

        QuickSortVisualization(low, pivot - 1);
        QuickSortVisualization(pivot + 1, high);
    }
}


void Merge(int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    std::vector<int> L(n1);
    std::vector<int> R(n2);

    for (int i = 0; i < n1; i++)
        L[i] = sticks[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = sticks[mid + 1 + j];

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            sticks[k] = L[i];
            i++;
            ++comparisons;
        }
        else {
            sticks[k] = R[j];
            j++;
        }
        k++;

        BeginDrawing();
        ClearBackground(BLACK);
        std::stringstream comparisonText, accessText, totalText;
        comparisonText << "Comparisons: " << comparisons;
        accessText << "Array Accesses: " << arrayAccesses;
        totalText << "Total Elements: " << numOfSticks;
        DrawText(comparisonText.str().c_str(), 10, 10, 20, WHITE);
        DrawText(accessText.str().c_str(), 260, 10, 20, WHITE);
        DrawText(totalText.str().c_str(), 510, 10, 20, WHITE);
        DrawSticks(k - 1, WHITE);
        EndDrawing();

        

        for (int d = 0; d < 1000000; d++) {}
    }

    while (i < n1) {
        sticks[k] = L[i];
        i++;
        k++;
        ++comparisons;
        BeginDrawing();
        ClearBackground(BLACK);
        DrawSticks(k - 1, RED);
        EndDrawing();

        for (int d = 0; d < 1000000; d++) {}
    }

    while (j < n2) {
        sticks[k] = R[j];
        j++;
        k++;
        ++comparisons;
        BeginDrawing();
        ClearBackground(BLACK);
        DrawSticks(k - 1, RED);
        EndDrawing();

        for (int d = 0; d < 1000000; d++) {}
    }
    arrayAccesses += 14;
}



void MergeSortVisualization(int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        MergeSortVisualization(left, mid);
        MergeSortVisualization(mid + 1, right);

        Merge(left, mid, right);
    }
}



void shuffleSortedArray(int* sortedArray, int size) {
    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(sortedArray, sortedArray + size, g);
}


const float stickHeightGap = (float)screenHeight / numOfSticks;


int main() {
    InitWindow(screenWidth, screenHeight, "Sorting Algorithm Visualization");

    for (int i = 0; i < numOfSticks; i++) {
        sticks[i] = i * stickHeightGap;
        stickColors[i] = WHITE;
    }

    shuffleSortedArray(sticks, numOfSticks);

    SetTargetFPS(60);


    bool sortFinished = false;
    bool greenDrawed = false;
    int count = 0;
    int i = 0;

    std::chrono::steady_clock::time_point start;
    std::chrono::steady_clock::time_point end;
    std::chrono::duration<double> duration;

    while (!WindowShouldClose()) {

        BeginDrawing();
        ClearBackground(BLACK);

        std::stringstream comparisonText, accessText, totalText, timeText;
        comparisonText << "Comparisons: " << comparisons;
        accessText << "Array Accesses: " << arrayAccesses;
        totalText << "Total Elements: " << numOfSticks;
        timeText << "Time passed: " << duration.count();


        

        if (sortFinished) {

            if (!greenDrawed) {
                stickColors[i] = GREEN;
            }
            else {
                DrawText(comparisonText.str().c_str(), 10, 10, 20, WHITE);
                DrawText(accessText.str().c_str(), 10, 40, 20, WHITE);
                DrawText(totalText.str().c_str(), 10, 70, 20, WHITE);
                DrawText(timeText.str().c_str(), 10, 100, 20, WHITE);
                stickColors[i] = WHITE;
            }
            
            
        }

        else {
            start = std::chrono::high_resolution_clock::now();
            //BubbleSortVisualization();
            MergeSortVisualization(0, numOfSticks - 1);
            //QuickSortVisualization(0, numOfSticks - 1);
            end = std::chrono::high_resolution_clock::now();
            
        }
        duration = end - start;
        DrawSticks(-1, stickColors[i]);

        if (i < 100)
            ++i;
        else {
            greenDrawed = true;
            i = 0;
        }
            
        sortFinished = true;
        EndDrawing();
    }

    CloseWindow();
    return 0;
}