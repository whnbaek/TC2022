#include <cassert>
#include <cstdio>
#include <cstring>

#define K_MAX 1000
#define LEN_MAX 1000
#define N_MAX 100000
// printable: 32 ~ 126
#define SIGMA_MAX 95
#define OFFSET 32

int k, n;
int len[K_MAX], out[K_MAX];
char pattern[K_MAX][LEN_MAX + 1], text[N_MAX + 1];

void get_input(char *filename) {
  FILE *fp = fopen(filename, "r");
  assert(fp);

  assert(fscanf(fp, "%d", &k) == 1);
  for (int i = 0; i < k; i++) {
    assert(fscanf(fp, "%s", pattern[i]) == 1);
    len[i] = strlen(pattern[i]);
    for (int j = 0; j < len[i]; j++)
      pattern[i][j] -= OFFSET;
  }
  assert(fscanf(fp, "%s", text) == 1);
  n = strlen(text);
  for (int i = 0; i < n; i++)
    text[i] -= OFFSET;
  assert(fclose(fp) == 0);
}

void get_output(char *filename) {
  FILE *fp = fopen(filename, "r");
  assert(fp);

  for (int i = 0; i < k; i++)
    assert(fscanf(fp, "%d", &out[i]) == 1);

  assert(fclose(fp) == 0);
}

void check() {
  for (int i = 0; i < k; i++) {
    int ans = 0;
    for (int j = 0; j < n - len[i] + 1; j++) {
      int l;
      for (l = 0; l < len[i]; l++) {
        if (text[j + l] != pattern[i][l])
          break;
      }
      if (l == len[i])
        ans ^= j;
    }
    if (ans != out[i]) {
      printf("WRONG\n");
      return;
    }
  }
  printf("CORRECT\n");
}

/* ./checker <input_file> <output_file> */
int main(int argc, char *argv[]) {
  assert(argc == 3);
  get_input(argv[1]);
  get_output(argv[2]);
  check();
  return 0;
}
