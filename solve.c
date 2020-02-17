#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

int max;
int n;
int n8;
int sum;
int *numbers;

void readInput(char *path) {
  FILE *file = fopen(path, "r");
  fscanf(file, "%d %d\n", &max, &n);
  n8 = (n + 7) / 8;
  numbers = malloc(sizeof(int) * n);
  fscanf(file, "%d", numbers);
  for (int i = 1; i < n; i++) {
    int *ptr = numbers + i;
    fscanf(file, " %d", ptr);
    sum += *ptr;
  }
  fclose(file);
}

typedef struct SResult {
  int sum, count;
  char *bytes;
} Result;

Result *Result_new() {
  Result *r = (Result *) malloc(sizeof(Result));
  r->sum = 0;
  r->count = 0;
  r->bytes = malloc(n8 * sizeof(char));
  return r;
}

void Result_free(Result *r) {
  free(r->bytes);
  free(r);
}

int Result_has(Result *r, int i) {
  return (r->bytes[i / 8] >> (i % 8)) & 1;
}

void Result_clear(Result *r) {
  r->count = 0;
  r->sum = 0;
  memset(r->bytes, 0, (n + 7) / 8);
}

void Result_init(Result *r) {
  for (int i = 0; i < n; i++) {
    if (rand() % 2 == 1) {
      r->count++;
      r->sum += numbers[i];
      r->bytes[i / 8] |= 1 << i % 8;
    }
  }
}

void Result_output(Result *r) {
  printf("%d\n", r->count);
  int first = 1;
  for (int i = 0; i < n; i++) {
    if (Result_has(r, i)) {
      if (first) {
        first = 0;
      } else {
        printf(" ");
      }
      printf("%d", i);
    }
  }
  printf("\n");
}

void Result_clone(Result *src, Result *dst) {
  memcpy(dst->bytes, src->bytes, n8);
  dst->sum = src->sum;
  dst->count = src->count;
}

void solve() {
  Result *r = Result_new();
  Result *best = Result_new();
  Result_clear(best);
  while (best->sum != max - 1) {
    printf("reset\n");
    Result_clear(r);
    Result_init(r);
    for (int i = 0; i < 100000; i++) {
      for (int i = 0; i < 100000 && best->sum != max - 1; i++) {
        int i;
        if (r->sum < max) {
          do {
            i = rand() % n;
          } while (Result_has(r, i));
          r->bytes[i / 8] |= 1 << i % 8;
          r->sum += numbers[i];
          r->count += 1;
        } else {
          do {
            i = rand() % n;
          } while (!Result_has(r, i));
          r->bytes[i / 8] &= ~(1 << i % 8);
          r->sum -= numbers[i];
          r->count -= 1;
        }
        if (r->sum > best->sum && r->sum < max) {
          Result_clone(r, best);
        }
      }
    }
    Result_clone(best, r);
  }
  Result_clone(best, r);
  Result_output(best);
  printf("sum: %d\n", best->sum);
  Result_free(r);
  Result_free(best);
}

int main() {
  char *inputs[5];
  inputs[0] = "a_example.in";
  inputs[1] = "b_small.in";
  inputs[2] = "c_medium.in";
  inputs[3] = "d_quite_big.in";
  inputs[4] = "e_also_big.in";
  srand(time(NULL));
  for (int i = 0; i < 5; i++) {
    readInput(inputs[i]);
    solve();
  }
}
