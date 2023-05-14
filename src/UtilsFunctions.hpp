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

    /**
     * Функция переводит строку в число
     * @tparam Num_t числовой тип в который происходит конвертация
     * @param strValue строка с числом
     * @param numberVar ссылка на переменную принимающую значение
     * @return true при возникновении ошибки конвертации, false otherwise
     */
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

    /**
     * Находит и устанавливает переменные лоигческого типа из переданного отображения в переданную ссылку переменной
     * @tparam Values переменной длины массив строковых значений ключей
     * @param args массив ключей
     * @param boolVar переменная для установки в нее логического значения
     * @param values строки для поиска ключей
     */
    template<class ...Values>
    static void findAndSetBoolArg(const posArgs_t &args, bool &boolVar, Values... values){
        if(std::find_if(std::begin(args), std::end(args), [&values...](const std::string &argGiven) -> bool {
            return std::any_of(std::begin({values...}), std::end({values...}), [&argGiven](const std::string &argNeeded) -> bool {
                return argGiven == argNeeded;
            });
        }) != std::end(args)){
            boolVar = true;
        } else{
            boolVar = false;
        }
    }
};

#endif //FS_UTILS_FUNCTIONS_HPP
