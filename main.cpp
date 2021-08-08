#include <cctype>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <string>
#include <map>

struct TestCase { std::string name{}; std::ostringstream failures{}; };
template <typename T> void operator | (TestCase& testCase, T&& testBody) {
    testBody(); auto failures = testCase.failures.str();
    if (failures.empty()) std::cout << testCase.name << ": SUCCESS\n";
    else std::cerr << testCase.name << ": FAILURE\n" << failures;
}
void addFailure(std::ostream& os, const char* file, unsigned line, const char* condition) {
    os << file << ":" << line << ": Failed: " << condition << "\n";
}
#define TEST(name) TestCase name{#name}; name | [&, &failures = name.failures]
#define EXPECT(cond) if (cond) {} else addFailure(failures, __FILE__, __LINE__, #cond)
#define ASSERT(cond) if (cond) {} else return addFailure(failures, __FILE__, __LINE__, #cond)

class Translator
{
public:
    Translator() = delete;
    Translator(std::string) = delete;

    static std::string translateLettersToMorseCode(const std::string& text)
    {
        std::string output;

        if(!text.empty())
        {
            char temp;
            for(const auto el : text)
            {
                if(el == ' ')
                {
                    output.append(" ");
                }
                else
                {
                    temp = el;
                    if(el >= 'a' && el <= 'z')
                        temp = std::toupper(el);
                    temp-='A';
                    output.append(morseCode[temp]);
                    output.append(" ");
                }
            }
            output.pop_back();
            std::cout << "Morse Code: " << output << std::endl;
        }
        return output;
    }

    static std::string translateMorseCodeToLetters(const std::string& text)
    {
        std::string output;

        if(!text.empty())
        {
            std::string temp;

            for(const auto el : text)
            {
                if(el == ' ')
                {
                    for(uint32_t i=0; i<letterCount; i++)
                    {
                        if(temp == morseCode[i])
                        {
                            output.push_back(i+'A');
                            temp.clear();
                            break;
                        }

                        if(temp == "")
                        {
                            output.push_back(' ');
                            temp.clear();
                            break;
                        }
                    }
                }
                else
                {
                    temp.push_back(el);
                }
            }

            for(uint32_t i=0; i<letterCount; i++)
            {
                if(temp == morseCode[i])
                {
                    output.push_back(i+'A');
                    break;
                }
            }
        }
        std::cout << "Text: " << output << std::endl;
        return output;
    }

    static std::vector<int32_t> translateMorseCodeToBytes(std::string morseCode)
    {
        std::vector<int32_t> output;

        std::transform(morseCode.begin(), morseCode.end(), std::back_inserter(output), [](const auto& el){
            return mapOfBytes.at(el);
        });

        return output;
    }
private:
    inline static const uint32_t letterCount = 26;
    inline static const std::map<char, uint32_t> mapOfBytes = {{' ', 0b00}, {'.', 0b01}, {'-', 0b10}};
    inline static const std::array<std::string, 26> morseCode = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--.." };
    //ASCII 'A' = 65, 'B' = 66, 'C' = 67...
};

int main()
{
    TEST(GivenOlaExpectCorrectResult)
    {
        EXPECT(Translator::translateLettersToMorseCode("Ola") == "--- .-.. .-");
    };
    TEST(GivenMorseCodeOlaExpectCorrectInput)
    {
        EXPECT(Translator::translateMorseCodeToLetters("--- .-.. .-") == "OLA");
    };
    TEST(GivenMorseCodeA_BExpectCorrectInput)
    {
        EXPECT(Translator::translateMorseCodeToLetters(".-  -...") == "A B");
    };
    TEST(GivenMorseCodeABExpectCorrectLetter)
    {
        EXPECT(Translator::translateMorseCodeToLetters(".- -...") == "AB");
    };
    TEST(GivenMorseCodeAExpectCorrectLetter)
    {
        EXPECT(Translator::translateMorseCodeToLetters(".-") == "A");
    };
    TEST(GivenEeAndExpectVectorOfBytes)
    {
        std::vector<int32_t> testVec {0b01,0b00,0b01};
        std::string testString = Translator::translateLettersToMorseCode("Ee");
        EXPECT(Translator::translateMorseCodeToBytes(testString) == testVec);
    };
    TEST(GivenEEAndExpectVectorOfBytes)
    {
        std::vector<int32_t> vec {0b01,0b00,0b01};
        std::string testString = Translator::translateLettersToMorseCode("EE");
        EXPECT(Translator::translateMorseCodeToBytes(testString) == vec);
    };
    TEST(GivenAAndExpectVectorOfBytes)
    {
        std::string testString = ".-";
        std::vector<int32_t> testVec {0b01, 0b10};
        EXPECT(Translator::translateMorseCodeToBytes(testString) == testVec);
    };
    TEST(GivenA_AExpectCorrectResult)
    {
        EXPECT(Translator::translateLettersToMorseCode("A A") == ".-  .-");
    };
    TEST(GivenEmptyStringExpectCorrectResult)
    {
        EXPECT(Translator::translateLettersToMorseCode("") == "");
    };
    TEST(GivenAAExpectCorrectResult)
    {
        EXPECT(Translator::translateLettersToMorseCode("AA") == ".- .-");
    };
    TEST(GivenCExpectCorrectResult)
    {
        EXPECT(Translator::translateLettersToMorseCode("C") == "-.-.");
    };
    TEST(GivenBExpectCorrectResult)
    {
        EXPECT(Translator::translateLettersToMorseCode("B") == "-...");
    };
    TEST(GivenAExpectCorrectResult)
    {
        EXPECT(Translator::translateLettersToMorseCode("A") == ".-");
    };
};
