#!/bin/python
from mat_kniznica import Library
import cProfile

def odchylka(numbers, n):
    num_array = str_to_int(numbers)
    # a= (1 / (n-1))
    a = Library.delenie(1, n-1)
    # b = sum of xi^2
    b = suma_2(num_array)
    # c = n * (suma / n)^2
    c = Library.nasobenie(n, Library.umocnovanie( Library.delenie(suma_of(num_array), n), 2) )
    # d= (sum of xi^2) - (n * (suma / n)^2)
    d = Library.minus(b, c)
    res = Library.nasobenie(a, d)
    #return = √(a * d)
    return Library.odmocnina(res, 2)
    
def suma_of(numbers):
    suma = 0
    for number in numbers:
        suma = Library.plus(suma, number)
    return suma

def suma_2(numbers):
    total = 0
    for number in numbers:
        total = Library.plus((Library.umocnovanie(number, 2)), total)
    return total

def str_to_int(numbers):
    num_array = [float(num_str) for num_str in numbers]
    return num_array

if __name__ == '__main__':
    string_input = []
    out = ''
    # ak je vstup subor, kazde cislo musi byt na novom riadku
    print("Za kazdym zadanym cislom stlacte 'ENTER', pre ukoncenie vstupu stlacte 'Ctrl+D':")
    while True:
        try:
            string_input.append(input())
        except EOFError:
            out = '\n'.join(string_input)
            break
    print(odchylka(string_input, len(string_input)))