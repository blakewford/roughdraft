#include <string.h>
#include "roughdraft.h"

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

        rds get_shape()
        {
            if(!strcmp(m_shape, "line"))
            {
                return line;
            }
            else if(!strcmp(m_shape, "circle"))
            {
                return circle;
            }

            return unknownShape;
        };

        rdl get_layer()
        {
            if(!strcmp(m_layer, "black"))
            {
                return black;
            }

            return unknownLayer;
        };

        long get_weight(){ return m_weight; };
        long get_width(){ return m_w; };
        long get_height(){ return m_h; };
        long get_x1(){ return m_x1; };
        long get_y1(){ return m_y1; };
        long get_x2(){ return m_x2; };
        long get_y2(){ return m_y2; };

    private:
        char m_shape[MAX_BUFFER_SIZE];
        char m_layer[MAX_BUFFER_SIZE];
        long m_weight;
        long m_w;
        long m_h;
        long m_x1;
        long m_y1;
        long m_x2;
        long m_y2;
};

void parse(const char* json, command* c);
