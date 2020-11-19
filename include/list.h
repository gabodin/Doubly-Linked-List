#ifndef LIST_H
#define LIST_H

#include <iostream>
#include <initializer_list>
#include <iterator>
#include <stdexcept>

using size_t = unsigned int;

namespace ls {

    template <typename T>
    class list {

        private:
            struct DNode {
                T data;
                DNode * next;
                DNode * prev;

                DNode(T v = T(), DNode * n = nullptr, DNode * p = nullptr) : data{v}, next{n}, prev{p}
                {}
            };

            DNode *m_head;
            DNode *m_tail;
            size_t m_size;

        
        public:
            class const_iterator : public std::iterator<std::bidirectional_iterator_tag, T> 
            {
                public:
                    const_iterator( const DNode * ptr = nullptr ) : m_ptr{ ptr }
                    {}
                    
                    ~const_iterator ( void ) = default;
                    const_iterator (const const_iterator&  ) = default;

                    const_iterator & operator=( const const_iterator& ) = default;
                    const DNode & operator* (void) { return *m_ptr; }
                    DNode * operator&(void) const { return m_ptr; }

                    const_iterator & operator++ (void) { m_ptr = m_ptr->next; return *this; } // ++it
                    const_iterator operator++ ( int ) { iterator ret = *this; m_ptr = m_ptr->next; return ret; } // it++
                    const_iterator & operator-- () { m_ptr = m_ptr->prev; return *this; } // --it
                    const_iterator operator-- ( int ) { iterator ret = *this; m_ptr = m_ptr->prev; return ret; } //it--
                    bool operator== ( const const_iterator & rhs ) const { return m_ptr == rhs.m_ptr; }
                    bool operator!= ( const const_iterator & rhs ) const { return !(m_ptr == rhs.m_ptr); }

                private:
                    const DNode * m_ptr; // Ponteiro encapsulado da classe                   
            };

            class iterator : public std::iterator<std::bidirectional_iterator_tag, T> 
            {
                public:
                    iterator( DNode * ptr = nullptr ) : m_ptr{ ptr }
                    { }
                                        
                    ~iterator ( void ) = default;
                    iterator ( const iterator& ) = default;

                    iterator& operator=( const iterator& ) = default;
                    DNode & operator*(void) { return *m_ptr; }
                    DNode * operator&(void) const { return m_ptr; }

                    iterator & operator++ (void) { m_ptr = m_ptr->next; return *this; } // ++it
                    iterator operator++ ( int ) { iterator ret = *this; m_ptr = m_ptr->next; return ret; } // it++
                    iterator & operator-- (void) { m_ptr = m_ptr->prev; return *this; } // --it
                    iterator operator-- ( int ) { iterator ret = *this; m_ptr = m_ptr->prev; return ret; } //it--
                    bool operator== ( const iterator & rhs ) const { return m_ptr == rhs.m_ptr; } 
                    bool operator!= ( const iterator & rhs ) const { return !(m_ptr == rhs.m_ptr); }

                protected:
                    DNode * m_ptr; // Ponteiro encapsulado da classe
            };


            // [I] SPECIAL MEMBERS
            list() { // Construtor default - cria uma lista vazia
                m_head = new DNode();
                m_tail = new DNode();
                m_head->next = m_tail;
                m_tail->prev = m_head;
                m_size = 0;
            }
                

            explicit list(size_t count) : list() // Constrói uma list com count nós default
            {
                size_t i = 0;
                DNode * itr( m_tail); 

                while (i < count) {
                    DNode * prev{(*itr).prev};

                    DNode * temp = new DNode();

                    // Conecta o nó adicionado ao antecessor de itr e a itr, respectivamente
                    temp->prev = prev; 
                    temp->next = itr;

                    // Conecta o nó anterior a itr ao novo nó e conecta itr ao novo nó
                    prev->next = temp;
                    (*itr).prev = temp;                    
                    ++i;
                } 
            }

            ~list( ) { // Destructor
                clear();
                delete m_head;
                delete m_tail;
            }

            list(const std::initializer_list<T> & ilist ) : list() // Constrói uma lista com os conteúdos da lista inicializadora ilist
            {
                auto itr = ilist.begin();
                while ( itr != ilist.end()) {
                    push_back(*itr);
                    ++itr;
                }
            }

            // Constrói a lista com os conteúdos no intervalo [first, last]
            list( const_iterator first, const_iterator last ) : list() 
            {   

                while ( first != last ) 
                 {
                    push_back((*first).data);
                    ++first;
                 }
            }

