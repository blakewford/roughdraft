#include <string.h>

class container_object;

#define MAX_BUFFER_SIZE 64

class command
{
    public:
        command(){ memset(m_name, '\0', MAX_BUFFER_SIZE); }
        void populate(container_object* obj);
        const char* get_name(){ return m_name; }
        virtual void populate_specific(container_object* obj) = 0;

    private:
        char m_name[MAX_BUFFER_SIZE];
};

class set_command: public command
{
    public:
        set_command():
            m_weight(0)
        {
            memset(m_shape, '\0', MAX_BUFFER_SIZE); 
            memset(m_layer, '\0', MAX_BUFFER_SIZE);
        }
        void populate_specific(container_object* obj);
        const char* get_shape(){ return m_shape; };
        const char* get_layer(){ return m_layer; };
        long get_weight(){ return m_weight; };

    private:
        char m_shape[MAX_BUFFER_SIZE];
        char m_layer[MAX_BUFFER_SIZE];
        bool m_weight;

};

void parse(const char* json, command* c);
