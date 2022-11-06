import random
from tqdm import tqdm

# CHRS = bytes("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789?! ,.:;\n", encoding='ascii')
CHRS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789?! ,.:;\n"
LENGTH = 2 ** 24

def main() -> None:
    # with open('1.in', 'wb') as f:
    #     for _ in tqdm(range(LENGTH)):
    #         f.write(bytes(random.choices(population=CHRS, k=LENGTH)))
    with open('2.in', 'r') as f:
        text = f.read()
    for c in text:
        if c not in CHRS:
            text = text.replace(c, ' ')
    text = (LENGTH // len(text) + 1) * text
    text = bytes(text, encoding='ascii')
    with open('2.in', 'wb') as f:
        f.write(text[:LENGTH])


if __name__ == '__main__':
    main()