#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>

/*           The way files must be filled

Decrypt_this.txt                Encrypt_this.txt
+---------------------+         +---------------------+
| key                 |         | key                 |
| abcdefgh            |         | Encrypt me          |
|                     |         |                     |
+---------------------+         +---------------------+

*/
enum readwrite {encryption, decryption};
class CryptoClass {
    private:
        std::string keyWord;
        std::vector<char> cipher;
        std::vector<char> message;
        u_char matrix[5][5];
        bool type;
        char letter = 'a';

        void showResult(std::vector<char>*);
        char getLetter();
        void fillMatrix();
        void readPairs(bool);
        void readKey();
        std::tuple<int, int> getIndexesByElement(char);
        void encryptPair(std::tuple<int, int>&, std::tuple<int, int>&);
        void decryptPair(std::tuple<int, int>&, std::tuple<int, int>&);
        int decrypt();
        int encrypt();
        void enc_dec(bool);

    public:
        std::ifstream fin;
        CryptoClass(char*);
        void operator()(bool type);
};

CryptoClass::CryptoClass(char* file) {
    fin.open(file, std::ios_base::in);
};
void CryptoClass::showResult(std::vector<char> *t_arg) {
    for (auto it = t_arg->begin(); it != t_arg->end(); it++)
        std::cout << "Result: " << *it;
};
char CryptoClass::getLetter() {
    return (keyWord.find(letter++) == std::string::npos) ? letter - 1 : getLetter();
};
void CryptoClass::fillMatrix() {
    auto it = keyWord.begin();
    for (auto &pass : matrix)
        for (auto &element : pass)
            element = (it != keyWord.end()) ? *it++ : getLetter();
};
void CryptoClass::readPairs(bool type) {
    std::string input;
    getline(fin, input);
    for (auto it = input.begin(); it != input.end(); ) {
        (type ? cipher : message).push_back(*it++);
    }
    if ((type ? cipher : message).size() % 2 != 0) {
        (type ? cipher : message).push_back('a');
    }
};
void CryptoClass::readKey() {
    getline(fin, keyWord);
};
std::tuple<int, int> CryptoClass::getIndexesByElement(char required) {
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++)
            if (matrix[i][j] == required)
                return std::make_tuple(i, j);
};
void CryptoClass::encryptPair(std::tuple<int, int>& first, std::tuple<int, int>& second) {
    if (std::get<0>(first) == std::get<0>(second)) {
        cipher.push_back(matrix[std::get<0>(first) + 1][std::get<1>(first)]);
        cipher.push_back(matrix[(std::get<0>(second) + 1) % 5][std::get<1>(second)]);
    }
    else if (std::get<1>(first) == std::get<1>(second)) {
        cipher.push_back(matrix[std::get<0>(first)][std::get<1>(first) + 1]);
        cipher.push_back(matrix[std::get<0>(second)][(std::get<1>(second) + 1) % 5]);
    }
    else {
        cipher.push_back(matrix[std::get<0>(first)][std::get<1>(second)]);
        cipher.push_back(matrix[std::get<0>(second)][std::get<1>(first)]);
    }
};
void CryptoClass::decryptPair(std::tuple<int, int>& first, std::tuple<int, int>& second) {
    if (std::get<0>(first) == std::get<0>(second)) {
        if (std::get<0>(first) - 1 < 0) 
            message.push_back(matrix[5][std::get<1>(first)]);
        else 
             message.push_back(matrix[std::get<0>(first) - 1][std::get<1>(first)]);
        message.push_back(matrix[std::get<0>(second) - 1][std::get<1>(second)]);
    }
    else if (std::get<1>(first) == std::get<1>(second)) {
        if (std::get<1>(first) - 1 < 0)
            message.push_back(matrix[std::get<0>(first)][5]);
        else
            message.push_back(matrix[std::get<0>(first)][std::get<1>(first) - 1]);
        message.push_back(matrix[std::get<0>(second)][std::get<1>(second) - 1]);
    }
    else {
        message.push_back(matrix[std::get<0>(first)][std::get<1>(second)]);
        message.push_back(matrix[std::get<0>(second)][std::get<1>(first)]);
    }
};

void CryptoClass::enc_dec(bool type) {
    auto *val = type ? &cipher : &message;
    for (auto it = val->begin(); it != val->end(); ) {
        auto first_letter = getIndexesByElement(*it++);
        auto second_letter = getIndexesByElement(*it++);
        (type == encryption) ? encryptPair(first_letter, second_letter) : decryptPair(first_letter, second_letter);
    }
    showResult(type ? &message : &cipher);
};

void CryptoClass::operator()(bool type) {
    readKey();
    readPairs(type);
    fin.close();
    fillMatrix();
    enc_dec(type);
};

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cout << "Please, write type of action [enc (encryption), dec (decryption)] and file [cipher]\n";
        return 1;
    }
    CryptoClass cryptographer(argv[2]);
    if (!cryptographer.fin.is_open()) {
        std::cout << "Error. Please check filepath\n";
        return 1;
    }
    if (std::string(argv[1], 3) == "enc")
        cryptographer(encryption);
    else if (std::string(argv[1], 3) == "dec")
        cryptographer(decryption);
    else {
        std::cout << "Error. Bad command. Choose encryption (enc) or decryption (dec)\n";
        return 1;
    }
    return 0;
};