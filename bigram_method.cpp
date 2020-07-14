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
template <class T>
int main(int argc, char *argv[]) {
    enum readwrite {encription, decription};
    class CryptoClass {

        private:
            std::string keyWord;
            std::vector<char> cipher;
            std::vector<char> message;
            u_char matrix[5][5];

        public:
            char letter = 'a';
            std::ifstream fin;

            CryptoClass::CryptoClass(char* file) {
                fin.open(file, std::ios_base::in);
            };

            void showResult(T t_arg) {
                for (auto it = t_arg.begin(); it != t_arg.end(); it++)
                    std::cout << *it;
            };

            char getLetter() {
                return (keyWord.find(letter++) != std::string::npos) ? letter - 1 : getLetter();
            };

            void fillMatrix() {
                auto it = keyWord.begin();
                for (auto &pass : matrix) {
                    for (auto &element : pass) {
                        element = (it != keyWord.end()) ? *it++ : getLetter();
                    }
                }
            };

            void readPairs(bool type) {
                std::string input;
                getline(fin, input);
                for (auto it = input.begin(); it != input.end(); ) {
                    if (type == decription) {
                        if (*it++ == 'x' && it != input.end())
                            cipher.push_back(*it);
                    }
                    else {
                        message.push_back(*it);
                    }
                }
                if (type == decription && (cipher.size() % 2 != 0)) {
                    std::cout << "Error, wrong bigramm\n";
                    cipher.clear();
                }
                if (type == encription && (message.size() % 2 != 0)) {
                    std::cout << "Error, wrong message\n";
                    message.clear();
                }
            };
            
            void readKey() {
                getline(fin, keyWord);
            };

            std::tuple<int, int> getIndexesByElement(char required) {
                for (int i = 0; i < 6; i++)
                    for (int j = 0; j < 6; j++)
                        if (matrix[i][j] == required)
                            return std::make_tuple(i, j);
            };

            void encryptPair(std::tuple<int, int>& first, std::tuple<int, int>& second) {
                if (std::get<0>(first) == std::get<0>(second)) {
                    message.push_back(matrix[std::get<0>(first) + 1][std::get<1>(first)]);
                    message.push_back(matrix[(std::get<0>(second) + 1) % 6][std::get<1>(second)]);
                }
                else if (std::get<1>(first) == std::get<1>(second)) {
                    message.push_back(matrix[std::get<0>(first)][std::get<1>(first) + 1]);
                    message.push_back(matrix[std::get<0>(second)][(std::get<1>(second) + 1) % 6]);
                }
                else {
                    message.push_back(matrix[std::get<0>(first)][std::get<1>(second)]);
                    message.push_back(matrix[std::get<0>(second)][std::get<1>(first)]);
                }
            };

            void decryptPair(std::tuple<int, int>& first, std::tuple<int, int>& second) {
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

            int decrypt() {
                readPairs(decription);
                if (cipher.empty())
                    return 1;
                for (auto it = cipher.begin(); it != cipher.end(); ) {
                    auto first_letter = getIndexesByElement(*it++);
                    auto second_letter = getIndexesByElement(*it++);
                    decryptPair(first_letter, second_letter);
                }
                showResult(message);
                return 0;
            };

            int encrypt() {
                readPairs(encription);
                if (message.empty())
                    return 1;
                for (auto it = cipher.begin(); it != cipher.end(); ) {
                    auto first_letter = getIndexesByElement(*it++);
                    auto second_letter = getIndexesByElement(*it++);
                    encryptPair(first_letter, second_letter);
                }
                showResult(cipher);
                return 0;
            };

            int operator()(bool type) {
                readKey();
                fin.close();
                fillMatrix();
                return (type == encription) ? encrypt() : decrypt();
            };
            
    };
    if (argc != 3) {
        std::cout << "Please, write type of action [enc (encryption), dec (decription)] and file [cipher]\n";
        return 1;
    }
    CryptoClass cryptographer(argv[2]);
    if (!cryptographer.fin.is_open()) {
        std::cout << "Error. Please check filepath\n";
        return 1;
    }
    if (argv[1] == "enc")
        return cryptographer(encription);
    else if (argv[1] == "dec")
        return cryptographer(decription);
    else {
        std::cout << "Error. Bad command. Choose encryption (enc) or decription (dec)\n";
        return 1;
    }
};