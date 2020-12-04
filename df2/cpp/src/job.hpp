#ifndef DF2_JOB
#define DF2_JOB

// built-in
#include <cstdint>
#include <cstring>
#include <utility>

struct Job
{
    uint8_t min, max;
    char ch;
    char* password = nullptr;

    Job(const uint8_t min, const uint8_t max, char ch, const char* const password)
    :   min(min), max(max), ch(ch)
    {
        this->password = new char[strlen(password) + 1];
        strcpy(this->password, password);
        
        // alternativas:  
        //     this->password = strdup(password);
    }

    ~Job()
    {
        delete[] this->password;
    }

    // lvalue
    
    Job(const Job& other): min(other.min), max(other.max), ch(other.ch)
    {
        // construção através de cópia de referencia(lvalue)
        // é necessário copiar os valores, já que o objeto continua sendo válido após a cópia

        this->password = new char[strlen(other.password) + 1];
        strcpy(this->password, other.password);

        // alternativas:  
        //     this->password = strdup(password);
    }

    Job& operator=(const Job& other)
    {
        // quase a mesma coisa que o construtor por cópia de referencia
        // mas nesse caso temos que desalocar (ou não) os valores desse objeto

        this->min = other.min;
        this->max = other.max;
        this->ch = other.ch;

        size_t this_pswd_size = strlen(this->password) + 1;
        size_t other_pswd_size = strlen(other.password) + 1;

        #ifndef PRIOR_MEMORY
            if (this_pswd_size < other_pswd_size)
            {
                delete[] this->password;
                this->password = new char[other_pswd_size]; 
            }
            else
            {
                memset(this->password + other_pswd_size - 1, 0x00, this_pswd_size - other_pswd_size + 1);
            }
        #else
            if (this_pswd_size != other_pswd_size)
            {
                delete[] this->password;
                this->password = new char[other_pswd_size]; 
            }
        #endif
        
        strcpy(this->password, other.password);

        return *this;
    }

    // rvalue

    Job(Job&& other)
    {
        // construção através de apossamento de rvalue
        // é mais rápido, visto que não é necessário copiar valores, oque implica em alocar memória
        // podemos "roubar" os valores do objeto visto que um rvalue não possui identificador,
        // logo, ele provavelmente não vai existir após a chamada dessa função
        
        // se estivermos comparando o objeto com ele mesmo,
        // estariamos fazendo computação desnecessária
        if (this == &other)
        {
            return;
        }

        this->swap(*this, other);
    }

    Job& operator=(Job&& other)
    {
        // literalmente a mesma coisa que o construtor através de rvalue

        if (this != &other)
        {
            this->swap(*this, other);
        }
        return *this;
    }

    // utilitárias

    inline static void swap(Job& a, Job& b)
    {
        #define mswap(a, b) \
            a ^= b;         \
            b ^= a;         \
            a ^= b
        
        mswap(a.min, b.min);
        mswap(a.max, b.max);
        mswap(a.ch, b.ch);

        // ponteiros não suportam XOR nativamente, seria necessário fazer uma conversão
        std::swap(a.password, b.password);
    }

};

#endif