            // Constrói a lista com os conteúdos no intervalo [first, last]
            list( iterator first, iterator last ) : list() 
            {   

                while ( first != last ) 
                 {
                    push_back((*first).data);
                    ++first;
                 }
            }

            list( list & other ) : list() // Copy constructor
            {   
                iterator itr = other.begin();
                while ( itr != other.end()) 
                 {
                    push_back((*itr).data);
                    ++itr;
                 }
            }

            // [II] ITERATORS

            iterator begin ( ) // Retorna um iterator apontado para o primeiro nó da lista
            {   
                return iterator( m_head->next ); 
            }

            const_iterator cbegin ( ) const // Retorna um iterator apontado para o primeiro nó da lista 
            {
                return const_iterator( m_head->next );
            }

            iterator end ( ) // Retorna um iterator apontado para o endereço seguinte ao último nó da lista
            {
                return iterator( m_tail ); 
            }
            
            const_iterator cend ( ) const // Retorna um const_iterator apontado para o endereço seguinte ao último nó da lista
            {
                return const_iterator( m_tail );
            }

            // [III] Capacity
            size_t size( )  // Retorna o número de nós contidos na lista
            {
                DNode * temp {m_head->next};
                m_size = 0;
                while( temp != m_tail ) {
                    temp = temp->next;
                    ++m_size;
                }
                return m_size;

            }
                                    
            bool empty( ) const // Retorna verdadeiro caso a lista esteja vazia, senão retorna falso
            {
                if(m_head->next == m_tail) return true;
                    else return false;
            } 

            // [IV] Modifiers
            
            void clear () const {
                if(empty()) { // Caso a lista não tenha membros, não há o que ser feito
                    return;
                }
                else {
                    DNode * curr{m_head->next};

                    while (curr != m_tail) {
                        DNode * aux {curr->next};
                        delete curr;
                        curr = aux;
                    }
                    m_head->next = m_tail;
                    m_tail->prev = m_head;
                    return;             
                }

            }
            // Nas funções seguintes, front e back, espera-se que o usuário não chame o método com uma lista vazia

            T & front() // Retorna o atributo data do primeiro nó da lista
            {
                DNode * temp = m_head->next;
                return temp->data;
                
            }

            const T & front( ) const // Retorna o atributo data do primeiro nó da lista
            {
                DNode * temp = m_head->next;
                return temp->data;                
            }

            T & back( ) // Retorna o atributo data do último nó da lista
            {
                DNode * temp = m_tail->prev;
                return temp->data;                 
            }
            
            const T & back( ) const // Retorna o atributo data do último nó da lista
            {
 
                DNode * temp = m_tail->prev;
                return temp->data;

            }

            void push_front( const T & value ) { // Adiciona um nó na primeira posição da lista
                if(this->empty()) {
                    DNode * temp = new DNode(value, m_tail, m_head);
                    m_head->next = temp;
                    m_tail->prev = temp;
                } else {
                    DNode * temp = new DNode(value);
                    temp->prev = m_head;
                    temp->next = m_head->next;
                    (m_head->next)->prev = temp;
                    m_head->next = temp;
                }
            }

            void push_back( const T & value) { // Adiciona um nó na última posição da lista
                
                if(this->empty()) {
                    DNode * temp = new DNode(value, m_tail, m_head);
                    m_head->next = temp;
                    m_tail->prev = temp;
                } else {             
                    DNode * temp = new DNode(value);
                                   
                    temp->next = m_tail;
                    temp->prev = m_tail->prev;
                    (m_tail->prev)->next = temp;
                    m_tail->prev = temp;
                }
            }

            void pop_front( ) { // Remove o primeiro nó da lista
                if(this->empty()) {
                    return;
                } else {
                    DNode * temp{m_head->next};
                    DNode * next{temp->next};
                    m_head->next = next;
                    next->prev = m_head;
                    delete temp;
                }
            }
            
            void pop_back( ) { // Remove o último nó da lista
                if(this->empty()) {
                    return;
                } else {
                    DNode * temp{m_tail->prev};
                    DNode * prev{temp->prev};
                    m_tail->prev = prev;
                    prev->next = m_tail;
                    delete temp;
                }
            }
            void assign(const T & value ) {
                DNode * curr{m_head->next};

                while (curr != m_tail) {
                    curr->data = value;
                    DNode * aux {curr->next};
                    curr = aux; 
                }
                
            } 

            // [IV-a] Modifiers with iterators

