//
// Created by evezers on 13/05/23.
//

#ifndef FS_UTILS_FUNCTIONS_HPP
#define FS_UTILS_FUNCTIONS_HPP

class UtilsFunctions{
public:
    static bool isASCII(const std::string &str){
        return !std::any_of(std::begin(str), std::end(str), [](const char character) -> bool {
            return character < 0;
        });
    }

    template<class Num_t>
    static bool convertToNumber(const std::string &strValue, Num_t& numberVar){
        try {
            numberVar = std::stoi(strValue);
        } catch (std::out_of_range const &ex) {
            return true;
        } catch (std::invalid_argument const &ex) {
            return true;
        }

        return false;
    }
};

#endif //FS_UTILS_FUNCTIONS_HPP
