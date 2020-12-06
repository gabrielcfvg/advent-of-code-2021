
invalidos2 = set()
invalidos1 = set()

def main2():

    data = open("input.txt", "r").read().split("\n\n")
    karr = {"byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"}
    file_exit = open("saida2", "w")

    print("len2: ", len(data))

    idx = 0
    count = len(data)
    for A in data:
        idx += 1
        for B in karr:
            if B not in A:
                file_exit.write(A+"\n\n")
                invalidos2.add(idx)
                count -= 1
                break

    print(count)


def read_input(file_path: str) -> list:
    result = []
    vec_buffer = []

    for line in open(file_path, "r").readlines():

        if len(line) == 1:
            result.append(vec_buffer)
            vec_buffer = []
            continue

        for kv in line[:-1].split(" "):
            vec_buffer.append(kv.split(":"))

    return result

def main():

    data = read_input("input.txt")
    file_exit = open("saida", "w")

    for A in range(0, 10):
        for B in data[A]:
            print(f" {B[0]}:{B[1]} ", end="")
        print()

    karr = {"byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"}
    total = len(data)
    
    idx = 0
    for A in data:
        idx += 1
        ltotal = 0
        already = []
        for B in A:
            
            if B[0] not in already and B[0] in karr:
                ltotal += 1
                already.append(B[0])
        
        if not ltotal >= 7:
            total -= 1
        # else:
        #     
        #     saida = ""
        #     for B in A:
        #         saida += f" {B[0]}:{B[1]} "

        #     file_exit.write(saida+"\n\n")
        #     invalidos1.add(idx)

    saida1 = ""
    for B in data[0]:
        saida1 += f" {B[0]}:{B[1]} "

    saida2 = ""
    for B in data[-1]:
        saida2 += f" {B[0]}:{B[1]} "
    
    print("=" * 50)
    print("main1 primeiro\n")
    print(saida1)
    print("=" * 50)
    
    print("=" * 50)
    print("main1 segundo\n")
    print(saida2)
    print("=" * 50)


    print("len: ", len(data))
    print("total: ", total)



main()
main2()

print(invalidos1.difference(invalidos2))