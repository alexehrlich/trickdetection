import sys
import os

def main():
    if len(sys.argv) != 2:
        print("Wrong number of arguments")
        return

    path = sys.argv[1]

    if not os.path.exists(path):
        print("Error opening file")
        return

    trickname = ""
    timestamp  = ""
    buf        = []

    with open(path) as f:
        for line in f:
            if line.startswith('---'):
                parts     = line.split(',')
                trickname = parts[1]
                timestamp = parts[2].strip()
                continue

            if line.startswith('result'):
                result = line.split(',')[1].strip()
                filename = f"{trickname}_{timestamp}_{result}.csv"
                with open(filename, 'w') as out:
                    for buffered_line in buf:
                        out.write(buffered_line.strip())
                        out.write("\n")
                print(f"Written: {filename} ({result})")
                buf.clear()
            else:
                buf.append(line)

if __name__ == '__main__':
    main()