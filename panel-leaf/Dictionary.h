#ifndef Dictionary_h
#define Dictionary_h

#ifdef Arduino_h
#define MAX_DICT_SIZE 20
#endif

#ifndef Arduino_h
#define MAX_DICT_SIZE 100
#endif

template <typename T, typename J>
struct Node
{
    T key;
    J value;
    Node* next;
};

template <typename V>
class Dictionary
{
private:
    Node<const char*, V>** hash;

public:
    Dictionary()
    {
        // Create a new hash list with null pointers
        hash = new Node<const char*, V>*[MAX_DICT_SIZE] {};
    }

    void set(const char* key, V value)
    {
        // Discovery the hash number
        int pos = Dictionary::hashFunction(key);
        Node<const char*, V>* node = this->hash[pos];
        Node<const char*, V>* newNode = new Node<const char*, V>{ key, value };

        // Case is the first element append at start
        if (node == NULL)
        {
            this->hash[pos] = newNode;
            return;
        }

        // Loop through the colision list
        while (node)
        {
            // Case is the same key update value
            if (strcmp(node->key, key) == 0)
            {
                node->value = value;
                return;
            }

            // Case ends the linked list append at final
            else if (node->next == NULL)
            {
                node->next = newNode;
                return;
            }

            node = node->next; // Call the next node
        }
    }

    V& get(const char* key)
    {
        // Discovery the hash number
        int pos = Dictionary::hashFunction(key);
        Node<const char*, V>* node = this->hash[pos];

        // Loop through the colision list
        while (node)
        {
            // Compare the key
            if (strcmp(node->key, key) == 0)
                return node->value;

            node = node->next; // Call the next node
        }

        return *(new V());
    }

    V& operator[](const char* key)
    {
        return this->get(key);
    }

    void pop(const char* key)
    {
        // Discovery the hash number
        int pos = Dictionary::hashFunction(key);
        Node<const char*, V>* node = this->hash[pos];
        Node<const char*, V>* previousNode = NULL;

        // Loop through the colision list
        while (node)
        {
            // Compare the key
            if (strcmp(node->key, key) == 0)
            {
                if (previousNode)
                    previousNode->next = node->next; // Case isn't the firt element
                else
                    this->hash[pos] = node->next; // Case is the first element

                return;

            }
            

            previousNode = node; // Atual node becoumes the previos nod
            node = node->next; // Call the next node
        }
    }

    void clear()
    {
        // Dealocate from memory
        delete[] hash;

        // Realocate in memory
        hash = new Node<const char*, V>*[MAX_DICT_SIZE] {};
    }

    static int hashFunction(const char* key, unsigned long seed = 4242)
    {
        unsigned long hash = seed;
        unsigned char c;

        // Interrate over the key
        while (c = *key++)
        {
            // Sum the hash
            hash = ((hash << 5) / 3) + hash + c;
        }

        // Limit the value from 0 to above of array size
        return hash % MAX_DICT_SIZE;
    }
};

#endif
