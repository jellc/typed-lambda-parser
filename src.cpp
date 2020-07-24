#include <deque>
#include <iostream>
#include <unordered_map>
using std::string;

// instead of 'λ'.
const char lambda='%';

// error handling.
void myassert(bool pred, const char prob[]="")
{
    if(pred) return;
    std::cerr<<"parse failed.\n";
    if(prob!="") std::cerr<<"problem: "<<prob<<"\n";
    exit(1);
}


struct judged_type : std::deque<char>
{
    bool ismap=false;

    void apply(judged_type rhs)
    {
        if(rhs.empty()) return;
        if(front()=='(')
        {
            pop_front();
            while(!rhs.empty())
            {
                myassert(rhs.front()==front(), "applying type doesn't match.");
                rhs.pop_front();
                pop_front();
            }
            myassert(front()==')', "bracket not closed.");
            pop_front();
        }
        else
        {
            while(!rhs.empty())
            {
                myassert(front()!='-' and front()!='>',"applying type doesn't match.");
                myassert(rhs.front()==front(),"applying type doesn't match.");
                rhs.pop_front();
                pop_front();
            }
        }
        myassert(front()=='-',"applying type doesn't match.");
        pop_front();
        myassert(front()=='>',"applying type doesn't match.");
        pop_front();
    }

    void mapto(judged_type rhs)
    {
        if(ismap)
        {
            push_front('(');
            push_back(')');
        }
        push_back('-');
        push_back('>');
        while(!rhs.empty())
        {
            push_back(rhs.front());
            rhs.pop_front();
        }
        ismap=true;
    }

    void append(char c)
    {
        if(c=='>') ismap=true;
        push_back(c);
    }

    friend std::ostream &operator<<(std::ostream& os, judged_type jty)
    {
        std::string str;
        while(!jty.empty())
        {
            str+=jty.front();
            jty.pop_front();
            if(str.back()==')') // remove unnecessary bracket.
            {
                auto itr=str.end();
                do { --itr; } while(*itr!='>' and *itr!='(');
                if(*itr=='(')
                {
                    str.erase(itr);
                    str.pop_back();
                }
            }
        }
        return os<<str;
    }
}; // struct judged_type


std::unordered_map<string, judged_type> type_of;

bool normal(char chr)
{
    const char lst[]="() .,:\n"; // end with a null character.
    for(char c: lst) if(chr == c) return false;
    return true;
}

void skip_space(char *&ptr) { while(*ptr==' ') ++ptr; }


judged_type parse(char *&ptr)
{
    skip_space(ptr);

    if(*ptr=='(')
    {
        ++ptr;
        auto lhs=parse(ptr);
        lhs.apply(parse(ptr));

        skip_space(ptr);
        myassert(*ptr==')');
        ++ptr;

        return lhs;
    }

    if(*ptr==')') return judged_type();

    if(*ptr==lambda)
    {
        ++ptr;
        string var;
        while(normal(*ptr))
        {
            var+=*ptr;
            ++ptr;
        }
        myassert(!type_of.count(var),"multiple definition of variable.");

        skip_space(ptr);
        myassert(*ptr==':',"missing a \':\' in declaration.");
        ++ptr;

        skip_space(ptr);
        judged_type &lhs=type_of[var];
        while(normal(*ptr) or *ptr=='(' or *ptr==')')
        {
            if(*ptr!=' ') lhs.append(*ptr);
            ++ptr;
        }

        skip_space(ptr);
        myassert(*ptr=='.',"missing a \'.\' in declaration.");
        ++ptr;

        lhs.mapto(parse(ptr));
        return lhs;
    }

    if(*ptr!='\0' or *ptr!='\n')
    {
        string var;
        while(normal(*ptr))
        {
            var+=*ptr;
            ++ptr;
        }
        myassert(type_of.count(var),"variable not declared."); // should be already declared.
        return type_of[var];
    }

    return judged_type();
} // judged_type parse(char*&)


signed main(int argc, char *args[])
{
    if(argc==1)
    {
        std::cerr<<"args empty.\n";
        return 0;
    }

    char *ptr=args[1];

    if(argc>2)
    {
        skip_space(ptr);
        while(*ptr!='\0')
        {
            string var;
            while(*ptr!=':')
            {
                var+=*ptr;
                ++ptr;
            }
            myassert(!type_of.count(var),"multiple definition of variable.");
            skip_space(ptr);
            myassert(*ptr==':');
            ++ptr;
            skip_space(ptr);
            judged_type &ty=type_of[var];
            while(normal(*ptr) or *ptr=='(' or *ptr==')')
            {
                ty.append(*ptr);
                ++ptr;
            }
            skip_space(ptr);
            if(*ptr==',') skip_space(++ptr);
        }
        ptr=args[2];
    }

    std::cout<<parse(ptr)<<"\n";
}