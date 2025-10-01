#include <string.h>
using std::string;

class Filter
{
private:
    string m_name;
public:
    virtual void apply (ImageContainer& source);
    string getName () {
        return m_name;
    }
    void setName (string newName) {
        m_name = newName;
    }
};