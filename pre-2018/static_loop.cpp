#include <iostream>

void convert(const int& value1, int& value2);

struct ConversionValidator
{
    static bool validate()
    {
        for (int i = 0; i < 10; i++)
        {
            int j;
            convert(i, j);
            if (j > 20)
            {
                return false;
            }
        }

        return true;
    }
};

void convert(const int& value1, int& value2)
{
    static bool is_valid = ConversionValidator::validate();
    (void)is_valid;

    value2 = value1 + 1;
}

int main()
{
    const int i = 1;
    int j;
    convert(i, j);

    std::cout << "j = " << j << std::endl;

    return 0;
}