            void assign( size_t count, const T& value ) {
                
                this->clear(); // Apaga o conteúdo anterior da lista 
                size_t aux = 0; // Guarda o número de nós inseridos na lista

                while (aux < count) // Enquanto o aux for menor do que count, nós com cópias de value são inseridas na lista
                {
                    push_back(value);
                    ++aux;
                }

            }

            
            void assign( iterator first, iterator last ) 
            {             
                if(first == nullptr || last == nullptr) return;//Se first ou last apontarem para nullptr, retorna-se nulo

                this->clear(); // Apaga o conteúdo anterior da lista 

                while (first != last) // Enquanto first for diferente de last, cópias do conteúdo de first são adicionadas à lista
                {
                    push_back(*first);
                    ++first;
                }
            }

            void assign( const_iterator first, const_iterator last ) 
            {             
                if(first == nullptr || last == nullptr) return;//Se first ou last apontarem para nullptr, retorna-se nulo

                this->clear(); // Apaga o conteúdo anterior da lista 

                while (first != last) // Enquanto first for diferente de last, cópias do conteúdo de first são adicionadas à lista
                {
                    push_back((*first).data);
                    ++first;
                }
            }

            void assign(const std::initializer_list<T> & ilist ) // Substitui o conteúdo original da lista pelo conteúdo da lista inicializadora
            {
                this->clear();
                auto itr = ilist.begin();
                while ( itr != ilist.end()) {
                    push_back(*itr);
                    ++itr;
                }
            }

            const_iterator insert( const_iterator itr, const T & value ) 
            {
                if(itr == nullptr) return nullptr;

                DNode * prev{(&itr).prev};

                DNode * temp = new DNode(value);

                // Conecta o nó adicionado ao antecessor de itr e a itr, respectivamente
                temp->prev = prev; 
                temp->next = &itr;

                // Conecta o nó anterior a itr ao novo nó e conecta itr ao novo nó
                prev->next = temp;
                (&itr).prev = temp;

                return temp; // Retorna o endereço do nó adicionado

            }

            iterator insert( iterator itr, const T & value ) 
            {
                if(itr == nullptr) return nullptr;

                DNode * prev{(*itr).prev};

                DNode * temp = new DNode(value);

                // Conecta o nó adicionado ao antecessor de itr e a itr, respectivamente
                temp->prev = prev; 
                temp->next = &itr;

                // Conecta o nó anterior a itr ao novo nó e conecta itr ao novo nó
                prev->next = temp;
                (*itr).prev = temp;

                return temp; // Retorna o endereço do nó adicionado
            }

            
            iterator insert( iterator & pos, const_iterator & first, const_iterator & last )  
            {
                while ( first != last ) 
                 {
                    insert(pos, (*first).data);
                    ++first;
                 }
            }

            iterator insert( iterator & pos, iterator & first, iterator & last )  
            {
                while ( first != last ) 
                 {
                    insert(pos, (*first).data);
                    ++first;
                 }
            }

            iterator insert( const_iterator & pos, const_iterator & first, const_iterator & last )  
            {
                while ( first != last ) 
                 {
                    insert(pos, (*first).data);
                    ++first;
                 }
            }

            iterator insert( const_iterator & pos, iterator & first, iterator & last )  
            {
                while ( first != last ) 
                 {
                    insert(pos, (*first).data);
                    ++first;
                 }
            }

            const_iterator insert(const const_iterator & pos, const std::initializer_list<T>  & ilist ) 
            {
               auto itr = ilist.begin(); // Aponta para o início da lista inicializadora

               while ( itr != ilist.end())
                {
                    insert(pos, *itr); // Insere um nó na posição anterior a pos   
                    ++itr; // Avança uma posição na lista inicializadora
                } 
            }

            iterator insert( iterator & pos, const std::initializer_list<T>  & ilist ) 
            {
               auto itr = ilist.begin(); // Aponta para o início da lista inicializadora

               while ( itr != ilist.end())
                 {
                    insert(pos, *itr); // Insere um nó na posição anterior a pos   
                    ++itr; // Avança uma posição na lista inicializadora
                 } 
            }

            const_iterator erase( const_iterator itr ) 
            {
                if(itr == nullptr) return nullptr; //Se itr apontar para nullptr, retorna-se nullptr

                DNode * prev{(*itr).prev};
                DNode * next{(*itr).next};

                prev->next = next; // Conecta o nó anterior do itr ao itr->next
                next->prev = prev; // Conecta o nó posterior do itr ao itr->prev

                delete itr; // Deleta o nó passado pelo iterator
                return next; // Retorna o nó seguinte ao iterator
            }

