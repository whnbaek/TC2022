import random
import sys

# CAND = ['A', 'T', 'C', 'G']
CAND = [chr(i) for i in range(32, 32 + 95)]

def main():
    with open(sys.argv[1], 'w') as f:
        text = []
        for _ in range(100000):
            text.append(random.choice(CAND))
        text = ''.join(text)

        f.write('1000\n')
        for i in range(1000):
            f.write(CAND[i % 95] + CAND[i // 95] + '\n')
        f.write(text + '\n')

if __name__ == '__main__':  # python tcgen.py <input_file>
    main()
