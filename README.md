Eu decidi refazer o projeto em C++ ~~por algum tipo de masoquismo~~ para aprender melhor a linguagem, eu criei um makefile pra deixar a compilação mais fácil, só com
    
    make

ou

    make all

Eu estou usando o WSL2 pra compilar o programa, simplesmente por preferência. Eu dei uma mexida no Makefile pra ele também funcionar no Windows, eu testei aqui e compilar ele compila de boa, mas o `make clean` não apaga os diretórios.

Agora caso não tenha o `make` instalado, dá pra fazer do jeito mais difícil,  você pode ir dentro de src e fazer algo do tipo:

    g++ *.cpp lexical/*.cpp -o ../compilador.exe

Mas isso não rodou pra mim, o Windows não expandiu os `*`. O que sobre então é especificar todos os .cpp.