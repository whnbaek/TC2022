#include <cassert>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stack>
#include <vector>
#include <unordered_map>

#define NUM_CHRS 71L
#define INIT_DIGIT 7L

char version, chr2idx[128], digit = INIT_DIGIT;
long size, max_idx, num_bits, cnt;
int input, output;
FILE *in, *out;
char chrs[] = "#abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01"
              "23456789?! ,.:;\n";
class Trie {
public:
  Trie(long idx_, int chr_, Trie *parent_)
      : idx(idx_), chr(chr_), parent(parent_) {}
  long idx;
  int chr;
  Trie *parent;
  std::unordered_map<char, Trie *> edges;
} * root;
std::vector<Trie *> idx2trie;

void init() {
  // insert root node
  root = new Trie(-1L, 0, NULL);

  // insert basic character nodes
  idx2trie.push_back(NULL);
  for (long i = 1; i < NUM_CHRS; i++) {
    chr2idx[chrs[i]] = i;
    idx2trie.push_back(new Trie(i, chrs[i], root));
    root->edges[i] = idx2trie.back();
  }
  max_idx = NUM_CHRS;
}

void idx2bit(long idx) {
  long start = num_bits / 8L, end = (num_bits + digit + 7L) / 8L;
  for (long j = start; j < end; j++) {
    if (cnt < j) {
      assert(fputc(output, out) == output);
      cnt++;
      output = 0;
    }
    long left = std::max(j * 8L, num_bits),
         right = std::min((j + 1L) * 8L, num_bits + digit);
    output |= ((idx >> (left - num_bits)) & ((1L << (right - left)) - 1L))
              << (left - j * 8L);
  }
  num_bits += digit;
}

long bit2idx() {
  long start = num_bits / 8L, end = (num_bits + digit + 7L) / 8L, idx = 0L;
  for (long j = start; j < end; j++) {
    if (cnt <= j) {
      input = fgetc(in);
      cnt++;
    }
    long left = std::max(j * 8L, num_bits),
         right = std::min((j + 1L) * 8L, num_bits + digit);
    idx |= ((input >> (left - j * 8L)) & ((1L << (right - left)) - 1L))
           << (left - num_bits);
  }
  num_bits += digit;
  return idx;
}

void encode() {
  input = fgetc(in);
  while (true) {
    // find the longest string from i
    Trie *curr = root;
    while (input != EOF && curr->edges.count(chr2idx[input])) {
      curr = curr->edges[chr2idx[input]];
      input = fgetc(in);
    }

    // emit curr->idx to output
    idx2bit(curr->idx);

    // stop if eof
    if (input == EOF)
      break;

    // add symbol to dictionary
    idx2trie.push_back(new Trie(max_idx++, input, curr));
    curr->edges[chr2idx[input]] = idx2trie.back();
    if (max_idx > (1L << digit))
      digit++;
  }

  // emit 0 to output
  idx2bit(0L);

  // flush last character
  assert(fputc(output, out) == output);
}

void decode() {
  Trie *curr = root;
  while (true) {
    // read index from input
    long idx = bit2idx();

    // stop if eof
    if (idx == 0L)
      break;

    // emit symbol to output
    Trie *next = idx < max_idx ? idx2trie[idx] : curr;
    std::stack<int> stack;
    while (next != root) {
      stack.push(next->chr);
      next = next->parent;
    }
    int top = stack.top();
    while (!stack.empty()) {
      assert(fputc(stack.top(), out) == stack.top());
      stack.pop();
    }
    if (idx >= max_idx)
      assert(fputc(top, out) == top);

    // add symbol to dictionary
    if (!curr->edges.count(chr2idx[top])) {
      idx2trie.push_back(new Trie(max_idx++, top, curr));
      curr->edges[chr2idx[top]] = idx2trie.back();
      if (max_idx >= (1L << digit))
        digit++;
    }

    curr = idx2trie[idx];
  }
}

int main(int argc, char *argv[]) {
  // check argument format
  assert(
      argc == 4 &&
      "usage: ./main {execution version} {input file path} {output file path}");

  // get version
  version = atol(argv[1]);
  assert((version == 1L || version == 2L) &&
         "version should be 1 (compress) or 2 (decompress)");

  // open files
  assert((in = fopen(argv[2], "rb")) != NULL && strerror(errno));
  assert((out = fopen(argv[3], "wb")) != NULL && strerror(errno));

  init();
  version == 1L ? encode() : decode();

  // close files
  assert(fclose(in) == 0 && strerror(errno));
  assert(fclose(out) == 0 && strerror(errno));
  return 0;
}
