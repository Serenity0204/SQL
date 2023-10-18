#include "ftokenize.h"

FTokenizer::FTokenizer(char* fname)
    : _pos(0), _blockPos(0)
{
    this->_f.open(fname);
    assert(!this->_f.fail());
    this->_more = this->get_new_block();
}
SToken FTokenizer::next_token()
{
    SToken t;
    this->_stk >> t;

    this->_pos += t.token_str().length();
    this->_blockPos += t.token_str().length();

    if (this->_stk.done()) this->_more = get_new_block();
    return t;
}

bool FTokenizer::get_new_block() // gets the new block from the file
{
    if (!this->_f.eof())
    { // NOT EOF
        this->_blockPos = 0;
        char buffer[MAX_BLOCK];
        _f.read(buffer, MAX_BLOCK - 1);
        buffer[_f.gcount()] = '\0';
        this->_stk.set_string(buffer);
        std::cout << "----- New Block -------------------- [" << _f.gcount() << "] bytes" << std::endl;
        return true;
    }
    this->_f.close();
    std::cout << "END OF FILE" << std::endl;
    return false;
}