            iterator erase( iterator & itr ) 
            {
                if(itr == nullptr) return nullptr; //Se itr apontar para nullptr, retorna-se nullptr

                DNode * prev{(*itr).prev};
                DNode * next{(*itr).next};

                prev->next = next; // Conecta o nó anterior do itr ao itr->next
                next->prev = prev; // Conecta o nó posterior do itr ao itr->prev

                delete itr; // Deleta o nó passado pelo iterator
                return next; // Retorna o nó seguinte ao iterator               
            }

            const_iterator erase( const_iterator & first, const_iterator & last ) 
            {
                if(first == nullptr || last == nullptr) return nullptr; //Se first ou last apontarem para nullptr, retorna-se nulo

                while (first != last) // Itera pela lista até que o iterator first seja igual ao iterator last
                    {
                        DNode * itr{first}; // Nó que vai ser apagado
                        DNode * prev{(*itr).prev}; // Nó posterior ao itr
                        DNode * next{(*itr).next}; // Nó anterior ao itr

                        prev->next = next; // Conecta o nó anterior do itr ao itr->next
                        next->prev = prev; // Conecta o nó posterior do itr ao itr->prev

                        delete itr; // Deleta o nó passado pelo iterator
                        ++first; // Avança o first para o próximo endereço                 
                    }
                
                return last;
            }

            iterator erase( iterator & first, iterator & last ) 
            {
                if(first == nullptr || last == nullptr) return nullptr; //Se first ou last apontarem para nullptr, retorna-se nulo

                while (first != last) // Itera pela lista até que o iterator first seja igual ao iterator last
                    {
                        DNode * itr{&first}; // Nó que vai ser apagado
                        DNode * prev{(*itr).prev}; // Nó posterior ao itr
                        DNode * next{(*itr).next}; // Nó anterior ao itr

                        prev->next = next; // Conecta o nó anterior do itr ao itr->next
                        next->prev = prev; // Conecta o nó posterior do itr ao itr->prev

                        delete itr; // Deleta o nó passado pelo iterator
                        ++first; // Avança o first para o próximo endereço                 
                    }

                return last;
            }


            

            // [V] Operator Overload

            bool operator== ( list& rhs) //Sobrecarga do operador == para verificar se duas listas são iguais 
            {
                if(this->size() != rhs.size()) return false; // Se elas têm tamanhos diferentes, retorna falso

                auto itr1 = this->begin;
                auto itr2 = rhs.begin();

                while (itr1 != this->end()) {
                    if( *itr1 != *itr2 ) return false; // Se o conteúdo de um iterator é diferente do outro, retorna falso
                    ++itr1;
                    ++itr2;
                }

                return true; // Caso as listas tenham o mesmo tamanho e seus nós sejam todos iguais, retorna verdadeiro
            }

            bool operator!= (const list& rhs) //Sobrecarga do operador == para verificar se duas listas são diferentes
            {
                if(this->size() != rhs.size()) return true; // Se elas têm tamanhos diferentes, retorna verdadeiro

                auto itr1 = this->begin();
                auto itr2 = rhs.begin();

                while (itr1 != this->end() && itr2 != rhs.end()) {
                    if( *itr1 != *itr2 ) return true; // Se o conteúdo de um iterator é diferente do outro, retorna verdadeiro
                    ++itr1;
                    ++itr2;
                }

                return false; // Caso as listas tenham o mesmo tamanho e seus nós sejam iguais, retorna falso 
            }

            list& operator=( list& other ) 
            {
                this->m_head = new DNode();
                this->m_tail = new DNode();
                this->m_head->next = m_tail;
                this->m_tail->prev = m_head;
                this->m_size = 0;
                
                this->clear();

                auto itr = other.begin();

                while ( itr != other.end()) 
                  {
                    this->push_back((*itr).data);
                    ++itr;
                  }

                return *this;                
            }

            list& operator=(const std::initializer_list<T> & ilist ) 
            {
                this->m_head = new DNode();
                this->m_tail = new DNode();
                this->m_head->next = this->m_tail;
                this->m_tail->prev = this->m_head;
                this->m_size = 0;
                
                this->clear();

                auto itr = ilist.begin();

                while ( itr != ilist.end()) {
                    this->push_back(*itr);
                    ++itr;
                }
            }

        // [VI] Print

        void print () 
        {

        }




    };

}



#endif