#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstdio>
#include <cstring>
#include <list>

#define K_MAX 1000
#define LEN_MAX 100
#define Q_MAX 1000
#define N_MAX 10000
// printable: [97, 122]
#define SIGMA_MAX 26
#define OFFSET 97

/* input data */
int version; // 1: 2D array, 2: linked list
int k, len[K_MAX], q, n[Q_MAX];
char pattern[K_MAX][LEN_MAX + 1], text[Q_MAX][N_MAX + 1];

/* trie data */
int m = 1;
int depth[K_MAX * LEN_MAX + 1], count[K_MAX * LEN_MAX + 1],
    order[K_MAX * LEN_MAX + 1]; // for count sort
int prev[K_MAX * LEN_MAX + 1], alpha[K_MAX * LEN_MAX + 1];
int array_2d[K_MAX * LEN_MAX + 1][SIGMA_MAX]; // for version 1
std::list<std::pair<int, int>>
    linked_list[K_MAX * LEN_MAX + 1]; // for version 2

/* fail, output data */
int fail[K_MAX * LEN_MAX + 1];
std::list<int> output[K_MAX * LEN_MAX + 1];

/* answers */
bool ans[Q_MAX];

int find_from_list(std::list<std::pair<int, int>> &list, int target) {
  for (auto &[key, value] : list)
    if (key == target)
      return value;
  return 0;
}

void get_input() {
  // FILE *fp = fopen(filename, "r");
  // assert(fp);

  assert(scanf("%d", &k) == 1);
  for (int i = 0; i < k; i++) {
    assert(fgets(pattern[i], LEN_MAX, stdin) == pattern[i]);
    len[i] = strlen(pattern[i]);
    while (pattern[i][len[i] - 1] == '\n') {
      pattern[i][len[i] - 1] = 0;
      len[i]--;
    }
    for (int j = 0; j < len[i]; j++)
      pattern[i][j] -= OFFSET;
  }
  assert(scanf("%d", &q) == 1);
  for (int i = 0; i < q; i++) {
    assert(fgets(text[i], N_MAX, stdin) == text[i]);
    n[i] = strlen(text[i]);
    while (text[i][n[i] - 1] == '\n') {
      text[i][n[i] - 1] = 0;
      n[i]--;
    }
    for (int j = 0; j < n[i]; j++)
      text[i][j] -= OFFSET;
  }

  // assert(fclose(fp) == 0);
}

void construct_trie_v1() {
  for (int i = 0; i < k; i++) {
    int node = 0;
    for (int j = 0; j < len[i]; j++) {
      int &next = array_2d[node][pattern[i][j]];
      if (next == 0) {
        next = m++;
        depth[next] = depth[node] + 1;
        prev[next] = node;
        alpha[next] = pattern[i][j];
      }
      node = next;
    }
    output[node].push_back(i);
  }
}

void construct_trie_v2() {
  for (int i = 0; i < k; i++) {
    int node = 0;
    for (int j = 0; j < len[i]; j++) {
      int next = find_from_list(linked_list[node], pattern[i][j]);
      if (next == 0) {
        linked_list[node].emplace_back(pattern[i][j], m);
        next = m++;
        depth[next] = depth[node] + 1;
        prev[next] = node;
        alpha[next] = pattern[i][j];
      }
      depth[next] = depth[node] + 1;
      node = next;
    }
    output[node].push_back(i);
  }
}

void count_sort() {
  int max = 0;
  for (int i = 0; i < m; i++) {
    count[depth[i]]++;
    max = std::max(max, depth[i]);
  }
  for (int i = 1; i <= max; i++)
    count[i] += count[i - 1];
  for (int i = 0; i < m; i++)
    order[--count[depth[i]]] = i;
}

void compute_fail_v1() {
  for (int i = 0; i < m; i++) {
    int node = order[i];
    if (depth[node] <= 1)
      continue;
    int x = fail[prev[node]], y = alpha[node], z;
    while ((z = array_2d[x][y]) == 0 && x != 0)
      x = fail[x];
    fail[node] = z;
    output[node].insert(output[node].end(), output[z].begin(), output[z].end());
  }
}

void compute_fail_v2() {
  for (int i = 0; i < m; i++) {
    int node = order[i];
    if (depth[node] <= 1)
      continue;
    int x = fail[prev[node]], y = alpha[node], z;
    while ((z = find_from_list(linked_list[x], y)) == 0 && x != 0)
      x = fail[x];
    fail[node] = z;
    output[node].insert(output[node].end(), output[z].begin(), output[z].end());
  }
}

void search_v1() {
  for (int l = 0; l < q; l++) {
    int node = 0, i;
    for (i = 0; i < n[l]; i++) {
      int next;
      while ((next = array_2d[node][text[l][i]]) == 0 &&
            node != 0)
        node = fail[node];
      node = next;
      // for (auto &j : output[node])
      //   ans[j] ^= i - len[j] + 1;
      if (!output[node].empty())
        break;
    }
    ans[l] = !(i == n[l]);
  }
}

void search_v2() {
  for (int l = 0; l < q; l++) {
    int node = 0, i;
    for (i = 0; i < n[l]; i++) {
      int next;
      while ((next = find_from_list(linked_list[node], text[l][i])) == 0 &&
            node != 0)
        node = fail[node];
      node = next;
      // for (auto &j : output[node])
      //   ans[j] ^= i - len[j] + 1;
      if (!output[node].empty())
        break;
    }
    ans[l] = !(i == n[l]);
  }
}

void print_output() {
  // FILE *fp = fopen(filename, "w");
  // assert(fp);

  for (int i = 0; i < q; i++)
    (void)printf("%s\n", ans[i] ? "YES" : "NO");

  // assert(fclose(fp) == 0);
}

double calculate_time(void (*function)()) {
  auto begin = std::chrono::high_resolution_clock::now();
  function();
  auto end = std::chrono::high_resolution_clock::now();
  return std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin)
             .count() *
         1e-6;
}

/* ./main <version> <input_file> <output_file> */
int main(int argc, char *argv[]) {
  // assert(argc == 4);
  // version = atoi(argv[1]);
  version = 1;
  // version = 2;
  // assert(version == 1 || version == 2);

  get_input();

  double t1, t2, t3, t4;
  if (version == 1) {
    t1 = calculate_time(construct_trie_v1);
    t2 = calculate_time(count_sort);
    t3 = calculate_time(compute_fail_v1);
    t4 = calculate_time(search_v1);
  } else {
    t1 = calculate_time(construct_trie_v2);
    t2 = calculate_time(count_sort);
    t3 = calculate_time(compute_fail_v2);
    t4 = calculate_time(search_v2);
  }

  print_output();
  // (void)printf("%lf,%lf,%lf,%lf\n", t1, t2, t3, t4);

  return 0;
}
