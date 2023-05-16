import sys

def gen_frames(bits_number: int, result_filename: str):
    pass

if __name__ == '__main__':
    try:
        n = int(sys.argv[1])
        result_filename = sys.argv[2]

        gen_frames(n, result_filename)

    except:
        print('Invalid Arguments!')
