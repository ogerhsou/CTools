//  Copyright 2013 Google Inc. All Rights Reserved.
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <mm_malloc.h>

const long long max_size = 100;         // max length of strings
const long long N = 40;                  // number of closest words that will be shown

int main(int argc, char **argv) {
    FILE *f;
    long long int bestw[N];
    char file_name[max_size];
    float dist, len, bestd[N], vec[max_size];
    long long words, size, a, b, c, d;
    float *M;
    if (argc < 4) {
        printf("Usage: ./distance <FILE> words_num vector_size\nwhere FILE contains word projections in the BINARY FORMAT\n");
        return 0;
    }
    strcpy(file_name, argv[1]);
    f = fopen(file_name, "rb");
    if (f == NULL) {
        printf("Input file not found\n");
        return -1;
    }
//    words = 40619595;
    words = atoi(argv[2]);
    size = atoi(argv[3]);
    M = (float *)malloc((long long)words * (long long)size * sizeof(float));
    if (M == NULL) {
        printf("Cannot allocate memory: %lld MB    %lld  %lld\n", (long long)words * size * sizeof(float) / 1048576, words, size);
        return -1;
    }
    for (b = 0; b < words; b++) {
        for (a = 0; a < size; a++) /*fread(&M[a + b * size], sizeof(float), 1, f);*/
            fscanf(f, "%f", &M[a + b * size]);
        len = 0;
        for (a = 0; a < size; a++) len += M[a + b * size] * M[a + b * size];
        len = sqrt(len);
        for (a = 0; a < size; a++) M[a + b * size] /= len;
    }
    fclose(f);
    while (1) {
        for (a = 0; a < N; a++) bestd[a] = 0;
        for (a = 0; a < N; a++) bestw[a] = 0;
        printf("Enter id (-1 to break): ");
        a = 0;
        int input_id = -1;
        scanf("%d", &input_id);
        if (input_id == -1) break;
        if (input_id == -1) continue;
        printf("\n                                              Word       Cosine distance\n------------------------------------------------------------------------\n");
        for (a = 0; a < size; a++) vec[a] = 0;
        for (a = 0; a < size; a++) vec[a] += M[a + input_id * size];
        for (a = 0; a < N; a++) bestd[a] = -1;
        for (a = 0; a < N; a++) bestw[a] = 0;
        for (c = 0; c < words; c++) {
            if (input_id == c)
                continue;
            dist = 0;
            for (a = 0; a < size; a++) dist += vec[a] * M[a + c * size];
            for (a = 0; a < N; a++) {
                if (dist > bestd[a]) {
                    for (d = N - 1; d > a; d--) {
                        bestd[d] = bestd[d - 1];
                        bestw[d] = bestw[d - 1];
                    }
                    bestd[a] = dist;
                    bestw[a] = c;
                    break;
                }
            }
        }
        for (a = 0; a < N; a++) printf("%lld\t\t%f\n", bestw[a], bestd[a]);
    }
    return 0;
}
